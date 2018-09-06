/************************************************************************
Filename:serial.h

Description:interface for the CSerial class.

Data:2010/05/03

Author: Hugo
************************************************************************/

#ifndef _SERIAL_H_
#define _SERIAL_H_


#define MAX_RX_BUF_LEN									(1024)
#define MAX_TX_BUF_LEN                                  (1024)


//define receive data callback function
typedef void (CALLBACK* PFNOnSerialRead)(void *pCaller, BYTE *pBuf, DWORD dwLen);

typedef struct COMM_DCB 
{
    int portNo;     /*port number*/
    int baud;	    /*baudrate*/
    int parity;     /*parity check bits*/
    int databits;	/*data bits*/
    int stopbits;  /*stop bits*/

}COMM_DCB_T, *PCOMM_DCB_T;

//CE serial communication class
class CSerial
{

public:
	CSerial();
	virtual ~CSerial();

public:
	//open port
	BOOL OpenPort(UINT portNo	= 1,	        /*port number*/
				  UINT baud		= 115200,	    /*baudrate*/
				  UINT parity	= NOPARITY,     /*parity check bits*/
				  UINT databits	= 8,		    /*data bits*/
				  UINT stopbits	= ONESTOPBIT    /*stop bits*/
				  );
	
	void ClosePort(void);//close port
	
	BOOL SetTimeouts(COMMTIMEOUTS CommTimeOuts);//set timeouts
		
	BOOL WritePort(const char *buf, DWORD dwLen); //write chars to port	

    void RegisterPort(void *pCaller, PFNOnSerialRead pfnOnserialRead);

protected:

	static  DWORD WINAPI ReadThreadFunc(LPVOID lpParam);
	static  DWORD WINAPI WriteThreadFunc(LPVOID lpParam);
	
private:
	HANDLE	m_hComm;	
	HANDLE m_hReadExitEvent;
	HANDLE m_hWriteExitEvent;
		
	HANDLE m_hReadThread;		
	DWORD m_dwReadThreadID;	
	HANDLE m_hWriteThread;
	DWORD m_dwWriteThreadID;

    LPVOID m_pCaller;
	PFNOnSerialRead m_pfnOnSerialRead;

};


#endif // _SERIAL_H_
