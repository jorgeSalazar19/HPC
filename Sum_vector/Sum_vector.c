#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define CHUNKSIZE 10

void Array_dinamic(int size , float *vectorA){

  int chunk = CHUNKSIZE;
  #pragma omp parallel shared(vectorA,chunk)
  {
    if (vectorA==NULL)
        printf ("Error de Mem.");

    printf("%s", "Vector = [" );
    #pragma omp for schedule(dynamic,chunk)
      for (int i=0; i<size; i++){
      float random = rand()%100;
          vectorA[i]=random;
          printf("%.2f ", vectorA[i] );
          random = 0;
      }
    printf("%s", "]" );
    printf("\n");
  }
}


void Sum_vector(float *vectorA , float *vectorB , float *result , int size){
	printf("%s", "Vector Resultado = [" );
	for (int i = 0; i < size; ++i)
	{
		result[i] = vectorA[i] + vectorB[i];
		printf("%.2f ", result[i] );
	}
	printf("%s", "]" );
	printf("\n");
}

void save_vector(float *Vector, int size){
  FILE *f = fopen("result_vector.csv", "a");

  if (f == NULL){
    printf("File error\n");
    exit(-1);
  }

  for (int i = 0; i < size; i++) {
    if(size - 1 == i){
      fprintf(f, "%.2f", Vector[i]);
    }
    else{
      fprintf(f, "%.2f, ", Vector[i]);
    }
  }
  fprintf(f, "\n");
  fclose(f);
}

int main(int argc, char const *argv[])
{
	/* code */
	int valorUsuario;
  int tiempo_inicio, tiempo_final;
  double segundos;

  tiempo_inicio = clock();

	printf("%s\n\n", "Suma de vectores" );
	printf("Introduzca tamaño de lo vectores: ");
    scanf("%d", &valorUsuario);

	float *vectorA;
	float *vectorB;
	float *result;

	vectorA = (float *)malloc(valorUsuario*sizeof(float));
	vectorB = (float *)malloc(valorUsuario*sizeof(float));
	result = (float *)malloc(valorUsuario*sizeof(float));

    Array_dinamic(valorUsuario , vectorA);
    Array_dinamic(valorUsuario , vectorB);
    Sum_vector(vectorA , vectorB , result , valorUsuario);
    save_vector(result,valorUsuario);

  

  free(vectorA);
  free(vectorB);
  free(result);

  tiempo_final = clock();

  segundos = (double)(tiempo_inicio - tiempo_final) / CLOCKS_PER_SEC;

  printf("%f\n", segundos );

	return 0;
}