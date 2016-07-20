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
    int             wCmdID;         // ����ID
    int             nPort;          // �˿ں�
    int             nSpUnitID;      // ��Ԫ����(�豸��ַ)
    char            cState;         // ����״̬(0:Empty, 1:Running, 2:Completed 3:Waritting 4:Valid)
    char            szCmdStr[130];  // ����SpUnitID�������ʶ�Լ�����

    //time_t          tmArrive;       // ������յ�ʱ��

    char            cResult;        // ִ�н��
}SLogCmd;

typedef struct 
{
	int nPort ; //1501IO���Ӵ��ں�
	int nAddr ; //1501IO�豸��ַ

	int nPort1 ; //0602���Ӵ��ں�
	int nAddr1 ; //0602IO�豸��ַ

	int nCh_Air ; // �յ�         
	int nAlarm_Air;

	int nCh_GLine ; // ���׽ӵ�        
	int nAlarm_GLine;

	int nCh_BatLine ; // �����       
	int nAlarm_BatLine;

	int nCh_PowLine ; // �����      
	int nAlarm_PowLine;

	int nCh_Shake;	  //�𶯣������ţ���ǽ
	int nAlarm_Shake;

	int nCh_Illegalopen;    //�Ƿ�����
	int nAlarm_Illegalopen; 

	int nCh_BfangStatus;    //����/����״̬
	int nAlarm_BfangStatus;

	char Cmd_Light_ON[16];//�ƿ�������
	char Cmd_Light_OFF[16];
   
	char Cmd_Alarm_ON[16];//����澯����
	char Cmd_Alarm_OFF[16];
	
	char Cmd_Horn_ON[16];//���ȸ澯����
	char Cmd_Horn_OFF[16];

}Log_Param;

typedef struct 
{
	BOOL bAlarm_Air;   //True Ϊ�澯��False ����
	BOOL bAlarm_GLine;
	BOOL bAlarm_BatLine;
	BOOL bAlarm_PowLine;
	BOOL bAlarm_Shake;
	BOOL bAlarm_Illegalopen;
	BOOL bAlarm_BfangStatus;

}Log_State;

// �ص����� 
typedef BOOL ( *ENUMVALUEPROC )(int nPortNo, int nSpUnit, int nChlNo, float* fValue);   //, int nID=0, LPVOID lP=NULL);
typedef BOOL ( *ENUMCONTROLPROC )(SLogCmd *strCmd, BYTE byFlag); // int nID=0, LPVOID lP=NULL );byFlag=1,ִ������,2-��ѯ������

// SO��Init��ں���
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





