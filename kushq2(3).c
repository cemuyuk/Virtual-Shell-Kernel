/**
 * KUSH shell interface program
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

#define MAX_LINE       80 /* 80 chars per line, per command, should be enough. */

int parseCommand(char inputBuffer[], char *args[],int *background);

int main(void)
{
  char inputBuffer[MAX_LINE]; 	        /* buffer to hold the command entered */
  int background;             	        /* equals 1 if a command is followed by '&' */
  char *args[MAX_LINE/2 + 1];	        /* command line (of 80) has max of 40 arguments */
  pid_t child;            		/* process id of the child process */
  int status;           		/* result from execv system call*/
  int shouldrun = 1;
  int i;
  int stdoutsave; //saved stdout
  FILE *file; //for part 3
  char a1[2]; //for part 3
  char a2[2]; //for part 3
  int j=1; //num of commands
  int k; //for loop
  time_t t1;
  time_t t2;
  struct tm * ti1;
  struct tm * ti2;
  char *temp; //for holding the initial time
  int w; //waiting time in own command
  char g; //game selection in own command
  time(&t1);
  ti1 = localtime(&t1);
  temp = asctime(ti1);
  char *temp1;
  char *temp2;
  while (shouldrun){            		/* Program terminates normally inside setup */
    background = 0;
    shouldrun = parseCommand(inputBuffer,args,&background);       /* get next command */
		
		
    if (strncmp(inputBuffer, "exit", 4) == 0)
      shouldrun = 0;     /* Exiting from kush*/
	if (strncmp(args[0], "cd", 2)==0){ //if our command is cd
		if(chdir(args[1])==0){ //if directory successfully changed
		setenv("PWD", args[1], 1); //set the pwd variable to new directory
		}else{
			printf("No such directory.\n");
		}
		

	}
    if (shouldrun) {
	
		child = fork();
		if(child<0){ // fork cant happen 
			printf("Fork didnt work.");
		} else if( child==0){ //Child
			stdoutsave=dup(STDOUT_FILENO); //part2
			if(strncmp(args[3], ">", 1)==0 || strncmp(args[3], ">>", 2)==0 ){ //part2
				args[3] = NULL; //part2
				freopen(args[4], "a", stdout);	//part2
				args[4]=NULL; //part2
		}
	
			
			if(strncmp(args[0],"ls", 2)==0){ // this is to get the path of ls
				args[0]= "/bin/ls";
			}
			if(strncmp(args[0], "clear", 5)==0){ // getting path of clear
				args[0]= "/usr/bin/clear";
			}
			if(strncmp(args[0], "env", 3)==0){ // getting path of env
				args[0]= "/usr/bin/env";
			}
			if(strncmp(args[0], "pwd", 3)==0){ // gettin path of pwd
				args[0]= "/bin/pwd";
			}
			if(strncmp(args[0], "echo", 4)==0){ //  getting path of echo
				args[0]= "/bin/echo";
			}
			if(strncmp(args[0], "cat", 3)==0){ // getting path of cat
				args[0]= "/bin/cat";
			}
			if(strncmp(args[0], "make", 4)==0){
				args[0] = "/usr/bin/make";
			}
			if(strncmp(args[0], "dmesg", 5)==0){
				args[0] = "/bin/dmesg";
			}
			if(strncmp(args[0], "sudo", 4) ==0){
				args[0] = "/usr/bin/sudo";
			}
			if(strncmp(args[0], "schedInfo", 9)==0){ //formatting schedInfo
				temp1 = args[3];
				temp2 = "PID=";
				strcat(temp2, args[1]);
				args[3] = temp2;
				temp2 = "Policy=";
				strcat(temp2, args[2]);
				args[4] = temp2;
				temp2 = "Priority=";
				strcat(temp2, temp1);
				args[5] = temp2;
				args[0] = "sudo";
				args[1] = "insmod";
				args[2] = "myfileinfo.ko";
				
				//args[3] = "PID=121";
				//args[4] = "Priority=80";
				//args[5] = "Policy=6";
				
				
			}
			// Implementation for part3
			if(strncmp(args[0], "trash", 5)==0 && strncmp(args[1], "-l",2)!=0 && strncmp(args[1], "-r",2)!=0){ //need to enter 4th argument(little bug in the code)
				// we couldnt find the error but if we type "trash 07.15 /path" the code fails
				// however when we add 4th argument like: "trash 07.15 /path a"
				// it works and job is added to the crontab.
				args[0]= "/usr/bin/crontab";
				a1[0] = args[1][3]; //getting the minute
				a1[1] = args[1][4]; //getting the minute
				a2[0] = args[1][0]; //getting the hour
				a2[1] = args[1][1]; //getting the hour
				args[3]=NULL; //setting other arguments to NULL
				//so that we will only have crontab tasks.txt at the end
			file = fopen("tasks.txt", "w"); //opens a file to hold crontab command
			fprintf(file, "%s %s * * * /bin/find %s -mtime +1 -exec rm -f {}\n", a1, a2, args[2]);
			//we found the above crontab from internet 
			//it deletes the files in the specified directory daily at the given time
			fclose(file);
			args[1] = "tasks.txt";
			args[2] = NULL;
			args[3] = NULL;

			} // to distinguish between trash and trash -l
			if(strncmp(args[0], "trash", 5)==0 && strncmp(args[1], "-l",2)==0){
				args[0] = "/usr/bin/crontab"; // trash -l
				
			}
			//to distinguish between trash and trash -r
			if(strncmp(args[0], "trash", 5)==0 && strncmp(args[1], "-r",2)==0){
				args[0] = "/usr/bin/crontab"; // trash -r
				
			}
			//our own command 
			if(strncmp(args[0], "info", 4)==0){
				//shows the number of commands entered
				printf("Commands entered in this session: %d\n", j);
				//shows when we started our shell
				printf("Program started at: %s", temp);
				//playing a game while waiting
				printf("Play a game while waiting 1/0: ");
				scanf("%d", &g);
				//enter time before kush becomes active again
				printf("Enter waiting time before next input: ");
				scanf("%d", &w);
				//w is the waiting time
				child = fork();
				if(child==0){
					execvp("gnome-mines", args);
					exit(0);
				} else {
					
				for(k=0; k<w; k++){
					//prints current time every second
					time(&t2);
				ti2 = localtime(&t2);
				printf("Current time: %s", asctime(ti2));
		
				
				//starting the game
				
				sleep(1);
				
			}
				kill(child, SIGTERM);
			}
			}
			//dup2(stdoutsave, STDOUT_FILENO); // part2
			// we couldnt redirect the output to
		//stdout so for part2 output redirection, comment the line above. Output
		//redirection works but redirection to stdout didnt work
		
				i=execv(args[0], args); //child executing		
					
			
				if(i<0){ //if invalid command
					if(strncmp(args[0], "cd", 2)!=0 && strncmp(args[0], "info", 4)!=0 ){ //if command is not cd
					printf("\x1B[31m%s: \x1B[0mcommand not found\n", inputBuffer);
				} //because we cant make child execute cd
					exit(1);
				}
		
		} else { //Parent
			if(background==0){ //for foreground processes
				j++; // counting the number of commands
			wait(0);
			
	
		} else { //for background processes
			printf("\x1B[32mBackground process\n\x1B[0m");
			sleep(1);
			waitpid(-1, &status, WNOHANG); //for making child background
		}
		
					
		}
		
      /*
	After reading user input, the steps are.
	(1) Fork a child process using fork()
	(2) the child process will invoke execv()
	(3) if command included &, parent will invoke wait()
       */
    }
  }
  return 0;
}

/** 
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */

int parseCommand(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct;       /* index of where to place the next parameter into args[] */
      
    
    ct = 0;
	
    /* read what the user enters on the command line */
    do {
	  printf("\x1B[32mkush>\x1B[0m"); // making the kush> green colored and rest normal
	  fflush(stdout);
	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
	  
	
    }
    while (inputBuffer[0] == '\n'); /* swallow newline characters */
	
    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string. 
     */    
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
    
    /** 
     * the <control><d> signal interrupted the read system call 
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value 
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }
    
    /**
     * Parse the contents of inputBuffer
     */
    
    for (i=0;i<length;i++) { 
      /* examine every character in the inputBuffer */
      
      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
	if(start != -1){
	  args[ct] = &inputBuffer[start];    /* set up pointer */
	  ct++;
	}
	inputBuffer[i] = '\0'; /* add a null char; make a C string */
	start = -1;
	break;
	
      case '\n':                 /* should be the final char examined */
	if (start != -1){
	  args[ct] = &inputBuffer[start];     
	  ct++;
	}
	inputBuffer[i] = '\0';
	args[ct] = NULL; /* no more arguments to this command */
	break;
	
      default :             /* some other character */
	if (start == -1)
	  start = i;
	if (inputBuffer[i] == '&') {
	  *background  = 1;
	  inputBuffer[i-1] = '\0';
	}
      } /* end of switch */
    }    /* end of for */
    
    /**
     * If we get &, don't enter it in the args array
     */
    
    if (*background)
      args[--ct] = NULL;
    
    args[ct] = NULL; /* just in case the input line was > 80 */
    
    return 1;
    
} /* end of parseCommand routine */


