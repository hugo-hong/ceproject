/************************************************************************
Filename:ECCP.h

Description:interface for car comm protocol .

Data:2013/08/08

Author: Hugo
************************************************************************/

#ifndef __ECCP_H__
#define __ECCP_H__
#include <assert.h>
#include "serial.h"
#include "loopqueue.h"

#define DEFAULT_BUF_LEN                 (1024)

#define CCP_DATA_ZONE_TAG               'CCPD'

#define CCP_FILE_MAP                    _T("CCP_DATA_ZONE")
#define CCP_LOCK_NAME                   _T("CCP_MUTEX_LOCK")

enum Caller_ID_E
{
    HOME,
    RADIO,
    BACKCAR,
    EPLAYER,
    IPOD,
    TV,
    SETTING,
    MAX_ID
};

enum Caller_Role_E
{
    MASTER,
    SLAVER,
};

#define Is_Valid_ID(ID)             (ID >= HOME && ID < MAX_ID)

#define CMD_HEAD_TAG0               0xA5
#define CMD_HEAD_TAG1               0x5A
#define CMD_TAIL_TAG0               0x5A
#define CMD_TAIL_TAG1               0xA5

enum Comm_CMD_ID_E
{
    CMD_UNKNOWN,
    CMD_INIT,//init 
    CMD_POWER, //power state
    CMD_BRAKE, //brake state
    CMD_TEMPERATURE,
    CMD_RADIO,
    CMD_BACKCAR,
    CMD_DISK,
    CMD_IPOD,
    CMD_TV,
    CMD_CANBUS,
    CMD_TOUCHSCREEN,
    CMD_KEY,
    CMD_STEER,
};

enum Comm_CMD_PARAM_E
{
    //INIT cmd param
    INIT_PARAM_COMM = 1,
    INIT_PARAM_OPEN_BACKLIGHT,
    INIT_PARAM_INIT_DATA,
};

//define register callback function
typedef void (CALLBACK* PFNOnReceiveCmdInd)(void *pCaller, BYTE *pCmd, DWORD dwLen);

typedef struct _Caller_Node_T
{
    int nCallerId;
    int nCmdId;
    LPVOID pCaller;
    PFNOnReceiveCmdInd pfnReceiveCmdInd;
    _Caller_Node_T *pNext;
}Caller_Node_T, *PCaller_Node_T;

class CarCommProtocol 
{
public:
    CarCommProtocol();
    virtual ~CarCommProtocol();

    void PreInit(void);
    BOOL Init(void);
    void DeInit(void);
    BOOL PostInit(void);

public:   
    void AddRef(void);
    void DelRef(void);

protected:
    DWORD ProcessData(char *pBuf, int length);
    DWORD ParserData(char *pBuf, int length);
    DWORD DispatchData(char *pBuf, int length);

    BYTE CalcChecksum(char *pCmd, int length);
    BOOL DoChecksum(char *pCmd, int length);
    
    void MakeCommand(char *pData, int length, char *pCmd, int& nCmdLen);
         

    friend void OnRxDataInd(void *pCaller, PBYTE pData, DWORD dwLen);
    friend DWORD CCP_ReceiveDataInt(LPVOID lpPara);
    friend DWORD CCP_ProcessCmdInt(LPVOID lpPara);

private:
    CSerial m_serial;
    CLoopQueue m_loopQueue;    

    int m_eRole;
    
    HANDLE m_hReceiveThread;
    DWORD m_dwReceiveThreadId;

    HANDLE m_hProcessThread;
    HANDLE m_hExitProcessEvent;

};

typedef struct  
{
    DWORD m_dwTag; //tag flag for data zone
   
    DWORD m_dwRef;

    HANDLE m_ahRxEvent[MAX_ID];
    HANDLE m_ahTxEvent[MAX_ID];
    DWORD m_dwRxDataLen;
    DWORD m_dwTxDataLen;
    char m_rxData[DEFAULT_BUF_LEN];    
    char m_txData[DEFAULT_BUF_LEN];

}CCP_ENTRY_DATA_T, *PCCP_ENTRY_DATA_T;

typedef struct
{    
    HANDLE m_hFileMap;
    LPVOID m_pEntryData;
    Caller_Node_T m_callerList;
    COMM_DCB_T m_commDcb;
    CarCommProtocol m_ccp;    
}CCP_CONTEXT, *PCCP_CONTEXT;


extern "C"
{
BOOL CCP_Entry(UINT portNo,	            /*port number*/
                UINT baudRate,			/*baudrate*/
                UINT parity,			/*parity check bits*/
                UINT dataBits,			/*data bits*/
                UINT stopBits			/*stop bits*/);

void CCP_Exit(void);

BOOL CCP_RegisterCaller(int nCallerId, 
                        LPVOID pCaller,
                        PFNOnReceiveCmdInd pfnReceiveCmdInd);

BOOL CCP_SendCommand(int nCallerId, UINT32 u4Param1, UINT32 u4Param2);
};

#endif //__ECCP_H__