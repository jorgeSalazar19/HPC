#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define CHUNKSIZE 20

void prin_matrix(float **matriz ,int row ,int col){
  for (int i = 0; i < col; ++i)
  {
    for (int j = 0; j < row; ++j)
    {
      printf("%.2f %s", matriz[i][j] , " " );
    }
  printf("\n");
  }
}

void mult_matriz(float **matriz1 , float **matriz2 , float **matrizR, int col1, int  row1, int col2, int row2 ){
	int tid, chunk , i , j , k;
  chunk = CHUNKSIZE;
  float cont;
  #pragma omp parallel shared(matriz1,matriz2,matrizR,chunk) private(tid,i,j,k)
  {
    #pragma omp for schedule(dynamic,chunk)
  	for (i = 0; i < col1; ++i){
  	    for (j = 0; j < row2; ++j){
  	      cont = 0,0;
  	      for (k = 0; k < row2; ++k){
  	        cont += matriz1[i][k] * matriz2[k][j];
  	      }
  	      matrizR[i][j] = cont;
  	    }
  	  }
    }
  }


int validate_matriz(int colM1 , int rowM2){
	if (colM1 == rowM2)
	{
		return 1;
	}
	else{
		return 0;
	}
}

void fill_matriz(int col , int row , float **matriz){
	if (matriz==NULL)
        printf ("Error de Mem.");

    for (int i = 0; i < row; ++i)
    {
    	for (int j = 0; j < col; ++j)
    	{	
    		matriz[i][j] = 0,0;
    	}
    }
}

void matrix_dinamic(int col , int row , float **matriz ){

	if (matriz==NULL)
        printf ("Error de Mem.");
    for (int i = 0; i < row; ++i)
    {
    	for (int j = 0; j < col; ++j)
    	{	
    		float random = rand()%100;
    		matriz[i][j] = random;
    	}
    }

}

void save_matriz(float **Matrix, int row, int col){
  FILE *f = fopen("result_mult.csv", "a");

  if (f == NULL){
    printf("File error\n");
    exit(-1);
  }

  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; ++j){
      if(col - 1 == j){
        fprintf(f, "%.2f", Matrix[i][j]);
      }
      else{
        fprintf(f, "%.2f, ",  Matrix[i][j]);
      }
    }
     fprintf(f, "\n");   
  }

  fprintf(f, "\n");
  fclose(f);

  return;
}

int main(int argc, char const *argv[])
{

	   int flag;
    int Scol1;
    int Srow1;
    int Scol2;
    int Srow2;

    clock_t time_ini,time_end;
    printf("%s\n\n", "Multiplicación de matrices" );
	printf("Introduzca el numero de columnas matriz 1: ");
    scanf("%d", &Scol1);
    printf("Introduzca el numero de filas matriz 1: ");
    scanf("%d", &Srow1);
    printf("Introduzca el numero de columnas matriz 2: ");
    scanf("%d", &Scol2);
    printf("Introduzca el numero de filas matriz 2: ");
    scanf("%d", &Srow2);

    flag = validate_matriz(Scol1 , Srow2);

    if (flag == 1)
    {
    	float **matriz1;
		matriz1 = (float **)malloc (Srow1*sizeof(float *));

		for (int i=0;i<Srow1;i++)
			matriz1[i] = (float *) malloc (Scol1*sizeof(float));

		float **matriz2;
		matriz2 = (float **)malloc (Srow2*sizeof(float *));

		for (int i=0;i<Srow2;i++)
			matriz2[i] = (float *) malloc (Scol2*sizeof(float));

		float **matrizR;
		matrizR = (float **)malloc (Srow1*sizeof(float *));

		for (int i=0;i<Srow1;i++)
			matrizR[i] = (float *) malloc (Scol2*sizeof(float));

		printf("%s\n", "-----------Matriz 1----------" );
		matrix_dinamic(Scol1 , Srow1 , matriz1);
    prin_matrix(matriz1,Srow1,Scol1);
		printf("%s\n", "-----------Matriz 2----------" );
		matrix_dinamic(Scol2 , Srow2 , matriz2);
    prin_matrix(matriz2,Srow2,Scol2);
		printf("%s\n","------------Resultado Multiplicación-----------" );
		fill_matriz(Scol2 , Srow1 , matrizR);
    time_ini = clock();
		mult_matriz(matriz1,matriz2,matrizR,Scol1,Srow1,Scol2,Srow2);
    time_end = clock();
    prin_matrix(matrizR,Srow1,Scol2);
		save_matriz(matrizR , Srow1 , Scol2);
    free(matriz1);
    free(matriz2);
    free(matrizR);
    printf("%s %.6f\n", "Tiempo de ejecución: ", (double) (time_end - time_ini)/CLOCKS_PER_SEC );
    	
    }
	
	else{
		printf("%s\n", "Las matrices no son compatibles para multiplicar" );
	}


	return 0;
}