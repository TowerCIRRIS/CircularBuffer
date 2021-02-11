
#include <iostream>
using namespace std;

#include "CircularBuffer.h"
#include <string.h>


void writeByteTest();
void writeFixedLenghtTest();
void writeTokkenTest();
void testTokkenNotFound();

// CircularBufferHandle hCircularBuffer;

#define BUFFER_SIZE 50
unsigned char buffer[BUFFER_SIZE];

#define TESTDATA_LEN 4
unsigned char dataBufferWrite[TESTDATA_LEN] = {'A','B','C','D'};
unsigned char dataBufferRead[TESTDATA_LEN+5];
char dataBufferReadChar[100];

CircularBuffer circularBuffer;
int main(void)
{

   //writeByteTest();
   // writeFixedLenghtTest();
   //writeTokkenTest();
   testTokkenNotFound();

    return EXIT_SUCCESS;
}

void writeByteTest()
{

    circularBuffer.init(buffer,BUFFER_SIZE);

    circularBuffer.writeBytes(dataBufferWrite,TESTDATA_LEN);
    cout << buffer << "\n";

    dataBufferWrite[0] = '0';
    circularBuffer.writeBytes(dataBufferWrite,TESTDATA_LEN);
    cout << buffer << "\n";

    dataBufferWrite[0] = '1';
    circularBuffer.writeBytes(dataBufferWrite,TESTDATA_LEN);
    cout << buffer << "\n";

    dataBufferWrite[0] = '_';

    for(int i = 0 ; i < 20; i++)
    {
        cout << "\n\rTest " << i << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUsedSpace() << " Free space" << (int)circularBuffer.getFreeSpace();
        int writeStatus = (int)circularBuffer.writeBytes(dataBufferWrite,TESTDATA_LEN);
        cout << " Write Status =  " << writeStatus;
    }

    for(int i = 0 ; i < 5; i++)
    {
        cout << "\n\rTest " << i+20 << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUsedSpace() << " Free space" << (int)circularBuffer.getFreeSpace();
        int readStatus = (int)circularBuffer.readBytes(dataBufferRead,TESTDATA_LEN);
        cout << " Read Status =  " << readStatus<< " Data: " << dataBufferRead[0] << dataBufferRead[1] << dataBufferRead[2] << dataBufferRead[3];
    }

    for(int i = 0 ; i < 10; i++)
    {
        cout << "\n\rTest " << i << ": ";
        cout << "Buffer used: " << (int)circularBuffer.getUsedSpace() << " Free space" << (int)circularBuffer.getFreeSpace();
        int writeStatus = (int)circularBuffer.writeBytes(dataBufferWrite,TESTDATA_LEN);
        cout << " Write Status =  " << writeStatus;
    }


}


void writeFixedLenghtTest()
{

    int bufferUse, freeSpace, status;
    int dataLen = TESTDATA_LEN;

    circularBuffer.init(buffer,BUFFER_SIZE,MODE_FIXED_LENGHT,dataLen);

    int tesIteration = (BUFFER_SIZE / dataLen) + 1; // do 1 more iteration than the number needed to fill buffer to test overflow

    cout << "\n\r Starting Write test for " << tesIteration << " Cycles";

    for(int i = 0 ; i < tesIteration; i++)
    {
        int writeStatus = (int)circularBuffer.writeBytes(dataBufferWrite);

        bufferUse = circularBuffer.getUsedSpace();
        freeSpace = circularBuffer.getFreeSpace();

        cout << "\n\rTest " << i << ": ";
        cout << "Buffer size:" << BUFFER_SIZE << "  used: " << bufferUse << "  Free space: " << (int)circularBuffer.getFreeSpace();
        cout << "  Write Status =  " << writeStatus;

        // Pass condition
        if(writeStatus == CIRCULAR_BUFFER_WRITE_SUCCESS)
        {
            if((bufferUse == (i+1)*dataLen) && (freeSpace == BUFFER_SIZE - (i+1)*dataLen))
            {
                cout << "  --> PASS ";
            }
            else
            {
                cout << "  --> FAILE USE COUNT NOT OK \n\r";
                cout << "\n\rexpected Use:" << ((i+1)*dataLen) << "Received" << bufferUse;
                cout << "\n\rexpected FreeSpace:" << (BUFFER_SIZE - bufferUse) << "Received" << freeSpace;
                cout << "\n\n\n\r";
                return;

            }
        }else if(CIRCULAR_BUFFER_NOT_ENOUGH_SPACE)
                {
                   // Pass if buffer is full and we are trying to write
                   // buffer should be full @ BUFFER_SIZE == (i) * dataLen so when i = (BUFFER_SIZE / dataLen) 
                   if(i == (BUFFER_SIZE / dataLen))
                   {
                       cout << "  --> PASS ";
                   }
                   else
                   {
                       cout << "  --> FAIL ON WRITE STATUS+";
                       cout << "\n\r Expected to fail at " << ((BUFFER_SIZE / dataLen)) << " Failed at: " << i;
                       cout << "\n\n\n\r";
                       return;
                   }
                }
    }

    tesIteration = 2;
    cout << "\n\n\r Starting Read test for " << tesIteration << " Cycles";

    int initialBufferUse = circularBuffer.getUsedSpace();
    int initialFreeSpace = circularBuffer.getFreeSpace();

    for(int i = 0 ; i < tesIteration; i++)
    {

        int readStatus = (int)circularBuffer.readBytes(dataBufferRead);

        bufferUse = circularBuffer.getUsedSpace();
        freeSpace = circularBuffer.getFreeSpace();

        cout << "\n\rTest " << i << ": ";
        cout << "Buffer size:" << BUFFER_SIZE << "  used: " << bufferUse << "  Free space: " << (int)circularBuffer.getFreeSpace();
        cout << " Read Status =  " << readStatus << " Data: " << dataBufferRead[0] << dataBufferRead[1] << dataBufferRead[2] << dataBufferRead[3];

        if((bufferUse == initialBufferUse - (i+1)*dataLen) && (freeSpace == initialFreeSpace + (i+1)*dataLen))
        {
            cout << "  --> PASS ";
        }
        else
        {
            cout << "  --> FAILE USE COUNT NOT OK \n\r";
            cout << "\n\rexpected Use:" << (initialBufferUse - (i+1)*dataLen) << "Received" << bufferUse;
            cout << "\n\rexpected FreeSpace:" << (initialFreeSpace + (i+1)*dataLen) << "Received" << freeSpace;
            cout << "\n\n\n\r";
            return;
        }
    }

    cout << "\n\n\r Starting Empty test";
    circularBuffer.empty();

    bufferUse = circularBuffer.getUsedSpace();
    freeSpace = circularBuffer.getFreeSpace();

    if(bufferUse == 0 && freeSpace == BUFFER_SIZE)
    {
         cout << "  --> PASS "; 
    }
    else
    {
        cout << "  --> FAILE USE COUNT NOT OK \n\r";
        cout << "\n\rexpected Use:" << 0 << "Received" << bufferUse;
        cout << "\n\rexpected FreeSpace:" << BUFFER_SIZE << "Received" << freeSpace;
        cout << "\n\n\n\r";
        return;
    }

    cout << "\n\n\r Starting write loop test";

    tesIteration = ((BUFFER_SIZE / dataLen) * 3);
    int i = 0;    
    for(i = 0 ; i < tesIteration; i++)
    {
        int writeStatus = (int)circularBuffer.writeBytes(dataBufferWrite);
        int readStatus = circularBuffer.readBytes(dataBufferRead);

        bufferUse = circularBuffer.getUsedSpace();
        freeSpace = circularBuffer.getFreeSpace();

        if(writeStatus !=  CIRCULAR_BUFFER_WRITE_SUCCESS)
        {
            cout << "Test Fail at iteation: " << i << "  Status: " << writeStatus<< "  --> FAIL ";
            return;
        }
        if(dataBufferRead[0] != 'A' || dataBufferRead[1] != 'B' ||dataBufferRead[2] != 'C' ||dataBufferRead[3] != 'D')
        {
            cout << "Data Read Error at iteration : " << i << "  --> FAIL ";
            return;
        }

    }
    cout << "\n\r"<< i << " Iterations done --> PASS ";
}


void writeTokkenTest()
{

    int bufferUse, freeSpace, status;
    
    const char *  message = "Salut !  ";
    int dataLen = 1 + strcspn(message, "\0"); // sizeof(message)+1;

    circularBuffer.init(buffer,BUFFER_SIZE,MODE_CHAR_TOKKEN,0,'\0');

    int tesIteration = (BUFFER_SIZE / dataLen) + 1; // do 1 more iteration than the number needed to fill buffer to test overflow

    cout << "\n\r Starting Write test for " << tesIteration << " Cycles";

    for(int i = 0 ; i < tesIteration; i++)
    { 
        int writeStatus = (int)circularBuffer.writeChar(message);

        bufferUse = circularBuffer.getUsedSpace();
        freeSpace = circularBuffer.getFreeSpace();

        cout << "\n\rTest " << i << ": ";
        cout << "Buffer size:" << BUFFER_SIZE << "  used: " << bufferUse << "  Free space: " << (int)circularBuffer.getFreeSpace();
        cout << "  Write Status =  " << writeStatus;

        // Pass condition
        if(writeStatus == CIRCULAR_BUFFER_WRITE_SUCCESS)
        {
            if((bufferUse == (i+1)*dataLen) && (freeSpace == BUFFER_SIZE - (i+1)*dataLen))
            {
                cout << "  --> PASS ";
            }
            else
            {
                cout << "  --> FAILE USE COUNT NOT OK \n\r";
                cout << "\n\rexpected Use:" << ((i+1)*dataLen) << "Received" << bufferUse;
                cout << "\n\rexpected FreeSpace:" << (BUFFER_SIZE - bufferUse) << "Received" << freeSpace;
                cout << "\n\n\n\r";
                return;

            }
        }else if(CIRCULAR_BUFFER_NOT_ENOUGH_SPACE)
                {
                   // Pass if buffer is full and we are trying to write
                   // buffer should be full @ BUFFER_SIZE == (i) * dataLen so when i = (BUFFER_SIZE / dataLen) 
                   if(i == (BUFFER_SIZE / dataLen))
                   {
                       cout << "  --> PASS ";
                   }
                   else
                   {
                       cout << "  --> FAIL ON WRITE STATUS+";
                       cout << "\n\r Expected to fail at " << ((BUFFER_SIZE / dataLen)) << " Failed at: " << i;
                       cout << "\n\n\n\r";
                       return;
                   }
                }
    }

    tesIteration = 2;
    cout << "\n\n\r Starting Read test for " << tesIteration << " Cycles";

    int initialBufferUse = circularBuffer.getUsedSpace();
    int initialFreeSpace = circularBuffer.getFreeSpace();

    for(int i = 0 ; i < tesIteration; i++)
    {

        int readStatus = (int)circularBuffer.readChar(dataBufferReadChar);

        bufferUse = circularBuffer.getUsedSpace();
        freeSpace = circularBuffer.getFreeSpace();

        cout << "\n\rTest " << i << ": ";
        cout << "Buffer size:" << BUFFER_SIZE << "  used: " << bufferUse << "  Free space: " << (int)circularBuffer.getFreeSpace();
        cout << " Read Status =  " << readStatus << " Data: ";
        for(int j = 0 ; j < dataLen; j++)
        {
                cout <<  " char " << j << "|" <<  dataBufferReadChar[j] << "|";
        } 

        if((bufferUse == initialBufferUse - (i+1)*dataLen) && (freeSpace == initialFreeSpace + (i+1)*dataLen))
        {
            cout << "  --> PASS ";
        }
        else
        {
            cout << "  --> FAILE USE COUNT NOT OK \n\r";
            cout << "\n\rexpected Use:" << (initialBufferUse - (i+1)*dataLen) << "Received" << bufferUse;
            cout << "\n\rexpected FreeSpace:" << (initialFreeSpace + (i+1)*dataLen) << "Received" << freeSpace;
            cout << "\n\n\n\r";
            return;

        }
    }

    cout << "\n\n\r Starting Empty test";
    circularBuffer.empty();

    bufferUse = circularBuffer.getUsedSpace();
    freeSpace = circularBuffer.getFreeSpace();

    if(bufferUse == 0 && freeSpace == BUFFER_SIZE)
    {
         cout << "  --> PASS ";
        
    }
    else
    {
        cout << "  --> FAILE USE COUNT NOT OK \n\r";
        cout << "\n\rexpected Use:" << 0 << "Received" << bufferUse;
        cout << "\n\rexpected FreeSpace:" << BUFFER_SIZE << "Received" << freeSpace;
        cout << "\n\n\n\r";
        return;

    }

    cout << "\n\n\r Starting write loop test";

    tesIteration = ((BUFFER_SIZE / dataLen) * 3);
    int i = 0;    
    for(i = 0 ; i < tesIteration; i++)
    {
        //int writeStatus = (int)circularBuffer.writeBytes(dataBufferWrite);
        int writeStatus = (int)circularBuffer.writeChar(message);
        int readStatus = circularBuffer.readChar(dataBufferReadChar);

         bufferUse = circularBuffer.getUsedSpace();
         freeSpace = circularBuffer.getFreeSpace();

        if(writeStatus !=  CIRCULAR_BUFFER_WRITE_SUCCESS)
        {
            cout << "Test Fail at iteration: " << i << "  Status: " << writeStatus<< "  --> FAIL ";
            return;
        }
    }
    cout << "\n\r"<< i << " Iterations done --> PASS ";




}


void testTokkenNotFound(){


    int bufferUse, freeSpace, status;
    
    char phrase1[] = "test1234X";
    char phrase2[] = "0123456789012345678901234567890123456789012345678X";

    
    // const char *  message = "Salut !  ";
    // int dataLen = 1 + strcspn(message, "\r"); // sizeof(message)+1;
    int readLen = 0;
     unsigned int originalTail,originalHead,finalTail,finalHead;

    circularBuffer.init(buffer,BUFFER_SIZE,MODE_CHAR_TOKKEN,0,'X');
    
    //Test tokken on an empty buffer
        cout << "\n\n\n\r-----------Test tokken on an empty buffer-----------\n\r ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        originalHead = circularBuffer.hBuffer.HeadPointer; originalTail = circularBuffer.hBuffer.TailPointer;
        readLen = circularBuffer.readChar(dataBufferReadChar);
        cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        if(circularBuffer.hBuffer.TailPointer == originalTail && originalHead == circularBuffer.hBuffer.HeadPointer && readLen == 0)
        {
            cout << "\n\r --> PASS";
        }
        else{
            cout << "\n\r --> FAIL \n\n";
            return;
        }

   // Buffer is free but ptr not at 0. Filling buffer with 50 car will loop.
        cout << "\n\n\n\r-----------Buffer is free but ptr not at 0. -----------\n\r ";
        // Dummy write and read to move read/write ptr
        circularBuffer.writeChar(phrase1); circularBuffer.writeChar(phrase1);
        circularBuffer.readChar(dataBufferReadChar); circularBuffer.readChar(dataBufferReadChar);

        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        originalHead = circularBuffer.hBuffer.HeadPointer; originalTail = circularBuffer.hBuffer.TailPointer;
        readLen = circularBuffer.readChar(dataBufferReadChar);
        cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        if(circularBuffer.hBuffer.TailPointer == originalTail && originalHead == circularBuffer.hBuffer.HeadPointer && readLen == 0)
        {
            cout << "\n\r --> PASS";
        }
        else{
            cout << "\n\r --> FAIL \n\n";
            return;
        }



    //Test token on a half filled buffer, with loop : Write ptr < Read ptr with tokken in it
        cout << "\n\n\n\r-----------Test token on a half filled buffer, with loop : Write ptr < Read ptr with tokken in it-----------\n\r ";

        //phrase2[] = "01234567890123456789X";
        strcpy(phrase2,"01234567890123456789X");

        // Dummy write and read to move read/write ptr
        cout << "\n\r dummy WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        circularBuffer.writeChar(phrase1); circularBuffer.writeChar(phrase1);
        circularBuffer.readChar(dataBufferReadChar); circularBuffer.readChar(dataBufferReadChar); 
         cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r Big message  WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        originalHead = circularBuffer.hBuffer.HeadPointer; originalTail = circularBuffer.hBuffer.TailPointer;
        circularBuffer.writeChar(phrase2);
        readLen = circularBuffer.readChar(dataBufferReadChar);
        cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r Read Length: " << readLen;

        for(int i = 0; i < readLen; i++)
        {
             cout << dataBufferReadChar[i];
        }

        if(readLen == 21)
        {
            cout << "\n\r --> PASS";
        }
        else{
            cout << "\n\r --> FAIL \n\n";
            return;
        }


    
    

    //Test token on a half filled buffer, with loop : Write ptr < Read ptr without tokken in it

    //Test token on a 100% filled buffer with tokken in it
           cout << "\n\n\n\r-----------Test token on a 100% filled buffer with tokken in it-----------\n\r ";

        strcpy(phrase2,"0123456789012345678901234567890123456789012345678X");

        // Dummy write and read to move read/write ptr
        cout << "\n\r dummy WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        circularBuffer.writeChar(phrase1); circularBuffer.writeChar(phrase1);
        circularBuffer.readChar(dataBufferReadChar); circularBuffer.readChar(dataBufferReadChar); 
         cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r Big message  WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        originalHead = circularBuffer.hBuffer.HeadPointer; originalTail = circularBuffer.hBuffer.TailPointer;
        circularBuffer.writeChar(phrase2);
        readLen = circularBuffer.readChar(dataBufferReadChar);
        cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r ";
        for(int i = 0; i < readLen; i++)
        {
             cout << dataBufferReadChar[i];
        }

        if(readLen == 50)
        {
            cout << "\n\r --> PASS";
        }
        else{
            cout << "\n\r --> FAIL \n\n";
            return;
        }

    //Test token on a 100% filled buffer without tokken in it
        cout << "\n\n\n\r-----------Test token on a 100% filled buffer without tokken in it-----------\n\r ";

        strcpy(phrase2,"01234567890123456789012345678901234567890123456789");

        // Dummy write and read to move read/write ptr
        cout << "\n\r dummy WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        circularBuffer.writeChar(phrase1); circularBuffer.writeChar(phrase1);
        circularBuffer.readChar(dataBufferReadChar); circularBuffer.readChar(dataBufferReadChar); 
         cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r Big message  WRITE / READ";
        cout << "\n\rBefore";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;
        originalHead = circularBuffer.hBuffer.HeadPointer; originalTail = circularBuffer.hBuffer.TailPointer;
        circularBuffer.writeChar(phrase2,50);
        readLen = circularBuffer.readChar(dataBufferReadChar);
        cout << "\n\rAfter";
        cout << "\n\rTail: " << circularBuffer.hBuffer.TailPointer << "  Head: " << circularBuffer.hBuffer.HeadPointer;

        cout << "\n\r ";
        for(int i = 0; i < readLen; i++)
        {
             cout << dataBufferReadChar[i];
        }

        if(circularBuffer.hBuffer.TailPointer == originalTail && originalHead == circularBuffer.hBuffer.HeadPointer && readLen == 0)
        {
            cout << "\n\r --> PASS";
        }
        else{
            cout << "\n\r --> FAIL \n\n";
            return;
        }


    cout << "\n\n";

}