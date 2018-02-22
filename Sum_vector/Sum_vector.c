#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#define CHUNKSIZE 2
#define ID_1 1
#define ID_2 2

void print_vector(float *vectorA ,int size ,int ID){

  if (ID == 3)
  {
    printf("%s%d%s", "vector Resultado ", ID ," = [ " );
  }
  else{
      printf("%s%d%s", "vector ", ID ," = [ " );
  }
  for (int i = 0; i < size; ++i)
  {
      printf("%.4f %s", vectorA[i] , " " );
  }
  printf("%s\n", "]" );
}

void Array_dinamic(int size , float *vectorA){

  int tid, chunk;
  chunk = CHUNKSIZE;

  #pragma omp parallel shared(vectorA,chunk) private(tid)
  {
  	if (vectorA==NULL)
    {
          printf ("Error de Mem.");
    }
      
      #pragma omp for schedule(dynamic,chunk)
        for (int i=0; i<size; i++){
    		float random = rand()%100;
            vectorA[i]=random;
            random = 0;

        }
    }
  }


void Sum_vector(float *vectorA , float *vectorB , float *result , int size){
  int tid, chunk;
  chunk = CHUNKSIZE;

  #pragma omp parallel shared(vectorA,chunk) private(tid)
  {
    tid = omp_get_thread_num();
    printf("Hilo %d inicia...\n",tid );
    #pragma omp for schedule(dynamic,chunk)
  	for (int i = 0; i < size; ++i)
  	{
  		result[i] = vectorA[i] + vectorB[i];
      printf("Hilo %d valor %.4f \n", tid , result[i] );
  	}
  }
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
  int tiempo_inicio , tiempo_final;
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
    print_vector(vectorA , valorUsuario , ID_1);
    print_vector(vectorB , valorUsuario , ID_2);
    Sum_vector(vectorA , vectorB , result , valorUsuario);
    save_vector(result,valorUsuario);
    print_vector(result, valorUsuario , 3);

  tiempo_final = clock();
  segundos = (double)(tiempo_inicio-tiempo_final)/ CLOCKS_PER_SEC;
  printf("%f\n", segundos );
	return 0;
  free(vectorA);
  free(vectorB);
  free(result);
}