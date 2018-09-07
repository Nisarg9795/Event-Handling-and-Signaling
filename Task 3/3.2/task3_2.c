#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "task3_2.h"


sigset_t b_mask;     
sigset_t unblocking_mask;                             //Signal mask

pthread_t tid[5] = {0};                              //thread ids

int flag=0, t_flag=0;                                // t_flag is to ensure only one thread picks up and flag holds main till thread return

void sig_func(int signum)                           // Signal handler function
{
	if (signum == SIGIO)
	{
		printf("=========================================================\n");

		if (pthread_equal(pthread_self(),tid[0]))
		{
			printf("\nSIGIO received!\nEntering Thread 1 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[1]))
		{
			printf("\nSIGIO received!\nEntering Thread 2 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[2]))
		{
			printf("\nSIGIO received!\nEntering Thread 3 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[3]))
		{
			printf("\nSIGIO received!\nEntering Thread 4 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[4]))
		{
			printf("\nSIGIO received!\nEntering Thread 5 signal handler\n");
		}
		else
		{
			printf("Error signal handling\n");
		}
	}

}

void* t_func1(void* arg)                                    
{
	sigset_t ub_mask1;
	sigfillset(&ub_mask1);                                       // Fills all signals in set
	sigdelset(&ub_mask1, SIGIO);
	printf("\nThread 1 waiting for signal\n");
	sigsuspend(&unblocking_mask);
	printf("\nSignal caught by Thread 1\n");
	printf("\nThread 1 PID : %u\n", (unsigned int)pthread_self()); //Getting thread id 
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());                              // sends signal to all registered 
	}
	flag=1;                                                     // When flag is 1 main returns
	
	return NULL;
}


void* t_func2(void* arg)
{
	sigset_t ub_mask2;

	sigfillset(&ub_mask2);
	sigdelset(&ub_mask2, SIGIO);
	printf("\nThread 2 waiting for signal\n");

	sigsuspend(&ub_mask2);
	printf("\nSignal caught by Thread 2\n");
	printf("\nThread 2 PID : %u\n", (unsigned int)pthread_self());
	pthread_kill(tid[3],SIGIO);
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}

void* t_func3(void* arg)
{
	sigset_t ub_mask3;
	sigfillset(&ub_mask3);
	sigdelset(&ub_mask3, SIGIO);
	printf("\nThread 3 waiting for signal\n");
	sigsuspend(&ub_mask3);
	printf("\nSignal caught by Thread 3\n");
	printf("\nThread 3 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag = 1;
		signal_tid(pthread_self());
	}
	
	flag=1;
	
	return NULL;
}

void* t_func4(void* arg)
{
	sigset_t b_mask4;
	sigemptyset(&b_mask4);
	sigaddset(&b_mask4, SIGIO);
	if(pthread_sigmask(SIG_BLOCK, &b_mask4, NULL) != 0)
	{
		printf("Error while masking signal in Main\n");
	}

	printf("\nThread 4 waiting for signal\n");
	sigsuspend(&b_mask4);
	printf("\nSignal caught by Thread 4\n");
	printf("\nThread 4 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}

void* t_func5(void* arg)
{
	sigset_t b_mask5;
	sigemptyset(&b_mask5);
	sigaddset(&b_mask5, SIGIO);
	
	if(pthread_sigmask(SIG_BLOCK, &b_mask5, NULL) != 0)
	{
		printf("Error while masking signal in Main\n");
	}

	printf("\nThread 5 waiting for signal\n");
	sigsuspend(&b_mask5);
	printf("\nSignal caught by Thread 5\n");
	printf("\nThread 5 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}

int main()
{
	printf("=======================================\n");
	printf("SIGIO remains Unblocked for threads 1,2 and 3\n");
	printf("SIGIO remains Blocked for threads 4 and 5\n");
	printf("=======================================\n");

	pthread_create(&tid[0],NULL,&t_func1,NULL);
	usleep(100);
	pthread_create(&tid[1],NULL,&t_func2,NULL);
	usleep(100);
	pthread_create(&tid[2],NULL,&t_func3,NULL);
	usleep(100);
	pthread_create(&tid[3],NULL,&t_func4,NULL);
	usleep(100);
	pthread_create(&tid[4],NULL,&t_func5,NULL);
	usleep(100);
    
 	register_tid(tid[0]);                            // Registers to send signal
	register_tid(tid[1]);
	register_tid(tid[2]);

	sleep(3);
	
	struct sigaction sig_act;

	sig_act.sa_handler = &sig_func;                     // Adding handler function

	sigemptyset(&sig_act.sa_mask);                      // Setting handler mask

	sigaction(SIGIO, &sig_act, NULL);                   // Registering structure

	sigemptyset(&b_mask);                              // mask for main thread
	sigaddset(&b_mask, SIGIO);
	
	if(pthread_sigmask(SIG_BLOCK, &b_mask, NULL) != 0)  // masking main
	{
		printf("Error while masking signal in Main\n");
	}
	
	kill(0,SIGIO);                                     // Sending process directed signal SIGIO
	while(flag == 0);                                  // Waits till flag is 1
	usleep(10000);
	printf("Terminating.\n");      
	return 0;
}

