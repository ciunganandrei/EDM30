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


#include <stdio.h>

#include <stdarg.h>

#include "basetypes.h"
#include <pthread.h>


//types for simulating win32
typedef const char	*LPCTSTR;
typedef void		*LPOVERLAPPED;

#define DLLExport 


typedef struct
{
    int             wCmdID;         // 命令ID
    int             nPort;          // 端口号
    int             nSpUnitID;      // 单元编码(设备地址)
    char            cState;         // 命令状态(0:Empty, 1:Running, 2:Completed 3:Waritting 4:Valid)
    char            szCmdStr[130];  // 包含SpUnitID和命令标识以及参数

    //time_t          tmArrive;       // 命令接收的时间

    char            cResult;        // 执行结果
}SLogCmd;

typedef struct 
{
	int nPort ; //1501IO所接串口号
	int nAddr ; //1501IO设备地址

	int nPort1 ; //0602所接串口号
	int nAddr1 ; //0602IO设备地址

	int nCh_Air ; // 空调         
	int nAlarm_Air;

	int nCh_GLine ; // 防雷接地        
	int nAlarm_GLine;

	int nCh_BatLine ; // 电池线       
	int nAlarm_BatLine;

	int nCh_PowLine ; // 配电线      
	int nAlarm_PowLine;

	int nCh_Shake;	  //震动，如破门，破墙
	int nAlarm_Shake;

	int nCh_Illegalopen;    //非法开门
	int nAlarm_Illegalopen; 

	int nCh_BfangStatus;    //布防/撤防状态
	int nAlarm_BfangStatus;

	char Cmd_Light_ON[16];//灯开关命令
	char Cmd_Light_OFF[16];
   
	char Cmd_Alarm_ON[16];//声光告警命令
	char Cmd_Alarm_OFF[16];
	
	char Cmd_Horn_ON[16];//喇叭告警命令
	char Cmd_Horn_OFF[16];

}Log_Param;

typedef struct 
{
	BOOL bAlarm_Air;   //True 为告警，False 正常
	BOOL bAlarm_GLine;
	BOOL bAlarm_BatLine;
	BOOL bAlarm_PowLine;
	BOOL bAlarm_Shake;
	BOOL bAlarm_Illegalopen;
	BOOL bAlarm_BfangStatus;

}Log_State;

// 回调函数 
typedef BOOL ( *ENUMVALUEPROC )(int nPortNo, int nSpUnit, int nChlNo, float* fValue);   //, int nID=0, LPVOID lP=NULL);
typedef BOOL ( *ENUMCONTROLPROC )(SLogCmd *strCmd, BYTE byFlag); // int nID=0, LPVOID lP=NULL );byFlag=1,执行命令,2-查询命令结果

// SO库Init入口函数
typedef BOOL (*INITSOPROC)(ENUMVALUEPROC, ENUMCONTROLPROC);
typedef BOOL (*STOPSOPROC)( );

#ifdef __cplusplus
extern "C"
{
#endif


DLLExport BOOL DriverInit( ENUMVALUEPROC fnGetVal, ENUMCONTROLPROC fnLogCtrl );
DLLExport BOOL DriverStop( );

#ifdef __cplusplus
}
#endif





