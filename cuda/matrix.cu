#include <cuda.h>
#include <stdio.h>
#include <malloc.h>

__host__

void fill_vector(float* matrix , int size){
	float aux = 2.0;

	for (int i = 0; i < size; ++i)
	{
		matrix[i] = (((float)rand())/(float)(RAND_MAX)) * aux;
	}
}

__host__
void print(float *V, int len){
  for (int i = 0; i < len; i++) {
    printf("%.2f ", V[i]);
  }
  printf("\n");
}

__global__

void matrixMult(float* d_Matrix , float* d_Result , int width){

	int row = blockIdx.y*blockDim.y+threadIdx.y;

	if(row < width){

		d_Result[row] = d_Matrix[row]*2;
	}
}


int main(){

	int n = 1000;

	int width = n * n * sizeof(float);

	float *h_Matrix = (float*) malloc(width);
	float *h_Result = (float*) malloc(width);

	fill_vector(h_Matrix,n);
	print(h_Matrix,n)

	float *d_Matrix, *d_Result;
	cudaMalloc ((void **) &d_Matrix, width);
	cudaMalloc ((void **) &d_Result, width);

	cudaMemcpy(d_Matrix,h_Matrix,width,cudaMemcpyHostToDevice);

	dim3 bloques(ceil(n/20.0),1,1);
	dim3 hilos(10,1,1);

	matrixMult<<<bloques,hilos>>>(d_Matrix,d_Result,width);

	cudaMemcpy(h_Result,d_Result,width,cudaMemcpyDeviceToHost);

	print(h_Result,n)

	cudaFree(d_Matrix);
	cudaFree(d_Result);
	free(h_Matrix);
	free(h_Result);
}
