/**
 * @file CircularBuffer.h
 * @author Simon Latour
 * @brief   A circular buffer for bytes or characters with manual, fixed lenght or character tokken mode support 
 * @version 0.1
 * @date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

typedef unsigned char cbError_t;

#define CIRCULAR_BUFFER_WRITE_SUCCESS	    0
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
     
    /**
     * @brief Function ot initialize the circular buffer
     * 
     * @param buffer        Pointer to the data storage buffer
     * @param bufferSize    Size of the buffer
     * @param mode          Circular buffer can be used  in 3 modes
     *                      Mode 1 - Manual lenght mode (default):  Need to specify the number of bytes every write/read, only writeBytes(data,len)/readBytes(..) function can be used
     *                      Mode 2 - Fixed length: Need to specify lenght at init with dataLenght. Can use the  writeBytes(data)/read(data), no need to specify lenght 
     *                               Usefull when handling fixed size packed to write / read 1 packet at the time
     *                      Mode 3 - Char packet delimited by char tokken defined at init. USe writeChar/read without needing to specify lenght. This will write/read until tokken
     *                               is found. The Tokken written in buffer and returned when buffer is read.  
     *                               Ex1: if tokken is '\0' and data is "HELLO X ", the read will return size 9 and buffer[8] will have '\0'
     *                               Ex2: if tokken is 'X' and data is "HELLO X ", the read will return size 7 and buffer[6] will have 'X'   
     * @param dataLength    By default 0 because deffautl mode is manual
     * @param token         By detault is '\0' to accomodate standard string but can be any character.
     */
    void init(unsigned char *buffer, unsigned int  bufferSize, CircularBufferMode mode = MODE_MANUAL_LENGHT, int dataLength = 0, char token = '\0');
    
    /**
     * @brief Get the Used spaced in the buffer
     * 
     * @return int the number off bytes used
     */
    int getUsedSpace();

    /**
     * @brief Get the Free Space in the buffer
     * 
     * @return int the number of bytes available
     */
    int getFreeSpace();

    /**
     * @brief Empties buffer (Reset the bufffer write and read point)
     * \note Does not delete data in buffer
     */
    void empty();

    /**
     * @brief Write characters in buffer
     * 
     * @param data          Pointer to the data to write
     * @param lenght        the number of char to write. Default:0 --> for Token mode or fixed lenght. If manual mode is used, lenght need to be specified
     * @return cbError_t    status code 
     */
    cbError_t writeChar(const char *data, unsigned int lenght = 0);

    /**
     * @brief Write a byte (unsigned char) in bufer
     * 
     * @param data          Pointer to the data to write
     * @param lenght        the number of char to write. Default:0 --> for fixed lenght. If manual mode is used, lenght need to be specified
     * @return cbError_t    status code 
     */
    cbError_t writeBytes(const unsigned char *data, unsigned int lenght = 0);
 
    /**
     * @brief Thif functions reads char in buffer
     * 
     * @param data   Pointer to the buffer that wil receive the data
     * @param lenght Number of characters to read. 
     *                  0 by default uses configured mode  ( fixed length or tokken mode)
     *                  Warning: Setting a numbere here will forced manual mode an override configured mode
     * @return unsigned int the number of char read
     */
    unsigned int readChar(char *data, unsigned int lenght=0);

    /**
     * @brief This functions read the buffer until the desirred tokken character is found
     *      Warning: Using this function will force tokken mode and may create issies if Fixed lenght mode is used
     * 
     * @param data              Pointer to the buffer that wil receive the data
     * @param tokken            The tooken that is searched
     * @return unsigned int     The number of char read
     */
    unsigned int readToTokken(unsigned char *data, char tokken);

    /**
     * @brief This function reads bytes in buffer
     * \note this function only works with manual mode or fixed lenght mode. Not Tokken mode
     * 
     * @param data              Pointer to the buffer that wil receive the data
     * @param lenght            The number of bytes to read.
     *                          0 by default to fixed length mode 
     * @return unsigned int     The number of bytes read
     */
    unsigned int readBytes(unsigned char *data, unsigned int lenght = 0);

};



#endif



