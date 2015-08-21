
/************************************************************************************
  Inclusion files
************************************************************************************/
//#include <SPI.h>
#include "Protocol.h"
#include <avr/wdt.h>

/************************************************************************************
  My function
************************************************************************************/
unsigned int  HexToInt(char c);
char IntToHex(unsigned int i);

void PutNibble(unsigned int value);
unsigned int  GetNibble();

void PutByte(unsigned int value);
unsigned int  GetByte();

void PutInt(unsigned int value);
unsigned int  GetInt();

void PutCheck();
bool GetCheck();

void PutStart();
unsigned int  GetStart();

void PutEnd();
unsigned int  GetEnd();

void PutCmd(unsigned int value);
unsigned int  GetCmd();

bool Receive(char c);
char* GetBuffer();

void ResetClientTimeOut();
bool GetClientTimeOut();

/*************************************************************************
  Variables
*************************************************************************/
char inBuffer[256];          
unsigned int  inBufferCount=0;   
char outBuffer[256];        
unsigned int  outBufferCount=0; 
bool syntax = false;
unsigned int clientTimeOut = 0;
unsigned int oldClientTimeOut = 0;

/*************************************************************************
  Convert HEX to unsigned int
*************************************************************************/
unsigned int HexToInt(char c)
{
    switch (c)
    {
        case '0':
            return (0);
        case '1':
            return (1);
        case '2':
            return (2);
        case '3':
            return (3);
        case '4':
            return (4);
        case '5':
            return (5);
        case '6':
            return (6);
        case '7':
            return (7);
        case '8':
            return (8);
        case '9':
            return (9);
        case 'A':
            return (10);
        case 'B':
            return (11);
        case 'C':
            return (12);
        case 'D':
            return (13);
        case 'E':
            return (14);
        case 'F':
            return (15);
    }
    return (0);
}

/*************************************************************************
  Convert unsigned int to HEX 
*************************************************************************/
char IntToHex(unsigned int i)
{
    switch (i)
    {
        case 0:
             return ('0');
        case 1:
            return ('1');
        case 2:
            return ('2');
        case 3:
            return ('3');
        case 4:
            return ('4');
        case 5:
            return ('5');
        case 6:
            return ('6');
        case 7:
            return ('7');
        case 8:
            return ('8');
        case 9:
            return ('9');
        case 10:
            return ('A');
        case 11:
            return ('B');
        case 12:
            return ('C');
        case 13:
            return ('D');
        case 14:
            return ('E');
        case 15:
            return ('F');
    }
    return ('?');
}

/*************************************************************************
  Write a 8 bit unsigned int
*************************************************************************/
void PutNibble(unsigned int value)
{
    outBuffer[outBufferCount++] = IntToHex(value);
}

/*************************************************************************
  Read an 8bit char
*************************************************************************/
unsigned int GetNibble()
{
    unsigned int value = 0;

    value += HexToInt(inBuffer[inBufferCount++]);
    return (value);
}

/*************************************************************************
  Write a 8 bit unsigned int
*************************************************************************/
void PutByte(unsigned int value)
{
    outBuffer[outBufferCount++] = IntToHex(value / 16);
    value = value % 16;
    outBuffer[outBufferCount++] = IntToHex(value);
}

/*************************************************************************
  Read an 8bit char
*************************************************************************/
unsigned int GetByte()
{
    unsigned int value = 0;

    value += HexToInt(inBuffer[inBufferCount++]) * 16;
    value += HexToInt(inBuffer[inBufferCount++]);
    return (value);
}

/*************************************************************************
  Write a 16 bit unsigned int
*************************************************************************/
void PutInt(unsigned int value)
{
    outBuffer[outBufferCount++] = IntToHex(value / 4096);
    value = value % 4096;
    outBuffer[outBufferCount++] = IntToHex(value / 256);
    value = value % 256;
    outBuffer[outBufferCount++] = IntToHex(value / 16);
    value = value % 16;
    outBuffer[outBufferCount++] = IntToHex(value);
}

/*************************************************************************
  Write a 16 bit unsigned long
*************************************************************************/
void PutLong(unsigned long value)
{ 
    outBuffer[outBufferCount++] = IntToHex(value / 0x1000000);
    value = value % 0x1000000;
    outBuffer[outBufferCount++] = IntToHex(value / 0x100000);
    value = value % 0x100000;
    outBuffer[outBufferCount++] = IntToHex(value / 0x10000);
    value = value % 0x10000;
    outBuffer[outBufferCount++] = IntToHex(value / 0x1000);
    value = value % 0x1000;
    outBuffer[outBufferCount++] = IntToHex(value / 0x100);
    value = value % 0x100;
    outBuffer[outBufferCount++] = IntToHex(value / 0x10);
    value = value % 0x10;
    outBuffer[outBufferCount++] = IntToHex(value);
}

/*************************************************************************
  Read a 32 bit unsigned long
*************************************************************************/
unsigned long GetLong()
{
    unsigned long value = 0;
    value =  HexToInt(inBuffer[inBufferCount++]) * 0x1000000;
    value += HexToInt(inBuffer[inBufferCount++]) * 0x100000;
    value += HexToInt(inBuffer[inBufferCount++]) * 0x10000; 
    value += HexToInt(inBuffer[inBufferCount++]) * 0x1000;
    value += HexToInt(inBuffer[inBufferCount++]) * 0x100;
    value += HexToInt(inBuffer[inBufferCount++]) * 0x10;
    value += HexToInt(inBuffer[inBufferCount++]);
    return value;
}


/*************************************************************************
  Read a 16 bit unsigned int
*************************************************************************/
unsigned int GetInt()
{
    unsigned int value = 0;

    value =  HexToInt(inBuffer[inBufferCount++]) * 4096;
    value += HexToInt(inBuffer[inBufferCount++]) * 256;
    value += HexToInt(inBuffer[inBufferCount++]) * 16;
    value += HexToInt(inBuffer[inBufferCount++]);
    return value;
}

/*************************************************************************
  Write the CheckSum char
*************************************************************************/
void PutCheck()
{
    unsigned int check;

    check = outBuffer[1];
    for (int i = 2; i < outBufferCount; i++)
      check ^= outBuffer[i];
    outBuffer[outBufferCount++] = IntToHex(check / 16);
    check = check % 16;
    outBuffer[outBufferCount++] = IntToHex(check);
}

/*************************************************************************
  Read the CheckSum char 
*************************************************************************/
bool GetCheck()
{
    unsigned int value = 0;
    unsigned int check = 0;
    
    if (inBufferCount < 4)
        return false;

    check = inBuffer[1];
    for (int i = 2; i < (inBufferCount - 3); i++)
        check ^= inBuffer[i];
    value = HexToInt(inBuffer[inBufferCount - 3]) * 16;
    value += HexToInt(inBuffer[inBufferCount - 2]);

    if (value == check)
        return true;
    else
        return false;
}

/*************************************************************************
  Write the start char
*************************************************************************/
void PutStart()
{
    outBufferCount = 0;
    outBuffer[outBufferCount++] = '[';
}

/*************************************************************************
  Get/Read the start char 
*************************************************************************/
unsigned int GetStart()
{
    inBufferCount = 0;
    return inBuffer[inBufferCount++];
}

/*************************************************************************
  Write the end frame char 
*************************************************************************/
void PutEnd()
{
    outBuffer[outBufferCount++] = ']';
    outBuffer[outBufferCount] = '\0';
}

/*************************************************************************
  Get/Read the end frame char 
*************************************************************************/
unsigned int GetEnd()
{
    return inBuffer[inBufferCount++];
}

/*************************************************************************
  Write command 
*************************************************************************/
void PutCmd(unsigned int value)
{
    outBuffer[outBufferCount++] = IntToHex(value / 16);
    outBuffer[outBufferCount++] = IntToHex(value % 16);
}

/*************************************************************************
  Read command 
*************************************************************************/
unsigned int GetCmd()
{
    unsigned int value = 0;
    
    value = HexToInt(inBuffer[inBufferCount++]) * 16;
    value += HexToInt(inBuffer[inBufferCount++]);
    return value;
}

/*************************************************************************
  Manage the reception of char
*************************************************************************/
bool Receive(char c)
{   
    bool index = false;
  
    if (inBufferCount > 255)
    {
         inBufferCount = 0;
         syntax = 0;
    }
       
    if (c == '[')
    {
        inBufferCount = 0;
        inBuffer[inBufferCount++] = c;
        syntax = true;
    }
    else if (c == ']')
    {
        if (syntax)
        {
            inBuffer[inBufferCount++] = c;

            if (GetCheck())
            {
                index= true;                
            }
                
            syntax = false;
            inBufferCount = 0;
        }
    }
    else
    {
        inBuffer[inBufferCount++] = c;
    }  
    
    return index;  
}

/*************************************************************************
  Resistuisce il buffer di uscita
*************************************************************************/
char* GetBuffer()
{
    return outBuffer;
}

/*************************************************************************
  Cancella il time out della comunicazione
*************************************************************************/
void ResetClientTimeOut()
{
}

/*************************************************************************
  Testa il time out nella comunicazione
*************************************************************************/
bool GetClientTimeOut()
{
}

