#include <iostream>
#include <fstream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define READERS 5
#define WRITERS 1
using namespace std;

pthread_rwlock_t    rwlock;
pthread_mutex_t     mutex;

string msgbuf;
string file="out.txt";
bool isRead=true;

void *reader(void *arg)
{
	while(true){
        pthread_rwlock_rdlock(&rwlock);

        if (msgbuf=="quit"){
            pthread_rwlock_unlock(&rwlock);    
            return 0;
        }

        pthread_mutex_lock(&mutex);
        if(!isRead){
            ofstream out(file,ios::app);

            if(!out){
                pthread_mutex_unlock(&mutex);
                pthread_rwlock_unlock(&rwlock);
                cout<<"Can't open file."<<endl;
                return 0;
            }
        out<<"Thread "<<pthread_self()<<": "<<msgbuf<<endl;
        out.close();
        isRead=true;
        }
        pthread_mutex_unlock(&mutex);
    	pthread_rwlock_unlock(&rwlock);
    }
  return 0;
}

void *writer(void *arg){	
  while(true){
     pthread_rwlock_wrlock(&rwlock);

     if(isRead){
       cout<<"Input something please: ";
        cin>>msgbuf;
        if (msgbuf=="quit"){
          pthread_rwlock_unlock(&rwlock);
          return 0;
        }
        isRead=false;
     }

    pthread_rwlock_unlock(&rwlock);
  }
  return 0;
}

int main(int argc, char *argv[]){
  int writersNumber=WRITERS;
  int readersNumber=READERS;

  pthread_t threadREAD[readersNumber];
  pthread_t threadWRITE[writersNumber];
 
  pthread_rwlock_init(&rwlock, NULL);
  pthread_mutex_init(&mutex, NULL);

  for(int i=0;i<WRITERS;i++)
	{
		pthread_create(&(threadWRITE[i]),NULL,writer,NULL);
	}

	for(int i=0;i<READERS;i++)
	{
		pthread_create(&(threadREAD[i]),NULL,reader,NULL);
	} 

	for(int i=0;i<WRITERS;i++)
	{
		pthread_join(threadWRITE[i],NULL);
	}
	 for(int i=0;i<READERS;i++)
	{
		pthread_join(threadREAD[i],NULL);
	}

  pthread_rwlock_destroy(&rwlock);
  pthread_mutex_destroy(&mutex);
  return 0;
}