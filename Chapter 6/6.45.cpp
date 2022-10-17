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

void copyPartToTemp(int* src, int* tempMatrix, int dim, int tempMatrixDim, int posFrom, int jTo, int kTo);
void copyTempPartToDest(int* dst, int* tempMatrix, int dim, int tempMatrixDim, int posFrom, int jTo, int kTo, int tempPosStep = 0);

void myTransposeUsingMatrix(int *dst, int *src, int dim, int tempMatrixDim){
    int tempMatrixSize = tempMatrixDim*tempMatrixDim;
    int* tempMatrix = new int[tempMatrixSize];
    int dimEnd = dim - tempMatrixDim + 1;
    int pos = 0;
    int col = 0;
    int i = 0;
    int rest = dim%tempMatrixDim;

    for(; col < dimEnd; col += tempMatrixDim){
        for(; i < dimEnd; i+=tempMatrixDim){
            pos = i*dim+col;
            copyPartToTemp(src, tempMatrix, dim, tempMatrixDim, pos, tempMatrixDim, tempMatrixDim);

            pos = (col*dim)+i;
            copyTempPartToDest(dst, tempMatrix, dim, tempMatrixDim, pos, tempMatrixDim, tempMatrixDim);
        }

        if(rest){
            i = dim-rest;
            pos = i*dim+col;
            copyPartToTemp(src, tempMatrix, dim, tempMatrixDim, pos, rest, tempMatrixDim);

            pos = (col*dim)+i;
            copyTempPartToDest(dst, tempMatrix, dim, tempMatrixDim, pos, rest, tempMatrixDim, tempMatrixDim-rest);
        }
    
        i=0;
    }

    if(rest){
        col = dim-rest;
        i = 0;

        for(; i < dimEnd; i+=tempMatrixDim){
            pos = i*dim+col;
            copyPartToTemp(src, tempMatrix, dim, tempMatrixDim, pos, tempMatrixDim, rest);

            pos = (col*dim)+i;
            copyTempPartToDest(dst, tempMatrix, dim, tempMatrixDim, pos, tempMatrixDim, rest);
        }
        
        i = dim-rest;
        pos = i*dim+col;
        copyPartToTemp(src, tempMatrix, dim, tempMatrixDim, pos, rest, rest);

        pos = (col*dim)+i;
        copyTempPartToDest(dst, tempMatrix, dim, tempMatrixDim, pos, rest, rest, tempMatrixDim-rest);    
    }
    delete[] tempMatrix;
}

void copyPartToTemp(int* src, int* tempMatrix, int dim, int tempMatrixDim, int posFrom, int jTo, int kTo){
    for(int j = 0; j < jTo; j++, posFrom+=dim){
        for(int k = 0, line = 0; k < kTo; k++, line+=tempMatrixDim){
            tempMatrix[j+line]=src[posFrom+k];
        }
    }
}

void copyTempPartToDest(int* dst, int* tempMatrix, int dim, int tempMatrixDim, int posFrom, int jTo, int kTo, int tempPosStep){
    for(int k = 0, tempPos = 0; k < kTo; k++, posFrom+=dim, tempPos+=tempPosStep) {
        for(int j = 0; j < jTo; j++, tempPos++){
            dst[posFrom+j]=tempMatrix[tempPos];
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