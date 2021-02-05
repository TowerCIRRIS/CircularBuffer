
#include <iostream>
using namespace std;

#include "CircularBuffer.h"


CircularBufferHandle hCircularBuffer;

#define BUFFER_SIZE 50
unsigned char buffer[BUFFER_SIZE];

#define TESTDATA_LEN 4
unsigned char dataBufferWrite[TESTDATA_LEN] = {'A','B','C','D'};
unsigned char dataBufferRead[TESTDATA_LEN+5];

CircularBuffer circularBuffer;
int main(void)
{


    hCircularBuffer.buffer = buffer;
    hCircularBuffer.maxSize = BUFFER_SIZE;
    hCircularBuffer.dataLength = TESTDATA_LEN;


   circularBuffer.init(buffer,BUFFER_SIZE,TESTDATA_LEN);

   circularBuffer.write(dataBufferWrite);
   cout << buffer << "\n";

    dataBufferWrite[0] = '0';
    circularBuffer.write(dataBufferWrite);
    cout << buffer << "\n";

    dataBufferWrite[0] = '1';
    circularBuffer.write(dataBufferWrite);
    cout << buffer << "\n";

    dataBufferWrite[0] = '_';
    for(int i = 0 ; i < 20; i++)
    {
        cout << "\n\rTest " << i << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUse() << " Free space" << (int)circularBuffer.getFreeSpace();
        int writeStatus = (int)circularBuffer.write(dataBufferWrite);
        cout << " Write Status =  " << writeStatus;
    }

     for(int i = 0 ; i < 5; i++)
    {
        cout << "\n\rTest " << i+20 << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUse() << " Free space" << (int)circularBuffer.getFreeSpace();
        int readStatus = (int)circularBuffer.read(dataBufferRead);
        cout << " Read Status =  " << readStatus<< " Data: " << dataBufferRead[0] << dataBufferRead[1] << dataBufferRead[2] << dataBufferRead[3];
    }

        for(int i = 0 ; i < 10; i++)
    {
        cout << "\n\rTest " << i << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUse() << " Free space" << (int)circularBuffer.getFreeSpace();
        int writeStatus = (int)circularBuffer.write(dataBufferWrite);
        cout << " Write Status =  " << writeStatus;
    }



    return EXIT_SUCCESS;
}