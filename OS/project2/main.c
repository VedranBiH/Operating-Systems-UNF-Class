
#include "header.h"
int main(void){

FILE* data;
FILE* logData;
FILE* proc1;
FILE* proc2;

int pipe1[2];
int pipe2[2];
int pipe3[2];

char readbuffer[1080];
int nbytes;




//logData = fopen("LOG.DAT","a");


pipe(pipe1);
pipe(pipe2);
pipe(pipe3);



//store manager
int x = fork();
if(x == 0){
data = fopen("INIT.DAT","r");
close(pipe3[1]);

int i = 0;
while((fscanf(data,"%s %d",TABLE[i].id, &TABLE[i].value) == 2)){
i++;
}
fclose(data);

int y = 0;
//change to infinite when interrupts are working
while(y < 9){
nbytes = read(pipe3[0],readbuffer,sizeof(readbuffer));
printf("Receieved string %s\n",readbuffer);
y++;
}

exit(0);

}//end if

/*                           store1                       */
x = fork();
if(x == 0){
	char pid[64];
	snprintf(pid,10,"%d",(int)getpid());
	proc1 = fopen("TRANS1.DAT","r");
	char string1[501] = {0};
	close(pipe3[0]);



while(fgets(string1, 501, proc1) != NULL){
strtok(string1, "\n");	
strcat(string1," 1 ");
strcat(string1,pid);
write(pipe3[1],string1,(strlen(string1)+1));
sleep(2);
}
fclose(proc1);
exit(0);
}//end if

/*                                 store2    */
 x = fork();
if(x == 0)
{
proc2 = fopen("TRANS2.DAT","r");
char string[501];
char pid1[64];
close(pipe3[0]);
snprintf(pid1,10,"%d",(int)getpid());

while(fgets(string, 501, proc2) != NULL){
strtok(string,"\n");
strcat(string," 2 ");
strcat(string, pid1);
write(pipe3[1],string,(strlen(string)+1));
sleep(2);
}
fclose(proc2);
exit(0);
}//end if


/*while(wait(NULL)){
	if(errno == ECHILD)
		break;
}*/
//fclose(logData);
return 0;
}//end main