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
pthread_barrier_t barrier;

int readCount=0;
string msgbuf;
string file="out.txt";

void *reader(void *arg)
{
	while(true){
  		pthread_rwlock_rdlock(&rwlock);
      if (msgbuf=="quit"){
        pthread_rwlock_unlock(&rwlock);    
        return 0;
      }
      pthread_mutex_lock(&mutex);
      if(readCount<READERS && !msgbuf.empty()){
      cout<<pthread_self()<<endl;
        ofstream out(file,ios::app);

        if(!out){
          cout<<"Can't open file."<<endl;
          return 0;
        }
        out<<msgbuf<<endl;
        out.close();
        readCount++;
      }else{
        readCount=0;
        msgbuf="";
      }
      pthread_mutex_unlock(&mutex);
      pthread_barrier_wait(&barrier);
  		pthread_rwlock_unlock(&rwlock);
      pthread_yield();   
  }
  return NULL;
}

void *writer(void *arg){	
  while(true){
     pthread_rwlock_wrlock(&rwlock);
     if(msgbuf.empty()){
       cout<<"Input something please: ";
        cin>>msgbuf;
        if (msgbuf=="quit"){
          break;
        }
     }
    pthread_rwlock_unlock(&rwlock);
    pthread_yield();
  }
  pthread_rwlock_unlock(&rwlock);
  return 0;
}

int main(int argc, char *argv[]){
  int writersNumber=WRITERS;
  int readersNumber=READERS;

  pthread_t threadREAD[readersNumber];
  pthread_t threadWRITE[writersNumber];
 
  pthread_rwlock_init(&rwlock, NULL);
  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, READERS);

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
   pthread_barrier_destroy(&barrier);
  return 0;
}