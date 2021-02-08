#include "CircularBuffer.h"
#include <string.h>

CircularBuffer::CircularBuffer(/* args */)
{
}

CircularBuffer::~CircularBuffer()
{
}


void CircularBuffer::reset()
{
    
    hBuffer.empty = 1;
    hBuffer.full = 0;
    hBuffer.HeadPointer = 0;
    hBuffer.TailPointer = 0;
}

void CircularBuffer::init(unsigned char *buffer, unsigned int  bufferSize, CircularBufferMode bufferMode, int dataLength, unsigned char token)
{
    hBuffer.buffer = buffer;
    hBuffer.maxSize = bufferSize;
    hBuffer.token = token;
    hBuffer.dataLength = dataLength;

    mBufferMode = bufferMode;

    reset();
}

int CircularBuffer::getUse()
{

    if(hBuffer.empty) return 0;

    if(hBuffer.full) return hBuffer.maxSize;

    if(hBuffer.HeadPointer > hBuffer.TailPointer) return (hBuffer.HeadPointer - hBuffer.TailPointer);

    if(hBuffer.HeadPointer < hBuffer.TailPointer) return (hBuffer.maxSize - hBuffer.TailPointer) + hBuffer.HeadPointer;

    return -1;

}

int CircularBuffer::getFreeSpace()
{

    if(hBuffer.full) return 0;

    if(hBuffer.empty) return hBuffer.maxSize;

    if(hBuffer.HeadPointer > hBuffer.TailPointer) return (hBuffer.maxSize-(hBuffer.HeadPointer - hBuffer.TailPointer));

    if(hBuffer.HeadPointer < hBuffer.TailPointer) return (hBuffer.maxSize-(hBuffer.maxSize - hBuffer.TailPointer) + hBuffer.HeadPointer);

    return -1;

}



void CircularBuffer::empty()
{

    hBuffer.TailPointer = hBuffer.HeadPointer;
    hBuffer.empty = 1;
    hBuffer.full = 0;

}


unsigned char CircularBuffer::write(char *data)
{
    unsigned int writeLenght;


    switch(mBufferMode)
    {
        case MODE_FIXED_LENGHT:
            writeLenght = hBuffer.dataLength;
        break;

        case MODE_CHAR_TOKKEN:
            writeLenght = 1 + strcspn((char *)data,(char*)&hBuffer.token);
        break;

        default:
            return CIRCULAR_BUFFER_MODE_ERROR;
        break;
    }

    writeBytes((unsigned char *)data, writeLenght);

    return CIRCULAR_BUFFER_WRITE_SUCCESS;
}

unsigned char CircularBuffer::writeBytes(unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    unsigned int index;
    unsigned char *buf;

    index = hBuffer.HeadPointer;
    buf =  &(hBuffer.buffer[index]);

    if(getFreeSpace() < lenght)
    {
        return CIRCULAR_BUFFER_NOT_ENOUGH_SPACE;
    }

    if( hBuffer.full == 0)
    {
        hBuffer.empty = 0;

        index = hBuffer.HeadPointer;

        for(j=0;j<lenght;j++){

            buf[j] = data[j];

        }
        hBuffer.HeadPointer += lenght ;

        if (hBuffer.HeadPointer >= hBuffer.maxSize)
            hBuffer.HeadPointer = 0;
        if (hBuffer.TailPointer == hBuffer.HeadPointer) {
            hBuffer.full = 1;
        }


    }else{
        return CIRCULAR_BUFFER_FULL; // buffer full

    }

    return CIRCULAR_BUFFER_WRITE_SUCCESS;    //success
}


 unsigned char CircularBuffer::read(unsigned char *data)
 {

    unsigned int writeLenght;

    unsigned int j;
    unsigned int index;
    unsigned char *buf;

    index = hBuffer.TailPointer;
    buf =  &(hBuffer.buffer[index]);

    switch(mBufferMode)
    {
        case MODE_FIXED_LENGHT:
            writeLenght = hBuffer.dataLength;
        break;

        case MODE_CHAR_TOKKEN:
            writeLenght = 1 + strcspn((char *)data,(char*)&hBuffer.token);
        break;

        default:
            return CIRCULAR_BUFFER_MODE_ERROR;
        break;
    }

 }

unsigned char CircularBuffer::read(unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    unsigned int index;
    unsigned char *buf;


    index = hBuffer.TailPointer;
    buf =  &(hBuffer.buffer[index]);

    if( hBuffer.empty == 0 )
    {
        hBuffer.full = 0;

        index = hBuffer.TailPointer;

        for(j=0;j<lenght;j++){

            data[j] = buf[j];
        }

        hBuffer.TailPointer += lenght;

        if(hBuffer.TailPointer >= hBuffer.maxSize)
            hBuffer.TailPointer = 0;

        if(hBuffer.TailPointer == hBuffer.HeadPointer)
            hBuffer.empty  = 1 ;


    }

    return hBuffer.empty;
}



