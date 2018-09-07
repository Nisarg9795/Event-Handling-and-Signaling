#define _GNU_SOURCE
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<time.h>
#include<sched.h>

pthread_t thread1;                                       // Thread id
struct sched_param param;
volatile sig_atomic_t flag = 0;                         // safe variable that can be used as flag in signal handler

pthread_attr_t t;

struct sigaction sa;                                    // Sigaction structure used to register signal handling capabilities

void sighandler(int signum)                             // Signal handler function for the entire process
{
	if (signum == SIGUSR1)                              // Defining behavior for SIGUSR1 signal
   		{
   			printf("\nReceived SIGUSR1!\nExiting Signal Handler\n");
   			flag = 1;                                   // makes flag 1 to let thread 	
   		}
}

void *threadfunction(void *args)                        // thread function
{	
	printf("\nInside thread : Shouldn't print this before \"Exiting Signal Handler\"\n");   // Thread is preempted almost all the time due to low priority.
	if(flag == 1)																		 //so runs only after join
		printf("Flag changed to 1 by handler: successfully confirmed by thread\n");

	printf("\nExiting thread\n");
	pthread_exit(0);
}

int main()
{
	int ret;
	cpu_set_t cpuset;                                                        // Making program run on a single core even if trace cmd is not used to do so
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);
    pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t),&cpuset);

	memset(&sa,0,sizeof(sa));
	sa.sa_handler = &sighandler;                                             // Adding handler pointer in structure
	
	if(sigaction(SIGUSR1,&sa,NULL) == -1)                                    // Registering handler
		printf("error occured"); 

	param.sched_priority = 50;                                               // Setting priority of main more than that of thread
	ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	if(ret != 0)
		printf("Error in Main priority\n"); 

	printf("Main Priority set : 50\n");

	pthread_attr_init(&t);
    pthread_attr_setinheritsched(&t, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&t, SCHED_FIFO);
    param.sched_priority = 30;
    pthread_attr_setschedparam(&t,&param);
         
	pthread_create(&thread1,&t,&threadfunction,NULL);                      // Creating thread which will keep waiting because of low priority
	pthread_setaffinity_np(thread1,sizeof(cpu_set_t),&cpuset);
	printf("Thread created with Priority set: 30\n\n");

	
	printf("SIGUSR1 signal sent\n");
	pthread_kill(thread1,SIGUSR1);                                          // Sending thread directed signal SIGUSR1 to thread1

	printf("\nDoing some work in main\n");
	for(long i = 0; i< 99999999;i+=2)                                       // Keeping main busy to see handler cannot function till main gives away control
		i--;
	printf("Work done\nNow running pthread_join to let handler and thread return\n");

	pthread_join(thread1, NULL);                                            // Calling join so main allows thread and its handler to run and return
	printf("\nExiting program\n");
	return 0;
}

