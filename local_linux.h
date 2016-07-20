/*==========================================================================*
 *    Copyright(c) 2006-2010, Emerson Network Power Co., Ltd.
 *                     ALL RIGHTS RESERVED
 *
 *  PRODUCT  : IDU
 *
 *  FILENAME : Local.h
 *  CREATOR  :					 DATE: 2006-10-20 
 *  VERSION  : V1.00
 *  PURPOSE  : 
 *
 *
 *  HISTORY  :
 *
 *==========================================================================*/

#ifndef __LOCAL_LINUX_H
#define __LOCAL_LINUX_H
//#import   "c:\program files\common files\system\ado\msado15.dll"   no_namespace   rename("EOF","adoEOF") 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>

#include "basetypes.h"
#include "err_code.h"
#include "halcomm.h"
//#include "pubfunc.h"
#include <pthread.h>
//#include "string.h"


#ifdef ASSERT
#undef ASSERT
#define ASSERT		SAMPLER_ASSERT
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif

//types for simulating win32
typedef const char	*LPCTSTR;
typedef void		*LPOVERLAPPED;

#define DLLExport 

//typedef BOOL ( *ENUMSIGNALPROC )( int nChanNo, int nValue, char* CardNum );
typedef struct 
{

	bool Debug;			//True if the information is for debug, False otherwise. 
						//The same structure is returned by the device driver.
	int EventType;		//3 types of events are there. 1st - example card swipe. 
						//It does not have start-stop time or active/inactive state. 
						//In the 2nd type the driver tells us that the event has started. 
						//The driver keeps sending the next events and does not tell for each event 
						//whether it is active or inactive. Finally after sometime, driver sends us 
						//another event indicating that the event has stopped. In the 3rd type of event, 
						//the driver sends active/inactive information for each event. 
						//EventType = 0,only a signal,not an event;1-common event;2-card;3-fire

	bool Active;		//True or False. False indicates that event is inactive.
	time_t EventTime;	//If this value is NULL, both for signal or for event, IDU should 
						//get the system time and report it as event/signal time.
	int EventSeverity;					
	//char * EventInfo;	//First char field will contain datatype (int, float, struct, char * etc.). 
						//If it is char *, the next char byte shall indicate the length of data.
	//char  EventInfo[30];	//First char field will contain datatype (int, float, struct, char * etc.). 
	char  *EventInfo;
}SignalEvent;

typedef struct 
{
	int nUnitNo;			//the device address
	int PortNum;			//-1 indicates that the device is an Ethernet device. Other values 0 and above, 
							//give the actual port number of the device.
	char szPortSetting[30];	//This will of type 9600, n, 8, 1 for Serial devices and 10.111.111.111:5001 for 
							//the Ethernet devices.
	char szReserve[16];		//reserve
}DeviceStruct;



DLLExport BOOL Read(HANDLE hComm, int nUnitNo, void* pvData);
//typedef BOOL (*ENUMSIGNALPROC)(int nChanel,SignalEvent sigevent, DeviceStruct devstruct);
//typedef BOOL (*ENUMSIGNALPROC)(int nChanel,SignalEvent sigevent, LPVOID lpvoid);
typedef BOOL (*ENUMSIGNALPROC)(int, SignalEvent*, LPVOID);
BOOL WriteAsc(char *szFileName, LPCTSTR pszFormat, ...);
BOOL WriteHex(char *szFileName, char *pBuf, int nLen);
time_t ChangeTime( int year, int month, int date, int hour, int min, int sec );

// the functions are defined in win32_stub.c
#define PURGE_TXCLEAR		(0x01)
#define PURGE_RXCLEAR		(0x02)

//#define ITEM_OF( array )		(int)(sizeof(array) / sizeof(array[0]))

#define Sleep( mSeconds ) ( usleep(1000*mSeconds ) )
//#define Sleep sleep

//Êä³öº¯Êý
extern "C"
{
	//bool Query(HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc , DeviceStruct devstruct);
	bool Query(HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc , LPVOID lpvoid);
	bool Test(HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc , LPVOID lpvoid);
	int SetPortAttr(HANDLE hFile,char* lpParam);

	bool DriverStop(DeviceStruct *devstruct, HANDLE handle);
	//bool Debug(int DebugCommand, char * Command, DeviceStruct *devstruct, HANDLE PortHandle, char *ValuePair, char * DebugStrings);
	bool DriverInit(DeviceStruct *devstruct, HANDLE &PortHandle, ENUMSIGNALPROC GetDeviceData);

	BOOL Control( HANDLE hComm,int nUnitNo, char *pCmdStr); 
	char* DLLInfo( );

#define PurgeComm(hFile, dwFlags)									\
	((((((dwFlags) & PURGE_TXCLEAR) == PURGE_TXCLEAR)				\
		? CommControl( (hFile), COMM_PURGE_TXCLEAR, (void*)1, 0) : 0),\
	((((dwFlags) & PURGE_RXCLEAR) == PURGE_RXCLEAR)					\
		? CommControl( (hFile), COMM_PURGE_RXCLEAR, (void*)1, 0) : 0)))

#define ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,			\
			  lpNumberOfBytesRead, lpOverlapped)				\
	(((*(lpNumberOfBytesRead) = CommRead((hFile), (lpBuffer), (nNumberOfBytesToRead))), \
	 ((CommGetLastError(hFile) == ERR_COMM_OK) || (CommGetLastError(hFile) == ERR_COMM_TIMEOUT))))

#define WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite,		\
			   lpNumberOfBytesWritten, lpOverlapped)			\
	(((*(lpNumberOfBytesWritten) = CommWrite((hFile), (lpBuffer), (nNumberOfBytesToWrite))), \
	 ((CommGetLastError(hFile) == ERR_COMM_OK) || (CommGetLastError(hFile) == ERR_COMM_TIMEOUT))))

}

#endif
