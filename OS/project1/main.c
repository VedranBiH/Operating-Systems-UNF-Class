
#include "header.h"


main(int argc, char* argv[]){

struct sigaction blah1 = {0};
blah1.sa_handler = signal1;

sigaction(SIGUSR1,&blah1,0);

struct sigaction blah2 = {0};
blah2.sa_handler = signal2;
sigaction(SIGUSR2,&blah2, 0);

time_t now;
int status;
int o;
int input = 0;

FILE* data;
FILE* logData;
FILE* proc1;
FILE* proc2;

int pipe1[2];
int pipe2[2];
int pipe3[2];

char readbuffer[501];
int nbytes;


logData = fopen(argv[2],"a");


pipe(pipe1);
pipe(pipe2);
pipe(pipe3);



//store manager
int x = fork();
if(x == 0){
int i,y;
char* format;
char* id;
char* fcn;
char* proc;
char* pid;
char* numOfItems;

data = fopen(argv[1],"r");
close(pipe3[1]);

i = 0;
while((fscanf(data,"%s %d",TABLE[i].id, &TABLE[i].value) == 2)){
i++;
}
fclose(data);
y = 0;
//change to infinite when interrupts are working
while(1){

time(&now);

nbytes = read(pipe3[0],readbuffer,sizeof(readbuffer));

id = strtok(readbuffer," ");

if(strcmp(id,"R") == 0){
fcn = strtok(NULL," ");
proc = strtok(NULL," ");
pid = strtok(NULL," ");
}
else if(strcmp(id,"U") == 0){
fcn = strtok(NULL," ");
numOfItems = strtok(NULL," ");
proc = strtok(NULL," ");
pid = strtok(NULL," ");
}


if(strcmp(id,"R") == 0){
	logData = fopen(argv[2],"a");
int uq  = fprintf(logData,"Store Manager at time: %.24s received message: %s %s %s %s\n\n",
ctime(&now), proc, pid, id, fcn);
fclose(logData);

int z;

for(z = 0; z < i; z++){
time(&now);
if(strcmp(fcn,TABLE[z].id) == 0){
	logData = fopen(argv[2],"a");
int pq = fprintf(logData,"Store Manager at time: %.24s sent message: R 0 %s %d\n\n",ctime(&now),TABLE[z].id,TABLE[z].value);
fclose(logData);
if(strcmp(proc,"1") == 0)
{
				char send[255];
				strcpy(send,proc);
				write(pipe1[1], send,(strlen(send)+2));
}//end if
if(strcmp(proc,"2") == 0)
{
				char send[255];
				strcpy(send,proc);
				write(pipe2[1], send,(strlen(send)+2));
}//end if
break;

}
else{
	logData = fopen(argv[2],"a");
int xs = fprintf(logData,"Store Manager at time: %.24s sent message: R 1 %s 0\n\n",ctime(&now),TABLE[z].id);
fclose(logData);
}//end else

}//end for loop
z = 0;
}//end if

else if(strcmp(id,"U")== 0){

logData = fopen(argv[2],"a");
int wq = fprintf(logData,"Store Manager at time: %.24s received message: %s %s %s %s %s\n\n",ctime(&now),proc, pid, id, fcn,numOfItems);
fclose(logData);

	
int b;
for(b = 0; b < i; b++){
time(&now);
	if(strcmp(fcn,TABLE[b].id)==0){
		int num1 = atoi(numOfItems);
		TABLE[b].value = TABLE[b].value + num1;
		logData = fopen(argv[2],"a");
		int tu = fprintf(logData,"Store Manager at time: %.24s sent message R 1 %s %d\n\n",ctime(&now),TABLE[b].id,TABLE[b].value);
		fclose(logData);
		
			if(strcmp(proc,"1") == 0){
				char send[255];
				strcpy(send,proc);
				write(pipe1[1], send,(strlen(send)+2));
		}
			if(strcmp(proc,"2")==0)
			{
				char send[255];
				strcpy(send,proc);
				write(pipe2[1], send,(strlen(send)+2));
			}

break;
	}//end if

	else{
		logData = fopen(argv[2],"a");
		int oz = fprintf(logData,"Store manager at time: %.24s sent message R 0 %s 0\n\n",ctime(&now),TABLE[b].id);
		fclose(logData);

			}//end else 

		
	}//end for
b = 0;
}//end else if

else{
printf("File does not contain a U or R\n");
}

y++;
}//end while

//fclose(logData);

}//end if

/*                           store1                       */
int m = fork();
if(m == 0){
	processNum = 1;
	int get =0;
	char pid[64];
	snprintf(pid,10,"%d",(int)getpid());
	proc1 = fopen(argv[3],"r");
	char string1[501] = {0};
	close(pipe3[0]);



while(fgets(string1, 501, proc1) != NULL){
time(&now);
logData = fopen(argv[2],"a");
fprintf(logData,"Process 1 at time: %.24s sent message: 1 %d %s\n", ctime(&now),getpid(), string1); 
fclose(logData);
strtok(string1, "\n");	
strcat(string1," 1 ");
strcat(string1,pid);
write(pipe3[1],string1,(strlen(string1)+1));
total++;
char readbuf[80];
int n = read(pipe1[0],readbuf,sizeof(readbuf));
if(strcmp(readbuf,"1")== 0)
succeed++;
sleep(2);
}
fclose(proc1);
while(1);
}//end if

/*                                 store2    */
int v  = fork();
if(v == 0)
{
	processNum = 2;
proc2 = fopen(argv[4],"r");
char string[501];
char pid1[64];
close(pipe3[0]);
snprintf(pid1,10,"%d",(int)getpid());

while(fgets(string, 501, proc2) != NULL){
time(&now);
logData = fopen(argv[2],"a");
int puttingFiles = fprintf(logData,"Process 2 at time: %.24s sent message: 2 %d %s\n", ctime(&now),getpid(), string);
fclose(logData);
strtok(string,"\n");
strcat(string," 2 ");
strcat(string, pid1);
write(pipe3[1],string,(strlen(string)+1));
total++;
char readbuf[80];
int n = read(pipe2[0],readbuf,sizeof(readbuf));
if(strcmp(readbuf,"2")==0){
	succeed++;
}
sleep(2);
}
fclose(proc2);
while(1){};
}//end if


while(1){

printf("\nPlease enter for 1 for info on store 1\n");
printf("Please enter for 2 for info on store 2\n");
printf("Enter 3 to close all processes\n");
scanf("%d",&input);
if(input == 1){
	 kill(m,SIGUSR1);
}

else if(input == 2)
kill(v,SIGUSR2);

else if(input == 3){


kill(x,SIGKILL);

kill(v,SIGKILL);

 kill(m,SIGKILL);
break;
	}
	else{
		printf("Wrong input\n");
		break;
	}
}
/*while(wait(NULL) > 0){
if(errno = ECHILD){
break;
}
}
*/
}//end main

