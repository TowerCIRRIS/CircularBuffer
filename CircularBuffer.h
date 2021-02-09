#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

//#include "types.h"

// #define CB_MAX_SIZE 800


// #define SPI_BUFFER_MAX_MSG    10
// #define SPI_BUFFER_MSG_LEN		17

#define CIRCULAR_BUFFER_WRITE_SUCCESS	0


#define CIRCULAR_BUFFER_MODE_ERROR          252
#define CIRCULAR_BUFFER_NOT_ENOUGH_SPACE    253
#define CIRCULAR_BUFFER_FULL                254
#define CIRCULAR_BUFFER_WRITE_ERROR		    255


typedef struct{

    unsigned char *buffer;
    unsigned int  maxSize;
    unsigned char empty;
    unsigned char full;
    unsigned int TailPointer;
    unsigned int HeadPointer;
    char token;
    int dataLength;

}CircularBufferHandle;

typedef enum 
{
    MODE_MANUAL_LENGHT, // User needs to input the number of bytes to write/read using writeBytes(..) function
    MODE_FIXED_LENGHT,  // Lenght of data is set at init and user can use write(..), function will ignore the lenght parameter
    MODE_CHAR_TOKKEN,   // Usefull to write string, write / read function will use the 
    MODE_ENUM_LENGHT
}CircularBufferMode;


class CircularBuffer
{
    private:
        
    CircularBufferHandle hBuffer;
    CircularBufferMode mBufferMode = MODE_MANUAL_LENGHT;
    

    public:
        CircularBuffer(/* args */);
        ~CircularBuffer();
    
    
    
    
    void reset();
    
    
    void init(unsigned char *buffer, unsigned int  bufferSize, CircularBufferMode mode = MODE_MANUAL_LENGHT, int dataLength = 0, char token = '\0');
    
    int getUse();
    int getFreeSpace();
    void empty();

    //unsigned char write(unsigned char *data);
    unsigned char writeChar(char *data, unsigned int lenght = 0);
    unsigned char writeBytes(unsigned char *data, unsigned int lenght = 0);
    unsigned char read(unsigned char *data);
    unsigned int readToTokken(unsigned char *data, char tokken);
    unsigned int read(unsigned char *data, unsigned int lenght);

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



