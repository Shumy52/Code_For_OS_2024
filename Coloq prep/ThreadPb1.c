#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

sem_t semRestaurant; //types usually are of the form "something_t"
// pthread_mutex_t waiterHandshake = PTHREAD_MUTEX_INITIALIZER;
sem_t semWaiterIdle;
sem_t semWaiter;
sem_t semClient;
sem_t semServing;
sem_t semPayment;

void* person(int id)
{
    //waits for space in the restaurant (P)
    sem_wait(&semRestaurant);
    printf("Client %d in the haus\n", id);

    //enters and waits the waiter to be available
    //Sounds like a handshake mutex to me
    // int status=pthread_mutex_lock(&waiterHandshake);
    // if(status<0){
    //     perror("Waiter handshake fuckup");
    //     exit(0);
    // }
        //But it's better with a simple semaphore, honestly
    printf("Waiting... %d\n", id);
    sem_post(&semWaiterIdle); //Someone's here!
    sem_wait(&semWaiter);
    
    sleep(1);
    //gives the order(sleep)

    printf("Cook me something fierce!\n");    
    sem_post(&semClient);//signal that he's done waiting for an order

    printf("Waiting...\n");
    sem_wait(&semServing);
    //waits for the waiter to bring the order

    sleep(1);
    sem_post(&semPayment);
    //pays(sleep)

    //let the waiter go
        //Do I need to do anything special?

    //eat(sleep)

    //leaves
    sem_post(&semRestaurant);//exit the restaurant


}

void* waiter()
{
    while(1){
        sem_post(&semWaiter);//waiter available!
        
    //waits for a person to enter
    sem_wait(&semWaiterIdle);//but nobody's here...

    //waits for them to give an order
    sem_wait(&semClient);

    sleep(1);//cooking up something fierce

    sem_post(&semServing);
    //brings the order
    
    
    sem_wait(&semPayment);
    //waits for the person to pay

    }


}

    int P;
    int N;

int main(int argc, char* argv[])
{
    N=atoi(argv[1]);
    P=atoi(argv[2]); //this could be redesigned in a for through all the argc

    int status=sem_init(&semRestaurant, 0, P);
    if(status<0){
        perror("Semaphore print fuckup");
        exit(-1);
    }

    status=sem_init(&semWaiter, 0, 0);//want the waiter to be first
    if(status<0){
        perror("Semaphore print fuckup");
        exit(-1);
    }
    status=sem_init(&semClient, 0, 0);//want the client to be first... kinda
    if(status<0){
        perror("Semaphore print fuckup");
        exit(-1);
    }

    int* args=malloc(N*sizeof(int));
    pthread_t* thread=malloc(N*sizeof(pthread_t));
    for(int i=1; i<=N; i++){
        args[i]=i;
        pthread_create(&thread[i], NULL, person, &args[i]);
    }

    for(int i=1; i<=N; i++)
        pthread_join(thread[i], NULL);

    //Waiter
    pthread_t waiterThread;
    pthread_create(&waiterThread, NULL, person, NULL);


    
}