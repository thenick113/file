#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct queue {
 	//int currentItem = 0;
  //int max = 32;
  //struct stack_item[32];
}

struct queue_item {
	int numToPrint;
	int timeToWait;
};

// Queue / buffer
// MAX_SIZE = 32
// isEmpty
// isFull
// getFront?
// dequeue
// enqueue

//Prototype
void *print_message_function(void *ptr);

// Producer function
void *producer(void *){
  int producingItem;
  int sleepTime;
  struct queue_item itemToProduce;
  
  // Process loop
  while(1){
    producingItem = 1;
    
    // Waits 3-7s before trying to add to the queue
    sleepTime = rand(4) + 3;
    sleep(sleepTime);
    
    // Produce item to add to the queue
    // Some number that will be printed
    // Sleep time for consumer between 2 - 9
    itemToProduce = ge
      
    // Stack access loop
    while(producingItem){
    	// Block until queue mutex is available to lock
      pthread_mutex_lock(&queue_mutex);
      
      // Check if queue is full
      if(queueFull()){
        // Unblock access to queue
        pthread_mutex_unlock(&queue_mutex);
        
      }else{
        // Add item to stack
        
        // Unblock access to the stack
        pthread_mutex_unlock(&stack_mutex);
        
        //Finish producing current item
        producingItem = 0;
    	}
    }
  }
}
  


// Consumer function
void *consumer(void *) {
  // Process loop / queue access loop when empty
  while(1){
    // Block until queue mutex is available to lock
    pthread_mutex_lock(&queue_mutex);

    // Check if queue is empty
    if(stackEmpty()){
      // Unblock access to queue
      pthread_mutex_unlock(&queue_mutex);

    }else{
      // Remove item from queue

      // Unblock access to the queue
      pthread_mutex_unlock(&queue_mutex);
      
      // Wait the items designated amount of time
      
      // Print item number

    }
  }
}


int main() {
    int iret1, iret2;
  	pthread consumer_thread, producer_thread;
    long t;
		
  	// Initialize queue of 32 elements
  
  
  	// Create mutex for queue
  	pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;  
  
  	// Create/start producer thread
  	pthread_create(&producer_thread),
		               NULL,
		               producer,
		               (void*)t);
  
  	// Create/start consumer thread
  	pthread_create(&consumer_thread),
		               NULL,
		               consumer,
		               (void*)t);
  
  	// Wait until users exits the program
  	// ctrl-c
    while(1){
    }
  	/*
	 	iret1 = pthread_create(&thread1, NULL, print_message_function, (void*)t);
    if(iret1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }   

    iret1 = pthread_create(&thread2, NULL, print_message_function, (void*)t);
    if(iret2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }   
    */

    return 0;
}


void *print_message_function(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World!\n", tid);
}
