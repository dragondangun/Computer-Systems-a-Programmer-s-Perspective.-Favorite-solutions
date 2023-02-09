#include "stdio.h"
#include <cstdlib>
#include <cstring>
#include <time.h> 

void col_convert(int *G, int dim) {
	int i, j;
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++)
			G[j*dim + i] = G[j*dim + i] || G[i*dim + j];
}

void myCol_convert(int *G, int dim){
    int len = (dim*dim+dim)/2;
    int* flat = new int[len];

    for(int i = 0, counter = 0; i < dim; i++){
        for(int j = i; j < dim; j++, counter++){
            flat[counter] = G[i*dim+j];
        }
    }

    for(int i = 1; i<dim; i++){
        for(int j = 0, space = dim-2, counter = i; j<i; j++, counter+=space+1, --space){
            flat[counter] |= G[i*dim + j];
        }
    }

    for(int i = 0, counter = 0; i < dim; i++){
        for(int j = i; j < dim; j++, counter++){
            G[i*dim+j] = flat[counter];
        }
    }

    for(int i = 1; i<dim; i++){
        for(int j = 0, space = dim-2, counter = i; j<i; j++, counter+=space+1, --space){
            G[i*dim + j] = flat[counter];
        }
    }
}

void randomFill(int* matrix, int N);
void showMatrix(int* matrix, int N);
bool isMatrixEqual(int* left, int* right, int dim);

int main(){
    srand(time(NULL));
    const int repeatMax = 10;
    const int N = 10000;
    int matrixSize = N*N;
    int* matrix = new int[matrixSize];
    int* matrixCpy = new int[matrixSize];

    for(int i = 0; i < repeatMax; i++){
        randomFill(matrix, N);
        memcpy(matrixCpy, matrix, matrixSize*sizeof(*matrix));
        col_convert(matrix, N);
        myCol_convert(matrixCpy, N);
        

        // if(isMatrixEqual(matrix, matrixCpy, N)){
        //     printf("Ok\n");
        // }
        // else{
        //     printf("Error!\nOriginal:\n");
        //     showMatrix(matrix, N);
        //     printf("\n\nConverted:\n");
        //     showMatrix(matrix, N);
        //     printf("\n\nMy converted:\n");
        //     showMatrix(matrixCpy, N);
        //     break;
        // }
    }

    printf("\n");

    delete[] matrix;
    delete[] matrixCpy;
}

void randomFill(int* matrix, int N){
    for(int i = 0; i < N; i++){
      for(int j = 0; j < N; j++){
          matrix[i*N+j] = rand() % 2;
      }
    }
}

void showMatrix(int* matrix, int N){
    for(int i = 0; i < N; i++){
      for(int j = 0; j < N; j++){
          printf("%d\t", matrix[i*N+j]);
      }
        printf("\n");
    }
}

bool isMatrixEqual(int* left, int* right, int dim){
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            if(left[i*dim+j] != right[i*dim+j]){
                return false;
            }
        }
    }
    return true;
}
