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

void CircularBuffer::init(unsigned char *buffer, unsigned int bufferSize, CircularBufferMode bufferMode, int dataLength, char token)
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

    if (hBuffer.empty)
        return 0;

    if (hBuffer.full)
        return hBuffer.maxSize;

    if (hBuffer.HeadPointer > hBuffer.TailPointer)
        return (hBuffer.HeadPointer - hBuffer.TailPointer);

    if (hBuffer.HeadPointer < hBuffer.TailPointer)
        return (hBuffer.maxSize - hBuffer.TailPointer) + hBuffer.HeadPointer;

    return -1;
}

int CircularBuffer::getFreeSpace()
{

    if (hBuffer.full)
        return 0;

    if (hBuffer.empty)
        return hBuffer.maxSize;

    if (hBuffer.HeadPointer > hBuffer.TailPointer)
        return (hBuffer.maxSize - (hBuffer.HeadPointer - hBuffer.TailPointer));

    if (hBuffer.HeadPointer < hBuffer.TailPointer)
        return (hBuffer.maxSize - (hBuffer.maxSize - hBuffer.TailPointer) + hBuffer.HeadPointer);

    return -1;
}

void CircularBuffer::empty()
{

    hBuffer.TailPointer = hBuffer.HeadPointer;
    hBuffer.empty = 1;
    hBuffer.full = 0;
}

// unsigned char CircularBuffer::write(unsigned char *data)
// {
//     unsigned int writeLenght;

//     switch (mBufferMode)
//     {
//         case MODE_FIXED_LENGHT:
//             writeLenght = hBuffer.dataLength;
//             break;

//         case MODE_CHAR_TOKKEN:
//             writeLenght = 1 + strcspn((char *)data, (char *)&hBuffer.token);
//             break;

//         default:
//             return CIRCULAR_BUFFER_MODE_ERROR;
//             break;
//     }

//     return writeBytes(data, writeLenght);
// }

unsigned char CircularBuffer::writeChar(char *data, unsigned int lenght)
{
    unsigned int writeLenght;

    unsigned char* ptr = reinterpret_cast<unsigned char *>(data);

    if(lenght > 0){

        writeLenght = lenght;
    }
    else
    {
        switch (mBufferMode)
        {
            case MODE_FIXED_LENGHT:
                writeLenght = hBuffer.dataLength;
                break;

            case MODE_CHAR_TOKKEN:

                writeLenght = 1 + strcspn(data, &hBuffer.token);
                break;

            default:
                return CIRCULAR_BUFFER_MODE_ERROR;
                break;
        }
    }
    
  
    return writeBytes(ptr, writeLenght);

    return CIRCULAR_BUFFER_MODE_ERROR;

}

unsigned char CircularBuffer::writeBytes(unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    unsigned int writeLenght;

    // if leght not specified ( default 0) assume we are in constant size mode
    if(lenght < 1)
    {
        if(mBufferMode == MODE_FIXED_LENGHT){
            writeLenght = hBuffer.dataLength;
        }
        else
        {
            return CIRCULAR_BUFFER_MODE_ERROR;
        }
        
    }

    if (writeLenght > getFreeSpace())
    {
        return CIRCULAR_BUFFER_NOT_ENOUGH_SPACE;
    }
    else
    {
        hBuffer.empty = 0;

        for (j = 0; j < writeLenght; j++)
        {
            hBuffer.buffer[hBuffer.HeadPointer] = data[j];

            hBuffer.HeadPointer++;

            if (hBuffer.HeadPointer >= hBuffer.maxSize)
                hBuffer.HeadPointer = 0;
        }

        if (hBuffer.TailPointer == hBuffer.HeadPointer)
            hBuffer.full = 1;
    }

    return CIRCULAR_BUFFER_WRITE_SUCCESS; //success
}

unsigned char CircularBuffer::read(unsigned char *data)
{

    unsigned int j;
    unsigned int index;
    unsigned char *buf;

    index = hBuffer.TailPointer;
    buf = &(hBuffer.buffer[index]);

    switch (mBufferMode)
    {
    case MODE_FIXED_LENGHT:
        return read(data, hBuffer.dataLength);
        break;

    case MODE_CHAR_TOKKEN:
        return readToTokken(data,hBuffer.token);
        break;

    default:
        return CIRCULAR_BUFFER_MODE_ERROR;
        break;
    }

    
}

unsigned int CircularBuffer::read(unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    //unsigned int index;
    //unsigned char *buf;

   // index = hBuffer.TailPointer;
   // buf = &(hBuffer.buffer[index]);

    if (hBuffer.empty == 0)
    {
        hBuffer.full = 0;

        //index = hBuffer.TailPointer;

        for (j = 0; j < lenght; j++)
        {
            data[j] = hBuffer.buffer[hBuffer.TailPointer];
            
            hBuffer.TailPointer++;

             if (hBuffer.TailPointer >= hBuffer.maxSize)    // loop tail pointer
                hBuffer.TailPointer = 0;

            if (hBuffer.TailPointer == hBuffer.HeadPointer){
                hBuffer.empty = 1;
                return (j+1); // Return number of bytes read
            }
            
        }

        return (j);    
        
    }

    return 0;   // no bytes read, buffer empy
}


unsigned int CircularBuffer::readToTokken(unsigned char *data, char tokken)
{

    unsigned int j = 0;;
    bool searchComplete = false;

    //unsigned int index;
    //unsigned char *buf;

   // index = hBuffer.TailPointer;
   // buf = &(hBuffer.buffer[index]);

    if (hBuffer.empty == 0)
    {
        hBuffer.full = 0;

        while(!searchComplete)
        {
            data[j] = hBuffer.buffer[hBuffer.TailPointer];
            
            if( tokken == *(char*)&data[j] )
            {
               searchComplete = true;
            }
            
            hBuffer.TailPointer++;

             if (hBuffer.TailPointer >= hBuffer.maxSize)    // loop tail pointer
                hBuffer.TailPointer = 0;

            if (hBuffer.TailPointer == hBuffer.HeadPointer){
                hBuffer.empty = 1;
                return (j+1); // Return number of bytes read
            }

            j++;
            
        }

        return j;    
        
    }

    return 0;   // no bytes read, buffer empy
}