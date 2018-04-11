#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* Programming(void* ID);
// Student doing its Programming.

void* Assisting();
// TA Assisting in cae Student gets doubt.

sem_t sem_STU;
sem_t sem_TA;
// TA Semaphore to make TA sleep.

pthread_mutex_t lock;
// Mutex lock to lock the Shared Variables.

int chair[3];

int front=0,rear=0,waiting=0,sleeping=0;
//   front : This is for seating position.
//    rear : This is for Teaching status(position).
// waiting : This is to check that How many student are waiting.
// sleeping: This is to keep track of Teaching Assistant's sleep.

int main()
{
int n=0,t;

do
{
// Office Time
printf("Enter Current Time in 24 hr Format...\n");
scanf("%d",&t);

if(t<8||t>20)
{
printf("It is Not an Office Hour.TA is Sleeping... Visit Later.\n");
continue;
}
// Number of Student.
printf("Enter the number of Students...\n");
scanf("%d",&n);
}while((n<1)||(t<8)||(t>20));

// Defining Threads 
pthread_t Student[n];
pthread_t TA;

// Initilizing semaphore
sem_init(&sem_STU,0,0);
sem_init(&sem_TA,0,1);

// Initilizing Mutex lock
pthread_mutex_init(&lock,NULL);

// Thread Creation
pthread_create(&TA,NULL,Assisting,NULL);
for(int i=0;i<n;i++)
pthread_create(&Student[i],NULL,Programming,(void*)(long)i);

// Thread Joining
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
// Started Assisting the Student.
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
// If no student is present in Hallway. TA will take Nap.
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
// If Student is waiting Let him wait.
	if(chair[0]==(long)ID+1||chair[1]==(long)ID+1||chair[2]==(long)ID+1)
	continue;
	
// Student Start Programming.
	printf("Student %ld programming.\n",(long)ID+1);
	sleep(rand()%5); // Random Sleep(Programming) Time

	pthread_mutex_lock(&lock);

	if(waiting<3)
	{
	chair[front]=(long)ID+1;
	waiting++;

// Student takes a seat in the Cabin & Hallway Accordingly.
	printf("--------------------------------------------------------\n");
	printf("Student %ld has doubt.\n",(long)ID+1);
	if(waiting>1)
	printf("Student %ld takes the seat in the Hallway.\n",(long)ID+1);
	else
	printf("Student %ld takes the seat in the Cabin.\n",(long)ID+1);
	front=(front+1)%3;

	pthread_mutex_unlock(&lock);

	sem_post(&sem_STU);
// Awake TA if Sleeping
	sem_wait(&sem_TA); 
	}
	else 
	{
// When Hallway's Chair are full Student will be send back.
	pthread_mutex_unlock(&lock);
	printf("Hallway is full.\n");
	printf("Student %ld will come back later...\n",(long)ID);
	}
}
}
