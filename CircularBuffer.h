#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

//#include "types.h"

// #define CB_MAX_SIZE 800


// #define SPI_BUFFER_MAX_MSG    10
// #define SPI_BUFFER_MSG_LEN		17

#define CIRCULAR_BUFFER_WRITE_SUCCESS	0

#define CIRCULAR_BUFFER_NOT_ENOUGH_SPACE    253
#define CIRCULAR_BUFFER_FULL                254
#define CIRCULAR_BUFFER_WRITE_ERROR		255


typedef struct{

    unsigned char *buffer;
    unsigned int  maxSize;
    unsigned char empty;
    unsigned char full;
    unsigned int TailPointer;
    unsigned int HeadPointer;
    unsigned char token;
    int dataLength;

}CircularBufferHandle;

// void circularBufferInit(CircularBufferHandle * hBuffer, unsigned char *data);

// void circularBufferReset(CircularBufferHandle * hBuffer);

// int getCircularBufferUse(CircularBufferHandle * hBuffer);
// int getCircularBufferFreeSpace(CircularBufferHandle * bBuffer);
// void emptyCircularBuffer(CircularBufferHandle * hBbuffer);

class CircularBuffer
{
    private:
        
    CircularBufferHandle hBuffer;

    public:
        CircularBuffer(/* args */);
        ~CircularBuffer();
    
    
    void reset();
    
    
    void init(unsigned char *buffer, unsigned int  bufferSize, int dataLength = 0, unsigned char token = 0);
    
    int getUse();
    int getFreeSpace();
    void empty();
    unsigned char write(unsigned char *data);
    unsigned char read(unsigned char *data);

    //void reset(CircularBufferHandle * hBuffer);
    //void init(CircularBufferHandle * hBuffer, unsigned char *data);
    // int getUse(CircularBufferHandle * hBuffer);
    // int getFreeSpace(CircularBufferHandle * hBuffer);
    // void empty(CircularBufferHandle * hBuffer);
    // unsigned char write(CircularBufferHandle * hBuffer,unsigned char *data);
    // unsigned char read(CircularBufferHandle * hBuffer,unsigned char *data);
  
  

};


// unsigned char writeCircularBuffer(CircularBufferHandle * hBuffer, unsigned char *data);
// unsigned char readCircularBuffer(CircularBufferHandle * hBuffer, unsigned char *data);


#endif



