//NAME: Don Le
//EMAIL: donle22599@g.ucla.edu
//UID: 804971410
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "SortedList.h"
#include <signal.h>
#include <sys/types.h>

#define length 3 //length of each elements key
//Intialize flags and threads
int numIters=1;
int numThreads=1; 
int numList = 1;
int opt_yield = 0; //Define the variable from SortedList.h
char syncCh = 'n';
//Holds pool of elements
SortedListElement_t *elementArray;
SortedListElement_t *headElement;
int numElements;
int errorThrown = 0;
long long * timeArray;
void handler()
{
  fprintf(stderr, "There was a segfault error");
  exit(1);
}
void errorMessage(char *message, int ret)
{
  errorThrown = 2;
  fprintf(stderr, message);
  exit(ret);
}

pthread_mutex_t *mutexlock;
volatile int *spinlock;
void *wrapper(void* startVoid)
{
  if (errorThrown != 0)//If on thread calls errorMessage, everyone exit
    exit(errorThrown);
  int start = * (int *)startVoid;
  int end = numIters + start;
  int timeIndex = start / numIters; //Use time index as list key
  int listKey = (start / numIters) % numList;
  if (syncCh == 's')
    {
      //       while (__sync_lock_test_and_set(&spinlock, 1)) {};
      int i = start;

      for (; i < end; i++)
	{
	  while (__sync_lock_test_and_set(&spinlock[listKey], 1)) {};      
	  SortedList_insert(&headElement[listKey], &elementArray[i]);
	  if (headElement[listKey].key != NULL)
	    errorMessage("There was a problem with insertion", 2);
	  __sync_lock_release(&spinlock[listKey]);
	}

      while (__sync_lock_test_and_set(&spinlock[listKey], 1)) {};
      if (SortedList_length(&headElement[listKey]) == -1)
	errorMessage("There was an inconsistency when getting length", 2);
      __sync_lock_release(&spinlock[listKey]);


      for (i = start; i < end; i++)
	{
	  while (__sync_lock_test_and_set(&spinlock[listKey], 1)) {};
	  SortedList_t * temp = SortedList_lookup(&headElement[listKey], elementArray[i].key);
	  if (temp == NULL)
	    errorMessage("There was an inconsistency when looking up", 2);
	  if (SortedList_delete(temp) == 1)
	    errorMessage("There was an inconsistency when deleting", 2);
	  __sync_lock_release(&spinlock[listKey]);
	}
      

      //              __sync_lock_release(&spinlock);
    }
  else if (syncCh == 'm')
    {
      //      pthread_mutex_lock(&lock);
      struct timespec begin;
      struct timespec finish;
      long long runtime;
      int i = start;
      for (; i < end; i++)
	{
	  clock_gettime(CLOCK_MONOTONIC, &begin);
	  pthread_mutex_lock(&mutexlock[listKey]);
	  clock_gettime(CLOCK_MONOTONIC, &finish);
	    runtime = ((1000000000* finish.tv_sec) + finish.tv_nsec) - ((1000000000* begin.tv_sec) + begin.tv_nsec);
	    timeArray[timeIndex] += runtime;
	  SortedList_insert(&headElement[listKey], &elementArray[i]);
	  if (headElement[listKey].key != NULL)
	    errorMessage("There was a problem with insertion", 2);
	  pthread_mutex_unlock(&mutexlock[listKey]);      
	}
      
      clock_gettime(CLOCK_MONOTONIC, &begin);
      pthread_mutex_lock(&mutexlock[listKey]);
      clock_gettime(CLOCK_MONOTONIC, &finish);
      runtime = ((1000000000* finish.tv_sec) + finish.tv_nsec) - ((1000000000* begin.tv_sec) + begin.tv_nsec);
      timeArray[timeIndex] += runtime;
      if (SortedList_length(&headElement[listKey]) == -1)
	errorMessage("There was an inconsistency when getting length", 2);
      pthread_mutex_unlock(&mutexlock[listKey]);


      for (i = start; i < end; i++)
	{

	  clock_gettime(CLOCK_MONOTONIC, &begin);
	  pthread_mutex_lock(&mutexlock[listKey]);
	  clock_gettime(CLOCK_MONOTONIC, &finish);
	  runtime = ((1000000000* finish.tv_sec) + finish.tv_nsec) - ((1000000000* begin.tv_sec) + begin.tv_nsec);
	  timeArray[timeIndex] += runtime;
	  SortedList_t * temp = SortedList_lookup(&headElement[listKey], elementArray[i].key);
	  if (temp == NULL)
	    errorMessage("There was an inconsistency when looking up", 2);
	  if (SortedList_delete(temp) == 1)
	    errorMessage("There was an inconsistency when deleting", 2);
	  pthread_mutex_unlock(&mutexlock[listKey]);
	}
      
      // pthread_mutex_unlock(&lock);
    }
  else
    {
	  int i = start;
	  for (; i < end; i++)
	    {
	      SortedList_insert(&headElement[listKey], &elementArray[i]);
	      if (headElement[listKey].key != NULL)
		errorMessage("There was a problem with insertion", 2);
	    }
	  if (SortedList_length(&headElement[listKey]) == -1)
	    errorMessage("There was an inconsistency when getting length", 2);
	  for (i = start; i < end; i++)
	    {
	      SortedList_t * temp = SortedList_lookup(&headElement[listKey], elementArray[i].key);
	      if (temp == NULL)
		errorMessage("There was an inconsistency when looking up", 2);
	      if (SortedList_delete(temp) == 1)
		errorMessage("There was an inconsistency when deleting", 2);
	    }
    }    
  return NULL;
}
void random_key(SortedListElement_t *element)
{
  char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *temp = malloc(length * sizeof(char));
  if (temp == NULL)
    errorMessage("Error allocating space for elements", 1);

  int i = 0;
  for (; i < length; i++)
    {
      int index = abs(rand() % (strlen(charset)-1));
      temp[i] = charset[index];
    }
  temp[length-1] = '\0';
  element->key = temp;
}

int main(int argc, char*argv[])
{
  //Handler
  signal(SIGSEGV, handler);
  //GET ARGUMENTS
  char *yieldName = calloc(10, sizeof(char)); //Get yield name for later
  //  yieldName = "none";
  static struct option command_options[] = {
    {"threads", required_argument, 0, 't'},
    {"iterations", required_argument, 0, 'i'},
    {"yield", required_argument, 0, 'y'},
    {"lists", required_argument, 0, 'l'},
    {"sync", required_argument, 0, 's'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;
  while (1)
    {
      int c = getopt_long(argc, argv, "l:y:t:i:s:", command_options, &option_index);
      if (c == -1)
        break;
      switch(c)
        {
        case 't':
          numThreads = atoi(optarg);
          break;
        case 'i':
          numIters = atoi(optarg);
          break;
        case 'l':
          numList = atoi(optarg);
          break;
        case 's':
          syncCh = optarg[0]; //First letter holds what type of sync
          break;
        case 'y':
	  opt_yield = 1;         
          unsigned int i = 0;
          for(; i < strlen(optarg); i++)
            {
              if (strlen(optarg) == 0)
                break;
              if (optarg[i] == 'i')
                opt_yield |= INSERT_YIELD;
              else if (optarg[i] == 'd')
		opt_yield |= DELETE_YIELD;
              else if (optarg[i] == 'l')
		opt_yield |= LOOKUP_YIELD;
              else
                {
                  fprintf(stderr, "Option not allowed. Use --yield=[idl]");
                  exit(1);
                }
	    }
	  strcpy(yieldName, optarg);
	  break;
        default:
          fprintf(stderr, "Option not allowed. Use --list=# --yield=[idl] --threads=# --iterations=#");
          exit(1);
        }
    }

  //INITIALIIZE LIST ELEMENTS
  //Allocate array of headElements
  numElements = numIters * numThreads; //numIters
  headElement = malloc(numList * sizeof(SortedListElement_t));
  mutexlock = calloc(numList, sizeof(pthread_mutex_t));
  spinlock = calloc(numList, sizeof(volatile int));
  if (headElement == NULL)
    errorMessage("Error allocating space for elements", 1);
  int h = 0;
  for (; h < numList; h++)
    {
      headElement[h].next = &headElement[h];
      headElement[h].prev = &headElement[h];
      headElement[h].key = NULL;
    }

  //Initialize time array
  timeArray = calloc(numThreads, sizeof(long long));
  //Make array of elements
  elementArray = malloc(sizeof(SortedListElement_t) * numElements);
  if (elementArray == NULL)
    errorMessage("Error allocating space for elements", 1);
  int i = 0;
  for (; i < numElements; i++)
    {
      //The elements will be intialized randomly
      //elementArray[i].next = NULL;
      //elementArray[i].prev = NULL;
      random_key(&elementArray[i]);
    }
  struct timespec start;
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  clock_gettime(CLOCK_MONOTONIC, &end);
  //MAKE THREADS
  int* startArr = malloc(numThreads * sizeof(int));
  pthread_t threads[numThreads];
  int rc;
  long t;
  int arrPos = 0;
  for (t = 0; t < numThreads;arrPos+=numIters, t++)
    {
      startArr[t] = arrPos;
      rc = pthread_create(&threads[t], NULL, wrapper, &startArr[t]);
      if (rc < 0)
	errorMessage("Error with creating threads", 1);
    }

  for (t = 0; t < numThreads; t++)
    {
      rc = pthread_join(threads[t], NULL);
      if (rc < 0)
	errorMessage("Error with joining threads", 1);
    }

  if (SortedList_length(&headElement[0]) != 0)
    errorMessage("The list length was not 0 at the end: ", 2);

  clock_gettime(CLOCK_MONOTONIC, &end);
  long long runtime = ((1000000000* end.tv_sec) + end.tv_nsec) - ((1000000000* start.tv_sec) + start.tv_nsec);
  long operations = numThreads * numIters * 3;
  long average = runtime/operations;
  int k = 0;
  long long totalLockTime = 0;
  if (syncCh == 'm'){
    for (; k < numThreads; k++)
      totalLockTime += timeArray[k];
  }
  //Each threads waits three times for the lock
  long long averageLockTime = totalLockTime / (numThreads * 3);
  if (yieldName[0]==0)
    yieldName = "none";
  if (syncCh == 'n')
    printf("list-%s-%s,%ld,%ld,%ld,%ld,%ld,%ld,0", yieldName, "none", (long int) numThreads, (long int) numIters,
	   (long int) numList, (long int) operations, (long int) runtime, (long int) average);
  else
    printf("list-%s-%c,%ld,%ld,%ld,%ld,%ld,%ld,%ld", yieldName, syncCh, (long int)numThreads, (long int) numIters,
	   (long int) numList, (long int) operations, (long int) runtime, (long int) average,(long int)averageLockTime);
  printf("\n");
  exit(0);
  //  free(yieldName);
}
