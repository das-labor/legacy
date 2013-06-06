#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

#define BUFFER_SIZE 12
 
struct Buffer {
  void* data[BUFFER_SIZE];
  int 	read; // zeigt auf das Feld mit dem ältesten Inhalt
  int 	write; // zeigt immer auf leeres Feld
  int 	skip;
} buffer = {{}, 0, 0, 0};

pthread_mutex_t mutex;

void BufferInit( int bufsize )
{
	int i;
	for( i = 0; i < BUFFER_SIZE; i ++ )
	{
		buffer.data[i] = malloc( bufsize );
	}
	pthread_mutex_init ( &mutex, NULL );
}

void BufferClean( void )
{
	int i;
	for( i = 0; i < BUFFER_SIZE; i ++ )
	{
		free( buffer.data[i] );
	}
	pthread_mutex_destroy( &mutex );
}

void* BufferIn_Start( void )
{
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit
    if( BufferFull() )
  	  return NULL;
  
  return buffer.data[buffer.write];
}
 
void BufferIn_Finish( void )
{
  pthread_mutex_lock (&mutex);
  if( !buffer.skip )
  {
  	  printf("Buffer ++\n");
  	  buffer.write = buffer.write + 1;
  	  if (buffer.write >= BUFFER_SIZE)
  	  	  buffer.write = 0;
  }
  else
  {
  	  fprintf( stderr, "Dropping buffer\n" );
  	  buffer.skip = 0;
  }
  pthread_mutex_unlock (&mutex);
}

void* BufferOut_Start( void )
{
  if( BufferEmpty() )
  	  return NULL;
  return buffer.data[buffer.read];
}

void BufferOut_Finish( void )
{
  pthread_mutex_lock (&mutex);
  buffer.read = buffer.read + 1;
  printf("Buffer --\n");
  if (buffer.read >= BUFFER_SIZE)
    buffer.read = 0;
  pthread_mutex_unlock (&mutex);
}

int BufferFull( void )
{  
  int val = 0;
  pthread_mutex_lock (&mutex);
  if ( (buffer.write + 1 == buffer.read) || ((buffer.read == 0) && (buffer.write + 1 == BUFFER_SIZE)) )
  {
    val = 1;
  }
  pthread_mutex_unlock (&mutex);
  return val;
}

int BufferEmpty( void )
{  
  int val = 0;
  pthread_mutex_lock (&mutex);
  if (buffer.read == buffer.write)
  {
     val = 1;
     printf("Buffer empty\n");
  }
  pthread_mutex_unlock (&mutex);
  return val;
}

void BufferSkip( void )
{
	if( BufferEmpty() )
	{
		pthread_mutex_lock (&mutex);
		buffer.skip = 1;
		pthread_mutex_unlock (&mutex);
	}
}
