#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UMASK 0x80000000UL /* most significant w-r bits */
#define LMASK 0x7fffffffUL /* least significant r bits */
#define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))

static unsigned long state[N]; /* the array for the state vector  */
static int left = 1;
static int initf = 0;
static unsigned long *next;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;  
struct item {
     unsigned long numToPrint;
     unsigned long timeToSleep;
};

struct node {
    struct item a_item;
    struct node * next;
};
//Prototype
unsigned long genrand_int32(void);


struct node * ptr = NULL; //pointer to the beginning of the que.
int itemsInQueue = 0;
int maxQueue = 32;

int isBufferFull(){
 return itemsInQueue == maxQueue;
}

int isBufferEmpty(){
	return itemsInQueue == 0;
}

// enqueue
int enqueue(struct item to_add) {
    if(!ptr) { //If the Queue was empty.
        ptr = malloc(sizeof(struct node));
        ptr->a_item.numToPrint = to_add.numToPrint;
        ptr->a_item.timeToSleep = to_add.timeToSleep;
        ptr->next = ptr;
    }

    else if(itemsInQueue < maxQueue){    //if the Queue was not empty, creating a new node and assigning the person object to that node.
        struct node * temp;
        temp = ptr->next;
        ptr->next = malloc(sizeof(struct node));
        ptr = ptr->next;
        ptr->a_item.numToPrint = to_add.numToPrint;
        ptr->a_item.timeToSleep = to_add.timeToSleep;
        ptr->next = temp;
    }
    itemsInQueue = itemsInQueue + 1;
  
    return 0;
}

struct item* dequeue(void) {
    if (!ptr) return 0; //CASE1: Empty list.
		
  	struct item *to_return = (struct item*) malloc(sizeof(struct item));
  	to_return -> numToPrint = ptr->a_item.numToPrint;
  	to_return -> timeToSleep = ptr->a_item.timeToSleep;
  	
    if (ptr == ptr->next) {//CASE2: List of One node
        free(ptr);
        ptr = NULL;
    }

    else {           //Case3: More than One node
			struct node * current = ptr;
      struct node * temp;
      while (current->next != ptr) {
        current = current->next;
      }
      	
        temp = ptr->next;
        free(ptr);   //Removing from the front (the first node added).
        ptr=temp;
        current->next=temp;
    }
  
  	itemsInQueue = itemsInQueue - 1;

	return to_return;
}


int display(void)
{
    if(!ptr)        //CASE 1: Empty List
        return 0;

    else if (ptr == ptr->next)   //CASE 2: List of One node 
        display_item(ptr->a_item);

    else {                    //CASE 3: More than One node
        struct node * current = ptr->next; //Moving current to point to the first node in the CLL.
        //Using a do while as the starting consition of current IS equal to ptr->next.
        do {
            display_item(current->a_item);
            current = current->next;
        }
        while (current != ptr->next);
    }

return 1;
}


int display_item(struct item a_item) {
    printf("numToPrint = %lu\n", a_item.numToPrint);
    printf("timeToSleep = %lu\n", a_item.timeToSleep);
}


// Producer function
void *producer(void * item){
   unsigned long producingItem;
   unsigned long sleepTime;
  struct item *itemToProduce;
  
  // Process loop
  while(1){
    producingItem = 1;
   // printf("producer start to sleep\n");
    // Waits 3-7s before trying to add to the buffer
    sleepTime = (genrand_int32() % 5) + 3;
    sleep(sleepTime);
    
    // Produce item to add to the buffer
    // Some number that will be printed
    // Sleep time for consumer between 2 - 9
    itemToProduce = (struct item*) malloc(sizeof(struct item));
    itemToProduce -> numToPrint = genrand_int32();
    itemToProduce -> timeToSleep = (genrand_int32() % 8) + 2;
      
    // Stack access loop
    while(producingItem){
    	// Block until buffer mutex is available to lock
      pthread_mutex_lock(&buffer_mutex);
      
      // Check if buffer is full
      if(isBufferFull()){
        // Unblock access to buffer
        pthread_mutex_unlock(&buffer_mutex);
        
        // Sleep a minimum amount of time
        sleep(1);
        
      }else{
//        printf("Producing item: \n");
//      	display_item(*itemToProduce);
        
        // Add item to buffer
        enqueue(*itemToProduce);
        
        // Free resources
        free(itemToProduce);
        
        // Unblock access to the buffer
        pthread_mutex_unlock(&buffer_mutex);
        
        //Finish producing current item
        producingItem = 0;
    	}
    }
  }
  
  return;
}
  


// Consumer function
void *consumer(void * item) {
  struct item *itemToConsume;
  
  // Process loop / buffer access loop when empty
  while(1){
    // Block until buffer mutex is available to lock
    pthread_mutex_lock(&buffer_mutex);

    // Check if buffer is empty
    if(isBufferEmpty()){
      // Unblock access to buffer
      pthread_mutex_unlock(&buffer_mutex);
        
      // Sleep a minimum amount of time
      sleep(1);

    }else{
//	printf("consumer get item!\n"); 
     // Remove item from buffer
	itemToConsume = dequeue();
        
      // Unblock access to the buffer
      pthread_mutex_unlock(&buffer_mutex);
      
      // Wait the items designated amount of time
      sleep(itemToConsume->timeToSleep);
      
      // Print item number
			//printf("%d", itemToConsume.numberToPrint);
     	printf("%lu\n", itemToConsume->numToPrint);
 //     display_item(*itemToConsume);
      
      // Free resource
      free(itemToConsume);
    }
  }
  
  return;
}

/* initializes state[N] with a seed */
void init_genrand(unsigned long s)
{
    int j;
    state[0]= s & 0xffffffffUL;
    for (j=1; j<N; j++) {
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array state[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        state[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }
    left = 1; initf = 1;
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        state[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { state[0] = state[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        state[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { state[0] = state[N-1]; i=1; }
    }

    state[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
    left = 1; initf = 1;
}

static void next_state(void)
{
    unsigned long *p=state;
    int j;

    /* if init_genrand() has not been called, */
    /* a default initial seed is used         */
    if (initf==0) init_genrand(5489UL);

    left = N;
    next = state;

    for (j=N-M+1; --j; p++)
        *p = p[M] ^ TWIST(p[0], p[1]);

    for (j=M; --j; p++)
        *p = p[M-N] ^ TWIST(p[0], p[1]);

    *p = p[M-N] ^ TWIST(p[0], state[0]);
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (long)(y>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (double)y * (1.0/4294967295.0);
    /* divided by 2^32-1 */
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return (double)y * (1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    unsigned long y;

    if (--left == 0) next_state();
    y = *next++;

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return ((double)y + 0.5) * (1.0/4294967296.0);
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void)
{
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
    return(a*67108864.0+b)*(1.0/9007199254740992.0);
}
/* These real versions are due to Isaku Wada, 2002/01/09 added */

int main() {
    int iret1, iret2;
  	pthread_t consumer_thread, producer_thread;
    long t;
		
  	// Initialize queue of 32 elements
  	//struct queue buffer;
  
  	// Create mutex for queue
  	
  
  	// Create/start producer thread
  	iret1 = pthread_create(&producer_thread,
		               NULL,
		               producer,
		               (void*)t);
  	
  	// Error handling for producer thread
  	if(iret1) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", iret1);
        exit(EXIT_FAILURE);
    } 
  
  	// Create/start consumer thread
  	iret2 = pthread_create(&consumer_thread,
		               NULL,
		               consumer,
		               (void*)t);
  	// Error handling for consumer thread
  	if(iret2) {
        fprintf(stderr,"Error - pthread_create() return code: %d\n", iret2);
        exit(EXIT_FAILURE);
    }
  
  	// Wait until users exits the program
  	// ctrl-c
    while(1){
        // Sleep a minimum amount of time
        sleep(1);
    }

    return 0;
}

