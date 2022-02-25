//author: pınar dilbaz
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "string.h"
#include "signal.h"
#include "unistd.h"
#include "fcntl.h"
#include <errno.h>
#include <math.h>
#include <time.h>


//signal function
/*
void sigParent(int signum){
  printf("Parent1 : Received a response signal from child \n");
}
void sigParent2(int signum){
  printf("Parent2 : Received a response signal from child \n");
}
*/
void sigChild(int signum){
  printf("CHILD1: signal is SIGUSR1 and process id is %d\n\n", getpid());
  sleep(1);
  //kill(getppid(),SIGUSR1);
}

void sigChild2(int signum){
  printf("CHILD2: signal is SIGUSR2 and process id is %d\n\n", getpid());
  sleep(1);
  //kill(getppid(),SIGUSR2);
}


//selection sort for process id is odd
double selectionSort(int *p, int size,int random)
{
    //printf("We are waiting for Selection Sort\n");
    clock_t starttime, endtime;
    double totaltime;
    int   i, j, temp, min;
    
    starttime = clock(); 
    
    for (i = 0; i < size - 1; i++) {
      min = i;
      for (j = i + 1; j < size; j++)
         if (p[min] > p[j])
            min = j;
      temp = p[min];
      p[min] = p[i];
      p[i] = temp;
   }
   
     endtime = clock(); 
     totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
     //printf("\n\nRANDOM NUMBER: %d\n\n", random);
     
     sleep(random);
     totaltime=totaltime+random;
     
     
     //printf("SELECTION SORT: %d",getpid());
     return totaltime;
}

//Insertion sort for process id is even
double insertionSort(int array[], int n,int random2) 
{ 
    //printf("We are waiting for Insertion Sort\n");
    int i, element, j; 
    clock_t starttime, endtime;
    double totaltime;
    starttime = clock(); 
    
    for (i = 1; i < n; i++) { element = array[i]; j = i - 1; while (j >= 0 && array[j] > element) { 
            array[j + 1] = array[j]; 
            j = j - 1; 
        } 
        array[j + 1] = element; 
    } 
    endtime = clock(); 
     totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
     
     
     //printf("\n\nRANDOM NUMBER: %d\n\n", random2);
     
     sleep(random2);
     totaltime=totaltime+random2;
     
     
     //printf("INSERTION SORT: %d",getpid());
     return totaltime;
   
} 
void sortExecutionTime(double array[], int n){
int i, j, swapped;
double temp;
for (i = 0; i < n; ++i)
{
    for (j = i + 1; j < n; ++j)
    {
        if (array[i] > array[j])
        {
            temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

}


int main(int argc, char** argv) {
    
    FILE *fptr;
    FILE *fptr2;
    FILE *file2;
    FILE *file3;
    FILE *filePipe;
    int control=0, i=0,arraySize=0,num_numbers=0;
    char fname[15] = "input";
    char str[20];
    char str2[]=".txt";
    char c; 
    int *arr = NULL;
    int size[2];
    int counter=1;
    int fileNumber,ch,value;
    pid_t pid[fileNumber];
    double executionTime;
    double pipe_executionTime;
    int random_number=0;
    int random_number2=0;
    int child_status;
    int *ex_arr=NULL;
    int choice ;
    int pipe_random=0,pipe_random2=0;
    int status;
    char Rsignal[20];
    char signal1[]="SIGUSR1";
    char signal2[]="SIGUSR2";
   //remove to OutputPipes file
   int result = remove("OutputPipes.txt");
       srand(time(NULL));
    	
    	printf("Welcome to Assignment1!\n");
    	printf("Please enter 1 for process with FILES\n");
    	printf("Please enter 2 for process with PIPES\n");
    	printf("Please enter 3 for process with QUIT\n");
    	printf("Enter your choice: ");
    	scanf("%d",&choice);
    	
    	//signal recieved
    	signal(SIGUSR1,sigChild);
    	signal(SIGUSR2,sigChild2);
    	
    	if (choice==3) return 0; //exit to system
    	if(choice==1){
    	
    	printf("Welcome to FILES PART!\n");
   	printf("How many file do we have: ");
    	scanf("%d",&fileNumber);
	
	//child status
	
	for(int i=1;i<=fileNumber;i++){
	int fd[2];
	//error in pipe
	if (pipe(fd) == -1)
        {
            printf("Error with the pipe.\n");
            return 1;
        }
	
	if ((pid[i]=fork()) == 0) {
	//close read
	close(fd[0]);
	//reset arraySize
	arraySize= 0;
	char fname[15] = "input";
	char fileName2[15] = "intermediate";
	char str[20];
        char str2[]=".txt";
        //for file name
	sprintf(str, "%d", counter);
	strcat(str,str2);
	strcat(fname, str);
	strcat(fileName2, str);
		
    //file read start
    fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        printf("Could not open file %s", fname);
        return 0;
    }
    
     while ((ch = fgetc(fptr)) != EOF && ch != '\n');
     while (fscanf(fptr, "%d", &value) == 1) {
        int *new_numbers = realloc(arr, (arraySize + 1) * sizeof(*new_numbers));
        if (!new_numbers) {
            fputs("Out of memory :(\n\n", stderr);
            free(arr);
            return EXIT_FAILURE;
        }
        arr = new_numbers;
        arr[arraySize++] = value;
    }

    fclose(fptr);
    //file read end
    
    random_number = rand() % 7 + 1;
    random_number2 = rand() % 7 + 1;
    
    for (int j = 0; j < arraySize; ++j) //printf("%d. num array is %d \n", i,arr[j]);
    
    if((getpid()%2)==1){
    executionTime=selectionSort(arr,arraySize,random_number);
    }
    else if((getpid()%2)==0){
    executionTime=insertionSort(arr,arraySize,random_number2);}
    
    //signal received
    
    /*
    signal(SIGUSR1,sigChild); // Register signal handler
    printf("Child: waiting for signal\n");
    pause();
    signal(SIGUSR2,sigChild2); // Register signal handler
    printf("Child: waiting for signal\n");
    pause();
    */
    /*
    if ((getpid()%2)==0){
    kill(getppid(),SIGUSR1);
    }
    else if((getpid()%2)==1){
    kill(getppid(),SIGUSR2);
    }
    */	
    
   //writing file start
   file2 = fopen (fileName2,"w");
 
   
   for(int x = 0; x < 4;x++){
   if(x==0){
   fprintf (file2, "<%d>\n",arraySize);
   }
   if(x==1){
   fprintf (file2, "<");
   for (int j = 0; j < arraySize; ++j) fprintf (file2,"%d ",arr[j]);
   fprintf (file2, ">");
   }
   if(x==2){
   
   fprintf (file2, "\n<%f>\n",executionTime);
   }
   if(x==3){
       if ((getpid()%2)==1){
         kill(getppid(), SIGUSR1);
         time_t recievedTime;
         struct tm *recievedT;
         time(&recievedTime);
         recievedT = localtime(&recievedTime);
         fprintf(file2, "<SIGUSR2> & %s", asctime(recievedT));
    }
      else if((getpid()%2)==0){
         kill(getppid(), SIGUSR2);
         time_t recievedTime;
         struct tm *recievedT;
         time(&recievedTime);
         recievedT = localtime(&recievedTime);
         fprintf(file2, "<SIGUSR1> & %s", asctime(recievedT));
                }
   //if ((getpid()%2)==1)fprintf (file2, "<SIGUSR1>");
   //else if((getpid()%2)==0)fprintf (file2, "<SIGUSR2>");
   }
       
   }
   fclose (file2);
   //writing file end
   
    exit(0);
  }  
  
   
   counter++;
   
} 
      //PARENT status start
  
  	struct parentProcess
	{
	char size[50];
    	char numbers[50];
    	char executionTime[50];
    	char signalReceived[50];
	}; 
	struct parentProcess Output[fileNumber];
  int count=0;
  counter=1;
   for(int i=0;i<fileNumber;i++){
   
   pid_t wpid = wait(&child_status);
   
   if (WIFEXITED(child_status)){
   int i;
   
  // printf("\nNow I am Parent!!\n");
   
    //signal start
    /*
    if(getpid()%2==0){signal(SIGUSR1,sigParent); // Register signal handler
    sleep(1);
    printf("Parent: sending signal to Child\n");
    kill(pid[i],SIGUSR1);
    printf("Parent: waiting for response\n");
    pause();
    }
    if(getpid()%2==1){signal(SIGUSR2,sigParent2); // Register signal handler
    sleep(1);
    printf("Parent: sending signal to Child\n");
    kill(pid[i],SIGUSR2);
    printf("Parent: waiting for response\n");
    pause();
    }
    */
    //signal end
   
   
   	char fileName3[15] = "intermediate";
	char str[20];
        char str2[]=".txt";
	sprintf(str, "%d", counter);
	strcat(str,str2);
	strcat(fileName3, str);
	
   //file read start
    char lyne[121];
    char *numbers;
    char *executionTime;
    char *signalReceived;
    int size = 0;
    int position = 0;
    char *item;
    
    int k;
   
    fptr2 = fopen(fileName3, "r");
    if (fptr2 == NULL)
    {
        printf("Could not open file %s", fileName3);
        return 0;
    }
    int deneme=0;
    while (fgets(lyne,120,fptr2)) {
    deneme++;
    if(deneme==1){
    strcpy(Output[count].size,lyne);
    //printf("%s",lyne);
    }
        if(deneme==2){
        strcpy(Output[count].numbers,lyne);}   
        
        if(deneme==3){
        strcpy(Output[count].executionTime,lyne);}   
        
        if(deneme==4){
        strcpy(Output[count].signalReceived,lyne);}        
                
                }
     
     fclose(fptr2);
     	           
    //return string to int
   
   int sz = 0;
   char ch;
   double ret = 0;
   double tempe;
   double val[20];
  
   
   while((ch = Output[count].executionTime[sz])!= '\0')
   {
   Output[count].executionTime[0]=' ';
   if ((ch = Output[count].executionTime[sz])== '>') Output[count].executionTime[sz] = ' ';
   sz++;
   }
   tempe = atof(Output[count].executionTime);
   val[count]= tempe;
   //for(int num=count;num<fileNumber;num++)val[num]=tempe;
   for(int num=0;num<fileNumber;num++)printf("%f\n",val[num]);
   
   //calling insertion sort
   if(i+1==fileNumber){
   sortExecutionTime(val,fileNumber);
   for(int num=0;num<fileNumber;num++)printf("After sort : %f\n ",val[num]);
   }
  //end parent process
  
    
   file3 = fopen ("Output.txt","w");
   for(int zt = 0; zt < fileNumber;zt++){
   
    fprintf (file3,"<%f>",val[zt]);
    fprintf (file3,"<%s>",Output[zt].numbers);
    fprintf (file3,"%s",Output[zt].signalReceived);
    //fputs("\n",file3);
    
  }
  fclose(file3);
    
   
    
 } //end if
    count++;
    counter++;
  
 } //end for 
 printf("Output.txt is prepared!!");
} //end files part choice 1 part ending
 
 //BEGINNING PIPE PARTS
 if(choice==2){
    	int counter3=1;
    	printf("Welcome to PIPES PART!\n");
   	printf("How many file do we have: ");
    	scanf("%d",&fileNumber);
	
	//child status
	
	for(int i=1;i<=fileNumber;i++){
	int fd[2];
	//error in pipe
	
	if (pipe(fd) == -1)
        {
            printf("Error with the pipe.\n");
            return 1;
        }
	int id = fork();
	
	if (id == 0) {
	//close read
	close(fd[0]);
	//reset arraySize
	arraySize= 0;
	char fname[15] = "input";
	char fileName2[15] = "intermediate";
	char str[20];
        char str2[]=".txt";
        //for file name 
	sprintf(str, "%d", counter3);
	strcat(str,str2);
	strcat(fname, str);
	strcat(fileName2, str);
	puts(fileName2);
    //file read start
    fptr = fopen(fname, "r");
    if (fptr == NULL)
    {
        printf("Could not open file %s", fname);
        return 0;
    }
    
     while ((ch = fgetc(fptr)) != EOF && ch != '\n');
     while (fscanf(fptr, "%d", &value) == 1) {
        int *new_numbers = realloc(arr, (arraySize + 1) * sizeof(*new_numbers));
        if (!new_numbers) {
            fputs("Out of memory :(\n\n", stderr);
            free(arr);
            return EXIT_FAILURE;
        }
        arr = new_numbers;
        arr[arraySize++] = value;
    }

    fclose(fptr);
    //file read end
    
    random_number = rand() % 7 + 1;
    random_number2 = rand() % 7 + 1;
    
    if((getpid()%2)==1){
    //printf("We are waiting for Selection Sort");
    executionTime=selectionSort(arr,arraySize,random_number);
    }
    if((getpid()%2)==0){
    //printf("We are waiting for Insortion Sort");
    executionTime=insertionSort(arr,arraySize,random_number2);}
    
   
    //signal received
    if ((getpid()%2)==0){
    kill(getppid(),SIGUSR1);
    }
    else if((getpid()%2)==1){
    kill(getppid(),SIGUSR2);
    }
   for (i=0;i<arraySize;i++)printf("%d ",arr[i]);
   //printf("Execution Time: %f\n",executionTime);
   //writing pipe start
    if (write(fd[1], &arraySize, sizeof(int)) < 0)
            {
                printf("Array size error with the pipe\n");
            }
    if (write(fd[1], arr, arraySize * sizeof(int)) < 0)
            {
                printf("Array error with the pipe\n");
            }
    if (write(fd[1], &executionTime, sizeof(double)) < 0)
            {
                printf("Execution Time error with the pipe\n");
            }
            close(fd[1]);
   
   //writing pipe end
    
    
   
  }  
       //PARENT status start
   else{
            wait(NULL);
            close(fd[1]);
            int mysize = 0;
            double myExecutionTime=0;
   
            
            if(read(fd[0], &mysize, sizeof(int))<0){
                printf("Error reading pipe\n");
            }
            int receivedArray[mysize];
            if(read(fd[0], receivedArray, mysize * sizeof(int))<0){
                printf("Error reading pipe\n");
            }
            if(read(fd[0], &myExecutionTime, sizeof(double))<0){
                printf("Error reading pipe\n");
            }
           
            close(fd[0]);
            printf("Execution Time in pipe: %f\n",myExecutionTime);
            double arrEx[fileNumber];
            arrEx[control]=myExecutionTime;
            sortExecutionTime(arrEx,fileNumber);
            
            filePipe=fopen("OutputPipes.txt","a+");
            fprintf(filePipe, "<%f>", myExecutionTime);
            fprintf(filePipe, "<");
            for(int t=0;t<mysize;t++){
                fprintf(filePipe,"%d ",receivedArray[t]);
            }
            fprintf(filePipe, ">");
            fprintf(filePipe,"\n");
            if ((getpid()%2)==1){
            time_t recievedTime;
            struct tm *recievedT;
            time(&recievedTime);
            recievedT = localtime(&recievedTime);
            fprintf(filePipe, "<SIGUSR2> & %s", asctime(recievedT));
            }
            else if((getpid()%2)==0){
            time_t recievedTime;
            struct tm *recievedT;
            time(&recievedTime);
            recievedT = localtime(&recievedTime);
            fprintf(filePipe, "<SIGUSR1> & %s", asctime(recievedT));
            }  
            fclose(filePipe);
     
  }  
  control++;
  counter3++;
 } //end for
 
} //end pipes part choice 2 part ending
 
 
 /* 
    if (choice==2){
    
    
    //printf("Welcome to PIPES PART!");
    //pipes with array start
    // child process will send an array of values to parent process

    // init file descriptors for pipe
   // int fd[2]; // fd[0] - read, fd[1] - write

    // error check pipe
    if (pipe(fd) == -1){
        printf ("error with the pipe. \n");
        return 1;
    }
    
    //fork and error check the fork operation
    int id = fork();
    if (id == -1){
       printf("error with the fork");
    }

    int pipe_i = 0, pipe_size = 0;
    int *data;
    double *arrExecution;
    int numberOfArray=0;
    int val=0;
    int mysize = 0;
    int *receivedArray;
    int myExecutionTime=0;
    //double *receivedExecutionArray;
    
   if (id == 0){
        close(fd[0]); // this is a child process, we do not need to read from the pipe so we close fd[0]
     fflush(stdin);
     printf("\nEnter number of Array: "); scanf ("%d", &numberOfArray);
     fflush(stdin);
     arrExecution = (double*) malloc (numberOfArray * sizeof(int)); // dynamically allocate array
     for(i = 0 ; i < numberOfArray; i++){   
        
  
        printf ("\nEnter array size: "); scanf ("%d", &pipe_size);
        data = (int*) malloc (pipe_size * sizeof(int)); // dynamically allocate array

        printf ("\nPopulate array numbers: \n");
        for (pipe_i = 0; pipe_i < pipe_size; pipe_i++){
            scanf ("%d", &data[pipe_i]);
        }

        // printf ("entered array: \n");
        for (pipe_i = 0; pipe_i < pipe_size; pipe_i++){
             //printf ("%d ", data[i]);
        }
        
        //send for sorting
        pipe_random = rand() % 7 + 1;
        pipe_random2 = rand() % 7 + 1;
        if((getpid()%2)==1)pipe_executionTime=selectionSort(data,pipe_size,pipe_random);
        if((getpid()%2)==0)pipe_executionTime=insertionSort(data,pipe_size,pipe_random2);
        
        for (pipe_i = 0; pipe_i < pipe_size; pipe_i++){
             //printf ("%d ", data[pipe_i]);
        }
        
        
        arrExecution[val]=pipe_executionTime;
        sortExecutionTime(arrExecution,numberOfArray);
        
        for (pipe_i = 0; pipe_i < numberOfArray; pipe_i++){
             printf ("%f ", arrExecution[pipe_i]);
        }
        
        
        // child process write() to pipe and error check
        //printf ("\nchild process writing to pipe..\n");   

        if (write(fd[1], &pipe_size, sizeof(int)) == -1){ // note variable reference here
            printf("error with the pipe. \n");
            return 4;
        }
        if (write(fd[1], &numberOfArray, sizeof(int)) == -1){ // note variable reference here
            printf("error with the pipe. \n");
            return 4;
        }

        if (write(fd[1], data, pipe_size * sizeof(int)) == -1){ // note array reference and size of array here
            printf("error with the pipe. \n");
            return 2;
        }
        if (write(fd[1], arrExecution, numberOfArray * sizeof(int)) == -1){ // note array reference and size of array here
            printf("error with the pipe. \n");
            return 2;
        }

        // close write fd because we are done writing to the pipe at this point.
        
   val++;
  }
  close(fd[1]);
  }//end child process
    else{ // start parent process
        close(fd[1]); // now reading from pipe
	wait(NULL);
        
        
        // parent process reads from pipe
        if (read(fd[0], &mysize, sizeof(int)) < 0) // note variable reference here
        {
            printf("error with the pipe. \n");
            return 5;
        }
        if (read(fd[0], &myExecutionTime, sizeof(int)) < 0) // note variable reference here
        {
            printf("error with the pipe. \n");
            return 5;
        }
        
        receivedArray = (int*) malloc (mysize * sizeof(int)); // dynamically allocate array
        //receivedExecutionArray = (double*) malloc (myExecutionTime * sizeof(int)); // dynamically allocate array
         double receivedExecutionArray[myExecutionTime];
	
	if (read(fd[0], receivedArray, mysize * sizeof(int)) < 0) // note array reference and size here
        {
            printf("error with the pipe. \n");
            return 9;
        }
          
	 
        if (read(fd[0], receivedExecutionArray, myExecutionTime * sizeof(int)) < 0) // note array reference and size here
        {
           printf("error with the pipe. \n");
           return 9;
        }
         
       
        close(fd[0]); // close pipe for read
        
        printf("My sorted array:");
        for (pipe_i = 0; pipe_i < mysize; pipe_i++){
            printf ("%d ", receivedArray[pipe_i]);
        }
        printf("\nMy execution time array:");
        printf ("\n%d \n", myExecutionTime);
        for (pipe_i = 0; pipe_i < myExecutionTime; pipe_i++){
            printf ("%f ", receivedExecutionArray[pipe_i]);
        }
       
       
    } //pipes with array end 

 
}
*/
  // end pipes part


} //end main 
  







