#include <cuda.h>
#include <stdio.h>
#include <malloc.h>


void save_matriz(float *Matrix, int row, int col){
  FILE *f = fopen("result_mult.csv", "a");

  if (f == NULL){
    printf("File error\n");
    exit(-1);
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; ++j){
      if(col - 1 == j){
        fprintf(f, "%.2f", Matrix[i * col + j]);
      }
      else{
        fprintf(f, "%.2f, ",  Matrix[i * col + j]);
      }
    }
     fprintf(f, "\n");
  }

  fprintf(f, "\n");
  fclose(f);

  return;
}

__host__
void print(float *M, int rows, int cols){
  printf("\n");
  printf("----------------------------------------\n");
  for(int i = 0; i < rows; i++) {
  		for(int j = 0; j < cols; j++) {
     		printf("%.2f ", M[i * cols + j]);
    	}
		printf("\n");
  }
  printf("----------------------------------------\n");
  printf("\n");
  return;
}


__global__ void matrixMultGPU(float *d_matrix1, float *d_matrix2, float *d_MatrixR, int rowM1 , int rowM2 , int colM1 , int colM2 ) {

	int k = 0;
	float sum = 0.0;
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int row = threadIdx.y + blockDim.y * blockIdx.y;

	if (col < colM2 && row < rowM1) {
		for (k = 0; k < rowM2; k++) {
			sum += d_matrix1[row * colM1 + k] * d_matrix2[k * colM2 + col];
		}
		d_MatrixR[row * colM2 + col] = sum;
		}
	}


__host__
void read_matrix_from_file(float *M, FILE *archivo, int rows, int cols){
	for (int i = 0; i < rows; ++i){
		for (int j = 0; j < cols; ++j){
			fscanf(archivo, "%f,", &M[i * cols + j]);
		}
	}
	fclose(archivo);
	return;
}

bool validate(int colM1 ,int rowM2){

	if (colM1 != rowM2){
		return true;
	}

	else{
		return false;
	}

}


int main(int argc, char** argv){

	if (argc != 3){
		printf("agregue los archivos como parametro\n");
		return 1;
	}

	float *h_matrix1, *h_matrix2, *h_MatrixR;
	int rowM1 , rowM2 , colM1 , colM2;
	cudaError_t error = cudaSuccess;

	FILE *file_1 , *file_2;
	file_1 = fopen(argv[1], "r");
	file_2 = fopen(argv[2], "r");

	fscanf(file_1, "%d", &rowM1);
	fscanf(file_1, "%d", &colM1);
	fscanf(file_2, "%d", &rowM2);
	fscanf(file_2, "%d", &colM2);

	if (validate(colM1,rowM2)){
		printf("Las matrices son incompatibles y no se pueden multiplicar");
		return 1;
	}

	float sizeM1 = rowM1 * colM1 * sizeof(float);
	float sizeM2 = rowM2 * colM2 * sizeof(float);
	float sizeMR = rowM1 * colM2 * sizeof(float);

	h_matrix1 = (float*)malloc(sizeM1);
	h_matrix2 = (float*)malloc(sizeM2);
	h_MatrixR = (float*)malloc(sizeMR);

	read_matrix_from_file(h_matrix1, file_1, rowM1, colM1);
	read_matrix_from_file(h_matrix2, file_2, rowM2, colM2);

	float *d_matrix1, *d_matrix2, *d_MatrixR;

	error = cudaMalloc ((void **) &d_matrix1, sizeM1);
	if (error != cudaSuccess){
	printf("Error solicitando memoria en la GPU para d_matrix1\n");
	exit(-1);
}
	error = cudaMalloc ((void **) &d_matrix2, sizeM2);
	if (error != cudaSuccess){
	printf("Error solicitando memoria en la GPU para d_matrix2\n");
	exit(-1);
}
	error = cudaMalloc ((void **) &d_MatrixR, sizeMR);
	if (error != cudaSuccess){
	printf("Error solicitando memoria en la GPU para d_MatrixR\n");
	exit(-1);
}
	cudaMemcpy(d_matrix1, h_matrix1, sizeM1, cudaMemcpyHostToDevice);
	cudaMemcpy(d_matrix2, h_matrix2, sizeM2, cudaMemcpyHostToDevice);

	dim3 bloques(ceil(colM2/16.0),ceil(rowM1/16.0),1);
	dim3 hilos(16,16,1);

	matrixMultGPU<<<bloques,hilos>>>(d_matrix1,d_matrix2,d_MatrixR, rowM1, rowM2 , colM1 , colM2);

	cudaMemcpy(h_MatrixR,d_MatrixR,sizeMR,cudaMemcpyDeviceToHost);

	print(h_matrix1, rowM1 , colM1);
	print(h_matrix2, rowM2 , colM2);
	print(h_MatrixR, rowM1 , colM2);
	save_matriz(h_MatrixR , rowM1, colM2);

	cudaFree(d_matrix1); cudaFree(d_matrix2); cudaFree(d_MatrixR);
	free(h_matrix1); free(h_matrix2); free(h_MatrixR);
}
