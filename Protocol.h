
/************************************************************************************
  Function prototypes
************************************************************************************/
unsigned int  HexToInt(char c);
char intToHex(unsigned int i);

void PutNibble(unsigned int value);
unsigned int  GetNibble();

void PutByte(unsigned int value);
unsigned int  GetByte();

void PutInt(unsigned int value);
unsigned int  GetInt();

void PutLong(unsigned long value);
unsigned long GetLong();

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

extern char outBuffer[];  
