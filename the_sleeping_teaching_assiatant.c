
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<unistd.h>

int chair[3];
int count=0;
int Index=0;
int front=0;
int rear=0;

sem_t Student_sem;
sem_t TA_sem;

pthread_mutex_t Chair_Access;

void *Student(void* ID)
{
while(1)
{
sleep(rand()%5+1);
if(count<3)
{
chair[front]=(long)ID;
if(count==0)
sem_post(&TA_sem);

pthread_mutex_lock(&Chair_Access);

int pt = (count+Index) % 3;
count++;

printf("Student Occupy Chair. Left Chair : %d \n",3-count);

pthread_mutex_unlock(&Chair_Access);
}

else
printf("No seat is left. Student will come afterward.\n");
}

}

void *Teacher()
{
while(1)
{
sem_wait(&TA_sem);
printf("Teacher Assiantant is awakened by a Student\n");

pthread_mutex_lock(&Chair_Access);
Index=(Index+1)%3;
pthread_mutex_unlock(&Chair_Access);

printf("Teacher Assiantant is Teaching Student\n");

sleep(rand()%5+1);

sem_post(&TA_sem);
}
}

int main()
{
pthread_t student[3];
pthread_t TA;

srand(time(NULL));

sem_init(&Student_sem,0,0);
sem_init(&TA_sem,0,1);

pthread_mutex_init(&Chair_Access,NULL);

int i;

for(i=0;i<3;i++)
pthread_create(&student[i],NULL,Student,(void*)(long)i);

pthread_create(&TA,NULL,Teacher,NULL);

for(i=0;i<3;i++)
pthread_join(student[i],NULL);

pthread_join(TA,NULL);

return 0;
}
