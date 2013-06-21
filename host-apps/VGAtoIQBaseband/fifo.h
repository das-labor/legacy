#ifndef __FIFO_H_
#define __FIFO_H_
#ifdef __cplusplus    
extern "C" { 
#endif
void* 	BufferIn_Start( void );
void 	BufferIn_Finish( void );
void* 	BufferOut_Start( void );
void 	BufferOut_Finish( void );
int 	BufferFull( void );
int 	BufferEmpty( void );
void 	BufferClean( void );
int 	BufferInit( int bufsize );
void 	BufferSkip( void );
#ifdef __cplusplus
}
#endif
#endif
