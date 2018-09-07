#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <string.h>
#include <linux/input.h>

#define NANOSECONDS_IN_500ms 500000000ul // converting 0.5 seconds to nanoseconds

int fd;
const char *mdevice = "/dev/input/event7"; // Decive file path to read mice events
struct input_event ie;                     // input structure to get mouse event

struct sigaction sig_struc;                // structure used to register handler
jmp_buf spot;                              // stores context

void* mclick(void* arg)                   // Thread function
{
    struct timespec curtime,prevtime; //Creates 2 objects of type timespec; defined in <time.h>
    int flag = 0;                     // Initialise flag as 0

    
    while(read(fd, &ie, sizeof(struct input_event)))   //Constantly checks for right mouse click
    {  
        if((ie.code == BTN_RIGHT) && (ie.value == 0))  // Sets flag 1 on very 1st right click of program
        {
            if (flag == 0)                             
            {
              clock_gettime(CLOCK_MONOTONIC, &prevtime); //Notes time to compare with next click
              flag = 1;
            }
            else
            {
                clock_gettime(CLOCK_MONOTONIC, &curtime);  // Gets new time for second click
                if(((curtime.tv_sec - prevtime.tv_sec) == 0) && ((curtime.tv_nsec - prevtime.tv_nsec) < NANOSECONDS_IN_500ms)) // Checks if this click arrived in 0.5s
                {
                   printf("\n\nDouble right click detected\n\n");
                   kill(0,SIGUSR1);                                // Sends signal SIGUSR1 to program on double right click
                   printf("SIGUSR1 signal sent\n\n");
                   break;
                }
                else
                  prevtime = curtime;                              // if not within 0.5s, current time becomes new previous time
            }        
        }
       
    }
    printf("Returning from thread\n\n");
    return NULL;
}


void sig_handler(int sig_num)
{
   printf("Entering Signal Handler\n\n");
   longjmp(spot,5);  // Returns buffer value as 5
}


int main()
{	
	int retjmp,tret,newnum,count = 1,sum = 0;  // sum and count are used in main work to be done before double right click
	pthread_t mid;

    sig_struc.sa_handler = &sig_handler;     // Adds handler pointer to structure

    if(sigaction(SIGUSR1, &sig_struc, NULL)== -1)
      printf("Error in signal registration\n");
    
    if((fd = open(mdevice, O_RDONLY)) < 0)
    {
      perror("opening device");
    }

    
    tret = pthread_create(&mid,NULL,mclick,NULL);       // Creates thread which detects double rightclick
    if (tret)
    {
      printf("ERROR. Return code from pthread_create() is %d\n", tret);
      exit(-1);
    }
    
    retjmp = setjmp(spot);  // Initial buffer value is set as 0. longjmp will return here
    if(retjmp == 0)
    {
        // Loop to continuously generate random numbers between 0-100 every second and in the end prints average of it

      printf("\nPrinting sequence of Random numbers between 0-100 :\n\n");
      printf("Double Right-click to abort and print average!\n\n");

        while(1)
        {
          newnum = rand()%101;                // New generated random number
          sum += newnum;                      // Adds it to the running sum
          printf("\nNumber%d : %d\n",count,newnum); 
          count++;                            // Keeps a track of number of iterations
          sleep(1);                           // Sleeps for 1s
        }
    }
    else if(retjmp == 5)                      // retjmp is 5 when it returns after longjmp. Program terminates after printing average
    {
        count--;
        printf("===============================\n\n");
        printf("Average of %d numbers is : %d\n\n\n",count,(sum/count));
    }
    
    return 0;
}

