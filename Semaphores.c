
/*
 * File: Semaphores.c
 * Authors: Christopher Jones, Austin Auger, Samdeesh Singh
 * 
 */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "buffer.h"

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int inCount = 0;
int remPoint = 0;

void *producer(void *param);
void *consumer(void *param);

//insert into the buffer
int insert_item(buffer_item item) {
	
	//Getting the empty semaphore
	sem_wait(&empty);
	
	//getting the mutex lock
	pthread_mutex_lock(&mutex);
	
	//item insertion
	buffer[inCount] = item;
	inCount = (inCount + 1) % BUFFER_SIZE;
	
	//releasing the mutex
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	
	return 0;
}

//removing objects from the buffer
int remove_item(buffer_item *item) {
	
	//getting the full semapphore
	sem_wait(&full);
	
	//mutex lock acquiring
	pthread_mutex_lock(&mutex);
	
	//item removal
	*item = buffer[inCount];
	buffer[inCount] = 0;
	inCount = (inCount - 1) % BUFFER_SIZE;
	
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	
	return 0;
}

void *producer(void *param) {
	
	buffer_item rNum;
	int randNum;
	
	while(TRUE) {
		
		randNum = rand() % BUFFER_SIZE;
		sleep(randNum);
		rNum = rand();
		
		if (insert_item(rNum))
			fprintf(stderr, "error encountered");
		
		printf("Producer produced %d \n", rNum);
	}
	
}

void *consumer(void *param) {
	
	buffer_item rNum;
	int randNum;
	
	while(TRUE) {
		
		randNum = rand() % BUFFER_SIZE;
		sleep(randNum);
		
		if (remove_item(&rNum))
			fprintf(stderr, "error in consuming");
		else if (rNum != 0)
			printf("Consumer consumed %d \n", rNum);
	}
	
}

int main(int argc, char *argv[]) {
	
	//declarations
	int timeSleep, proThread, conThread;
	int i, j;
	
	if (argc != 4)
	{
		fprintf(stderr, "Please fill in appropriate command line arguments - int int int");
		return -1;
	}
	
	//assigning command line arguments to variables
	timeSleep = atoi(argv[1]);
	proThread = atoi(argv[2]);
	conThread = atoi(argv[3]);
	
	//Initialization
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	srand(time(0));
	
	//creating producer and consumer threads for the semaphores
	for ( i = 0; i < proThread; i++) {
		
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, producer, NULL);
		
	}
	
	for ( j = 0; j < conThread; j++) {
		
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, consumer, NULL);
	}
	
	sleep(timeSleep);
	
	return 0;
}
	


































