#include <cuda.h>
#include <stdio.h>

__host__

void fill_vector(float* matrix , size){
	float aux = 2.0;

	for (int i = 0; i < size; ++i)
	{
		matrix[i] = (((float)rand())/(float)(RAND_MAX)) * aux;
	}
}

__global__

void matrixMult(float* d_Matrix , float* d_Result , int width){

	int row = blockIdx.y*blockDim.y+threadIdx.y;

	if(row < width){

		d_Result[row] = d_Matrix[row]*2
	}
}


int main(){

	int n = 1000;

	int width = n * n * sizeof(float)

	float *h_Matrix = (float*) Malloc(width)
	float *h_Result = (float*) Malloc(width)

	fill_vector(h_Matrix, width);

	float *d_Matrix, *d_Result;
	cudaMalloc ((void **) &d_Matrix, width)
	cudaMalloc ((void **) &d_Result, width)

	cudaMemcpy(d_Matrix,h_Matrix,width,cudaMemcpyHostToDevice);

	dim3 bloques(ceil(n/20.0),1,1);
	dim3 hilos(10,1,1);

	matrixMult<<<bloques,hilos>>>(d_Matrix,d_Result,width);

	cudaMemcpy(h_Result,d_Result,width,cudaMemcpyDeviceToHost);

	cudaFree(d_Matrix;
	cudaFree(d_Result);
	free(h_Matrix:
	free(h_Result);
}
