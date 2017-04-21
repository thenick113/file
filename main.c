#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct obj {
	int num;
	int wait;
};

//Prototype
void *print_message_function(void *ptr);

// Producer function
void *producer(){
  int producingItem = 1;
  // Process loop
  while(1){
    // Waits 3-7s before trying to add to the stack
    
    // Produce item to add to the stack
    
    // Stack access loop
    while(producingItem){
    	// Try to block the stack
      
      // Check if stack is full
      if(stackFull()){
        // Unblock access to stack
        
        // Wait
        
      }else{
        // Add item to stack
        
    }
    
  }
		
  
}
  


// Consumer function
void Consumer() {
  //check if thread has anything
  //Get sleep time value then sleep for wait time
  
}
// Waits 2-9s before trying to remove from the stack

int main() {
    int iret1, iret2;
    long t;
		
  	// Initialize stack of 32 elements
  
  	// Create mutex for stack
  	pthread_mutex thread1, thread2;  
  
  	// Create/start producer thread
  
  	// Create/start consumer thread
  
  	// Wait until users exits the program
  	// ctrl-c
  
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

    return 0;
}


void *print_message_function(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World!\n", tid);
}