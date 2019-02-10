#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <errno.h>

int semid;

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

		
    	struct sembuf sharedMemDec = {0, -1, 0}; /* set to decrement */
    	struct sembuf sharedMemInc = {0, 1, 0};  /* set to increment */
    	struct sembuf isEmptyDec = {1, -1, 0};  /* set to decrement */
    	struct sembuf isEmptyInc = {1, 1, 0};  /* set to increment */
    	struct sembuf isFullDec = {2, -1, 0};  /* set to decrement */
    	struct sembuf isFullInc = {2, 1, 0};     /* set to increment */
    	struct sembuf arg;