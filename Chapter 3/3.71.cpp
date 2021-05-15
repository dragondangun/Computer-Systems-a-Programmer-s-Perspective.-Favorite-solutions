#include <stdio.h>

#define BUF_MAX = 8;

void good_echo(){
    char buf[10]; //more for safe demonstration
    
    char* str = fgets(str, BUF_MAX, stdin); //gets

    if(str!=NULL){
        printf("%s", str); // puts
    }
}