
#include "header.h"

int randomNumber;

int main(int argc, char* argv[])
	{

        

        int* queue;
        int queueID;
        if((queueID = shmget(IPC_PRIVATE, 10*sizeof(int), 0777)) == -1){
            printf("Error in queue id");
            exit(1);

        }

        if((queue = (int *)shmat(queueID, (void *)0,0)) == (void *)-1){
            printf("Error in queue");
            exit(1);
        }

        int* head;
        int headID;

        if((headID = shmget(IPC_PRIVATE, sizeof(int), 0777)) == -1){
    
            printf("Error in head id");
            exit(1);
        }

        if((head = (int *)shmat(headID, (void *)0,0)) == (void*)-1){
        
            printf("Error in head");
            exit(1);

        }

        int* tail;
        int tailID;

        if((tailID = shmget(IPC_PRIVATE, sizeof(int), 0777)) == -1){

        printf("Error in tail id");
        exit(1);

        }

        if((tail = (int *)shmat(tailID, (void *)0,0)) == (void*)-1){
            exit(1);

        }

        int* totalProd;
        int totalProdID;

        if((totalProdID = shmget(IPC_PRIVATE,1*sizeof(int),0777)) == -1){
            printf("Error in totalProdID");
            exit(1);
        }

        if((totalProd = (int *)shmat(totalProdID,(void *)0,0)) == (void*)-1){
            exit(1);
        }

        int* totalCons;
        int totalConsID;

        if((totalConsID = shmget(IPC_PRIVATE,1*sizeof(int),0777)) == -1){
            printf("Error in totalProdID");
            exit(1);
        }

        if((totalCons = (int *)shmat(totalConsID,(void *)0,0)) == (void*)-1){
            exit(1);
        }

        int* remainder;
        int remainderID;

        if((remainderID = shmget(IPC_PRIVATE,sizeof(int),0777)) == -1){
            printf("Error in totalProdID");
            exit(1);
        }

        if((remainder = (int *)shmat(totalProdID,(void *)0,0)) == (void*)-1){
            exit(1);
        }




		union semun sem_val;
		int producers = atoi(argv[1]);
		int consumers = atoi(argv[2]);
		int numOfItems = atoi(argv[3]);
		key_t key = 69888800;

        int index;

       // srand(time(NULL));

        for(index = 0; index < 10; index++)
    {
        queue[index] = -1;
    }
    *head = 0;
    *tail = 0;
    *totalCons = 0;
    *totalProd = 0;

        semid = semget(key, 3, 0666 | IPC_CREAT);

		if (semid == -1) {
        printf("error creating semaphore \n");
        exit(1);
    	}

        //set the value to 0 to allow access
    	sem_val.val = 1;
    	semctl(semid,0, SETVAL, sem_val.val);

        //isempty
        sem_val.val = 0;
    	semctl(semid, 1, SETVAL, sem_val.val);

        //isfull
    	sem_val.val = 10;
    	semctl(semid, 2, SETVAL, sem_val.val);

        printf("pid    item   loc sem1 sem2  sem3    action  tot prod  tot con\n");


        srand(time(NULL));
        //printf("shared memory access %d", semctl(semid,0,GETVAL,arg.sem_num));
        //producer code
    	int i;
    	for(i = 0; i < producers; i++){
            
            //create producer processes		
    		int x = fork();
    		if(x == 0)
    		{

                randomNumber = rand()%100 + i;
           
    			int y;
                
                //store the number of products into the queue
    			for(y = 0; y < numOfItems; y++){

                            
                            semop(semid,&isFullDec,1);
    					   semop(semid,&sharedMemDec,1);
                           
                           
                           queue[(*tail)] = randomNumber;
                           (*tail)++;
                           (*totalProd)++;

                           printf("%d     %d     %d   %d   %d    %d    Producing     %d        %d\n", 
                            getpid(), randomNumber, *tail-1, semctl(semid, 0, GETVAL, 1), semctl(semid, 1, GETVAL, 1), 
                            semctl(semid, 2, GETVAL, 1), (*totalProd), (*totalCons));

                           if((*tail) == 10){

                            (*tail) = 0;

                           }


                           semop(semid,&isEmptyInc,1);
                           semop(semid,&sharedMemInc,1);
                           
                          // printf("Sem 2 is %d\n",semctl(semid, 1, GETVAL, 1));
                           
                           
                    }//end produce item loop

                //need this to destory the processes
    			exit(0);

    		}//end fork loop
            

    	}//end for producer loop

        //consumer code
        *remainder = (int)((producers*numOfItems)%consumers);
        int j;
        for(j = 0; j < consumers; j++){

            int extraNum; 
            if((*remainder)>0)
            {
                extraNum = 1;
                (*remainder)--;
            }
            else
            {
                extraNum = 0;
            }
            //create consumer processes
            int x = fork();
            if(x == 0){
                int y;
                for(y = 0; y < ((int)((producers * numOfItems)/consumers) + extraNum); y++){

                       
                                    semop(semid,&isEmptyDec, 1);
                                    //printf("Sem 2 is %d\n",semctl(semid, 1, GETVAL, 1));
                                    semop(semid,&sharedMemDec,1);

                                    int consumed;
                                    consumed = queue[*head];
                                    (*head)++;
                                    (*totalCons)++;

                                    printf("%d     %d     %d   %d   %d    %d    Consuming     %d        %d\n", 
                                    getpid(), consumed, *head-1, semctl(semid, 0, GETVAL, 1), semctl(semid, 1, GETVAL, 1), 
                                    semctl(semid, 2, GETVAL, 1), (*totalProd), (*totalCons));


                                    if((*head) == 10){
                                        (*head) = 0;
                                    }

                                    semop(semid,&isFullInc,1);
                                    semop(semid,&sharedMemInc,1);
                    }//end consume loop
                exit(0);
            }//end if(x==0)
            
        }//end fork consume for loop



    while ( wait(NULL) > 0)
    {
        if(errno == ECHILD)
        {
            break;
        }
    }

    printf("I am finsihed\n");

    if (shmctl(queueID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     } 

     if (shmctl(headID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     } 

     if (shmctl(tailID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     }

     if (shmctl(totalProdID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     }

     if (shmctl(totalConsID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     }

     if (shmctl(remainderID, IPC_RMID, 0) == -1) {
          printf("error in shmctl");
          exit(1);
     } 

        if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        printf("error in semctl");
        exit(1);
    	
	}

    return 0;
}





