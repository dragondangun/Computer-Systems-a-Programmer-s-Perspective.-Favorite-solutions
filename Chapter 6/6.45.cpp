#include "stdio.h"
#include <cstdlib>
#include <cstring>
#include <time.h> 

void transpose(int *dst, int *src, int dim) {
    int i, j;

    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[j*dim + i] = src[i*dim + j];

}

void copyPartSrcToDst(int* src, int* dst, int dim, int partDim, int srcFrom, int dstFrom, int jTo, int kTo);

void myTransposeUsingMatrix(int *dst, int *src, int dim, int partDim){
    int dimEnd = dim - partDim + 1;
    int srcFrom = 0;
    int dstFrom = 0;
    int col = 0;
    int i = 0;
    int rest = dim%partDim;

    for(; col < dimEnd; col += partDim, i = 0){ 
        for(; i < dimEnd; i+=partDim){
            srcFrom = i*dim+col;
            dstFrom = (col*dim)+i;
            copyPartSrcToDst(src, dst, dim, partDim, srcFrom, dstFrom, partDim, partDim);
        }

        if(rest){
            i = dim-rest;
            srcFrom = i*dim+col;
            dstFrom = (col*dim)+i;
            copyPartSrcToDst(src, dst, dim, partDim, srcFrom, dstFrom, rest, partDim);
        }
    }

    if(rest){
        col = dim-rest;

        for(; i < dimEnd; i+=partDim){;
            srcFrom = i*dim+col;
            dstFrom = (col*dim)+i;
            copyPartSrcToDst(src, dst, dim, partDim, srcFrom, dstFrom, partDim, rest);
        }
        
        int pos = (dim+1)*col;
        copyPartSrcToDst(src, dst, dim, partDim, pos, pos, rest, rest);
    }
}

void copyPartSrcToDst(int* src, int* dst, int dim, int partDim, int srcFrom, int dstFrom, int jTo, int kTo){
    for(int j = 0; j < jTo; j++, srcFrom+=dim){
        for(int k = 0, line = dstFrom; k < kTo; k++, line+=dim){
            dst[j+line]=src[srcFrom+k];
        }
    }

}

void myTransposeUsingMatrixWrapper(int *dst, int *src, int dim){
    myTransposeUsingMatrix(dst, src, dim, 8);
}

void randomFill(int* matrix, int N);
void showMatrix(int* matrix, int N);
bool isMatrixEqual(int* left, int* right, int dim);

int main(){
    srand(time(NULL));
    const int N = 10001;
    int matrixSize = N*N;
    int* matrix = new int[matrixSize];
    int* distMatrix = new int[matrixSize];
    int* myDistMatrixUsingMatrix = new int[matrixSize];

    randomFill(matrix, N);
    transpose(distMatrix, matrix, N);
    myTransposeUsingMatrixWrapper(myDistMatrixUsingMatrix, matrix, N);
    
    if(isMatrixEqual(distMatrix, myDistMatrixUsingMatrix, N)){
        printf("Ok");
    }
    else{
        printf("Error!\nOriginal:\n");
        showMatrix(matrix, N);
        printf("\n\nTransposed:\n");
        showMatrix(distMatrix, N);
        printf("\n\nMy transposed:\n");
        showMatrix(myDistMatrixUsingMatrix, N);
    }

    printf("\n");

    delete[] matrix;
    delete[] distMatrix;
    delete[] myDistMatrixUsingMatrix;
}

void randomFill(int* matrix, int N){
    for(int i = 0; i < N; i++){
      for(int j = 0; j < N; j++){
          matrix[i*N+j] = rand() % 10 + 1;
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
