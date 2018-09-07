# Event-Handling-and-Signaling
NAME: CSE 438 Assignment-4
_______________________________________________________________________________________________________________________________________________________________________________________________________

AUTHORS:  Team-1

Vishwakumar Dhaneshbhai Doshi (1213322381)
Nisarg Trivedi (1213314867)
_______________________________________________________________________________________________________________________________________________________________________________________________________

INCLUDED IN REPOSITORY:

There will be three sub-folders in the downloaded zip file:

1) Task 1
-> task1.c (Program Source code)
-> Makefile

2) Task 2
-> task2_1.c (Program Source code)
-> task2_2.c (Program Source code)
-> task2_3.c (Program Source code)
-> Makefile2_1
-> Makefile2_2
-> Makefile2_3

3) Task 3
-> task3_1.c (Program Source code)
-> task3_2.c (Program Source code)
-> Makefile3_1
-> Makefile3_2

-> README
_______________________________________________________________________________________________________________________________________________________________________________________________________

ABOUT: 

This project mainly is about three basic concepts:
1. To learn the basic programming technique for input devices in Linux kernel.
2. To learn event handling and signaling in Linux
3. To develop program patterns for imprecise comp
______________________________________________________________________________________________________________________________________________________________________________________________________

SYSTEM REQUIREMENTS:

-> Linux machine for host. Compilation must be done on this machine and not on the board.
-> LINUX KERNEL : Minimum version of 2.6.19 is expected.
-> USB mouse
_______________________________________________________________________________________________________________________________________________________________________________________________________

SETUP:

-> If your SDK is installed in other location than default one, change the path accordingly in makefile.
_______________________________________________________________________________________________________________________________________________________________________________________________________

COMPILATION:

-> On the host, extract all files in zip file in one directory, open directory in terminal in which files are present and type make.
_______________________________________________________________________________________________________________________________________________________________________________________________________

EXECUTION:

-> type ./main to run the code
_______________________________________________________________________________________________________________________________________________________________________________________________________

DEPLOYMENT and EXPECTED OUTPUT:

1) Deployment for Task_1

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> Makefile

-> Mouse events are detected through /dev/input/eventX device file. Here header is written for /dev/input/event4 file. Check on which file on your board & change in task1.c accordingly.

-> On Host, ensure that 666(rw- rw- rw-) file permissions exist for /dev/input/event2.

-> You can check by the following command ls -lrt /dev/input/event2. 

-> Otherwise change using the following command chmod 666 /dev/input/event2

-> Once above completed then run the below command to execute the program code
   
-> ./task1

Expected results

-> Prints the time after for every one second and terminates after 60 seconds if no mouse event is detected and if a double click occurs under 60 seconds it terminates the program


2.1) Deployment for Task2_1

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> make

-> Once above completed then run the below command to execute the program code
   
-> ./task2_1

Expected results

->  Main thread's priority , scheduling policy is set and a thread is created whose priority is set lower than that of the main thread now a signal is generated in main thread which will happen only after the main thread completes its process and then kills the runnable thread.
     
   
2.2) Deployment for Task2_2

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> make
   
-> Once above completed then run the below command to execute the program code
  
-> ./task2_2

Expected results
   
-> A thread is a created and a semaphore is intialized, now when the  signal is sent the thread it is killed and program exits.


2.3) Deployment for Task2_3

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> make

-> Once above completed then run the below command to execute the program code
   
-> ./task2_3

Expected results

-> A thread is created and nanaosleep is given , now when the signal is sent to the thread it is killed and program exits.

   
3.1) Deployment for Task3_1

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> make

-> Once above completed then run the below command to execute the program code
   
-> ./task3_1

Expected results

->Intially five threads are created , Now a signal is generated which is sent  one of the five threads randomly and kills it.

	
 
3.2) Deployment for Task3_2

-> Open the terminal & go to directory where files in installing part have been downloaded. [cd <Directory name>] 

-> Use below command to to compile 
 
-> make

-> Once above completed then run the below command to execute the program code
   
-> ./task3_2

Expected results

  
-> Intially five threads are created out of which three are unblocked and two are blocked, also the main thread is blocked, Now on arrival of the signal one of the unblocked thread is picked up randomly and killed after which remaining two unblocked threads are killed
