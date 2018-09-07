#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


int i=0,len=0;
pthread_t tid_arr[5]={0};

void register_tid(pthread_t new_tid)                               // Registers thread for consideration to send signal
{
	tid_arr[i]=new_tid;
	i++;
}

void signal_tid(pthread_t sig_tid)                               // Sends signal to all registered threads
{
	for(int j=0; j<5; j++)
	{
		if(tid_arr[j]!=0 && tid_arr[j]!=sig_tid)
		{
			printf("Sending kill signal for : %d\n", j+1);
			pthread_kill(tid_arr[j],SIGIO);
		}
	}
}