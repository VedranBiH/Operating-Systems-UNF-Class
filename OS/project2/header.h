#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAXLINE 180;

int returnNumOfLines(FILE* file);

struct {
      char id[26]; 
      int value;
    } TABLE[50];


int returnNumOfLines(FILE* file){
char* string;
int x = 0;

while((fscanf(file,"%[^\n]\n", string) == 1)){

x++;
}
return x;
}