/**************************************************************************
��Ȩ����:1999-2100����Ĭ��������Դ���޹�˾
�豸����:EDM30E������һ�廯�Ž�(**��ES2100�޸Ķ���**)
�� �� ��:EDM30E.cpp(��ʹ��Ctrl+H�滻���ܽ����ļ�����ȫ������Ϊ�ļ��͹��̵�����)
��������:VC6.0
��    ��:LYG
�� �� ��:V4.O0
��      ��:2005-11-25
�����ص�:����
Ӧ�õص�:
˵      ��:��Э�������ES2000�Ž��������޸Ķ��ɡ�

  �޸ļ�¼:�޸��ˡ����ڡ��޸�ԭ�򡢰汾�䶯�����
    1. (�����ES2100���޸ļ�¼)
       PJC 2007.5.20 ��������ͨ�潭·IDC�������IDU ���Բ��ɹ��޸� V4.00->V4.01->V5.00

    2. L&L 2008.04.08 һ�廯�Ž�Э��(���ο�����).�����ͺ�:EDM-11Z.�����ͺ�:EDM-30E.
       �汾��: ES2100.cpp => EDM30E.cpp
       �����޸ĵĵط�,ȫ�� ////L&L ADD/////��ǳ�����.
       ����������޸�û�б��

    3. L&L 2010.03.17 ����Ž����Ž����Լ���ǰ�����⣬��������Ϊ5.40
        1��ʱ������10s�����Զ�Уʱ��
        2���ӿ�ʱ�������п�����ɾ����ǰ�Ŀ����ټ��¿�
        3���������ɼ����ֻ��һ���һ�Ρ�
        4��MDB����������һ���ź�ȫ��һ�����ź�
        5, ����������ɼ�˳�򣬽�ʱ�����ɼ��������Ž��Զ�Уʱ���ܲ�ƥ��
        6, �Զ�У��ʱ�����ֵΪ30s

    4. qilei 2010.08.17 ����������н����Ž��⣬�޸���������
        1��ɾ��Control_old�������ҿ�����������fcloseall���á���Linux��fcloseall������ʵ��fflush������δʵ��fclose������
        2����ÿ��Control���ö�����ǰ����д��ini�ļ�����Ϊ������ı�ʱд��ini�ļ�
        3�������汾��Ϊ5.50
    5. qilei 2010.08.25 �޸�Read�������Զ�Уʱ���ִ��룬tm�ṹ�ֶ����䣻
    6. qilei 2010.09.03 �������룬�����յ������豸�쳣���ص���Ϣ���ߴ�����Ϣ�������ٽ������ݽ��գ����3�Σ����������豸�ĸ��ţ�
    7. qilei 2010.09.06 ������ջ����������ݣ�byData�����ȣ��Խ��ո�������ݰ����������ݣ������޸Ľ����֮ǰ��һ������:
        ��ͬһ���������豸�����Ž���Ϣʱ��������ݰ�̫�󣬻�����Ž�SO���ջ�������������봫�ݳ��Ȳ������ڻ��������ȣ���
    8. qilei 2010.09.06 ���ƴ�����Ϣ�ַ���������Э���ĵ�����04��05��E7�Ĵ���˵����
    9. zhj   2010.09.15 �޸����Bug,�������쳣ʱ������3�Ρ����ӷ������ݰ����ȵ��жϡ�������ͨ V5.50-->V5.60
    10. qilei 20101221 �����͵�ַΪ255ʱ��������ַ��������飻V5.60-V5.70��
    11.cxd 2013.10.31 ����BIN�ļ�V5.71
	12��WXG 2015-3-3 �������������������ӿ��������ź� V5.71--��V5.72
	13��WXG 2015-07-22 �޸�ɾ�������������� V5.72--��V6.00

  ˵    ��:�����ó����ļ���ɵ���Ҫ���ܵ�
  ��    ��:

    �����嵥:
    1. �汾��Ϣ����:
        DLLExport char* DLLInfo( void )
    2. V4.0/TSR/OCE ���ݲɼ�����:
        DLLExport BOOL Read( HANDLE hComm, int nUnitNo, void *pData )
    3. V4.0/PowerStar ���ƺ���������������:
        DLLExport BOOL Write( HANDLE hComm, int nUnitNo, char *pCmdStr )
    4. V4.1/V5.0 PowerStar ���ݲɼ�����:
        DLLExport BOOL Query( HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc, LPVOID lpvoid )
    5. V4.1/V5.0/TSR/OCE ���ƺ���:
        DLLExport BOOL Control( HANDLE hComm, int nUnitNo, char *pCmdStr )
    6. ���ٲ��Բɼ�����
        DLLExport BOOL Test( HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc, LPVOID lpvoid )

*****************************************************************************/
#include <stdio.h>
#define _DEBUG_
#define _LINUX_
#include "local_linux.h"
#include "halcomm.h"
#ifdef _LINUX_
    #include "local_linux.h"
    extern bool bTestFlag;
#else
    // ����ͷ�ļ��Ķ��壬������ VC++ �� BC++����
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <conio.h>
    #include <math.h>

    #ifdef _WINDOWS
        #define _DLL_
    #endif

    #ifdef _CONSOLE
        #define _DLL_
        #define _DEBUG_
    #endif

    #define _OCE_        // OCE����������

    #ifdef  _DLL_
        #undef  _OCE_    // DLL����������
    #endif

    #ifdef  _TSR_
        #undef  _OCE_    // TSR����������
    #endif

    #ifdef  _OCE_
        #define  _TSR_   // TSR����������
    #endif

    // ��̬��ʹ�ò���
    #ifdef _DLL_
        #pragma comment(lib,"Ws2_32.lib")
        #include "local.h"
        #include "snuoci5.h"
    // ���ٲ��Ա�־:TURE�����ٲ������У�Ҫ������Ϣ��FALSE����ͨ���С�
        BOOL bTestFlag = FALSE;
        extern int nExtOci5ID;
    #endif

    #ifndef _OCE_
        #ifdef _TSR_
            #include "struct.h"
        #endif
    #endif

    #ifdef _OCE_
        #include "Comset.h"
        #ifndef _TSR_
            #define _TSR_
        #endif
    #endif
#endif

#ifndef _TSR_
    // ���ٲ����õ����ݼ�¼�ļ���������ݾ���Э����ģ�
    char HEX_FILE[]={"EDM30E.HEX"};
    char ASC_FILE[]={"EDM30E.ASC"};
    char ErrLog_FILE[]={"EDM30E.Log"};
#endif

//////////////////////////////////////////////////////////////////////////////
#ifndef _LINUX_
    const int nMaxChanelNo = 600;    // ���ɼ��źŸ���
#else
    int nMaxChanelNo=600;
#endif

/////////////////////////////////////////////////////////////////////////////
//#define MAXINFOLEN 4096
#define MAXINFOLEN 512

#define BOITRYTIMES  100

//#define nMaxChanelNo 200 //�����������ݴ�С��Ϊ150��FLOAT

//ȫ�ֱ�������
BYTE g_byBOI = 0x7E;        // ��ͷ�ַ�
BYTE g_byEOI = 0x0D;        // ��β�ַ�
BYTE g_byVersion = 0x10;    // �汾0.0,ʵ��δʹ��

//BYTE byaPublicBuffer[MAXINFOLEN];
//BYTE g_byaTypeUnitRelation[256];

typedef struct
{
    int  nCmdNo;        // ������
    BYTE byCID2;        // CID2
    BYTE byCmdGroup;    // Command Group
    BYTE byCmdType;     // Command Type
    BYTE byData[50];    // Data
    int  nDataLen;      // Data Len
    WORD  nRetLen;       // Return Data Len
}
STRSET;

// �ɼ����:
STRSET strRead[] =
{
    //����ȡʱ��Ȳ����Ե�
    // 0��ȡSM���ţ����������Բ���
  //{ 0, '\x4a', '\xf2', '\xe0', "\x00", 1, 8 },
    { 0, '\x4a', '\xf2', '\xe8', "\x00", 1, 5 },
        // 1��ȡSM��һ����ʷ��¼:�Զ���ʽ
    { 1, '\x4a', '\xf2', '\xe2', "", 0, 14 },
    // 2��ȡSM�ļ��״̬
    { 2, '\x4a', '\xf2', '\xe7', "\x00", 1, 3 },
    // 3��ȡSM��ʵʱ��
  //{ 3, '\x4a', '\xf2', '\xe8', "\x00", 1, 5 },
    { 3, '\x4a', '\xf2', '\xe0', "\x00", 1, 8 },

	// 4��ȡSM����������û���Ŀ
    { 4, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },

    // 4��ȡSM����ʷ��¼��Ͱ
    //{ 4, '\x4a', '\xf2', '\xe1', "", 0, 9 },
    // 5��ȡSM��һ����ʷ��¼:ָ��λ�ã�����E1������
    { 5, '\x4a', '\xf2', '\xe2', "\x01\x00", 2, 14 },
    // 6��ȡSM��һ�鹤����׼��ʱ��
    { 6, '\x4a', '\xf2', '\xe3', "\x01", 1, 16 },
    // 7��ȡSM�ķǹ�����׼��ʱ��
    { 7, '\x4a', '\xf2', '\xe4', "\x01", 1, 16 },
    // 8��ȡSM����������û���Ŀ
    { 8, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },
    // 9��ȡSM��ָ���û�����
    { 9, '\x4a', '\xf2', '\xe6', "\x01\x00", 2, 16 },
    // 10��ȡSM���Ƶ���(����)֮�趨ÿ�����ڹ̶�����Ϣ��
    { 10, '\x4a', '\xf2', '\xe9', "\x00", 1, 2 },
    // 11��ȡSM���Ƶ���(����)֮�趨��Ϣ��(��������Ϣ�ճ���)
    { 11, '\x4a', '\xf2', '\xea', "\x00", 1, 16 },  // ���ո����ɱ�
    // 12��ȡSM���Ƶ���(����)֮����һ�������յ�׼��ʱ���
    { 12, '\x4a', '\xf2', '\xeb', "\x00\x01", 2, 24 },  // ���ո����ɱ�
};

// �������:
STRSET strSet[] =
{
    // 0����Ȩ��ȷ��
    { 60, '\x48', '\xf0', '\xe0', "\x00\x00\x00\x00\x00", 5, 0 },
        // 1����ȡ��SM������Ȩ��
    { 61, '\x48', '\xf0', '\xe1', "", 0, 0 },
    // 2�޸�SM�ķ�������
    { 62, '\x48', '\xf0', '\xe2', "\x00\x00\x00\x00\x00\x00", 6, 0 },

    // 3��������ʱ��:1999��08��24������2��17:00:00
    { 32, '\x49', '\xf1', '\xe0', "\x19\x99\x08\x24\x02\x17\x00\x00", 8, 0 },
    // 4���ù�����׼��ʱ��:00:00-23:59
    { 64, '\x49', '\xf1', '\xe1', "\x01\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", 17, 0 },
    // 5���÷ǹ�����׼��ʱ��:00:00-23:59
    { 65, '\x49', '\xf1', '\xe2', "\x01\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", 17, 0 },
    // 6�����û�
    { 36, '\x49', '\xf1', '\xe3', "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16, 0 },


    // 7ɾ���û�
    //LYG,2005-12-7
    { 37, '\x49', '\xf1', '\xe4', "\x00\x00\x00\x00\x00\x00", 6, 0 },



    // 8�趨SM���Բ���
    { 68, '\x49', '\xf1', '\xe5', "\x00", 1, 0 },
    // 9�趨�����̵���ִ��ʱ��:(0.1-25.5S)
    { 69, '\x49', '\xf1', '\xe6', "\x0a", 1, 0 },
    // 10�趨���ŵȴ�����ʱ��:(0.1-25.5S)
    { 30, '\x49', '\xf1', '\xe7', "\x0a", 1, 0 },
    // 11�趨���ⱨ����ȷ��ʱ��:(0.1-25.5S)
    { 71, '\x49', '\xf1', '\xe8', "\x0a", 1, 0 },
    // 12�趨��ر�������ʱʱ��:(0.1-25.4S��0���رգ�255��ȡ��)
    { 72, '\x49', '\xf1', '\xe9', "\x0a", 1, 0 },
    // 13�趨SM��һ��������Ϣ��:����������
    { 73, '\x49', '\xf1', '\xea', "\x06\x07", 2, 0 },
    // 14�趨SM�Ľڼ���:10��1��
    { 74, '\x49', '\xf1', '\xeb', "\x10\x01", 2, 0 },
    // 15ɾ��SM�ڵ�һ��ڼ���:10��1��
    { 75, '\x49', '\xf1', '\xec', "\x00\0x00", 2, 0 },
    // 16Զ�̿���
    { 10, '\x49', '\xf1', '\xed', "\x01", 1, 0 },
    // 17����ϵͳ֧�ָ�Ӧ�������༰��Ƭ��ŵĻ�ȡ����
    { 77, '\x49', '\xf1', '\xee', "\x00", 1, 0 },
    // 18�趨�����ؿ���/�رշ���
    { 78, '\x49', '\xf1', '\xef', "\x00\0x10", 2, 0 },
    // 19�趨SM�ڼ�¼����ָ��:�����ֽڷ�ʽ
    { 79, '\x49', '\xf1', '\xf0', "\x01\0x00\0x00", 3, 0 },
    // 20�趨SM�ڰ�����׼����ʱ����б�
    { 80, '\x49', '\xf1', '\xf1', "\x00\x03\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff \xff\xff\xff\xff \xff\xff\xff\xff", 14, 0 },

    // 21�趨ϵͳʹ�õĸ�Ӧ������:26��36��44��64
    { 81, '\x49', '\xf1', '\xf2', "\x26", 1, 0 },
    // 22�趨ϵͳ��Ӧ����ŵĻ�ȡ����
    { 82, '\x49', '\xf1', '\xf3', "\x00", 1, 0 },
    // 23�趨��������I2����Ч��ƽ
    { 83, '\x49', '\xf1', '\xf4', "\x00", 1, 0 },
    // 24�趨ˢ������ʱ���Ƿ������û�����:4��0��9
    { 84, '\x49', '\xf1', '\xf5', "\x00", 1, 0 },
    // 25�趨�����Ӧ���ڱ���ʱ����Ч�����ƽ
    { 85, '\x49', '\xf1', '\xf6', "\x00", 1, 0 },
    // 26�趨�ſ��ظ�Ӧ���ڿ���״̬ʱ����Ч�����ƽ
    { 86, '\x49', '\xf1', '\xf7', "\x00", 1, 0 },
    // 27�趨�Ƿ������������I2��״̬
    { 87, '\x49', '\xf1', '\xf8', "\x00", 1, 0 },
    // 28�趨�ſص������(����)����
    { 88, '\x49', '\xf1', '\xf9', "\x00", 1, 0 },
    // 29�趨�Ƿ���Ӻ����Ӧ����״̬
    { 89, '\x49', '\xf1', '\xfa', "\x00", 1, 0 },
    // 30�趨�Ƿ�����ſ��ظ�Ӧ����״̬
    { 90, '\x49', '\xf1', '\xfb', "\x00", 1, 0 },

    // �����ȡһЩRead���������Ĺ��ܣ�����ʱ������������
    // 31��ȡSM����ʷ��¼��Ͱ
    { 300, '\x4a', '\xf2', '\xe1', "\x00", 1, 9 },
    // 32��ȡSM�ķǹ�����׼��ʱ��
    { 301, '\x4a', '\xf2', '\xe4', "\x01", 1, 16 },
    // 33��ȡSM����������û���Ŀ
    { 302, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },
    // 34��ȡSM���Ƶ���(����)֮�趨ÿ�����ڹ̶�����Ϣ��
    { 303, '\x4a', '\xf2', '\xe9', "\x00", 1, 2 },
    // 35��ȡSM���Ƶ���(����)֮�趨��Ϣ��(��������Ϣ�ճ���)
    { 304, '\x4a', '\xf2', '\xea', "\x00", 1, 16 },  // ���ո����ɱ�

    // 36��ȡSM��һ�鹤����׼��ʱ��
    { 305, '\x4a', '\xf2', '\xe3', "\x01", 1, 16 },
    // 37��ȡSM��һ����ʷ��¼:ָ��λ�ã�����E1������
    { 306, '\x4a', '\xf2', '\xe2', "\x01\x00", 2, 12 },
    // 38��ȡSM��ָ���û�����
    { 307, '\x4a', '\xf2', '\xe6', "\x01\x00", 2, 16 },
    // 39��ȡSM���Ƶ���(����)֮����һ�������յ�׼��ʱ���
    { 308, '\x4a', '\xf2', '\xeb', "\x00\x01", 2, 24 },  // ���ո����ɱ�
};
#define nMaxCmdNo   40

/*****************************************************************/
// ��������:GetProtocolVer
// ��������:Э��汾�Ż�ȡ����
// �������:Ver - �汾��,
//           Code - Э�����
// �������:
// ��    ��:
// ��    ��:
/*****************************************************************/
DLLExport BOOL GetProtocolVer( char* Ver,    // �汾��
                              char* Code )   // Э�����

{
    strcpy(Ver,"6.00");
    strcpy(Code,"OEDM30E6-00");

    return TRUE;
}

char Info[] =
{
        "���������������������\n"
        "  EDM30E������һ�廯�Ž�         \n"
        "                               \n"
        "  PSMS4.39                     \n"
        //"  �汾: V6.00                   \n"
"  TEST: T1.00                     \n"
        //"  Э�����:OEDM30E6-00                     \n"
        "                                     \n"
        "  ͨѶģʽ: ����                      \n"
        "  ���䷽ʽ: RS485               \n"
        "  ͨѶ����: 9600(�ɵ�),n,8,1        \n"
        "  ��ַ:�ɵ�                        \n"
        "                               \n"
        "  ���������:LYG                   \n"
        "  ��������:  2005-11-25            \n"
        "  �� �� ��:L&L,zhj                 \n"
        "  �޸�ʱ��:2010.09.17              \n"
        "                                     \n"
        "  �˳���ֻ֧��PSMS4.39�ӿ�          \n"
        "  �������������ҵ��̨֧��         \n"
        "  ������������ʱ��:               \n"    // �����ڴ˺�������Ϣ!
        "                                    \n"    // �뱣������(�����ڴ���)
};

#ifndef _TSR_
//*****************************************************************
// ��������:DLLInfo();
// ��������:��̬��汾�н���Ϣ�� Info ����������汾��Ϣ�ȱ�־��
// �������:Info--�汾��Ϣ����
// �������:
// ����:    �汾��Ϣ����
// ����:
//*****************************************************************
DLLExport char* DLLInfo( )
{
    int nStrLen = strlen( Info );
    sprintf( Info+nStrLen-30, "%s ", __DATE__ );
    strcat( Info, __TIME__ );
    strcat( Info, " \n" );

    return Info;
}
#endif    // end of #ifdef _DLL_

char* caErrMessage[] =
{
        "����",           // 00
        "�汾��",       // 01
        "У���",       // 02
        "����У���",   // 03
        "��Ч��CID2",   // 04
        "����ʶ��������ʽ",    // 05
        "����������Ч", // 06
        "����Ȩ�޴���", // 07
        "����ȷ�ϴ���", // E0
        "�����޸Ĵ���", // E1
        "�洢�ռ���",   // E2
        "�޸�ʧ��",     // E3
        "�洢�ռ��",   // E4
        "����Ӧ��Ϣ��", // E5
        "�û�ID�ظ�",   // E6
        "��ͬ���ڸ���ͬ�û�", // E7
        "��������",     // EF
};

#define RETNORMAL   0x00    // ��������
#define nErrorEF    0xEF    // ����ʧ��
#define nErrorEE    0xEE    // ����ʧ��
#define nErrorED    0xED    // �����쳣  �ŻԽ�  2010.09.15
#define nErrorEC    0xEC    // ��������  �ŻԽ�  2010.09.15
#define nNoRecord   0xE5    // �޼�¼

void ASSERT(BOOL bFlag)
{
#ifdef _DEBUG_
    if (!bFlag)
    {
        printf("Assert Error!\n");
        getchar();
        exit(1);
    }
#endif
    return;
}


//////////////////////////////////////////////////////////////////////////////////////
//����:���ַ���S��cSepΪ�ָ����ֶΡ�
//���:S-Դ�ַ�����D-��һ���ַ�����cSep-�ָ�����
//����:��һ���ַ����ĸ�����
int StrToK(char *S, char *D, char cSep)
{
    int i = 0;
    while (S[i] && S[i] != cSep)
    {
        D[i] = S[i++];
    }
    D[i] = 0;

    // return S[i] ? i+1 : 0;
    if (S[i])
    {
        return i + 1;
    }

    return 0;
}

/*******************************************************************
��������:CheckSum
����˵��:����У���
����˵��:�����Ļ�����,��Ҫ����ĳ���
���˵��:���ؼ������У����
*******************************************************************/
WORD CheckSum(BYTE *Frame, int Len)
{
    ASSERT(Frame != NULL && Len > 0  && Len < MAXINFOLEN);

    WORD wSum = 0;

    for (int i = 0; i < Len; i++)
    {
        wSum += Frame[i];
    }

    // wSum %= 65535;
    wSum %= 65536;
    wSum = (~wSum) + 1;

    return wSum;
}

/*******************************************************************
��������:ValueToAscii
����˵��:��0--F��ֵ,ת��ΪASCII��
����˵��:��ת����ֵ
���˵��:ת�����ֵ����byAscii����
*******************************************************************/
void ValueToAscii(const BYTE byValue, BYTE& byAscii)
{
    ASSERT(byValue <= 0x0f);

    if (byValue > 9)    //����9��������A��ʼ��F
    {
        byAscii = (byValue + 0x41 - 0x0a);
    }
    else
    {
        byAscii = (byValue + 0x30);
    }
}

/*******************************************************************
��������:AsciiToValue
����˵��:��һ��ASCII��,ת��Ϊ0-F����,Ҫ���ASCII�뷶Χ��'0'��'F'֮��
����˵��:����Ϊ��ת����ASCII��
���˵��:ת�����ֵ�����byValue������
*******************************************************************/
void AsciiToValue(const BYTE byAscii, BYTE& byValue)
{
    ASSERT(byAscii >= 0x30 && byAscii <= 0x46); // 0--F

    if (byAscii > 0x39)
    {
        byValue = (byAscii + 0x0a - 0x41) & 0x0f;
    }
    else
    {
        byValue = (byAscii - 0x30) & 0x0f;
    }
}

/*******************************************************************
��������:LenCheckSum
����˵��:���㳤��У���
����˵��:����
���˵��:���س���У����
*******************************************************************/
BYTE LenCheckSum(WORD wLen)
{
    ASSERT(wLen < MAXINFOLEN);
    //������Ϊ0������Ҫ����
    if (wLen == 0)
    {
        return 0;
    }

    BYTE byLenCheckSum = 0;
    byLenCheckSum += wLen & 0x000F;         //ȡ���4��BIT
    byLenCheckSum += (wLen >> 4) & 0x000F;  //ȡ��4~7��BIT
    byLenCheckSum += (wLen >> 8) & 0x000F;  //ȡ��8~11��BIT
    byLenCheckSum %= 16;                    //ģ16
    byLenCheckSum = (~byLenCheckSum) + 1;   //ȡ����1
    byLenCheckSum &= 0x0F;                  //ֻȡ4��BIT

    return byLenCheckSum;
}

/*******************************************************************
��������:ChangeValueToSend
����˵��:��һ���ֽڱ�Ϊ�ɷ��͵������ֽ�
����˵��:��ת����һ���ֽ�
���˵��:ת���������(�����ֽ�)���ڻ�����pbySend��
*******************************************************************/
void ChangeValueToSend(const BYTE byValue, BYTE* pbySend)
{
    ASSERT(pbySend != NULL);

    BYTE byTmp;

    byTmp = (byValue >> 4);
    ValueToAscii(byTmp, *pbySend);

    byTmp = (byValue & 0x0f);
    ValueToAscii(byTmp, *(pbySend + 1));
}

/*******************************************************************
��������:ChangeRecvToValue
����˵��:���յ��������ֽڱ�Ϊʵ�ʵ�һ���ֽ�
����˵��:��ת���������ֽڵĻ�����
���˵��:ת�����һ���ֽڷ���byValue������
*******************************************************************/
void ChangeRecvToValue(BYTE& byValue, const BYTE* pbySend)
{
    ASSERT(pbySend != NULL);

    byValue = 0;

    BYTE byTmp;
    AsciiToValue(pbySend[0], byTmp);
    byValue += byTmp << 4;

    AsciiToValue(pbySend[1], byTmp);
    byValue += byTmp & 0x0f;
}

/*******************************************************************
��������:
����˵��:��4���ֽڵĳ��Ƚ���У��
����˵��:4���ֽڵĴ�У��ĳ��ȴ�
���˵��:��У����ȷ,����TRUE,��������ֵ����pwLen������,
��У���,����FALSE.
*******************************************************************/
BOOL MakeLength(const BYTE caLen[4], WORD* pwLen)
{
    BYTE byRecvSum;
    BYTE byTmp;

    AsciiToValue(caLen[0], byRecvSum);

    BYTE byCalcSum = 0;
    int i = 1;
    for ( i = 1; i < 4; i ++)
    {
        AsciiToValue(caLen[i], byTmp);
        byCalcSum += byTmp & 0x0f;
    }

    byCalcSum %= 16;
    byCalcSum = (~byCalcSum) + 1;
    byCalcSum &= 0x0f;

    if (byCalcSum != byRecvSum) //����У���
    {
        return FALSE;
    }
    WORD wRecvLen = 0;
    i = 1;
    for (i = 1; i < 4; i ++)
    {
        AsciiToValue(caLen[i], byTmp);
        wRecvLen |= (byTmp << (12 - i * 4));
    }

    *pwLen = wRecvLen;

    return TRUE;
}

/*******************************************************************
��������:SendPacket
����˵��:��һ����Ϣ��������Ӵ����Ϸ���
����˵��:���ھ��,ʹ��Э��汾,�ӻ���ַ,�ӻ�����,�����,
�������ݻ�����,���͵ĳ���
���˵��:�ɹ���������TRUE,���򷵻�FALSE
*******************************************************************/
BOOL SendPacket(HANDLE hComm, BYTE * byaPublicBuffer, BYTE byVer, BYTE byAdr,
                BYTE byCID1, BYTE byCmd, BYTE* pbyInfo, WORD wLen)
{
    ASSERT((pbyInfo != NULL) || (wLen == 0));

    int iCounter = 0; //������

    WORD wInfoSendLen = wLen * 2;

    //��ʼ�ֽڣ�֡ͷ��
    byaPublicBuffer[iCounter++] = g_byBOI;

    //Э��汾��
    ChangeValueToSend(byVer, byaPublicBuffer + iCounter);
    iCounter += 2;

    //�豸���ӻ�����ַ
    ChangeValueToSend(byAdr, byaPublicBuffer + iCounter);
    iCounter += 2;

    //�豸��������
    ChangeValueToSend(byCID1, byaPublicBuffer + iCounter);
    iCounter += 2;

    //������
    ChangeValueToSend(byCmd, byaPublicBuffer + iCounter);
    iCounter += 2;

    //��Ϣ����
    BYTE byTmp;
    ValueToAscii(LenCheckSum(wInfoSendLen), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen >> 8) & 0x000f, byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen >> 4) & 0x000f, byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen & 0x000f) , byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    //��Ϣ��
    for (int i = 0; i < wLen; i ++)
    {
        ChangeValueToSend(pbyInfo[i], byaPublicBuffer + iCounter);
        iCounter += 2;
    }

    //У����
    WORD wCheckSum = CheckSum(byaPublicBuffer + 1, wInfoSendLen + 12);

    ValueToAscii((wCheckSum >> 12), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii(((wCheckSum >> 8) & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii(((wCheckSum >> 4) & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii((wCheckSum & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    //֡������
    byaPublicBuffer[iCounter++] = g_byEOI;
    byaPublicBuffer[iCounter] = 0x00;

    DWORD lWritten;

    // Ϊ�˼��ٸ��ţ�����ǰ�巢��/���ջ�����
    PurgeComm(hComm, PURGE_TXCLEAR);
    PurgeComm(hComm, PURGE_RXCLEAR);

    WriteFile(hComm, (char*)byaPublicBuffer, iCounter, &lWritten, NULL);


    time_t now;
    struct tm  *timenow;
    time(&now);
    timenow = localtime(&now);
    WriteAsc( HEX_FILE, "\r\n%s\r\n",asctime(timenow));
    WriteAsc( ASC_FILE, "\r\n%s\r\n",asctime(timenow));

    // ����Ǹ��ٲ��ԣ��ҷ�OCI��5����(nExtOci5IDȱʡΪ-1)�����¼������Ϣ��
    //if( bTestFlag && nExtOci5ID<0 )
    //{
        //MessageBox( NULL, (char*)sSendStr, "������Ϣ", MB_OK );    // ��ʾ������Ϣ��
    //}   // end of if( bTestFlag )

#ifndef _TSR_
    WriteAsc( ASC_FILE, "\r\nSend:%d char", (int)lWritten );
    WriteAsc( HEX_FILE, "\r\nSend:%d char", (int)lWritten );
    WriteAsc( ASC_FILE, "%s", (char*)byaPublicBuffer );
    WriteHex( HEX_FILE, (char*)byaPublicBuffer, iCounter );

#endif


    return TRUE;
}


/*******************************************************************
��������:ReadPacket
����˵��:�Ӵ����϶���һ֡���ݣ���У��֡����ȷ��
����˵��:��Ҫ����������ǻ�����ָ�뼰����
���˵��:һ����������TRUE,���򷵻�FALSE
���صİ汾�Ŵ��� byVer,�豸��ַ����byAdr,
�ӻ����ʹ����byCID1, ����������byRet
���ص���Ϣ�����pbyInfo,���ص���Ϣ���ȴ����wLen(����󳤶�)
*******************************************************************/
BOOL ReadPacket(HANDLE hComm, BYTE * byaPublicBuffer, BYTE& byVer, BYTE& byAdr,
                BYTE& byCID1, BYTE& byRet, BYTE* pbyInfo, WORD& wLen,
                const WORD wInfoBufLen)
{
    DWORD lRead;
    //��ս��ջ�����
    memset(byaPublicBuffer, 0, sizeof(byaPublicBuffer));
    Sleep(500);
    //��ȡ֡ͷ����������BOITRYTIMES��û���ҵ�֡ͷ����Ϊʧ�ܡ�
    int iTryTimes;

    for (iTryTimes = 0; iTryTimes < BOITRYTIMES; iTryTimes ++)
    {
        ReadFile(hComm, (char*)byaPublicBuffer, 1, &lRead, NULL);
        if ((1 == lRead) && (g_byBOI == byaPublicBuffer[0]))
        {
            break;
        }
        if (lRead == 0)
        {
            WriteAsc(ASC_FILE,"\r\nû���յ�֡ͷ\r\n");
            WriteAsc(HEX_FILE,"\r\nû���յ�֡ͷ\r\n");
            return FALSE; //û�ҵ�֡ͷ��ʧ��
        }
    }

    if (iTryTimes >= BOITRYTIMES)
    {
        wLen = 0;
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\nδ�յ���ͷ��Ϣ\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\nδ�յ���ͷ��Ϣ\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\nδ�յ���ͷ��Ϣ\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Find Head char Error!", 1, 23, 0x1f);
#endif

        return FALSE;   //û�ҵ�֡ͷ��ʧ��
    }

//    ReadFile(hComm, (char*)byaPublicBuffer + 1, 12, &lRead, NULL);
    int i=0;
    int ReadCount = 12;
    do
    {
        ReadFile( hComm, (char*)byaPublicBuffer + 1 + i, 1, &lRead, NULL );
        if( lRead )
        {
            i++;
        }
        if (i == ReadCount)
        {
            break;
        }
#ifdef _LINUX_
        usleep(1);
#endif
    }while( lRead==1);
    lRead = i;


    if (12 != lRead)
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
           // MessageBox(NULL, "\r\n���ṹ��Ϣ����\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n���ṹ��Ϣ����\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\n���ṹ��Ϣ����\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Data Struct Error!", 1, 23, 0x1f);
#endif

        return FALSE; //û�������ֽڣ�ʧ��
    }

    WORD wInfoLen;

    if (!MakeLength(byaPublicBuffer + 9,&wInfoLen))
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\n���ṹ�г�����Ϣ����\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s","\r\n���ṹ�г�����Ϣ����\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s","\r\n���ṹ�г�����Ϣ����\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Data Struct Len Error!", 1, 23, 0x1f);
#endif

        return FALSE;
    }
    wLen = wInfoLen / 2;

    ASSERT(wLen <= wInfoBufLen);

    if( (wLen > wInfoBufLen) || (wInfoLen > (MAXINFOLEN-18)) )
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\n���ݰ����ȴ���\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n���ݰ����ȴ���\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\n���ݰ����ȴ���\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Data Len Error!", 1, 23, 0x1f);
#endif

        return FALSE;//Ҫ�յ����ݰ��Ȼ�������
    }

    //�����13��Ϊ������Ϣǰ����13�ֽڵ���������
    //�����5��Ϊ������Ϣ������4���ֽڵ�У���1���ֽڵ�֡β
//    ReadFile(hComm, (char*)byaPublicBuffer + 13, wInfoLen + 5, &lRead, NULL);
    i=0;
    ReadCount = wInfoLen + 5;
    do
    {
        ReadFile( hComm, (char*)byaPublicBuffer + 13 + i, 1, &lRead, NULL );
        if( lRead )
        {
            i++;
        }
        if (i == ReadCount)
        {
            break;
        }
#ifdef _LINUX_
        usleep(1);
#endif
    }while( lRead==1);
    lRead = i;
    byaPublicBuffer[13 + lRead] = 0x00;


    time_t now;
    struct tm  *timenow;
    time(&now);
    timenow = localtime(&now);
    WriteAsc( HEX_FILE, "\r\n%s\r\n",asctime(timenow));
    WriteAsc( ASC_FILE, "\r\n%s\r\n",asctime(timenow));
#ifndef _TSR_
    //if( bTestFlag )
    //{
        //MessageBox( NULL, (char*)sRecStr, "������Ϣ", MB_OK );  // ��ʾ������Ϣ��
    //}   // end of if( bTestFlag )

    WriteAsc( ASC_FILE, "\r\nRecv:%d char", (int)lRead+13 );
    WriteAsc( HEX_FILE, "\r\nRecv:%d char", (int)lRead+13 );
    WriteAsc( ASC_FILE,"%s", (char*)byaPublicBuffer );
    WriteHex( HEX_FILE, (char*)byaPublicBuffer, (int)lRead+13 );
#endif    // end of #ifdef _DLL_

    if (wInfoLen + 5 != (WORD)lRead)
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\nβ���ݰ����ȴ���\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\nβ���ݰ����ȴ���\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\nβ���ݰ����ȴ���\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Next Data Len Error!", 1, 23, 0x1f);
#endif

        return FALSE;    //���ݰ�����
    }

    if (g_byEOI != byaPublicBuffer[13 + wInfoLen + 4])
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\n��β����\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n��β����\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\n��β����\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("End char Error!", 1, 23, 0x1f);
#endif

        return FALSE; // ֡β��
    }

    WORD wCalcSum = CheckSum(byaPublicBuffer + 1, wInfoLen + 12);

    BYTE bySumTmp[4];   // ��ȡ���հ��е�У����
    memcpy(bySumTmp, byaPublicBuffer + 13 + wInfoLen, 4);

    WORD wRecvSum = 0;
    BYTE byTmp;
    i = 0;
    for ( i = 0; i < 4; i ++)
    {
        AsciiToValue(bySumTmp[i], byTmp);
        wRecvSum |= byTmp << (12 - 4 * i);
    }

    if (wCalcSum != wRecvSum)
    {
#ifndef _TSR_
        if (bTestFlag)    // ����Ǹ��ٲ��ԣ����¼������Ϣ��
        {
            //MessageBox(NULL, "\r\n���ݰ�У�����\r\n", "�澯��Ϣ", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n���ݰ�У�����\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\n���ݰ�У�����\r\n");  // д�뵽�ı��ļ���
        }
#endif

#ifdef _TSR_
        WriteString("Check Sum Error!", 1, 23, 0x1f);
#endif

        return FALSE;  //У��ʹ�
    }

    ChangeRecvToValue(byVer,  byaPublicBuffer + 1);   // ��ȡ���հ��еİ汾��
    ChangeRecvToValue(byAdr,  byaPublicBuffer + 3);   // ��ȡ���հ��еĵ�ַ
    ChangeRecvToValue(byCID1, byaPublicBuffer + 5);   // ��ȡ���հ��е�CID1
    ChangeRecvToValue(byRet,  byaPublicBuffer + 7);   // ��ȡ���հ��еķ�����
     i = 0;
    for (i = 0; i < wLen; i ++)                         // ��ȡ���հ��е�:��Ч���ݶ�
    {
        BYTE byTmp1, byTmp2;
        AsciiToValue(byaPublicBuffer[13 + 2*i], byTmp1);
        AsciiToValue(byaPublicBuffer[13 + 2*i + 1], byTmp2);
        pbyInfo[i] = (byTmp1 << 4) | (byTmp2);
    }

    return TRUE;
}


/*******************************************************************
��������:GetResponeData
����˵��:
����˵��:���ھ��,����Э��汾,Ŀ���豸��ַ,������ݻ�����ָ��,
���ɼ������ݳ���,������ݻ������Ĵ�С.
//���˵��:�ɼ��ɹ�,����TRUE,���򷵻�FALSE.
����:������Э��涨��0���ɹ���������������Ϣ��
*******************************************************************/
int GetResponeData(HANDLE hComm,       // ���ھ��
                   const BYTE byVer,   // ͨѶЭ��汾��
                   const BYTE byAdr,   // �豸��ַ
                   const BYTE byCID1,  // �豸���ͱ�ʶ������
                   const BYTE byCID2,  // ����������CID2
                   int   nSendLen,     // ���������������(����CmdGroup+CmdType+����)
                   BYTE *pbyData,      // ������������ָ��
                   WORD& wRecvLen,     // �������ݳ���
                   // int nLoop,          // �����
                   BYTE*pSendTmp)     // ���ݲ���ָ��
{
    BYTE byaPublicBuffer[MAXINFOLEN]={0};

    if (!SendPacket(hComm, byaPublicBuffer, g_byVersion, byAdr, byCID1, byCID2,
        pSendTmp, nSendLen))
    {
        return nErrorEF;
    }

    Sleep(30); //pjc 2007.5.20 add

    BYTE byReadVer, byReadAdr, byReadCID1, byRet;
    WORD wReadLen;
    int i = 0;
    for(i = 0;i < 3;i ++)
    {
        if (!ReadPacket(hComm, byaPublicBuffer, byReadVer, byReadAdr, byReadCID1, byRet,
            pbyData, wReadLen, MAXINFOLEN/2))
        {
            continue;
        }

        if ( ((byAdr != 255) && (byAdr != byReadAdr))    // qilei 20101221 ��ַ255�������
            || (0x80 != byReadCID1)
            || (g_byVersion != byReadVer)
            )
        {
#ifndef _TSR_
            WriteAsc(ASC_FILE, "\r\n��ͷ��Ϣ����:Addr: %d, CID1: %d, Version: %d\r\n", byReadAdr, byReadCID1, byReadVer);
            WriteAsc(HEX_FILE, "\r\n��ͷ��Ϣ����:Addr: %d, CID1: %d, Version: %d\r\n", byReadAdr, byReadCID1, byReadVer);
#endif    // _TSR_

            continue;
        }
        //�ŻԽ� ������ͨ  ���� 2010.09.17
        if (wReadLen!=wRecvLen && byRet==0x00)
        {
            continue;
        }
/*
		if (byRet!=0x00)
        {
            continue;
        }*/

        break;
    }
    if(i == 3)
    {
        return nErrorEE;
    }

    if(byRet != RETNORMAL)
    {
#ifndef _TSR_
        if (byRet >= 0xe0 && byRet <= 0xe7)
        {
            WriteAsc(ASC_FILE, "\r\n����:%s\r\n", caErrMessage[byRet-0xd8]);
            WriteAsc(HEX_FILE, "\r\n����:%s\r\n", caErrMessage[byRet-0xd8]);
        }
        else if (byRet>0 && byRet <= 7)
        {
            WriteAsc(ASC_FILE, "\r\n����:%s\r\n", caErrMessage[byRet]);
            WriteAsc(HEX_FILE, "\r\n����:%s\r\n", caErrMessage[byRet]);
            return nErrorED;  //�ŻԽ�����  2010.09.15
        }
        else
        {
            WriteAsc(ASC_FILE, "\r\n����:δ�������\r\n");
            WriteAsc(HEX_FILE, "\r\n����:δ�������\r\n");
            return nErrorEC;  //�ŻԽ�����  2010.09.15
        }
#endif  // _TSR_
    }

    return byRet;
}

// ����SM�Ž�ʵʱʱ������:����7������(�ꡢ�¡��ա����ڡ�ʱ���֡���)
void FixSMClock(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)((byData[0] >> 4)*10+ (byData[0]&0x0f))*100
        + (byData[1] >> 4)*10 + (byData[1]&0x0f);  // �Ž�����:��
    int i = 0;
    for ( i = 1; i < 7; i++)    // �Ž����ڡ�ʱ��:�¡��ա����ڡ�ʱ���֡���
    {
        pfRecvData[i] = (float)(byData[i + 1] >> 4)*10 + (byData[i + 1]&0x0f);
    }
}

// �����ȡ��¼����E2����:����34������
void FixRecord(float* pfRecvData, BYTE* byData)
{
    // ���ݴ����˳��:��0���Ϳ���1����¼ʱ����2����3����4��ʱ5����6����7
    //       �¼�״̬8-15����·״̬16-23�������¼�24,REMARK��25,�����޸ı�־26-33

    // ������:ͨ��nReadE2+0Ϊ��λ��ͨ��nReadE2+1Ϊ��λ
    unsigned long lTempCardNo=0l;

    int i = 0;
    for ( i = 0; i < 4; i++)
    {
        lTempCardNo += (unsigned long)(byData[4-i] * pow(256, i));
    }
    //pfRecvData[0] = (float)byData[0];  // ���Ÿ�λ 2002.01.23 CMQ�޸�
    unsigned long lHiCardNo = (unsigned long)byData[0];
    memcpy( &pfRecvData[1], &lTempCardNo, 4);    // zhx    19991130
    memcpy( &pfRecvData[0], &lHiCardNo, 4);      // cmq 2002.01.23

    // Ϊ�˵���С���ŷ��㣬�ر�д�������:2000.08.02
    char szTemp[5]={0};
    memcpy( &szTemp, &pfRecvData[1], 4 );

    pfRecvData[2] = (float)((byData[5] >> 4)*10 + (byData[5]&0x0f))*100
        + (float)(byData[6] >> 4)*10 + (byData[6] & 0x0f);  // ��
     i = 0;
    for (i = 0; i < 5; i++)    // ˢ����¼���ڡ�ʱ��:�¡��ա�ʱ���֡���
    {
        pfRecvData[3 + i] = (float)(byData[i + 7] >> 4)*10 + (byData[i + 7] & 0x0f);
    }

    // ״̬�ֽ�:STATUS����REMARK���ж��ඨ��:0��4��7��5+6+8-11��

    // REMARK��D7��1��ʾ���ź��Ѷ�ȡ��
    if ((byData[13]&0x7f) >= 0 && (byData[13]&0x7f) <= 3)
    {
        // �¼�״̬(8-15)״̬��:�ͣ�>��
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[8 + i] = (float)((byData[12] >> i) & 0x01);
        }

        // ��·״̬16-23�������¼�24��REMARK��25�������޸ı�־26-33
         i = 0;
        for (i = 0; i < 18; i++)
        {
            pfRecvData[16 + i] = 20.0f; // δ����(REMARK���ڱ���������и���)
        }
    }
    else if ((byData[13]&0x7f) == 7) // �ڲ����Ʋ������޸ĵļ�¼
    {
        // �¼�״̬8-15����·״̬16-23�������¼�24
        i = 0;
        for (i = 0; i < 17; i++)
        {
            pfRecvData[8 + i] = 20.0f; // ״̬δ����
        }

        // ���޸����ݺ����ִ洢�ڿ���λ�ã����轫����(��λ)����
        pfRecvData[1] = 0.0f;
         i = 0;
        for (i = 0; i < 8; i++)    // �����޸ı�־26-33״̬��:�ͣ�>��
        {
            pfRecvData[26 + i] = (float)((byData[4] >> i) & 0x01);  // ��������ֽ�
        }
    }
    else if ((byData[13]&0x7f) == 4)//��ʱδ���ż�¼
    {
        // �򱨾����ݺ����ִ洢�ڿ���λ�ã����轫����(��λ)����
        pfRecvData[0] = 0.0f;//ˢ����ʱ��ʱ��,�¼��ǿ���
        pfRecvData[1] = 0.0f;
        for(i=0; i<8; i++)
        {
            pfRecvData[8 + i]  = 20.0f;
            pfRecvData[16 + i] = 20.0f;
            pfRecvData[26 + i] = 20.0f;
        }
        pfRecvData[24] = 20.0f;//����ԴAS��Ч
    }
    else    // �澯��¼:������SM���硢��Ч�������ڿ�����ʱ�Ρ�����ȷ�ϴ���
    {
        if ((byData[13]&0x7f) == 5)  // �����¼�24
        {
            // �򱨾����ݺ����ִ洢�ڿ���λ�ã����轫����(��λ)����
            pfRecvData[1] = 0.0f;
            pfRecvData[24] = (float)byData[4];    // ��������ֽ�
        }
        else
        {
            pfRecvData[24] = 20.0f;   // ״̬δ����
        }
         i = 0;
        for (i = 0; i < 8; i++)    // ��·״̬(16-23)״̬��:�ͣ�>��
        {
            pfRecvData[16 + i] = (float)((byData[12] >> i) & 0x01);
        }

        // �¼�״̬8-15
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[8 + i] = 20.0f;    // ״̬δ����
        }

        // �����޸ı�־26-33
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[26 + i] = 20.0f;   // ״̬δ����
        }
    }

    pfRecvData[25] = (float)(byData[13]);   // ��ע��

    //2004-03-26,LYG,ES2100����Remark=34,���������¼�����"�¼���Դ��5�ֽ�ȫ0ʱ"��������ʼ��
    //��"�¼���Դ��4�ֽ�ȫ0ʱ+�����ֽڣ�1"������������Ϊ����������޸����£�34����������ʼ��
    //35������������
//    if(pfRecvData[25]==34)
//        pfRecvData[25] += (float)byData[4];   // ��ע��
}

/////L&L ADD/////
// �����Ž�SM���״̬E7��������:(��2100����Ǹ��ֽ�) ����1������
void FixSMMonitorStatus_add(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)byData[2];
}

// �����Ž�SM���״̬E7��������:����12������
void FixSMMonitorStatus(float* pfRecvData, BYTE* byData)
{
    int i = 0;
    for ( i = 0; i < 8; i++)    // SM�Ĺ���״̬:�ͣ�>��
    {
        pfRecvData[i] = (float)((byData[0] >> i) & 0x01);
    }


    //lyg,2005-12-8
    // SM�ļ����·״̬:�ͣ�>��D0�����̵���������D1�ֶ�����ť��D3�ſ���
     i = 0;
    for (i = 0; i < 4; i++)
    {
        pfRecvData[8 + i] = (float)((byData[1] >> i) & 0x01);
    }
}

// �����Ž�SM���Բ���E8��������:����12������
void FixSMSpecialty(float*pfRecvData, BYTE* byData)
{
    int i = 0;
    for ( i = 0; i < 8; i++)         // �������ء�����״̬������:�ͣ�>��
    {
        pfRecvData[i] = (float)((byData[0] >> i) & 0x01);
    }
    pfRecvData[8] = (float) byData[1];  // �����̵���ִ��ʱ��
    pfRecvData[9] = (float) byData[2];  // ���ŵȴ�������ʱʱ��

    //pfRecvData[10] = (float) byData[3]; // ���ⱨ��������ȷ�ϵ���ʱʱ��
    //pfRecvData[11] = (float) byData[4]; // ��Ӧ���Ķ����ͺż����Ż�ȡ�����ֽ�
    pfRecvData[10] = (float)0.0;             //Es2100û�д���
    pfRecvData[11] = (float)0.0;
}

void FixCarNum(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)((*(byData)<<8)+(*(byData+1)));
}

// ����:����Э�����ݶ�ȡ�йصĲɼ����ݣ�������������δ����ΪPSMS�Ž��ӿ�
BOOL GetData(HANDLE hComm, const BYTE byVer, const BYTE byAdr, const BYTE byCID1,
             float* pfRecvData)// , WORD& wRecvLen)
{
    BYTE byData[MAXINFOLEN/2]={0};    // �ɼ���Ч���ݻ�����
    BYTE pSendTmp[50]={0};  // �������:Command Group + Command Type + DataF
    int  nRet = 0;      // �ɼ����ر�ʶ
    int  nLoop = 0;     // �ɼ����ݰ����
    int  i = 0;

    // ��ȡSM��ʵʱ��E0��������ݵ�λ�ã���7��
#define nReadE0 0
    // ��ȡһ����¼E2��������ݵ�λ�ã���34������
#define nReadE2 7
    // ��ȡSM�ļ��״̬E7��������ݵ�λ�ã���12������
#define nReadE7 41
    // ��ȡSM���Ƶ���֮���Բ���E8��������ݵ�λ�ã���12������
#define nReadE8 53

    // ��ȡSM���Ƶļӿ�����������1������
#define nReadE5 66

    /////L&L ADD/////
    // ��ȡSM�ļ��״̬E7���� EDM������3���ֽ�,ԭ����ES2100ֻ��2���ֽ�
    // ֻ��1������
#define nReadE7add 66   //55��ͨ��û����

    do
    {
        //v5.40���������ֻ��ÿ��ɼ�һ��

        int j = 0;
        for (j = 0; j < 3; j++)    // ���Դ���
        {
            pSendTmp[0] = strRead[nLoop].byCmdGroup;    // ������:command group
            pSendTmp[1] = strRead[nLoop].byCmdType;     // ��������:command type
            for (i = 0; i < strRead[nLoop].nDataLen; i++)
            {
                pSendTmp[2 + i] = strRead[nLoop].byData[i];
            }
            nRet = GetResponeData(hComm, byVer, byAdr, byCID1, strRead[nLoop].byCID2,
                // strRead[nLoop].nDataLen+2, byData, nLoop, pSendTmp);
                strRead[nLoop].nDataLen + 2, byData,strRead[nLoop].nRetLen, pSendTmp);

            // ���ɼ��з��أ�����ʾ�ɼ��ɹ�������:����ʧ�ܡ�����Ӧ��У�鲻��ȷ��������

            //�ŻԽ����� 2010.09.15
            if (nRet==nErrorED || nRet==nErrorEC)
            {
                continue;
            }

            //lyg,2004-03-26
            //if (nLoop == 1 && (nRet != 0 && nRet != nNoRecord) )
            //�ŻԽ��޸�  2010.09.15
            if (nLoop == 1 && (nRet != 0 && nRet != nNoRecord) && nRet!= 0xE4 )
            {
                if (j==2)
                {
                    return FALSE;   // �ɼ���¼�����з��м�¼���з��޼�¼������ʱ�����
                }
                else
                {
                    continue;
                }

            }
            else if (nLoop != 1 && nRet)
            {
                if (j==2)
                {
                    return FALSE;   // �����������д��������
                }
                else
                {
                    continue;
                }

            }

            if( nRet!=nErrorEE && nRet!=nErrorEF  )
            {
                break;
            }

        }

        switch (nLoop)  // ����������ţ��������ݴ���
        {
        case 3: // ��ȡSM��ʵʱ��:7��
            FixSMClock(pfRecvData + nReadE0, byData);
            break;
        case 1: // ��ȡSM��һ����ʷ��¼:34��
            // ���ݴ����˳��:��0���Ϳ���1����¼ʱ����2����3����4��ʱ5����6����7
            //   �¼�״̬8-15����·״̬16-23�������¼�24�������޸ı�־25-32,REMARK��33

            if (nRet == nNoRecord || nRet==0xE4 )          // �޼�¼
            {
                i = 0;
                for (i = 0; i < 8; i++)     // ��������
                {
                    pfRecvData[nReadE2 + i] = 0.0f;  // ģ������������
                }
               // pfRecvData[nReadE2 + 33] = 0.0f;
                i = 0;
                for (i = 0; i < 26; i++)    // ��������
                {
                    // ���������ݺ�REMARK����δ����
                    pfRecvData[nReadE2 + 8 + i] = 20.0f;
                }
            }
            else
            {
                FixRecord(pfRecvData + nReadE2,  byData);
            }
            break;
        case 2: // ��ȡSM�ļ��״̬:12��
            FixSMMonitorStatus(pfRecvData + nReadE7, byData);
            /////L&L ADD/////
            FixSMMonitorStatus_add(pfRecvData + nReadE7add, byData);
            break;
        case 0: // ��ȡSM����/�������Բ���
            FixSMSpecialty(pfRecvData + nReadE8, byData);
            break;
		case 4:
			FixCarNum(pfRecvData + nReadE5, byData);
            break;
        default:
            break;
        }
        nLoop++;
    }while(nLoop < 5);

    return TRUE;
}

// �������Ž�PSMS���ݽӿڹ淶�������ݡ�
void TurnToPsmsForm(float* fpreData,   // ת��ǰ����
                    float* fData)      // ת��������
{
    int i = 0;
    for ( i = 0; i < 8; i++)
    {
        fData[i] = fpreData[7 + i];   // ˢ����¼�Ŀ���+����+ʱ��
    }

    //2000.08.02 cmq ΪOCEС�����޷���ʾ�޸�
    memcpy(&fData[1],&fpreData[8],4);   // ��λ����
    memcpy(&fData[0],&fpreData[7],4);   // ��λ���� 2002.01.23 CMQ����


    if( fpreData[32]==1.0f )
    {
        fData[8] = 1.0f;                // 585�����ű�־:˫���
    }
    else
    {
        fData[8] = 0.0f;                // 505�����ű�־:�����
    }

    // �ϱ�:0-�ޱ�־,1-��Ч��,2-��Ȩ����,3-�Ƿ�ʱ��,4-�������,5-�Ƿ�����,6-�Ƿ���
    if (((BYTE)fpreData[32] & 0x7f) == 0)    // ��עREMARK=0:����ˢ�����ż�¼
    {
        fData[9] = 1.0f;
    }

    else if (((BYTE)fpreData[32]&0x7f)>0 && ((BYTE)fpreData[32]&0x7f) < 8)
    {
        // 11-�û���+���뿪��,12-Զ�̿���,13-�ֶ�����,14-��������,
        // 15-����,16-SM����,17-�ڲ����Ʋ������޸�
        fData[9] = 10.0f + ((BYTE)fpreData[32] & 0x7f);
    }


    else if (((BYTE)fpreData[32]&0x7f) == 8)    // ��Ч��ˢ����¼
    {
        fData[9] = 6.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 9)    // �û�����Ч���ѹ�
    {
        fData[9] = 2.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 10)   // ��ǰʱ����û���Ȩ����
    {
        fData[9] = 3.0f;
    }

    else
    {
        fData[9] = 20.0f;  // δ֪����
    }

    fData[10] = 1.0f;   // �ź�

    // added by zhx    19991124
    if (fData[1] == 0)
    {
        fData[10] = 0.0f;   // �ź�
    }

    fData[11] = fpreData[32];   // REMARK��(ע:�ڽӿڹ淶��δ�����ֵ)
    i = 0;
    for (i = 0; i < 3; i++)
    {
        fData[13 + i] = fpreData[i];      // �Ž�����:�ꡢ�¡���
    }
    i = 0;
    for (i = 0; i < 3; i++)
    {
        fData[16 + i] = fpreData[i + 4];    // �Ž�ʱ��:ʱ���֡���
    }

    fData[19] = fpreData[3];    // �Ž�����

    // 20��Ȩ���澯״̬0:��ȷ,1:����,2:�ޱ�־
    if (((BYTE)fpreData[32]&0x7f) == 8 )
    {
        fData[20] = 1.0f;   // ��Ч
    }
    else if (((BYTE)fpreData[32]&0x7f) < 1)
    {
        fData[20] = 0.0f;   // ��ȷˢ�����û��ſ���
    }
    else
    {
        fData[20] = 2.0f;   // �ޱ�־
    }

    if ( ( (BYTE)fpreData[32]&0x7f ) == 9 )
    {
        fData[21] = 1.0f;   // ����
    }
    else if (((BYTE)fpreData[32]&0x7f) < 1)
    {
        fData[21] = 0.0f;   // ��ȷˢ�����û��ſ���
    }
    else
    {
        fData[21] = 2.0f;   // �ޱ�־
    }

    // 22�Ƿ�ʱ���澯״̬0:��ȷ,1:����,2:�ޱ�־
    if (((BYTE)fpreData[32]&0x7f) == 10.0f)
    {
        fData[22] = 1.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 0.0f || ((BYTE)fpreData[32]&0x7f) == 1.0f)
    {
        fData[22] = 0.0f;   // ��ȷˢ�����û��ţ�����
    }
    else
    {
        fData[22] = 2.0f;
    }

    // 23�Ƿ������澯״̬0:��ȷ,1:����,2:�ޱ�־
    fData[23] = 2.0f;

    // 24���ų�ʱ�澯0:����,1:�澯,2:�ޱ�־
    if (fpreData[19] == 20.0f)
    {
        fData[24] = 2.0f;
    }
    else
    {
        fData[24] = fpreData[19];
    }

   /* // 25ˢ��δ���Ÿ澯0:����,1:�澯,2:�ޱ�־
    if (fpreData[20] == 20.0f)
    {
        fData[25] = 2.0f;
    }
    else
    {
        fData[25] = fpreData[20];
    }
    */
    // �ſ���¼ 0:δ����,1:����,2:�ޱ�־
    if (fpreData[31] == 2.0f) //Remark=5,ʱ��AS=2
    {
        fData[26] = 1.0f;
    }
    else
    {
        fData[26] = 2.0f;
    }

    // 27����澯0:δ����,1:����,2:�ޱ�־
    //fData[27] = fpreData[51];   // E7����

    // 28�Ŵſ���״̬0:��,1:��,2:��״̬
    fData[28] = fpreData[52];   // E7����

    // 29���Ⲽ������״̬0:����,1:����,2:��״̬
    //fData[29] = fpreData[44];   // E7����

    // 30��������״̬0:��,1:��,2:��״̬
    fData[30] = fpreData[42];   // E7����

    // 4.39ֻ��39ͨ��
    fData[39] = 4.0f;       // �Ž�����(DC505 = 1, ES2100 = 2,ES2000 = 3,EDM30E=4)
	fData[40] = 250;       //�Ž����濨���� EDM30EΪ250�ſ�
	fData[41] = fpreData[66];  //�Ѽӿ�����
    //fData[39] = 16.0f;  // 16������ʾ����
    //fData[59] = 10.0f;  // ��󿨺ų���10λ
    //fData[79] = 1.0f;   // ����ź�:������
    i = 0;
    for (i = 0; i < 50; i++) // ��ԭ���ݿ�����100�Ժ������
    {
        fData[100 + i] = fpreData[15 + i];
    }
    /////L&L ADD/////����E7����һ������
    fData[100 + 51] = fpreData[15 + 51];

}

/*******************************************************************
��������:Read
����˵��:��̬���Read����,�˺���������psms4.0��TSR��
�˺����ɼ��豸�ϵ�����,�����д���,�������ڸ����Ļ�����
����˵��:���ھ��,�豸��ַ,��Ųɼ����ݵ�ָ��
���˵��:�ɼ��ʹ���ɹ�,����TRUE,�����ɼ������ݷ��ڻ�����
�ɼ�����ʧ��,����FALSE.
********************************************************************/
DLLExport BOOL Read(HANDLE hComm, int nUnitNo, void* pvData)
{

    float* fData = (float*)pvData;
    float fpreData[160] = {0.0f};
    ASSERT(nUnitNo >= 0 && nUnitNo < 256);
    if (nUnitNo < 0 || nUnitNo > 255)
    {
        return FALSE; //��ַ����
    }

    BYTE byCID1 = 0x80;     // ���Ž�������

    if (!GetData(hComm, g_byVersion, (BYTE)nUnitNo, byCID1, fpreData))
        // ԭʼ�ɼ�ʱ��,����ת��ΪPSMS�ӿڸ�ʽ
        // if(!GetData(hComm, g_byVersion, (BYTE)nUnitNo, byCID1, fData))
    {
        return FALSE;
    }

    //add by liugang ,20070801,�����¼��Ĵ������޸���Ӧ��
    if( fpreData[66] >=0 &&  fpreData[66] <=3 )
    {
        if( fpreData[52] !=0)
        {
            fpreData[52] = 168 ; //�ſ���״̬ & �Ŵſ���״̬����Ӧͨ����[137]��[28]
        }
        if( fpreData[42] ==1 )
        {
            fpreData[42] = 168 ; //��������״̬����Ӧͨ����[30]
        }

        fpreData[19] = 0 ; //���ų�ʱ�澯״̬����Ӧͨ����[24]
        fpreData[41] = 0 ; //�Ž��澯״̬�޸ģ���Ӧͨ����[126]
    }

    //add by liugang, 20071205,�޸��Ž��澯״̬��Ϣ
    if( fpreData[66] == 12 ) //��ʱδ����
    {
        fpreData[41] = 2 ; //�Ž��澯״̬�޸ģ���Ӧͨ����[126]
        fpreData[52] = 3 ; //�ſ���״̬ & �Ŵſ���״̬����Ӧͨ����[137]��[28]
    }
    if( fpreData[66] == 11 )//��ʱδ����
    {
        fpreData[41] = 3 ; //�Ž��澯״̬�޸ģ���Ӧͨ����[126]
    }
    if( fpreData[66] == 9 )//�Ƿ�����
    {
        fpreData[41] = 4 ; //�Ž��澯״̬�޸ģ���Ӧͨ����[126]
    }

    // �����Ž�����ת��ΪPSMS�Ž��淶����
    TurnToPsmsForm(fpreData, fData);

    //5.40�Ž�У��ʱ��:����10s���Զ�
#ifdef _LINUX_
    char pCmdStr[60];
    memset(pCmdStr, 0, 60);

    //��ȡʱ��
    struct tm *local;
    time_t t;
    t=time(NULL);
    local=localtime(&t); //�������Ҫ������local����һ��ָ�룬û�з����ַ��



    //WriteAsc( ASC_FILE,"IDU��ǰ��ʱ�����:%d\r\n", t );
    //WriteAsc( HEX_FILE,"IDU��ǰ��ʱ�����:%d\r\n", t );


    int IDU_year = local->tm_year + 1900;//IDU��ʱ����
    int IDU_month= local->tm_mon + 1;//IDU��ʱ����
    int IDU_day  = local->tm_mday;
    int IDU_hour = local->tm_hour;
    int IDU_min  = local->tm_min;
    int IDU_sec  = local->tm_sec;
    int IDU_wday = local->tm_wday;

    //WriteAsc( ASC_FILE, "IDU��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
    //WriteAsc( HEX_FILE, "IDU��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
    struct tm cardtime;
    memset(&cardtime, 0, sizeof(cardtime));
    cardtime.tm_year = (int)fData[13] - 1900;
    cardtime.tm_mon  = (int)fData[14] - 1;
    cardtime.tm_mday = (int)fData[15] ;
    cardtime.tm_hour = (int)fData[16] ;
    cardtime.tm_min  = (int)fData[17] ;
    cardtime.tm_sec  = (int)fData[18] ;

    /*
    WriteAsc( ASC_FILE, "�Ž� ��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", \
                                         (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
    WriteAsc( HEX_FILE, "�Ž� ��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", \
                                         (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
    */

    time_t t_Door=mktime(&cardtime);
    //WriteAsc( ASC_FILE,"�Ž� ��ǰ��ʱ�����:%d\r\n", t_Door );
    //WriteAsc( HEX_FILE,"�Ž� ��ǰ��ʱ�����:%d\r\n", t_Door );


    if ( abs( t_Door-t )>30 ) //����ʱ�䳬��30�룬�Զ�У��ʱ��
    {

        WriteAsc( ASC_FILE,"IDU��ǰ��ʱ�����:%d\r\n", t );
        WriteAsc( HEX_FILE,"IDU��ǰ��ʱ�����:%d\r\n", t );

        WriteAsc( ASC_FILE, "IDU��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
        WriteAsc( HEX_FILE, "IDU��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );

        WriteAsc( ASC_FILE, "�Ž� ��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", \
            (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
        WriteAsc( HEX_FILE, "�Ž� ��ǰ��ʱ��:%d-%d-%d %d:%d:%d\r\n", \
            (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );

        WriteAsc( ASC_FILE,"�Ž� ��ǰ��ʱ�����:%d\r\n", t_Door );
        WriteAsc( HEX_FILE,"�Ž� ��ǰ��ʱ�����:%d\r\n", t_Door );


        char DataStr[50];
        memset(DataStr, 0, 50);
        sprintf(DataStr, "%04d%02d%02d%02d%02d%02d%d\0x00", IDU_year, IDU_month, \
            IDU_day, IDU_hour, IDU_min,  IDU_sec, IDU_wday );
        //�������
        FILE *fp;
        if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"rb")) == NULL)
        {
            //printf("�����ļ������ڣ�\n");
            WriteAsc( ASC_FILE,"%s", "\r\n�����ļ������ڣ�\r\n");  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"%s", "\r\n�����ļ������ڣ�\r\n");

            sprintf(pCmdStr, "32,%s,%s", "00000", DataStr);//�����ļ������ڣ�����Ϊ000000
        }
        else
        {
            char str[20];
            memset(str, 0, 20);
            fgets(str,6,fp);
            //printf("str = %s\n", str);

            WriteAsc( ASC_FILE,"\r\n�������ļ�:%s��\r\n",str);  // д�뵽�������ļ���
            WriteAsc( HEX_FILE,"\r\n�������ļ�:%s��\r\n",str);

            fclose(fp);//�ر��ļ�

            sprintf(pCmdStr, "32,%s,%s", str, DataStr);//������ļ��ж���

        }

        Sleep(200);
        //���ÿ��ƽӿ�
    //    bTestFlag = TRUE;
        WriteAsc( ASC_FILE,"%s", "\r\n�Ž��Զ�У��ʱ��\r\n");  // д�뵽�������ļ���
        WriteAsc( HEX_FILE,"%s", "\r\n�Ž��Զ�У��ʱ��\r\n");
    //    bTestFlag = FALSE;


        Control( hComm, nUnitNo, pCmdStr);
    }

#endif

    return TRUE;
}

/**********************************************************************
��������:Query
����˵��:�˺����Ƕ�̬�����Ҫ���ں���֮һ,
�����ɼ��ʹ����豸�ϵ�����
����˵��:����Ϊ���ھ��,�豸��ַ,���ݴ�����,��һ����������ʹ�õ�ָ��
���˵��:�����ݲɼ��ɹ�ʱ����TRUE,�������ݴ�����������ݴ���
�����ݲɼ�ʧ��ʱ����FALSE.
***********************************************************************/
#ifdef _DLL_
// V4.1����4.x��/V5.0�� �����ݲɼ��ϱ�������
DLLExport BOOL Query(
                     HANDLE hComm,              // ͨѶ�ھ��
                     int nUnitNo,               // �ɼ�����Ԫ��ַ
                     ENUMSIGNALPROC EnumProc,   // ö�ٺ���
                     LPVOID lpvoid              // ������ָ��
                     )
{

/*
// ԭʼ�ӿ�
float p[nMaxChanelNo] = {0.0f};

  // ���ݲɼ�
  if (!Read(hComm, nUnitNo, p))
  {
  return FALSE;
  }

    // �����ϱ�
    for (int i = 0; i < nMaxChanelNo; i++)
    // for(int i = 0; i < 65; i++)   // ԭʼ�ɼ�ʱ��,����ת��ΪPSMS�ӿڸ�ʽ
    {
    EnumProc(i, p[i], lpvoid);
    }

      return TRUE;

    //*/


    float p[nMaxChanelNo] =  {0.0f};

    // ���ݲɼ�
    if (!Read(hComm, nUnitNo, p))
    {
        return FALSE;
    }

#define DT_FLOAT        0            //��������
#define DT_INT            1            //�������ͣ��ݲ�ʹ�ã�
#define DT_DOOR            100            //��Ѷͨ�Ž���������
#define DT_LONGDOORID    101            //���Ž����Ÿ�λ
#define DT_CARDIDLOW    102            //���Ž����ŵ�λ


    struct CCardData
    {
        short            iDoorID;            //��ID
        UINT            nCardID;            //����
        time_t            tmFreshTime;        //ˢ��ʱ��
        char            chEntryFlag;        //������־,0���롣1�뿪
    };
    struct CNewCardData
    {
        short            iDoorID;            //��ID
        UINT            nCardIDHigh;        //���Ÿ�λ
        UINT            nCardIDLow;        //���ŵ�λ
        UINT            nSpunitID;            //��ǰ�û���д
        UINT            nParam_One;            //����
        UINT            nParam_Two;            //����
        BYTE            byteTemp;            //����
        time_t            tmFreshTime;        //ˢ��ʱ��
        char            szCardID[50];        //����δ��
        char            chEntryFlag;        //������־,0���롣1�뿪
        char            chValidFlag;        //�Ƿ�Ƿ�ˢ����־
    };

    // struct CCardData;
    //������������
    union    CVarData
    {
        float        fltVal;
        UINT        nVal;
        CCardData    cardVal;
        char        pStr[100];
        CNewCardData    newcarddata;
    };

    struct DATA_VARIANT
    {
        DWORD        dwDataType;
        CVarData    VarData;
    };

    DATA_VARIANT  _DATA_VARIANT;

    //����������ڽṹ��
    _DATA_VARIANT.VarData.newcarddata.iDoorID = (char)((int)p[10]);    // �ź�

    //����p[0]��[1]��DWORD����
    union   DATA_UNION
    {
        UINT    uTmp;
        float    fTmp;
    };
    DATA_UNION    _DATA_UNION;

    _DATA_UNION.fTmp = p[0];  // ����λ
    _DATA_VARIANT.VarData.newcarddata.nCardIDHigh = _DATA_UNION.uTmp;

    unsigned short int    uSglFlag = (unsigned short int)_DATA_UNION.uTmp;
    _DATA_UNION.fTmp = p[1];  // ����λ
    _DATA_VARIANT.VarData.newcarddata.nCardIDLow = _DATA_UNION.uTmp;
    uSglFlag += (unsigned short int)_DATA_UNION.uTmp;

    // ˢ��ʱ��:�ꡢ�¡��ա�ʱ���֡���
    struct tm timeRecord;
    timeRecord.tm_year = (int)p[2];
    timeRecord.tm_mon  = (int)p[3];
    timeRecord.tm_mday = (int)p[4];
    timeRecord.tm_hour = (int)p[5];
    timeRecord.tm_min  = (int)p[6];
    timeRecord.tm_sec  = (int)p[7];

    // addded by zhx    19991124
    if (timeRecord.tm_year >= 1900)
    {
        timeRecord.tm_year -= 1900;
    }
    timeRecord.tm_mon  -= 1;

    _DATA_VARIANT.VarData.newcarddata.tmFreshTime = mktime(&timeRecord);

    _DATA_VARIANT.VarData.newcarddata.chEntryFlag =    (char)((int)p[8]);    // ������־
    _DATA_VARIANT.VarData.newcarddata.chValidFlag = (char)((int)p[9]);    // �Ƿ�ˢ����־

    // �����ϱ�
    union    DATA_ADDR_UNION
    {
        float           fAddr;
        DATA_VARIANT*   pAddr;
    };
    DATA_ADDR_UNION     _DATA_ADDR_UNION;
    _DATA_ADDR_UNION.pAddr = &_DATA_VARIANT;

    if (uSglFlag == 0 && p[10] == 0.0f)
    {       // Ϊ0����ʱ����2ͨ����ʼ����
        for (int nChannel = 13; nChannel <= nMaxChanelNo; nChannel++)
        {
            EnumProc(nChannel, p[nChannel], lpvoid);
        }
    }
    else    // ֻ�в�Ϊ0����ʱ�Ŵ���
    {
        // �����Ÿ�λ
        _DATA_VARIANT.dwDataType = DT_LONGDOORID;   // ���Ž���־
        EnumProc(0x04000000, _DATA_ADDR_UNION.fAddr, lpvoid);

        // �����ŵ�λ
        _DATA_VARIANT.dwDataType = DT_CARDIDLOW;    // ���Ž���λ��־
        _DATA_UNION.fTmp = p[1];                    // ����λ
        _DATA_VARIANT.VarData.nVal = _DATA_UNION.uTmp;
        EnumProc(0x04000001, _DATA_ADDR_UNION.fAddr, lpvoid);

        // ����ͨ����������
        for (int nChannel = 2; nChannel <= nMaxChanelNo; nChannel++)
        {
            EnumProc(nChannel, p[nChannel], lpvoid);
        }
    }

    return TRUE;
}

// ���ٲ�����ں���
// ����:�ڲ���ǰ����ȫ�ֱ�־ bTestFlag ��λ�����Ժ󣬸�λ��־��
//*****************************************************************
// ��������:Test
// ��������:���ٲ�����ں������ڲ���ǰ����ȫ�ֱ�־ bTestFlag ��λ��
//           ���Ժ󣬸�λ��־��
// �������:hComm - ͨ�ž��,
//           nUnitNo - �ɼ�����Ԫ��ַ,
//           pData - �ϱ����ݻ�����ָ��
// �������:
// ��    ��:TRUE���ɹ���FALSE��ʧ�ܡ�
// ��    ��:
//*****************************************************************
DLLExport BOOL Test(HANDLE hComm,               // ͨѶ�ھ��
                    int nUnitNo,                // �ɼ�����Ԫ��ַ
                    ENUMSIGNALPROC EnumProc,    // ö�ٺ���
                    LPVOID lpvoid)              // ������ָ��
{
    // �����Ա�־��λ
    bTestFlag = TRUE;

    // ���òɼ������ɼ����ݣ����л�����Ա�־����λ����ʾ������Ϣ��
    BOOL bFlag = Query( hComm, nUnitNo, EnumProc, lpvoid );

    WriteAsc( ASC_FILE, "\r\n���βɼ�����\r\n", 16 );
    WriteAsc( HEX_FILE, "\r\n���βɼ�����\r\n", 16 );

    // �����Ա�־��λ
    bTestFlag = FALSE;

    return bFlag;
}
#endif //_DLL_


// ���·��������ڡ�ʱ�����ת��Ϊ���������
BOOL PrepareDateTime(char* szValue,    // �·������
                     BYTE *bySendTmp)  // ת���õ��������
{
    // �·������ʽ:199912312359595��>1999��12��31��23��59��59����5
    BYTE byTmp[50]={0};     // ��ʱ������
    int i = 0;
    for (i = 0; i < 4; i++)            // ���͡��ꡢ�¡���
    {
        memcpy(byTmp, szValue + 2*i, 2);
        byTmp[2] = 0;
        if( (byTmp == NULL)||(*byTmp == 0x00) )
        {
            return FALSE;
        }
        bySendTmp[2 + i] = (BYTE)strtol((char*)byTmp, NULL, 16);
    }

    memcpy(byTmp, szValue + 14, 1);         // ����
    byTmp[1] = 0;
    if( (byTmp == NULL)||(*byTmp == 0x00) )
    {
        return FALSE;
    }
    bySendTmp[6] = (BYTE)strtol((char*)byTmp, NULL, 16);
    i = 0;
    for (i = 0; i < 3; i++)                 // ʱ���֡���
    {
        memcpy(byTmp, szValue + 8 + 2*i, 2);
        byTmp[2] = 0;
        if( (byTmp == NULL)||(*byTmp == 0x00) )
        {
            return FALSE;
        }
        bySendTmp[7 + i] = (BYTE)strtol((char*)byTmp, NULL, 16);
    }

    return TRUE;
}

// ���·����������û����ת��Ϊ���������
BOOL PrepareAddUser(char* szValue,    // �·������
                    BYTE *bySendTmp,  // ת���õ��������
                    char *ADDCardID )
{
    // �·������ʽ:(����),(�û���),(4λ����)(��Ч��)(�û�Ȩ��'�ַ���')
    BYTE byTmp[50]={0};     // ��ʱ������
    BYTE byBuf[5]={0};      // ��ʱ������
    char c[10]={0};
    int i=0;
    int nValue=0;

    // ����
    int nPoint = StrToK(szValue, (char*)byTmp, ',');
    if( (byTmp == NULL)||(*byTmp == 0x00) )
    {
        return FALSE;
    }

    // ��PSMS4.357�����·�Ϊ16�����ַ�����ʽ����ע��
    //nValue = atoi((char*)byTmp);        // ��ȡ����
    //sprintf((char*)byTmp, "%010X", nValue); // 5λ����
    //sprintf((char*)byTmp, "%010S", byTmp); // 5λ����
    if (strlen((char*)byTmp)>10)
    {
        return FALSE;   // ����̫��
    }

    strcpy(ADDCardID,(char*)byTmp); //���濨��

    i = 0;
    for (i = 0; i < 10; i = i + 2)
    {
        strncpy(c, (char*)byTmp + i, 2);
        c[2] = 0;
        bySendTmp[2 + i/2] = (BYTE)strtol(c, NULL, 16);
    }

    // �û���:ʮ����
    nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
    if (byTmp == NULL || *byTmp == 0x00 || strlen((char*)byTmp)>4)
    {
        return FALSE;
    }
    nValue = atoi((char*)byTmp);              // ��ȡ�û����ID
    sprintf((char*)byTmp, "%08d", nValue);    // 4λ�û����
    i = 0;
    for (i = 0; i < 8; i = i + 2)
    {
        strncpy(c, (char*)byTmp + i, 2);
        c[2] = 0;
        bySendTmp[7 + i/2] = (BYTE)strtol(c, NULL, 16);
    }

    // 2�ֽ��û�����+4�ֽ���Ч��+1�ֽ��û�Ȩ��
    nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');

    if (byTmp == NULL || *byTmp == 0x00 || strlen((char*)byTmp) != 14)
    {
        return FALSE;
    }
    i = 0;
    for (i = 0; i < 14; i += 2)    // 2�ֽ��û�����+4�ֽ���Ч��+1�ֽ��û�Ȩ��
    {
        memcpy(byBuf, byTmp + i, 2);
        byBuf[2] = 0;

        bySendTmp[11 + i/2] = (BYTE)strtol((char*)byBuf, NULL, 16);
    }

    if( bySendTmp[17]< 0x10)
    {
        bySendTmp[17]=bySendTmp[17]+0x40;//Ĭ�ϼӿ�D6=1,һ���û�
    }


    return TRUE;
}

// ���·�����ɾ���û����ת��Ϊ���������
BOOL PrepareDelUser(char* szValue,    // �·������
                    BYTE *bySendTmp)  // ת���õ��������
{
    // �·������ʽ:(ɾ������0-2),ɾ������(����/�û���)
    BYTE byTmp[50]={0};     // ��ʱ������
    int nValue = 0;
    int i = 0;

    int nPoint = StrToK(szValue, (char*)byTmp, ',');
    int nParam = atoi((char*)byTmp);    // ��ʽ��
    bySendTmp[2] = nParam;
    if (nParam == 0)                    // ������ɾ��
    {
        // ��ȡ����
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        // PSMS4.357�·���Ϊ�ַ�������ֱ��ʹ��
        //nValue = atoi((char*)byTmp);
        //sprintf((char*)byTmp, "%010X", nValue); // ����
        if (strlen((char*)byTmp)>10)            // ���ų���
        {
            return FALSE;
        }
        i = 0;
        char c[3];
        for (i = 0; i < 10; i = i + 2)
        {
            strncpy(c, (char*)byTmp + i, 2);
            c[2] = 0;
            bySendTmp[3 + i/2] = (BYTE)strtol(c, NULL, 16);
        }
    }
    else if (nParam == 1) // ���û����ɾ��
    {
        bySendTmp[3] = 0x00;
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);              // ��ȡ�û����
        sprintf((char*)byTmp, "%08d", nValue);    // 4λ�û����

        char c[3];
        i = 0;
        for (i = 0; i < 8; i = i + 2)
        {
            strncpy(c, (char*)byTmp + i, 2);
            c[2] = 0;
            bySendTmp[4 + i/2] = (BYTE)strtol(c, NULL, 16);
        }
    }
    else if (nParam == 2) // ȫ��ɾ��
    {
        i = 0;
        for (i = 0; i < 5; i++)
        {
            bySendTmp[3 + i] = 0x00;
        }
    }
	else
	{
		return FALSE;
	}

    return TRUE;
}


// ���·����ö��������ת��Ϊ���������
BOOL PrepareReadCmd(char* szValue,      // �·������
                    int nCmdNo,         // �����
                    BYTE *bySendTmp)    // ת���õ��������
{
    // �·������ʽ:(ɾ������0-2),ɾ������(����/�û���)
    BYTE byTmp[50]={0};     // ��ʱ������
    int nValue = 0;
    int nPoint = 0;
    int i = 0;
 //   WriteString("Prepaire step 1",1,24,0x1f);
//    Sleep(1000);
    if (nCmdNo < 36 && nCmdNo>30)    // ��ȡ����������ò���
    {
        for (i = 0; i < strSet[nCmdNo].nDataLen; i++)
        {
            bySendTmp[i+2] = strSet[nCmdNo].byData[i];
        }
    }
    else if (nCmdNo == 36)    // ��ȡSM��һ�鹤����׼��ʱ��
    {
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)nValue;        // ��ȡ��¼ָ���λ
    }
    else if (nCmdNo>36 && nCmdNo < 40)      // ����ʷ��¼λ��
    {
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)(nValue % 256);  // ��ȡ��¼ָ���λ
        bySendTmp[3] = (BYTE)(nValue / 256);  // ��ȡ��¼ָ���λ
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************
��������:SetDoor
����˵��:������������豸����
����˵��:���ھ�����豸��ַ������ţ��������
���˵��:���Ƴɹ�����TRUE�����򷵻�FALSE
*********************************************************************/
BOOL SetDoor(HANDLE hComm,  // ͨѶ�ھ��
             int nUnitNo,   // �ɼ�����Ԫ��ַ
             int nCmdNo,    // �����
             char* szValue, // �������
             BYTE*byData    // ���ƺ������(��Ҫ���ڶ�����)
             )
{
    BYTE bySendTmp[50]={0}; // �������:Command Group + Command Type + DataF
    BYTE byTmp[50]={0};     // ��ʱ������
    int  i = 0;
    int  nPoint = 0;    // ���������
    float fValue = 0;    // ����ֵ
    int nValue=0;
    int NN;//ʵ�ʿ��ų���
    int start_add;//����С��5λʱ��Ŀ�ʼλ��.
    char ADDCardID[50]={0}; //����

    //char *p;            // zhx    19991125
 //   WriteString(" setdoor step1",1,24,0x1f);
//        Sleep(1000);
    if (nCmdNo < 0 || nCmdNo>nMaxCmdNo)
    {
        return FALSE;   // �����
    }

    switch (nCmdNo)
    {
    case 0:     // ����Ȩ��ȷ��

        // ����:5���ֽڵ�����

        //p = szValue;
        //for (; p; )
        //{
        //    if (*p != ' ')
        //        break;
        //    p++;
       // }
        // strcpy(szValue, p);
        //assert(szValue = p);


        if (strlen(szValue) > 6) // ��һλ��д�������룬Ϊ��֤ͳһ���ʿ�ʹ��6λ����
        {
            return FALSE;   // ����̫��(�ü��̲���ʱΪ6λ�����ڵ�һλ��0)
            //              szValue[6] = 0; // zhx  19991125    ��"  0.000"(4357)�Ĵ���
        }
/*        fValue = (float)atof(szValue);

        sprintf((char*)byTmp, "%010.0f", fValue);
        char c[3];
        for (i = 0; i < 10; i = i + 2)
        {
            strncpy(c, (char*)byTmp + i, 2);
            c[2] = 0;
            bySendTmp[2 + i/2] = (BYTE)strtol(c, NULL, 16);
        }
*/
        memset(bySendTmp,0x00,12);//�����͵�5�ֽ���0
        NN=strlen(szValue);//ʵ������ĳ���.����123,����00123.����5λ.
        if (NN==0)//����û��
        {
            return FALSE;
        }
        if(NN>5)//6λ����ȡǰ5λ
        {
            NN=5;
        }
        start_add= 5-NN;

        int no;
        for( no=0;no<NN;no++)//ASCIIֱ��ת��������
        {
            if( (szValue[no]>='0')&&(szValue[no]<='9') )
            {
                bySendTmp[2+start_add+no] = szValue[no] - 0x30;
            }
            else if( (szValue[no]>='a')&&(szValue[no]<='f') )
            {
                bySendTmp[2+start_add+no] = szValue[no] - 0x61 + 10;
            }
            else if( (szValue[no]>='A')&&(szValue[no]<='F') )
            {
                bySendTmp[2+start_add+no] = szValue[no] - 0x41 + 10;
            }
        }

        break;
    case 1:    // SU����ȡ����SM������Ȩ��:�޲���
        break;
    case 2:    // �޸Ķ�SM�ķ�������
        // ����:5λ������+1λ��У����(��������)

        // ɾ���ź�
        nPoint = StrToK( szValue, (char*)byTmp, ',');
        if (byTmp== NULL || *byTmp == 0x00)
        {
            return FALSE;
        }

        if (strlen( (char*)byTmp ) > 6)
        {
            return FALSE;   // ����̫��(�ü��̲���ʱΪ6λ�����ڵ�һλ��0)
        }
/*        fValue = (float)atof(szValue+nPoint);

        sprintf((char*)byTmp, "%010.0f", fValue);
        bySendTmp[7] = 0;
        i = 0;
        for (i = 0; i < 10; i = i + 2)
        {
            strncpy(c, (char*)byTmp + i, 2);
            c[2] = 0;
            bySendTmp[2 + i/2] = (BYTE)strtol(c, NULL, 16);
            bySendTmp[7] ^= bySendTmp[2 + i/2];
        }
*/
        nPoint = StrToK( szValue+nPoint, (char*)byTmp, ',');

        if (strlen( (char*)byTmp ) > 6)
        {
            return FALSE;   // ����̫��(�ü��̲���ʱΪ6λ�����ڵ�һλ��0)
        }
        bySendTmp[7] = 0;

        memset(bySendTmp,0x00,10);//�����͵�5�ֽ���0
        NN=strlen((char*)byTmp);//ʵ������ĳ���.����123,����00123.����5λ.
        if(NN>5)//6λ����ȡǰ5λ
        {
            NN=5;
        }
        start_add= 5-NN;


        for( no=0;no<NN;no++)//ASCIIֱ��ת��������
        {
            if( (byTmp[no]>='0')&&(byTmp[no]<='9') )
            {
                bySendTmp[2+start_add+no] = byTmp[no] - 0x30;
            }
            else if( (byTmp[no]>='a')&&(byTmp[no]<='f') )
            {
                bySendTmp[2+start_add+no] = byTmp[no] - 0x61 + 10;
            }
            else if( (byTmp[no]>='A')&&(byTmp[no]<='F') )
            {
                bySendTmp[2+start_add+no] = byTmp[no] - 0x41 + 10;
            }

//            bySendTmp[7] ^= bySendTmp[2 + no];
        }
        for( no=0;no<5;no++)
        {
            bySendTmp[7] ^= bySendTmp[2 + no];
        }

        break;
    case 3:    // ����SM������ʱ��
        // ����:��(2)���£��գ����ڣ�ʱ���֣���
        if (!PrepareDateTime(szValue, bySendTmp))
        {
            return FALSE;
        }
        break;
    case 4:    // ���ù�����׼��ʱ��
    case 5:    // ���÷ǹ�����׼��ʱ��
        // ����:ʱ����ţ�ʱ��1��ʼʱ��ʱ��1��ʼ�֣�ʱ��1����ʱ��ʱ��1�����֣���4��
        if (nCmdNo == 14)
        {
            bySendTmp[2] = 1;           // �ǹ�����ֻ��һ��
        }
        else
        {
            memcpy(byTmp, szValue, 1);  // ���
            byTmp[1] = 0;
            if (byTmp == NULL || *byTmp == 0x00)
            {
                return FALSE;
            }
            bySendTmp[2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        }
        i = 0;
        for (i = 0; i < 32; i += 2)
        {
            memcpy(byTmp, szValue + 1 + i, 2);
            byTmp[2] = 0;
            if (byTmp == NULL || *byTmp == 0x00)
            {
                return FALSE;
            }
            bySendTmp[3 + i/2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        }
        break;
    case 6:    // �����û�
        // ����:5λ���ţ�4λ�û��ţ�2λ���룫��Ч��(YYYY,MM,DD)��Ȩ��(VIP,Group)
        if (!PrepareAddUser(szValue, bySendTmp, ADDCardID))
        {
            return FALSE;
        }
        break;
    case 7:    // ɾ���û�
        // ����:��ʽ��(0-2)+5λ����/00+4λ�û�ID
        if (!PrepareDelUser(szValue, bySendTmp))
        {
            return FALSE;
        }
        break;
    case 8:     // �趨�ſ������Բ���
    case 9:     // �趨�������̵���ִ��ʱ��
    case 10:    // �趨���ŵȴ�����ʱ��
    case 11:    // �趨�Ժ��ⱨ����ȷ��ʱ��
    case 17:    // ����ϵͳ֧�ָ�Ӧ�������༰��Ƭ��ŵĻ�ȡ����command type=EE
    case 12:    // �趨����ر�������ʱʱ��:50(ʮ����)��ʾ5��
    case 21:    // �趨ϵͳʹ�õĸ�Ӧ������:26��36��44��64
    case 22:    // �趨ϵͳ��Ӧ����ŵĻ�ȡ����command type=F3
    case 23:    // �趨��������I2����Ч��ƽ
    case 24:    // �趨ˢ������ʱ���Ƿ������û�����:4��0��9
    case 25:    // �趨�����Ӧ���ڱ���ʱ����Ч�����ƽ
    case 26:    // �趨�ſ��ظ�Ӧ���ڿ���״̬ʱ����Ч�����ƽ
    case 27:    // �趨�Ƿ������������I2��״̬
    case 28:    // �趨�ſص������(����)����
    case 29:    // �趨�Ƿ���Ӻ����Ӧ����״̬
    case 30:    // �趨�Ƿ�����ſ��ظ�Ӧ����״̬
        // Ϊ�˼��������Ž�����ȡ���źŲ�����
        //MessageBox(NULL, (char*)szValue, "����", MB_OK);
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)atoi(szValue+nPoint);
        break;
    case 13:    // �趨SM��һ��������Ϣ��:1��6Ϊ����һ������7Ϊ�����գ�����Ϊ����Ϣ
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = atoi((char*)byTmp);   // ��һ����Ϣ��
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = atoi((char*)byTmp);   // �ڶ�����Ϣ��
        break;
    case 14:    // �趨SM�Ľڼ���(��������������Ϣ��)
    case 15:    // ɾ��SM�ڵ�һ��ڼ���(���ֽھ�Ϊ0��ʾȫ��ɾ��)
        nPoint = StrToK(szValue, (char*)byTmp, ',');            // ��
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');  // ��
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = (BYTE)strtol((char*)byTmp, NULL, 16);
        break;
    case 16:    // SUԶ�̿���
        bySendTmp[2] = 1;  // ������ʽ:1�����ţ�0��������
        break;
    case 18:    // �趨SM�����ؿ���/�رշ���
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)atoi((char*)byTmp);  // ����/�رշ���������
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = (BYTE)atoi((char*)byTmp);  // ������ʱʱ��
        break;
    case 19:    // �趨SM�ڼ�¼����ָ��:(ʹ�����ֽڷ�ʽ:��ȡ��¼ָ��,��־)
        nPoint = StrToK(szValue, (char*)byTmp, ',');  // ��ȡ��¼ָ��2�ֽ�
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)(nValue % 256);  // ��ȡ��¼ָ���λ
        bySendTmp[3] = (BYTE)(nValue / 256);  // ��ȡ��¼ָ���λ

        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[4] = (BYTE)atoi((char*)byTmp);      // ��־
        break;
    case 20:    // �趨������׼����ʱ����б�
        memcpy(byTmp, szValue, 1);    // ���
        byTmp[1] = 0;
        //if (byTmp == NULL || *byTmp == 0x00)
		if (byTmp == NULL )
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        memcpy(byTmp, szValue + 1, 1);    // ���ں�
        byTmp[1] = 0;
        //if (byTmp == NULL || *byTmp == 0x00)
		if (byTmp == NULL )
        {
            return FALSE;
        }
        bySendTmp[3] = (BYTE)strtol((char*)byTmp, NULL, 16);
        i = 0;
        for (i = 0; i < 24; i += 2)
        {
            memcpy(byTmp, szValue + 2 + i, 2);
            byTmp[2] = 0;
            if (byTmp == NULL )//|| *byTmp == 0x00)
            {
                return FALSE;
            }
            bySendTmp[4 + i/2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        }
        break;
    default:            // ���ڲ�����
#ifndef _TSR_
        //bTestFlag = TRUE;   // ������ʱ��ʾ�շ���Ϣ
#endif

        if (!PrepareReadCmd(szValue, nCmdNo, bySendTmp))
        {
            return FALSE;
        }
        break;
    }

    bySendTmp[0] = strSet[nCmdNo].byCmdGroup;    // ������:command group
    bySendTmp[1] = strSet[nCmdNo].byCmdType;     // ��������:command type
 //       WriteString(" setdoor step2",1,24,0x1f);
//        Sleep(1000);
    i = 0;
    int nRet = 1;
    for (i = 0; i < 3; i++)
    {
        nRet = GetResponeData(hComm, 0x10, nUnitNo, 0x80, strSet[nCmdNo].byCID2,
            strSet[nCmdNo].nDataLen + 2, byData, strSet[nCmdNo].nRetLen,bySendTmp);

        // ��ʾ������:
        //char s[100]={0};
        //sprintf( s, "%X", nRet );
        //MessageBox(NULL, s, "���ƽ��", MB_OK);

        // ��ɾ����ʱ������洢�ռ��ѿգ������0xE4������Ϣ������û�д˿��������E5������Ϣ
        if( nCmdNo==7 && (nRet==0xE5||nRet==0xE4) )
        {
            nRet = 0;
        }

#ifndef _TSR_
        //bTestFlag = FALSE;  // ��ʾ�շ���Ϣ
#endif
//        WriteString(" setdoor step3",1,24,0x1f);
//        Sleep(1000);

        Sleep(100);         // ����������֮��ӵ���ʱ,��������ʧ��
        if (!nRet)          // �ɹ�
        {
            return TRUE;
        }
        else if (nRet != nErrorEF && nRet != nErrorEE)
        {
            break;
        }
    }

    //5.40   �����ӿ���ʱ�򣬷����Ѿ��п�����ɾ�����������ӿ�
    //add by L&L 2010.03.18
    if ( nCmdNo==6 && nRet==0xE7 )
    {
        //�������ӿ���szValue��byData,��szValue�л�ÿ���:ADDCardID
        //��ɾ����
        char ReAddszValue[50]={0};
        sprintf(ReAddszValue, "0,%s", ADDCardID);    // �����ݿ���ɾ��Э�鷽ʽ


        WriteAsc(ASC_FILE, "\r\n���п��ţ���ɾ������:%s \r\n", ReAddszValue);
        WriteAsc(HEX_FILE, "\r\n���п��ţ���ɾ������:%s \r\n", ReAddszValue);


        nRet = SetDoor(hComm, nUnitNo, 7, ReAddszValue, byData);



        if (nRet)  //ɾ�����ɹ�//�����ӿ�
        {
            //bTestFlag = TRUE;
            WriteAsc(ASC_FILE, "\r\n�����ӿ�:%s \r\n", szValue);
            WriteAsc(HEX_FILE, "\r\n�����ӿ�:%s \r\n", szValue);
            //bTestFlag = FALSE;

            nRet = SetDoor(hComm, nUnitNo, 6, szValue, byData);
            return nRet;
        }


    }

    return FALSE;
}


//*****************************************************************
// �������ƣ�Write
// ����������V4.0�Ȱ�Ŀ��ƺ�����
// ���������hComm - ͨ�ž��, nUnitNo - �ɼ�����Ԫ��ַ,
//           pCmdStr - �����������ǰ��������ں��á�,���ָ
// ���������
// ��    �أ�TRUE���ɹ���FALSE��ʧ�ܡ�
// ��    ����
//*****************************************************************
DLLExport BOOL Write(
                     HANDLE hComm,      // ͨѶ�ھ��
                     int nUnitNo,       // �ɼ�����Ԫ��ַ
                     char* pCmdStr )    // �������
{
    // ���ǵ�Ŀǰ������ϵͳ�汾�� Write�ӿڵ��ص㣬�ʲ���֧�ֱ����ܡ�
    //hComm;
    //nUnitNo;
    //pCmdStr;
    return FALSE;
}

/**********************************************************************
��������:Control_old�������PSMS4.357����
����˵��:�˺���Ϊ��������ӿڣ�����4.x��DLL��TSR��
���˵��:���Ƴɹ�,����TRUE,���򷵻�FALSE.
*********************************************************************/
DLLExport BOOL Control_old(HANDLE hComm, int nUnitNo, char *pCmdStr)
{
/*
#ifdef _DLL_
    fpASCII  = fopen("ES2100.ASC", "ab");
    fpBINARY = fopen("ES2100.HEX", "ab");
#endif
*/

#ifndef _TSR_
    if( bTestFlag )
    {
        //MessageBox(0, pCmdStr, "��������", MB_OK);
        //WriteAsc(ASC_FILE,"%s","\r\n��������\r\n");
        //WriteAsc(HEX_FILE,"%s","\r\n��������\r\n");
    }
#endif
//    WriteString(" controlold step1",1,24,0x1f);
    char sTarget[100]={ "0x00" };
    char szValue[100]={ "0x00" };
    BYTE byData[MAXINFOLEN/2]={0};    // ���Ʒ��ز���

    if (strlen(pCmdStr) > 100)
    {
#ifndef _TSR_
//        fcloseall();
#endif
        return FALSE;   // ���̫��
    }
    // Get nCmdNo
    int nPoint = StrToK(pCmdStr, sTarget, ',');

//    WriteString(" controlold step2",1,24,0x1f);
//    Sleep(1000);
    if (pCmdStr == NULL || *pCmdStr == 0x00)
    {
#ifndef _TSR_
//        fcloseall();
#endif
        return FALSE;
    }
    int nCmdNo = (int)atoi(pCmdStr);  // ��ȡ�����


    if (nPoint)
    {
        strcpy(szValue, pCmdStr + nPoint);  // ��ȡ�������
    }

    if (nCmdNo == 16 || nCmdNo == 17)  // �������������1/������0
    {
        sprintf(szValue, "%c", 65-nCmdNo);  // ����
        nCmdNo = 89;                        // �����
//        WriteString(" controlold step3",1,24,0x1f);
//        Sleep(1000);
    }
    else if (nCmdNo == 31)        // ˢ�������Ƿ���Ҫ����
    {
        nCmdNo = 84;
    }
    else if (nCmdNo == 36)        // ����
    {
        // ���š��û��š����롢Ⱥ���
        char sCardID[100], sCardNo[100], sPassWord[100], sGroup[100], sDate[20];
        nPoint = StrToK(szValue, sTarget, '+');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardNo, sTarget);  // �û���
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardID, sTarget);  // ����
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sGroup, sTarget);  // Ⱥ��(10����)
        //int nGroup = atoi( sGroup );��Ȩ�û�CO,�ô˺�����ʧЧ
        int nGroup = (BYTE)strtol(sGroup, NULL, 16);

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy( sPassWord, sTarget);  // ����

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy( sDate, sTarget);  // ��Ч��

        // Э��ĸ�ʽ:(����),(�û���),(4λ����)(��Ч��)(�û�Ȩ��)
        //sprintf(szValue, "%s,%s,%04s99991231%02X", sCardID, sCardNo, sPassWord, nGroup);
        //MessageBox(0, sDate, "��������", MB_OK);
        sprintf(szValue, "%s,%s,%04s%s%02X", sCardID, sCardNo, sPassWord, sDate, nGroup);
    }
    else if (nCmdNo == 37)    // ɾ����
    {
        char sCardID[50]={0}, sCardNo[50]={0};   // ���š��û���
        nPoint = StrToK(szValue, sTarget, '+');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardNo, sTarget);   // �û���
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardID, sTarget);   // ����
        sprintf(szValue, "0,%s", sCardID);    // �����ݿ���ɾ��Э�鷽ʽ
    }
    else if (nCmdNo == 42)          // ����������ʱ����
    {
        nCmdNo = 80;
        // ȡ�ź�
        nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
        if (pCmdStr == NULL || *pCmdStr == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        // ȡʱ�����:0-15
        nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
        if (pCmdStr == NULL || *pCmdStr == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        int nGroup = (int)atoi(sTarget) - 1;  // ��ȡʱ�����
        char cGroup;    // ʱ����
        if (nGroup < 10)
        {
            cGroup = nGroup + 0x30; // ת��Ϊ0��9
        }
        else
        {
            cGroup = nGroup + 0x37; // ת��ΪA��F
        }

        // ���ں�
        char cWeek = pCmdStr[nPoint + 24];
        if( '7' == cWeek)
        {
            //cWeek=0;������ַ���0x37
            cWeek=0x30;
        }

        if (nPoint)
        {
            // ��ȡ�������:ʱ���������ʱ���
            sprintf(szValue, "%c%c%s", cGroup, cWeek, pCmdStr + nPoint);
        }
    }
    else if (nCmdNo == 45)           // ɾ�����п�
    {
        sprintf(szValue, "2, 0");    // �����ݿ���ɾ��Э�鷽ʽ
        nCmdNo = 37;
    }
    else if (nCmdNo == 55)           // �������ˢ����¼
    {
        // ��ȡ��ǰ��ʷ��¼������:����Ų��þ������������
        if (!SetDoor(hComm, nUnitNo, 31, "", byData))
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        // ����ǰָ���ƶ��������洢���ݴ�
        sprintf(szValue, "%d,%d", byData[2] + byData[3]*256, byData[6]);
        nCmdNo = 79;    // ����ָ��:������辭�������������
    }


    // ��Ϊ�Ž����ϲ�ӿ����Ǳ仯������Ų�������ʹ�ñ����ҷ�ʽ��ʹ�Ժ��޸ķ��㡣
    for (int i = 0; i < nMaxCmdNo; i++)
    {
        if (nCmdNo == strSet[i].nCmdNo)
        {
            nCmdNo = i;
            int nRet = SetDoor(hComm, nUnitNo, nCmdNo, szValue, byData);

#ifndef _TSR_
//            fcloseall();
#endif
            return nRet;
        }
    }

#ifndef _TSR_
//    fcloseall();
#endif
    return FALSE;
}

#ifdef _TSR_
char sTarget[100]={0};
char sStr[120]={0};
char sCmdStr[120]={0};
char sTmp[10]={0};  // ��ʱ������
char sTime[50]={0}; // �����ʱ����
#endif

/**********************************************************************
��������:Control�������PSMS4.357
����˵��:�˺���Ϊ��������ӿ�
���˵��:���Ƴɹ�,����TRUE,���򷵻�FALSE.
ע������:ֻҪ������֤�ɹ��󣬲��ܿ����Ƿ�ɹ�������ȡ������
*********************************************************************/
DLLExport BOOL Control(HANDLE hComm, int nUnitNo, char *pCmdStr)
{
    int nRet=0;
    //bTestFlag = TRUE;

    //MessageBox(0, pCmdStr, "��������", MB_OK);
/*    if( hComm==0)
    {
        WriteString("Control hComm is NULL", 1, 24, 0x1f);
        return TRUE;
    }
  */

//#ifdef _DLL_

    WriteAsc(ASC_FILE,"6.00�������:%s\r\n",pCmdStr);
    WriteAsc(HEX_FILE,"6.00�������:%s\r\n",pCmdStr);

//#endif

#ifndef _TSR_
    char sTarget[100]={0};
#endif

    //  char sTarget[50]={0};
    if (strlen(pCmdStr) > 100)
    {
        return FALSE;   // ���̫��
    }

    // Get nCmdNo
    int nPoint = StrToK(pCmdStr, sTarget, ',');
    if (pCmdStr == NULL || *pCmdStr == 0x00)
    {
        return FALSE;
    }
    int nCmdNo = (int)atoi(pCmdStr);  // ��ȡ�����

    // ��ȡ��һ������
    // Get Password:����PSMS4.357����ÿ�����������һ��������Ϊ����



      nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }


    //WriteString("step1",1,24,0x1f);
    //Sleep(1000);
    // ��֤����


#ifndef _TSR_
    char sStr[120]={0};
#endif




        sprintf(sStr, "60,%s", sTarget);
        if (!Control_old(hComm, nUnitNo, sStr))
        {

            WriteAsc(ASC_FILE,"Ȩ��ȷ��ʧ��:���� %s\r\n",sTarget);
            WriteAsc(HEX_FILE,"Ȩ��ȷ��ʧ��:���� %s\r\n",sTarget);
            return FALSE;
        }

        WriteAsc(ASC_FILE,"Ȩ��ȷ�ϳɹ�:���� %s\r\n",sTarget);
        WriteAsc(HEX_FILE,"Ȩ��ȷ�ϳɹ�:���� %s\r\n",sTarget);



    ////add 5.40/////////////////////////////////////////////////////////////////////////
    //����Ȩ��ȷ��OK�����������ģ��򱣴�������

#ifdef _LINUX_
    FILE *fp;
    char opwd[8] = { 0 };
    if ((fp = fopen("/home/idu/SO/EDM30e.ini", "rb")) != NULL)
    {
        fgets(opwd, 7, fp);
        fclose(fp);
    }

    if( strcmp(opwd, sTarget) != 0 )    // ���벻ͬ 5.50
    {
        if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"wb+")) == NULL)
        {
            printf("�����ļ������ڻ��޷���ȡ���룡\n");
            return FALSE;
        }

        WriteAsc(ASC_FILE,"Ȩ��ȷ�ϳɹ�д������ %s\r\n",sTarget);
        WriteAsc(HEX_FILE,"Ȩ��ȷ�ϳɹ�д������ %s\r\n",sTarget);


        fprintf(fp, "%s\n", sTarget);
        fclose(fp);
    }
#endif


    //  WriteString("Error2",1,24,0x1f);
    //    Sleep(1000);
    // ��ȡ��һ���Ժ�Ĳ���:
#ifndef _TSR_
    char sCmdStr[120]={0};
#endif



    sprintf(sCmdStr,"%d,%s", nCmdNo, pCmdStr + nPoint);

    //42,mmmmmm,d,02,00012C1902582BC3E94B05145786406A47D06
    //42,mmmmmm,dd,tt,6��ʱ���1�ֽ�����.
    //mmmmmm:���룬dd:�źţ�tt:ʱ����ţ�ȡֵ[0,15]��    6��ʱ���36λ��ÿ��λ16����Ϊһ��ʱ�䣩����1λ
    // ���⴦��:��PSMS4.357�·����ڱ������轫ʱ���ѹ����:3��16��ֵ��>4��ʮ����
    if( nCmdNo==42 )    // ����������ʱ����
    {
        // ��ȡ�����
        nPoint = StrToK(sCmdStr, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");

            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // ��ȡ�ź�
        nPoint += StrToK(sCmdStr+nPoint, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // ��ʱ�����
        nPoint += StrToK(sCmdStr+nPoint, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
#ifndef _TSR_
        char sTmp[10]={0};  // ��ʱ������
        char sTime[50]={0}; // �����ʱ����
#endif
        int nTime=0;        // ʱ����
        // ʱ���鴦��:
        int i=0;
        //cxd for(  i=0; i<12; i++ )???
		for(  i=0; i<6; i++ )
        {
            strncpy( sTmp, sCmdStr+nPoint+i*4, 4 );
            nTime = (int)strtol( sTmp, NULL, 16 );
            sprintf( sTime+i*4, "%04X", nTime );
        }
        // ȡ���ں�
        char cWeek = sCmdStr[nPoint+i*4];
        sprintf( sCmdStr+nPoint, "%s%c", sTime, cWeek );
    }

    // ���⴦��:���PSMS4.357�޸Ŀ�
    if( nCmdNo==38 )    // �޸Ŀ�:��ɾ���ÿ��������Ӹÿ��������Ŵ���
    {
        // UID+����,Ȩ��,����

        // ��ȡ�����
        nPoint = StrToK(sCmdStr, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // ��ȡ�û���
        int nPoint1 = StrToK(sCmdStr+nPoint, sTarget, '+');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }

        // ��ȡ����
        nPoint1 += StrToK(sCmdStr+nPoint+nPoint1, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // ��ɾ���ÿ�:
#ifndef _TSR_
        char sStr[100]={0};
#endif
        sprintf( sStr, "37,0+%s", sTarget );
        if( !Control_old( hComm, nUnitNo, sStr ) )
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }

        // ���ӿ�:һ����˵���ӿ��ɹ�������ȡ��Ҳ��ɹ����ʲ������ɹ���־��
        // cmq 2000.04.04 ������־
        sprintf( sStr, "36,%s", sCmdStr+nPoint );
        nRet = Control_old( hComm, nUnitNo, sStr );
        if( !nRet )
        {
            // ����ȡ������Ȩ�ޡ�
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
    }//if
    else
    {
        // ��������:һ����˵���ӿ��ɹ�������ȡ��Ҳ��ɹ����ʲ������ɹ���־��
        // 2000.4.4 cmq�޸�Ϊ����
        nRet = Control_old(hComm, nUnitNo, sCmdStr);
        // ����ȡ������Ȩ�ޡ�
        sprintf(sStr, "61");
        Control_old(hComm, nUnitNo, sStr);

        if (nCmdNo==62)
        {
            // Get ����
            int nPoint = StrToK(sCmdStr, sTarget, ',');
            if (pCmdStr == NULL || *pCmdStr == 0x00)
            {
                return FALSE;
            }

            // �ź�
            nPoint += StrToK(sCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }

            // ������
            nPoint += StrToK(sCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }

            ////add 5.40/////////////////////////////////////////////////////////////////////////
            //����Ȩ��ȷ��OK�󣬱�������
#ifdef _LINUX_
            FILE *fp;
            if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"wb+")) == NULL)
            {
                printf("�����ļ������ڻ��޷���ȡ���룡\n");
                return FALSE;
            }

            WriteAsc(ASC_FILE,"�޸�����ɹ���д�������� %s\r\n",sTarget);
            WriteAsc(HEX_FILE,"�޸�����ɹ���д�������� %s\r\n",sTarget);


            fprintf(fp, "%s\n", sTarget);
            fclose(fp);
#endif

        }

        //bTestFlag=FALSE;
        return nRet;
    }
    // ����ȡ������Ȩ�ޡ�2000.04.04������Ʋ��ɹ���10���Ӻ󣬻��Զ�ȡ��Ȩ�ޣ��ʿ��ƽ��Ч����ԶЩ
    sprintf(sStr, "61");
    Control_old(hComm, nUnitNo, sStr);

    return TRUE;
}




////////////////////////////////////////////////////////////////////////////////
//
// �ֳ����ⲿ��:��ʼ�����ڡ��ɼ��Ϳ��ƽӿڵĵ��õȡ�
//
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG_
// �����ֳ����Ա��ʱ��ģ��ϵͳ���ú�����

#define nSetTimeOut 2000    // ���崮�ڳ�ʱ����:2S���ɸ�����Ҫ�޸ġ�

// ��ʼ������:ComPort�����ںţ�szSetting��ͨѶ��ʽ:�����ʡ�У�顢����λ��ֹͣλ��
HANDLE OpenComm(int ComPort, char *szSetting)
{
    char szCom[20]={0};
    HANDLE hComm;

    sprintf(szCom,"COM%d", ComPort);
    hComm = CreateFile(szCom, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, 0, NULL);

    if (hComm == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    COMMTIMEOUTS tm={ 0, 1, nSetTimeOut, 1, nSetTimeOut };
    DCB dcb;

    if (SetupComm(hComm, 1024, 1024))
    {
        GetCommState(hComm, &dcb);
        if (BuildCommDCB(szSetting, &dcb))
        {
            if (SetCommState(hComm, &dcb))
            {
                if (SetCommTimeouts(hComm,&tm))
                {
                    return hComm;
                }
            }
        }
    }

    CloseHandle(hComm);
    return FALSE;
}

void CloseComm(HANDLE hComm)
{
    CloseHandle(hComm);
}

//------------------------------------------------------------------
void main(void)
{
    float AData[200];
    HANDLE hComm;
    char CommSetting[100]={0};

    sprintf(CommSetting, "9600, n, 8, 1");
    hComm = OpenComm(1, CommSetting);
    if (!hComm)
        exit(0);

    // ����ͨ�����ƽ����Զ�̲����á�
    // sprintf(Buf,"%s\r\n","ATDT2402944");
    // WriteFile(hComm, Buf, 13, &lWritten, NULL);
    // ReadFile(hComm, Buf, 30, &lRead, NULL);

//    fpASCII  = fopen("ES2100.ASC", "ab");
//    fpBINARY = fopen("ES2100.HEX", "ab");

    //Control( hComm, 1,"307,000000");
    do
    {
        Read(hComm, 1, AData);
    }
    while(!kbhit());
    //Control    (hComm, 1, "42,123456,1,02,0000 0300 0400 0600 0700 1000 1200 1300 1400 1600 1700 2000 6");    // ����ʱ���
    //Control(hComm, 1, "42,123456,1,02,000 12C 190 258 2BC 3E9 4B0 514 578 640 6A4 7D0 6");    // ����ʱ���
    Control(hComm, 1, "42,123456,1,02,00012C1902582BC3E94B05145786406A47D06");    // ����ʱ���

    Control(hComm, 0, "45,000000");    // ɾ�����п�
    Control(hComm, 0, "10,000000,1");  // ����
    Control(hComm, 4, "31,000000,1,0");
    Control(hComm, 1, "30,0,1,20");
    Control(hComm, 4, "62,000000,1,111111");    // �޸�����
    Control(hComm, 4, "36,000000,11+000000BCDF,C0,1111");   // 3.357
    Control(hComm, 4, "36,000000,01+000000BCDF,C0,1111,19991231");   // 3.39
    Control(hComm, 4, "38,000000,11+000000BCDF,12,1111");
    Control(hComm, 1, "32,111111,199909141625002");  // ��������

    Control_old(hComm, 1, "38, 0, 0");
    Control_old(hComm, 1, "300");
    Control_old(hComm, 1, "61");       // ȡ������
    Control_old(hComm, 1, "60,222222");   // У������
    Control_old(hComm, 1, "62,1,222222"); // �޸�����

    Control_old(hComm, 1, "55");    // ������м�¼
    Control_old(hComm, 1, "32,199909141625002");  // ��������
    Control_old(hComm, 1, "64,100001200160018001900200022002359");    // ����ʱ���

    Control_old(hComm, 1, "37,0+5074664");  // ɾ����4D6EE8

    // Control_old(hComm, 1, "36, 5077815, 3, 000019991231C0");  // ���ӿ�
    // Control_old(hComm, 1, "36, 5074664, 1, 00000010010101");  // ���ӿ�4D6EE8
    // Control_old(hComm, 1, "36, 38379688, 800, 000019991231C0");  // ���ӿ�249a0a8
    Control_old(hComm, 1, "36, 800 + 38379688, C0, 0000");  // ���ӿ�249a0a8
    Control_old(hComm, 1, "16, 1");    // ���Ⲽ��

    Control_old(hComm, 1, "68, 0");    // �趨SM�ſ������Բ���
    Control_old(hComm, 1, "69, 81");   // 14��17ʱ��
    Control_old(hComm, 1, "73, 6, 7");  // ������Ϣ��
    Control_old(hComm, 1, "74, 1, 15"); // �ڼ���
    Control_old(hComm, 1, "75, 10, 1"); // ɾ���ڼ���
    Control_old(hComm, 1, "10, 1");    // ����

    Control_old(hComm, 1, "79, 390, 0");  // �趨��¼��ָ��

    // ����ʱ���
    Control_old(hComm, 1, "80, 05000003000400060007001000120013001400160017002000");

    char sStr[50]={0};  // ����׼����Ա��
    for (int i = 801; i < 3000; i++)
    {
        sprintf(sStr,"36,%d,%d,%04d1999123100", i, i, i);
        if (!Control_old(hComm, 1, sStr))
            break;
    }
    // ����ͨ�����ƽ����Զ�̲����á�
    // sprintf(Buf,"%s\r\n","+++ATH");
    // WriteFile(hComm, Buf, 6, &lWritten, NULL);
    // ReadFile(hComm, Buf, 30, &lRead, NULL);

    fcloseall();

    CloseComm(hComm);
}
#endif
