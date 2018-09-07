#define _GNU_SOURCE
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

pthread_t sig_thread;


struct sigaction sigvar;

void sighandler(int signum)
{
	if (signum == SIGUSR1)
   		{
   			printf("\nReceived SIGUSR1\nExiting Signal Handler\n");
   		}
}

void *threadfunction(void *args)
{	
	struct timespec delaytime,current;

	delaytime.tv_sec = 3;
	delaytime.tv_nsec = 0;
	printf("\nThread initialised and going to sleep for 3s\n");
	nanosleep(&delaytime,&current);
	printf("\nInside thread: Shouldn't print this before \"Exiting Signal Handler\"\nRemaining time is %lds and %ldns \nExiting thread\n",current.tv_sec,current.tv_nsec);
	pthread_exit(0);
}

int main()
{	
	cpu_set_t cpuset;                                                        // Making program run on a single core even if trace cmd is not used to do so
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);
    pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);

	memset(&sigvar,0,sizeof(sigvar));
	sigvar.sa_handler = &sighandler;                                         // Adding handler in structure

	if(sigaction(SIGUSR1,&sigvar,NULL) == -1)                                // Registering handler 
		printf("error occured"); 
	
	printf("Creating thread\n"); 	
	pthread_create(&sig_thread,NULL,&threadfunction,NULL);                   // creating thread which will sleep 
	pthread_setaffinity_np(sig_thread,sizeof(cpu_set_t),&cpuset);             
	
	printf("\nMain sleeping temporarily to let thread initialise\n");

	usleep(20000);

	printf("\nSIGUSR1 signal sent by main while thread sleeping\n");

	pthread_kill(sig_thread,SIGUSR1);                                       //Sending thread directed signal SIGUSR1 to sig_thread

	pthread_join(sig_thread, NULL);

	printf("\nExiting program\n");
	return 0;
}

