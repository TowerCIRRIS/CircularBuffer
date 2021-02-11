#include "CircularBuffer.h"
#include <string.h>

CircularBuffer::CircularBuffer(/* args */)
{
}

CircularBuffer::~CircularBuffer()
{
}

void CircularBuffer::empty()
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

    empty(); // set circular buffer to initial state
}

int CircularBuffer::getUsedSpace()
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

cbError_t CircularBuffer::writeChar(const char *data, unsigned int lenght)
{
    unsigned int writeLenght;

    const unsigned char* ptr = reinterpret_cast<const unsigned char *>(data);

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

cbError_t CircularBuffer::writeBytes(const unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    unsigned int writeLenght = lenght;

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

unsigned int CircularBuffer::readChar(char *data, unsigned int lenght)
{
    unsigned char* ptr = reinterpret_cast<unsigned char *>(data);

    if(lenght > 0)
    {
        return readBytes(ptr, lenght);
    }
    else
    {
        switch (mBufferMode)
        {
            case MODE_FIXED_LENGHT:
                return readBytes(ptr, hBuffer.dataLength);
                break;

            case MODE_CHAR_TOKKEN:
                return readToTokken(ptr,hBuffer.token);
                break;
            default:
                return CIRCULAR_BUFFER_MODE_ERROR;
                break;
        }
    }
    

    
}

unsigned int CircularBuffer::readBytes(unsigned char *data, unsigned int lenght)
{

    unsigned int j;
    unsigned int writeLenght = lenght;

    if(lenght < 1)
    {
        if(mBufferMode == MODE_FIXED_LENGHT){
            writeLenght = hBuffer.dataLength;
        }
        else
        {
            return 0;
        }
        
    }


    if (hBuffer.empty == 0)
    {
        hBuffer.full = 0;

        for (j = 0; j < writeLenght; j++)
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

    unsigned int j = 0;
    bool searchComplete = false;
    int tailPointerCopy = hBuffer.TailPointer;

    if (hBuffer.empty == 0)
    {
        

        while(!searchComplete)
        {
            data[j] = hBuffer.buffer[tailPointerCopy];
            
            if( tokken == *(char*)(&data[j]) )
            {
               searchComplete = true;
               hBuffer.TailPointer = tailPointerCopy+1;
               hBuffer.full = 0;
               return (j+1); // Return number of bytes read
            }
            
            tailPointerCopy++;

             if (tailPointerCopy >= hBuffer.maxSize)    // loop tail pointer
                tailPointerCopy = 0;

            if (tailPointerCopy == hBuffer.HeadPointer){
               
               // Tokken not found
               if(!searchComplete)
               {
                   return 0;
               }
               else
               {
                    hBuffer.full = 0;
                    hBuffer.empty = 1;
                    hBuffer.TailPointer = tailPointerCopy; // update real Tail
                    return (j+1); // Return number of bytes read
               }

            
            }

            j++;
            
        }

        return j;    
        
    }

    return 0;   // no bytes read, buffer empy
}
