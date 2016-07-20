//////////////////////////////////////////////////////////////////////
//  版权声明：1999-2100，艾默生网络能源有限公司
//  文 件 名：IDUDRV.cpp
//  作    者：XXX
//  日    期：XXX
//  说    明：协议SO库的联编主文件
//  修改记录：
//        1.修改日期：2010-8-19
//           修改人： 刘魏伟
//           修改说明：
//              将SO库写日志方式从原来的20k循环写改为只要写满10k就不再继续记录，除非删掉原有日志文件。
//              
//     
//////////////////////////////////////////////////////////////////////


#include "local_linux.h"
extern int nMaxChanelNo;
extern char ASC_FILE[];
extern char HEX_FILE[];

//if the device is door access system,then defien _DOOR_
//#define _DOOR_
bool bTestFlag = FALSE;
enum DataType
{
	Data_Type_Int		= 0,
	Data_Type_Float		= 1,
	Data_Type_Char		= 2,
	Data_Type_Array		= 3,	//char *,i.	First byte inside indicates the length of the array EXCLUDING the first byte. 
								//In other words it indicates the length of the actual data.
	Data_Type_Struct	= 4,	//First byte indicates the number of elements inside the structure
								//The fields inside the structure follow the pattern described above

	Data_Type_Time		= 5,
};

enum SigEventType
{
	SigEvent_Type_Signal	= 0,	//It is only a signal,not an event
	SigEvent_Type_Common	= 1,	//the driver sends active/inactive information for each event
	SigEvent_Type_Card		= 2,	//It does not have start-stop time or active/inactive state,example card swipe. 
	SigEvent_Type_Fire		= 3,	//While the event existing, the driver will keep sending the event with active flag, 
									//but the extra information of the event may be different. Eg, fire alarm,when the 
									//alarm generated, an active event will be sent, and while the event existing, 
									//driver will keep sending the event(active), but in these events, the smoke density 
									//is different.when the alarm decease,an inactive event will be sent. 
};


bool Test(HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc , LPVOID lpvoid)
{
    bTestFlag = TRUE;
    
    // 调用采集函数采集数据，其中会因调试标志的置位而显示调试信息。
    BOOL bFlag = Query( hComm, nUnitNo, EnumProc, lpvoid );
    
    WriteAsc( ASC_FILE, "\r\n本次采集结束\r\n", 16 );
    WriteAsc( HEX_FILE, "\r\n本次采集结束\r\n", 16 );

    // 将调试标志复位
    bTestFlag = FALSE;
    
    return bFlag;
}

//bool Query( HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc, LPVOID lpvoid ) 
bool Query( HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc, LPVOID lpvoid ) 
//bool Query(HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc , DeviceStruct devstruct)
{

	float fData[nMaxChanelNo];
//20100709 lww 初始化Float数组 start
	//memset( fData, 0, nMaxChanelNo*sizeof(float) );

//20100709 lww 初始化Float数组 end
	//SignalEvent sigevent;
	int i=0;
	for(i=0;i<nMaxChanelNo;i++)
	{
		fData[i]=0;
	}
	SignalEvent *sigevent;
	sigevent = new SignalEvent;
	sigevent->EventInfo = new char[30];

#ifdef _DOOR_	
    union   DATA_UNION
    {
        long	uTmp;
        float	fTmp;
        LPVOID  pVoid;
        BYTE    byTest[5];
    };
    DATA_UNION	_DATA_UNION;
	long cardHi;
	long cardLow;
#endif
	
	if( Read( hComm, nUnitNo, fData) )
	{
		//sigevent->EventInfo = new char[30];
		WriteAsc(HEX_FILE,"\r\n采集成功\r\n");
#ifdef _DOOR_	//it is a special process for door access
		memcpy( (char*)&(_DATA_UNION.fTmp), (char*)&fData[0],4);
		cardHi = _DATA_UNION.uTmp;
		memcpy( (char*)&(_DATA_UNION.fTmp), (char*)&fData[1],4);
		cardLow = _DATA_UNION.uTmp;

        //门禁
		if(cardHi!=0||cardLow!=0)
		{
			sigevent->Debug = 0;
			sigevent->EventType = SigEvent_Type_Card;
			sigevent->Active = 1;

			sigevent->EventTime = ChangeTime( (int)fData[2],(int)fData[3],(int)fData[4],(int)fData[5],(int)fData[6],(int)fData[7]);


			/*sigevent->EventInfo[0] = (char)Data_Type_Struct;
			sigevent->EventInfo[1] = 4;	//4 elements in the struct
			int nPtr = 2;
			char temp[20]={0x00};		
			sprintf(temp,"%u%u",cardHi,cardLow);
			sigevent->EventInfo[nPtr] = Data_Type_Array;	//type of the first element
			nPtr++;
			sigevent->EventInfo[nPtr] = strlen(temp);	//length of the array
			nPtr++;
			memcpy( &(sigevent->EventInfo[nPtr]),temp,sigevent->EventInfo[nPtr-1] );	//卡号
			nPtr += sigevent->EventInfo[nPtr-1];

			sigevent->EventInfo[nPtr++] = Data_Type_Char;	//type of the second element
			sigevent->EventInfo[nPtr++] = (char)((int)fData[8]);// 进出标志
			sigevent->EventInfo[nPtr++] = Data_Type_Char;	//type of the 3rd element
			sigevent->EventInfo[nPtr++] = (char)((int)fData[9]);// 非法刷卡标志
			sigevent->EventInfo[nPtr++] = Data_Type_Char;	//type of the 4th element
			sigevent->EventInfo[nPtr++] = (char)((int)fData[10]);// 门号
			*/

			sigevent->EventInfo[0] = (char)Data_Type_Struct;
			sigevent->EventInfo[1] = 2;	//2 elements in the struct
			int nPtr = 2;
			sigevent->EventInfo[nPtr++] = (char)Data_Type_Float;
			memcpy( &(sigevent->EventInfo[nPtr]),(char*)&fData[0],4 );	//卡号高位
			nPtr += 4;
			sigevent->EventInfo[nPtr++] = (char)Data_Type_Float;
			memcpy( &(sigevent->EventInfo[nPtr]),(char*)&fData[1],4 );	//卡号低位

			//EnumProc( 0,sigevent,lpvoid );
			EnumProc( 0, sigevent, lpvoid );


            /*sprintf(szMsg, "-----------------[0]=%d  [1]=%d  [2]=%d------------", 
                sigevent->EventInfo[0], sigevent->EventInfo[1], sigevent->EventInfo[2]);
            puts( szMsg );
			*/

		    for(int i=2;i<nMaxChanelNo;i++)
		    {
			    sigevent->EventType = SigEvent_Type_Signal;
			    sigevent->EventInfo[0] = (char)Data_Type_Float;
			    memcpy( &(sigevent->EventInfo[1]),(char*)&fData[i],4 );	

			    //EnumProc( i,sigevent,lpvoid );
			    EnumProc( i, sigevent, lpvoid );
		    }
		}
        else
        {
		    sigevent->EventTime = 0;

		    //for(int i=8;i<nMaxChanelNo;i++)
		    for(int i=13;i<nMaxChanelNo;i++)
		    {
			    sigevent->EventType = SigEvent_Type_Signal;
			    sigevent->EventInfo[0] = (char)Data_Type_Float;
			    memcpy( &(sigevent->EventInfo[1]),(char*)&fData[i],4 );	

			    //EnumProc( i,sigevent,lpvoid );
			    EnumProc( i, sigevent, lpvoid );
		    }
        }
#endif
		//end 门禁

		//非门禁 It is for non-door
#ifndef _DOOR_
		sigevent->EventTime = 0;
		sigevent->Debug = 0;
		for(int i=0;i<nMaxChanelNo;i++)
		{
			sigevent->EventType = SigEvent_Type_Signal;
			sigevent->EventInfo[0] = (char)Data_Type_Float;
			memcpy( &(sigevent->EventInfo[1]),(char*)&fData[i],4 );	
			EnumProc( i,sigevent, lpvoid );
		}
#endif


		delete [ ]sigevent->EventInfo;
		delete sigevent;
		return TRUE;
	}
	WriteAsc(HEX_FILE,"\r\n采集失败\r\n");
	delete [ ]sigevent->EventInfo;
	delete sigevent;

	return FALSE;
}

//when D.C exit the sampling process,D.C will call this function to notify the driver.
//driver may do some clean up here.
bool DriverStop(DeviceStruct *devstruct, HANDLE hPort)
{
	return TRUE;
}
/*
//this interface is for driver debugging.
//DebugCommand=1,query data,2-send command
//when DebugCommand is 1, the 2nd paramter "Command" is meaningless.if DebugCommand is 2,the 
//content of "Command" is the command string for control() function
//ValuePair is channel number and channel value pair.where the channel value follows the same 
//encoding format
//DebugStrings is the output message to show the driver's running information.It is written by
//the driver's developer,it is something like prtintf information.


bool Debug(int DebugCommand, char * Command, DeviceStruct *devstruct, HANDLE PortHandle, char *ValuePair, char * DebugStrings)
{
	return TRUE;
}
*/


//when D.C is initializing the sampler thread,It call this function. Sometimes the communication port
//should be opened by driver,then in this function,the port should be opened and return the handle of 
//the potr to D.C. when this function returns,D.C will check if the PortHandle is NULL or not,if it is
//still NULL,that means the port should be opened by D.C itself.then D.C will open it. if the 
//PortHandle is not NULL then it means the port has been opened by driver,then D.C needn't to open port,
//it just needs to use this hanlde to communicate with driver.
//And,some of the devices will send infomation proactively,in this case,we can create a thread in this 
//function to create a thread to deal with the information sent by device proactively.


bool DriverInit(DeviceStruct *devstruct, HANDLE &PortHandle, ENUMSIGNALPROC EnumProc)
{
	/*int nErrCode;
	PortHandle =CommOpen( "comm_net_tcpip.so",
				"",
				"142.100.6.167:8001",  
				//szPortInfo,
				0x01,
				20000,
				&nErrCode);*/
	

	return TRUE;
}


//the following are not export function,they are used internally.they needn't to be modified.
time_t ChangeTime( int year, int month, int date, int hour, int min, int sec )
{
	struct tm tmTime;
	time_t ltime;
	tmTime.tm_year= year-1900;
	tmTime.tm_mon = month-1;
	tmTime.tm_mday= date;
	tmTime.tm_hour=hour;
	tmTime.tm_min=min;
	tmTime.tm_sec=sec;

	ltime = mktime(&tmTime);

	return ltime;
}

// 设置串口通信参数
// lpParam:9600,N,8,1...
int SetPortAttr(HANDLE hFile,char* lpParam)
{
	int nRet = CommControl( (hFile), COMM_SET_COMM_ATTRIBUTES, (void*)lpParam, 0);
	return nRet;

}

#define MAX_DEBUG_FILE_SIZE		(10*1024)	// 10K 20100719 lww 日志文件最大10k
#define MAX_FILE_PATH	100

/*==========================================================================*
 * FUNCTION : MakeFullFileName
 * PURPOSE  : to make a full path as pszRootDir/pszSubDir/pszFileName
 * CALLS    : 
 * CALLED BY: 
 * ARGUMENTS: OUT char       *pFullName   : buffer to save full name, size must
 *                                          be greater than MAX_FILE_PATH(256)
 *            IN const char  *pszRootDir  : if NULL or "", treat it as "."
 *            IN const char  *pszSubDir   : if NULL or "", treat it as "."
 *            IN const char  *pszFileName : 
 * RETURN   : char *: return pFullName
 * COMMENTS : 
 * CREATOR  : 
 *==========================================================================*/
char *MakeFullFileName(OUT char *pFullName,
							  IN const char *pszRootDir,
							  IN const char *pszSubDir,
							  IN const char *pszFileName)
{
	if ((pszRootDir == NULL) || (*pszRootDir == 0))
	{
		pszRootDir = ".";
	}

	if ((pszSubDir == NULL) || (*pszSubDir == 0))
	{
		pszSubDir = ".";
	}

	snprintf(pFullName, MAX_FILE_PATH, "%s/%s/%s",
		pszRootDir, pszSubDir, pszFileName);

	return pFullName;
}


static FILE *OpenDebugFile(char *pszFileName, long nMaxAllowedSize)
{
	FILE	*fp;
	char	szFullName[MAX_FILE_PATH]={0x00};
    long    nLogSize;

    /*fp = fopen( 
		MakeFullFileName(szFullName,"/var/", NULL, pszFileName),
		"a+b");*/
	strcpy(szFullName,"/data/");
	strcat(szFullName,pszFileName);
	fp = fopen( szFullName,"a+b");
    if( fp == NULL )
    {
        fprintf( stderr, "Open log file %s fail.\n", szFullName );
	    return NULL;
    }

	fseek(fp, 0l, SEEK_END);
    nLogSize = ftell(fp); // at the end of file, we use a+ open
    
	if(nLogSize >= nMaxAllowedSize )
    {
//20100719 lww 不允许so库循环读写日志，如有需要，手动删除日志文件，重新读写。 start
//		ftruncate(fileno(fp), 0L);	// cut to 0
//		rewind(fp);
		fclose( fp );
		fp = NULL;
//20100719 lww 不允许so库循环读写日志，如有需要，手动删除日志文件，重新读写。 end
	}

	return fp;
}

//写ASCII码记录文件
BOOL WriteAsc(char *szFileName, LPCTSTR pszFormat, ...)
{
	va_list     arglist;
    char        szBuf[512];
    FILE        *fp;
	int			nLen;

	if(!bTestFlag)
		return FALSE;

    //ClearWDT(); /* clear watch dog */
	fp = OpenDebugFile(szFileName, MAX_DEBUG_FILE_SIZE);
    if( fp == NULL )
    {
        return FALSE;
    }

	va_start(arglist, pszFormat);
    nLen = vsnprintf(szBuf, sizeof(szBuf), pszFormat, arglist);
	va_end(arglist);

	if(szBuf[nLen-1] == '\n')	
	{
		szBuf[nLen-1] = 0; 
	}

    fprintf(fp, "%s\n", szBuf);

    fflush( fp );
    fclose( fp );

	return TRUE;
}



//写二进制记录文件
BOOL WriteHex(char *szFileName, char *pBuf, int nLen)
{
    FILE	*fp;
	int		k;

	if(!bTestFlag)
		return FALSE;

	fp = OpenDebugFile(szFileName, MAX_DEBUG_FILE_SIZE);
    if( fp == NULL )
    {
        return FALSE;
    }

    for( k=0; k<nLen; k++ )
    {
        fprintf( fp, "%02X ", (unsigned char)pBuf[k] );
    }

	//fprintf(fp, "\n");

	fflush( fp );
    fclose( fp );

	return TRUE;
}

//************************************************************
// 函数: TimeToString
//
// 说明:
//   时间转换为字符串
//    
// 参数：   
//   tmTime      时间
//   fmt         字符串格式
//   strTime     输出字符串
//   nLenStrTime 字符串长度
//
// 返回值: 
//   字符串的时间
//
//************************************************************
// 定义日期时间格式
#define TIME_CHN_FMT		       "%Y-%m-%d %H:%M:%S"	// YYYYMMDD hhmmss

char* TimeToString(IN time_t tmTime, const char *fmt, 
				   OUT char *strTime, IN int nLenStrTime)
{
	struct tm gmTime;

	gmtime_r(&tmTime, &gmTime);

	// convert time to yyyy-mm-dd hh:mm:ss
	strftime(strTime, (size_t)nLenStrTime, fmt, &gmTime);

	return strTime;
}
/*
 char szTimeLine[80];

// get current time and convert to YYYY-MM-DD hh:mm:ss.
TimeToString(time(NULL), TIME_CHN_FMT, szTimeLine, sizeof(szTimeLine));
*/





