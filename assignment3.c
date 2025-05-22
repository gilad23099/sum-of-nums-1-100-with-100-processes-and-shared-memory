#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>

void sem_wait(struct sembuf* sops,int sem_num);
void sem_signal(struct sembuf* sops,int sem_num);

union semun
{
    int val;
    struct semid_ds * buff;
    unsigned short *array;
    struct seminfo *_buf;
};

int *shareInt;
int semid,shmid, s ,i;

union semun semarg,semarg2;

struct sembuf sops[1]; 


void main()
{
       semid = semget ( IPC_PRIVATE , 1 , 0600 ); // 1 - create only one semaphore
       semarg.val = 1; // semaphore is unlocked
       semctl ( semid , 0 , SETVAL , semarg ); // 0 - only one semaphore . SETVAL - the command

       shmid = shmget (IPC_PRIVATE ,sizeof(int), 0600 ); // the size of the shared memory is the size of an integer
       shareInt = (int *)shmat(shmid , NULL , 0);
       *shareInt = 0; // Initialized the shared memory value to 0
       shmdt(shareInt);
       
       for(i = 0 ; i < 100 ; i++){
             
             pid_t pid = fork();
             
             if(pid == -1){
                printf("fork failed");
                exit(1);
             }
             else if(pid == 0){
                 
                 int my_index = i; // ensure each process get unique index
                 printf ("child : try to lock the semapore \n");
                 //try locking the semaphore
                 sem_wait(sops,0);
		 int val = semctl(semid,0,GETVAL,semarg);
                 printf ("child : semapore is locked now. the semaphore value is %d\n",val);
		 
		 // attach the memory, add proccess index to shareInt, dettach memory
		 shareInt=(int *)shmat(shmid , NULL , 0600 );
		 *shareInt += my_index+1;
		 printf("proccess number: %d , num value: %d \n",i,*shareInt);
                 shmdt ( shareInt );
                 
                 // unlocking semaphore
                 sem_signal(sops,0);
                 int val2 = semctl(semid,0,GETVAL,semarg);
             
                 printf ("child : semapore is unlocked now. the semaphore value is %d\n",val2);
             
                 printf("Child finished !\n");
                 
                 exit(0);
                 
             }
       
       }
       
       // wait until all 100 process terminates
       for(i = 0 ; i < 100 ; i++){
           wait(NULL);
       }
       
       // print final result of shareInt
       shareInt=(int *)shmat(shmid , NULL , 0600 );
       printf("\nsharedInt value at the end: %d \n",*shareInt);
       shmdt ( shareInt );
       
       //delete semaphore and shared memory
       printf("delete the semaphore\n");
       semctl ( semid , 0 , IPC_RMID );
       printf("delete the shared memory\n");
       shmctl ( shmid , IPC_RMID , 0 );
       
       
}



// wait on a semaphore
void sem_wait(struct sembuf* sops,int sem_num){
   
   sops->sem_op = -1; // decrease the sem. from 1 to 0
   sops->sem_num = sem_num;
   sops->sem_flg = 0;
   semop ( semid , sops , 1 ); // operate the decrease
   
}

// signal a semaphore
void sem_signal(struct sembuf* sops,int sem_num){
   
   sops->sem_op = 1; // increase the sem. from 0 to 1
   sops->sem_num = sem_num;
   sops->sem_flg = 0;
   semop ( semid , sops , 1 ); // operate the increase
   
}








