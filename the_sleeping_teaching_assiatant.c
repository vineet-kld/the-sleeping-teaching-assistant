#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* Programming(void* ID);
void* Assisting();

sem_t sem_STU;
sem_t sem_TA;

pthread_mutex_t lock;

int chair[3];

int front=0,rear=0,waiting=0,sleeping=0;

int main()
{
int n=3;
  
pthread_t Student[n];
pthread_t TA;

sem_init(&sem_STU,0,0);
sem_init(&sem_TA,0,1);

pthread_mutex_init(&lock,NULL);

pthread_create(&TA,NULL,Assisting,NULL);
for(int i=0;i<n;i++)
pthread_create(&Student[i],NULL,Programming,(void*)(long)i);

for(int i=0;i<n;i++)
pthread_join(Student[i],NULL);

pthread_join(TA,NULL);

return 0;
}

void* Assisting()
{
printf("--------------------------------------------------------\n");
while(1)
{
	if(waiting>0)
	{
	sleeping=0;
	
	sem_wait(&sem_STU);
	
	pthread_mutex_lock(&lock);
	
	printf("TA started assisting Student %d\n",chair[rear]);
	printf("           Students Wating = %d\n",(waiting-1));
	printf("--------------------------------------------------------\n");

	chair[rear]=0;
	waiting--;
	rear=(rear+1)%3;
	
	sleep(rand()%5);
	
	pthread_mutex_unlock( &lock );
	sem_post( &sem_TA );
	}
	else 
	{
	if(sleeping==0) 
	{	
	printf("No students are waiting.So TA is Sleeping...\n");
	sleeping=1;
	}
	}
}
}

void* Programming(void * ID) 
{
while(1)
{
	if(chair[0]==(long)ID+1||chair[1]==(long)ID+1||chair[2]==(long)ID+1)
	continue;
	
	printf("Student %ld programming.\n",(long)ID+1);
	sleep(rand()%5);

	pthread_mutex_lock(&lock);

	if(waiting<3)
	{
	chair[front]=(long)ID+1;
	waiting++;

	printf("--------------------------------------------------------\n");
	printf("Student %ld has doubt.\n",(long)ID+1);
	if(waiting>1)
	printf("Student %ld takes the seat in the Hallway.\n",(long)ID+1);
	else
	printf("Student %ld takes the seat in the Cabin.\n",(long)ID+1);
	front=(front+1)%3;

	pthread_mutex_unlock(&lock);

	sem_post(&sem_STU);
	sem_wait(&sem_TA);
	}
	else 
	{
	pthread_mutex_unlock(&lock);
	printf("Hallway is full.\n");
	printf("Student %ld will come back later...\n",(long)ID);
	}
}
}
