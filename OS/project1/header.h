#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>



struct {
      char id[26]; 
      int value;
    } TABLE[50];

int succeed = 0;
int total = 0;
int processNum = 0;

void signal1(int sig)
{
	char str[400];
	int send = sprintf(str,"Proccess %d has succceded %d out of %d total jobs.\n",processNum, succeed, total);
	write(0,&str,strlen(str));
	return;
}


void signal2(int sig);

void signal2(int sig)
{
	char str[400];
	int send = sprintf(str,"Proccess %d has succeded %d times out of %d times.\n",processNum,succeed,total);
	write(0,&str,strlen(str));
	return;
}