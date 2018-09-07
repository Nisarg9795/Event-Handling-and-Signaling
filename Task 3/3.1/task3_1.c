#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


sigset_t m_mask;                        //main mask
sigset_t t_mask;                        // thread mask
 

pthread_t tid[5] = {0};

int flag = 0;

void sig_func(int signum)             // signal handler
{
	if (signum == SIGIO)
	{
		printf("=========================================================\n");

		if (pthread_equal(pthread_self(),tid[0]))
		{
			printf("SIGIO received!\nEntering Thread 1 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[1]))
		{
			printf("SIGIO received!\nEntering Thread 2 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[2]))
		{
			printf("SIGIO received!\nEntering Thread 3 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[3]))
		{
			printf("SIGIO received!\nEntering Thread 4 signal handler\n");
		}
		else if (pthread_equal(pthread_self(),tid[4]))
		{
			printf("SIGIO received!\nEntering Thread 5 signal handler\n");
		}
		else
		{
			printf("Error signal handling\n");
		}
	}

}

void* t_func1(void* arg)                              // thread function
{
	printf("\nThread 1 waiting for signal\n");
	usleep(50);
	sigsuspend(&t_mask);                               // Suspends execution till it received a signal and mask is specified by mask
	printf("\nSignal caught by Thread 1\n");
	printf("\nThread 1 PID : %u\n", (unsigned int)pthread_self());
	flag=1;                                             // When flag is 1 main returns
	
	return NULL;
}


void* t_func2(void* arg)
{
	printf("\nThread 2 waiting for signal\n");
	usleep(50);
	sigsuspend(&t_mask);
	printf("\nSignal caught by Thread 2\n");
	printf("\nThread 2 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

void* t_func3(void* arg)
{
	printf("\nThread 3 waiting for signal\n");
	usleep(50);
	sigsuspend(&t_mask);
	printf("\nSignal caught by Thread 3\n");
	printf("\nThread 3 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

void* t_func4(void* arg)
{
	printf("\nThread 4 waiting for signal\n");
	usleep(50);
	sigsuspend(&t_mask);
	printf("\nSignal caught by Thread 4\n");
	printf("\nThread 4 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}

void* t_func5(void* arg)
{
	printf("\nThread 5 waiting for signal\n");
	usleep(50);
	sigsuspend(&t_mask);
	printf("\nSignal caught by Thread 5\n");
	printf("\nThread 5 PID : %u\n", (unsigned int)pthread_self());
	flag=1;
	
	return NULL;
}


int main()
{
	
		pthread_create(&tid[0],NULL,&t_func1,NULL);      //Creating threads
		usleep(1);
		pthread_create(&tid[1],NULL,&t_func2,NULL);
		usleep(1);
		pthread_create(&tid[2],NULL,&t_func3,NULL);
		usleep(1);
		pthread_create(&tid[3],NULL,&t_func4,NULL);
		usleep(1);
		pthread_create(&tid[4],NULL,&t_func5,NULL);
		usleep(1);
	
	sleep(3);											// sleeping

	
	struct sigaction sig_act;

	sig_act.sa_handler = &sig_func;                    // Adding handler pointer to structure

	sigemptyset(&sig_act.sa_mask);                     // setting hander mask

	sigaction(SIGIO, &sig_act, NULL);

	sigemptyset(&m_mask);                             // setting mask for main 
	sigaddset(&m_mask, SIGIO);
	
	sigfillset(&t_mask);                              // setting thread mask
	sigdelset(&t_mask, SIGIO);

	if(pthread_sigmask(SIG_BLOCK, &m_mask, NULL) != 0)  // Adding mask for main
	{
		printf("Error while masking signal in Main\n");
	}
	
	kill(0,SIGIO);                                    // Sending process directed signal SIGIO 
	usleep(1000);
	while(flag == 0);                                 // Waits for flag
	return 0;
}

