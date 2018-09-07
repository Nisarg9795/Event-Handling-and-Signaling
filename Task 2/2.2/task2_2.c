#define _GNU_SOURCE
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<semaphore.h>

sem_t var;                                       // Semaphore variable
pthread_t sig_thread;                            // Thread id

struct sigaction sigvar;                         // Sigaction structure to register signal handler

void sighandler(int signum)                      // Signal handler function
{
	if (signum == SIGUSR1)                       
   		{
   			printf("\nReceived SIGUSR1!\nHandler posting Semaphore\n");
   			sem_post(&var);                                               // Posting Semaphore to let thread return
   			printf("Exiting Signal Handler\n");
   		}
}

void *threadfunction(void *args)                       // thread function
{	
	printf("\nThread is Waiting for semaphore\n");
	sem_wait(&var);                                     // thread will wait and be blocked and main comes back
	printf("\nInside thread: Shouldn't print this before \"Exiting Signal Handler\"\n");
	printf("\nExiting thread\n");
	pthread_exit(0);
}

int main()
{
	cpu_set_t cpuset;                                                        // Making program run on a single core even if trace cmd is not used to do so
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);
    pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);

	sem_init(&var,0,0);														// Initialising semaphore with value 0 so the very 1st wait call blocks

	memset(&sigvar,0,sizeof(sigvar));
	sigvar.sa_handler = &sighandler;                                        // Adding handler to structure

	if(sigaction(SIGUSR1,&sigvar,NULL) == -1)                               // Registering handler
		printf("error occured"); 

	printf("Creating thread\n");
	pthread_create(&sig_thread,NULL,&threadfunction,NULL);
	pthread_setaffinity_np(sig_thread,sizeof(cpu_set_t),&cpuset);

	printf("\nMain Sleeping temporarily to allow thread to call sem_wait\n");
	usleep(1000);                                                     // Sleeping to let thread run and main will run again the moment thread waits for semaphore
	printf("\nSIGUSR1 signal sent by main\n");
	pthread_kill(sig_thread,SIGUSR1);                                 //Sending thread directed SIGUSR1 signal to sig_thread
	pthread_join(sig_thread, NULL);
	printf("\nExiting program\n");
	return 0;
}

