/**************************************************************************
版权声明:1999-2100，艾默生网络能源有限公司
设备介绍:EDM30E脉冲锁一体化门禁(**由ES2100修改而来**)
文 件 名:EDM30E.cpp(请使用Ctrl+H替换功能将本文件名称全部更换为文件和工程的名称)
开发工具:VC6.0
作    者:LYG
版 本 号:V4.O0
日      期:2005-11-25
开发地点:深圳
应用地点:
说      明:本协议软件在ES2000门禁基础上修改而成。

  修改记录:修改人、日期、修改原因、版本变动情况。
    1. (这个是ES2100的修改记录)
       PJC 2007.5.20 黑龙江网通湘江路IDC机房监控IDU 测试不成功修改 V4.00->V4.01->V5.00

    2. L&L 2008.04.08 一体化门禁协议(李鑫开发的).对内型号:EDM-11Z.对外型号:EDM-30E.
       版本由: ES2100.cpp => EDM30E.cpp
       里面修改的地方,全用 ////L&L ADD/////标记出来了.
       控制里面的修改没有标记

    3. L&L 2010.03.17 配合张进的门禁，以及以前的问题，重新升级为5.40
        1，时间差大于10s，就自动校时。
        2，加卡时，若是有卡，就删除以前的卡，再加新卡
        3，第四条采集命令，只用一天读一次。
        4，MDB库做两个，一个信号全，一个简单信号
        5, 调整了命令采集顺序，将时间最后采集，这样门禁自动校时功能才匹配
        6, 自动校正时间的阈值为30s

    4. qilei 2010.08.17 集成组从西研接手门禁库，修改其中问题
        1，删除Control_old中无用且可能造成问题的fcloseall调用。（Linux中fcloseall函数仅实现fflush操作，未实现fclose操作）
        2，将每次Control调用都将当前密码写入ini文件，改为当密码改变时写入ini文件
        3，升级版本号为5.50
    5. qilei 2010.08.25 修改Read函数中自动校时部分代码，tm结构手动分配；
    6. qilei 2010.09.03 调整代码，当接收到其他设备异常返回的信息或者错误信息后，重新再进行数据接收，最多3次；避免其他设备的干扰；
    7. qilei 2010.09.06 扩大接收缓冲区（数据，byData）长度，以接收更大的数据包（错误数据）；该修改解决了之前的一个问题:
        当同一串口其他设备误反馈门禁消息时，如果数据包太大，会造成门禁SO接收缓冲区溢出（代码传递长度参数大于缓冲区长度）；
    8. qilei 2010.09.06 完善错误信息字符串，根据协议文档增加04、05、E7的错误说明；
    9. zhj   2010.09.15 修改软件Bug,当返回异常时，重试3次。增加返回数据包长度的判断。北京联通 V5.50-->V5.60
    10. qilei 20101221 当发送地址为255时，反馈地址不再做检查；V5.60-V5.70；
    11.cxd 2013.10.31 增加BIN文件V5.71
	12、WXG 2015-3-3 添加最大读卡数量及已添加卡数量的信号 V5.71--》V5.72
	13、WXG 2015-07-22 修改删除卡函数并升级 V5.72--》V6.00

  说    明:描述该程序文件完成的主要功能等
  其    他:

    函数清单:
    1. 版本信息函数:
        DLLExport char* DLLInfo( void )
    2. V4.0/TSR/OCE 数据采集函数:
        DLLExport BOOL Read( HANDLE hComm, int nUnitNo, void *pData )
    3. V4.0/PowerStar 控制函数（参数有区别）:
        DLLExport BOOL Write( HANDLE hComm, int nUnitNo, char *pCmdStr )
    4. V4.1/V5.0 PowerStar 数据采集函数:
        DLLExport BOOL Query( HANDLE hComm, int nUnitNo, ENUMSIGNALPROC EnumProc, LPVOID lpvoid )
    5. V4.1/V5.0/TSR/OCE 控制函数:
        DLLExport BOOL Control( HANDLE hComm, int nUnitNo, char *pCmdStr )
    6. 跟踪测试采集函数
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
    // 共用头文件的定义，适用于 VC++ 和 BC++部分
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

    #define _OCE_        // OCE驱动程序用

    #ifdef  _DLL_
        #undef  _OCE_    // DLL驱动程序用
    #endif

    #ifdef  _TSR_
        #undef  _OCE_    // TSR驱动程序用
    #endif

    #ifdef  _OCE_
        #define  _TSR_   // TSR驱动程序用
    #endif

    // 动态库使用部分
    #ifdef _DLL_
        #pragma comment(lib,"Ws2_32.lib")
        #include "local.h"
        #include "snuoci5.h"
    // 跟踪测试标志:TURE－跟踪测试运行，要调试信息；FALSE－普通运行。
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
    // 跟踪测试用的数据记录文件名，请根据具体协议更改！
    char HEX_FILE[]={"EDM30E.HEX"};
    char ASC_FILE[]={"EDM30E.ASC"};
    char ErrLog_FILE[]={"EDM30E.Log"};
#endif

//////////////////////////////////////////////////////////////////////////////
#ifndef _LINUX_
    const int nMaxChanelNo = 600;    // 最大采集信号个数
#else
    int nMaxChanelNo=600;
#endif

/////////////////////////////////////////////////////////////////////////////
//#define MAXINFOLEN 4096
#define MAXINFOLEN 512

#define BOITRYTIMES  100

//#define nMaxChanelNo 200 //处理完后的数据大小，为150个FLOAT

//全局变量区域
BYTE g_byBOI = 0x7E;        // 包头字符
BYTE g_byEOI = 0x0D;        // 包尾字符
BYTE g_byVersion = 0x10;    // 版本0.0,实际未使用

//BYTE byaPublicBuffer[MAXINFOLEN];
//BYTE g_byaTypeUnitRelation[256];

typedef struct
{
    int  nCmdNo;        // 命令编号
    BYTE byCID2;        // CID2
    BYTE byCmdGroup;    // Command Group
    BYTE byCmdType;     // Command Type
    BYTE byData[50];    // Data
    int  nDataLen;      // Data Len
    WORD  nRetLen;       // Return Data Len
}
STRSET;

// 采集命令集:
STRSET strRead[] =
{
    //将读取时间喝参数对调
    // 0读取SM的门（门锁）特性参数
  //{ 0, '\x4a', '\xf2', '\xe0', "\x00", 1, 8 },
    { 0, '\x4a', '\xf2', '\xe8', "\x00", 1, 5 },
        // 1读取SM的一条历史记录:自动方式
    { 1, '\x4a', '\xf2', '\xe2', "", 0, 14 },
    // 2读取SM的监控状态
    { 2, '\x4a', '\xf2', '\xe7', "\x00", 1, 3 },
    // 3读取SM的实时钟
  //{ 3, '\x4a', '\xf2', '\xe8', "\x00", 1, 5 },
    { 3, '\x4a', '\xf2', '\xe0', "\x00", 1, 8 },

	// 4读取SM内已设入的用户数目
    { 4, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },

    // 4读取SM的历史记录柜桶
    //{ 4, '\x4a', '\xf2', '\xe1', "", 0, 9 },
    // 5读取SM的一条历史记录:指定位置，可由E1命令获得
    { 5, '\x4a', '\xf2', '\xe2', "\x01\x00", 2, 14 },
    // 6读取SM的一组工作日准进时段
    { 6, '\x4a', '\xf2', '\xe3', "\x01", 1, 16 },
    // 7读取SM的非工作日准进时段
    { 7, '\x4a', '\xf2', '\xe4', "\x01", 1, 16 },
    // 8读取SM内已设入的用户数目
    { 8, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },
    // 9读取SM内指定用户资料
    { 9, '\x4a', '\xf2', '\xe6', "\x01\x00", 2, 16 },
    // 10读取SM控制的门(门锁)之设定每星期内固定的休息日
    { 10, '\x4a', '\xf2', '\xe9', "\x00", 1, 2 },
    // 11读取SM控制的门(门锁)之设定休息日(星期内休息日除外)
    { 11, '\x4a', '\xf2', '\xea', "\x00", 1, 16 },  // 接收个数可变
    // 12读取SM控制的门(门锁)之星期一至星期日的准进时间表
    { 12, '\x4a', '\xf2', '\xeb', "\x00\x01", 2, 24 },  // 接收个数可变
};

// 控制命令集:
STRSET strSet[] =
{
    // 0设置权限确认
    { 60, '\x48', '\xf0', '\xe0', "\x00\x00\x00\x00\x00", 5, 0 },
        // 1主动取消SM的设置权限
    { 61, '\x48', '\xf0', '\xe1', "", 0, 0 },
    // 2修改SM的访问密码
    { 62, '\x48', '\xf0', '\xe2', "\x00\x00\x00\x00\x00\x00", 6, 0 },

    // 3设置日期时间:1999年08月24日星期2，17:00:00
    { 32, '\x49', '\xf1', '\xe0', "\x19\x99\x08\x24\x02\x17\x00\x00", 8, 0 },
    // 4设置工作日准进时段:00:00-23:59
    { 64, '\x49', '\xf1', '\xe1', "\x01\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", 17, 0 },
    // 5设置非工作日准进时段:00:00-23:59
    { 65, '\x49', '\xf1', '\xe2', "\x01\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", 17, 0 },
    // 6增加用户
    { 36, '\x49', '\xf1', '\xe3', "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 16, 0 },


    // 7删除用户
    //LYG,2005-12-7
    { 37, '\x49', '\xf1', '\xe4', "\x00\x00\x00\x00\x00\x00", 6, 0 },



    // 8设定SM特性参数
    { 68, '\x49', '\xf1', '\xe5', "\x00", 1, 0 },
    // 9设定门锁继电器执行时间:(0.1-25.5S)
    { 69, '\x49', '\xf1', '\xe6', "\x0a", 1, 0 },
    // 10设定开门等待进入时间:(0.1-25.5S)
    { 30, '\x49', '\xf1', '\xe7', "\x0a", 1, 0 },
    // 11设定红外报警的确认时间:(0.1-25.5S)
    { 71, '\x49', '\xf1', '\xe8', "\x0a", 1, 0 },
    // 12设定异地报警的延时时间:(0.1-25.4S，0－关闭，255－取消)
    { 72, '\x49', '\xf1', '\xe9', "\x0a", 1, 0 },
    // 13设定SM的一星期内休息日:周六、周日
    { 73, '\x49', '\xf1', '\xea', "\x06\x07", 2, 0 },
    // 14设定SM的节假日:10月1日
    { 74, '\x49', '\xf1', '\xeb', "\x10\x01", 2, 0 },
    // 15删除SM内的一组节假日:10月1日
    { 75, '\x49', '\xf1', '\xec', "\x00\0x00", 2, 0 },
    // 16远程开门
    { 10, '\x49', '\xf1', '\xed', "\x01", 1, 0 },
    // 17设置系统支持感应卡的种类及卡片编号的获取方法
    { 77, '\x49', '\xf1', '\xee', "\x00", 1, 0 },
    // 18设定红外监控开启/关闭方法
    { 78, '\x49', '\xf1', '\xef', "\x00\0x10", 2, 0 },
    // 19设定SM内记录区的指针:用三字节方式
    { 79, '\x49', '\xf1', '\xf0', "\x01\0x00\0x00", 3, 0 },
    // 20设定SM内按星期准进的时间段列表
    { 80, '\x49', '\xf1', '\xf1', "\x00\x03\x00\x00\x23\x59\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff \xff\xff\xff\xff \xff\xff\xff\xff", 14, 0 },

    // 21设定系统使用的感应卡种类:26、36、44、64
    { 81, '\x49', '\xf1', '\xf2', "\x26", 1, 0 },
    // 22设定系统感应卡编号的获取方法
    { 82, '\x49', '\xf1', '\xf3', "\x00", 1, 0 },
    // 23设定联动输入I2的有效电平
    { 83, '\x49', '\xf1', '\xf4', "\x00", 1, 0 },
    // 24设定刷卡开门时，是否输入用户密码:4个0－9
    { 84, '\x49', '\xf1', '\xf5', "\x00", 1, 0 },
    // 25设定红外感应器在报警时的有效输出电平
    { 85, '\x49', '\xf1', '\xf6', "\x00", 1, 0 },
    // 26设定门开关感应器在开门状态时的有效输出电平
    { 86, '\x49', '\xf1', '\xf7', "\x00", 1, 0 },
    // 27设定是否监视联动输入I2的状态
    { 87, '\x49', '\xf1', '\xf8', "\x00", 1, 0 },
    // 28设定门控电磁锁的(种类)特性
    { 88, '\x49', '\xf1', '\xf9', "\x00", 1, 0 },
    // 29设定是否监视红外感应器的状态
    { 89, '\x49', '\xf1', '\xfa', "\x00", 1, 0 },
    // 30设定是否监视门开关感应器的状态
    { 90, '\x49', '\xf1', '\xfb', "\x00", 1, 0 },

    // 方便读取一些Read函数不读的功能，控制时，不带参数。
    // 31读取SM的历史记录柜桶
    { 300, '\x4a', '\xf2', '\xe1', "\x00", 1, 9 },
    // 32读取SM的非工作日准进时段
    { 301, '\x4a', '\xf2', '\xe4', "\x01", 1, 16 },
    // 33读取SM内已设入的用户数目
    { 302, '\x4a', '\xf2', '\xe5', "\x00", 1, 2 },
    // 34读取SM控制的门(门锁)之设定每星期内固定的休息日
    { 303, '\x4a', '\xf2', '\xe9', "\x00", 1, 2 },
    // 35读取SM控制的门(门锁)之设定休息日(星期内休息日除外)
    { 304, '\x4a', '\xf2', '\xea', "\x00", 1, 16 },  // 接收个数可变

    // 36读取SM的一组工作日准进时段
    { 305, '\x4a', '\xf2', '\xe3', "\x01", 1, 16 },
    // 37读取SM的一条历史记录:指定位置，可由E1命令获得
    { 306, '\x4a', '\xf2', '\xe2', "\x01\x00", 2, 12 },
    // 38读取SM内指定用户资料
    { 307, '\x4a', '\xf2', '\xe6', "\x01\x00", 2, 16 },
    // 39读取SM控制的门(门锁)之星期一至星期日的准进时间表
    { 308, '\x4a', '\xf2', '\xeb', "\x00\x01", 2, 24 },  // 接收个数可变
};
#define nMaxCmdNo   40

/*****************************************************************/
// 函数名称:GetProtocolVer
// 功能描述:协议版本号获取函数
// 输入参数:Ver - 版本号,
//           Code - 协议编码
// 输出参数:
// 返    回:
// 其    他:
/*****************************************************************/
DLLExport BOOL GetProtocolVer( char* Ver,    // 版本号
                              char* Code )   // 协议编码

{
    strcpy(Ver,"6.00");
    strcpy(Code,"OEDM30E6-00");

    return TRUE;
}

char Info[] =
{
        "☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n"
        "  EDM30E脉冲锁一体化门禁         \n"
        "                               \n"
        "  PSMS4.39                     \n"
        //"  版本: V6.00                   \n"
"  TEST: T1.00                     \n"
        //"  协议编码:OEDM30E6-00                     \n"
        "                                     \n"
        "  通讯模式: 电总                      \n"
        "  传输方式: RS485               \n"
        "  通讯参数: 9600(可调),n,8,1        \n"
        "  地址:可调                        \n"
        "                               \n"
        "  程序设计者:LYG                   \n"
        "  开发日期:  2005-11-25            \n"
        "  修 改 者:L&L,zhj                 \n"
        "  修改时间:2010.09.17              \n"
        "                                     \n"
        "  此程序只支持PSMS4.39接口          \n"
        "  控制命令必须由业务台支持         \n"
        "  本程序最后编译时间:               \n"    // 请勿在此后增加信息!
        "                                    \n"    // 请保留此行(分配内存用)
};

#ifndef _TSR_
//*****************************************************************
// 函数名称:DLLInfo();
// 功能描述:动态库版本中将信息包 Info 输出，以作版本信息等标志。
// 输入参数:Info--版本信息数组
// 输出参数:
// 返回:    版本信息数组
// 其他:
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
        "正常",           // 00
        "版本错",       // 01
        "校验错",       // 02
        "长度校验错",   // 03
        "无效的CID2",   // 04
        "不能识别的命令格式",    // 05
        "发送数据无效", // 06
        "设置权限错误", // 07
        "密码确认错误", // E0
        "密码修改错误", // E1
        "存储空间满",   // E2
        "修改失败",     // E3
        "存储空间空",   // E4
        "无相应信息项", // E5
        "用户ID重复",   // E6
        "相同卡口给不同用户", // E7
        "其它错误",     // EF
};

#define RETNORMAL   0x00    // 返回正常
#define nErrorEF    0xEF    // 发送失败
#define nErrorEE    0xEE    // 结束失败
#define nErrorED    0xED    // 返回异常  张辉杰  2010.09.15
#define nErrorEC    0xEC    // 其他错误  张辉杰  2010.09.15
#define nNoRecord   0xE5    // 无记录

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
//功能:将字符串S以cSep为分隔符分段。
//入口:S-源字符串，D-第一段字符串，cSep-分隔符。
//返回:第一段字符串的个数。
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
函数名称:CheckSum
函数说明:计算校验和
输入说明:需计算的缓冲区,及要计算的长度
输出说明:返回计算出的校验码
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
函数名称:ValueToAscii
函数说明:把0--F的值,转换为ASCII码
输入说明:需转换的值
输出说明:转换后的值存入byAscii参数
*******************************************************************/
void ValueToAscii(const BYTE byValue, BYTE& byAscii)
{
    ASSERT(byValue <= 0x0f);

    if (byValue > 9)    //大于9的数，从A开始至F
    {
        byAscii = (byValue + 0x41 - 0x0a);
    }
    else
    {
        byAscii = (byValue + 0x30);
    }
}

/*******************************************************************
函数名称:AsciiToValue
函数说明:把一个ASCII码,转换为0-F的数,要求此ASCII码范围在'0'至'F'之间
输入说明:输入为需转换的ASCII码
输出说明:转换后的值存放于byValue参数中
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
函数名称:LenCheckSum
函数说明:计算长度校验和
输入说明:长度
输出说明:返回长度校验码
*******************************************************************/
BYTE LenCheckSum(WORD wLen)
{
    ASSERT(wLen < MAXINFOLEN);
    //若长度为0，不需要计算
    if (wLen == 0)
    {
        return 0;
    }

    BYTE byLenCheckSum = 0;
    byLenCheckSum += wLen & 0x000F;         //取最后4个BIT
    byLenCheckSum += (wLen >> 4) & 0x000F;  //取第4~7个BIT
    byLenCheckSum += (wLen >> 8) & 0x000F;  //取第8~11个BIT
    byLenCheckSum %= 16;                    //模16
    byLenCheckSum = (~byLenCheckSum) + 1;   //取反加1
    byLenCheckSum &= 0x0F;                  //只取4个BIT

    return byLenCheckSum;
}

/*******************************************************************
函数名称:ChangeValueToSend
函数说明:把一个字节变为可发送的两个字节
输入说明:需转换的一个字节
输出说明:转换后的数据(两个字节)放在缓冲区pbySend中
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
函数名称:ChangeRecvToValue
函数说明:把收到的两个字节变为实际的一个字节
输入说明:需转换的两个字节的缓冲区
输出说明:转换后的一个字节放于byValue参数中
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
函数名称:
函数说明:对4个字节的长度进行校验
输入说明:4个字节的带校验的长度串
输出说明:若校验正确,返回TRUE,并将长度值放于pwLen参数中,
若校验错,返回FALSE.
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

    if (byCalcSum != byRecvSum) //长度校验错
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
函数名称:SendPacket
函数说明:将一段信息打包，并从串口上发出
输入说明:串口句柄,使用协议版本,从机地址,从机类型,命令号,
发送内容缓冲区,发送的长度
输出说明:成功发出返回TRUE,否则返回FALSE
*******************************************************************/
BOOL SendPacket(HANDLE hComm, BYTE * byaPublicBuffer, BYTE byVer, BYTE byAdr,
                BYTE byCID1, BYTE byCmd, BYTE* pbyInfo, WORD wLen)
{
    ASSERT((pbyInfo != NULL) || (wLen == 0));

    int iCounter = 0; //计数器

    WORD wInfoSendLen = wLen * 2;

    //起始字节（帧头）
    byaPublicBuffer[iCounter++] = g_byBOI;

    //协议版本号
    ChangeValueToSend(byVer, byaPublicBuffer + iCounter);
    iCounter += 2;

    //设备（从机）地址
    ChangeValueToSend(byAdr, byaPublicBuffer + iCounter);
    iCounter += 2;

    //设备类型描述
    ChangeValueToSend(byCID1, byaPublicBuffer + iCounter);
    iCounter += 2;

    //命令码
    ChangeValueToSend(byCmd, byaPublicBuffer + iCounter);
    iCounter += 2;

    //信息长度
    BYTE byTmp;
    ValueToAscii(LenCheckSum(wInfoSendLen), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen >> 8) & 0x000f, byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen >> 4) & 0x000f, byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    ValueToAscii((wInfoSendLen & 0x000f) , byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    //信息域
    for (int i = 0; i < wLen; i ++)
    {
        ChangeValueToSend(pbyInfo[i], byaPublicBuffer + iCounter);
        iCounter += 2;
    }

    //校验码
    WORD wCheckSum = CheckSum(byaPublicBuffer + 1, wInfoSendLen + 12);

    ValueToAscii((wCheckSum >> 12), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii(((wCheckSum >> 8) & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii(((wCheckSum >> 4) & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;
    ValueToAscii((wCheckSum & 0x000f), byTmp);
    byaPublicBuffer[iCounter++] = byTmp;

    //帧结束符
    byaPublicBuffer[iCounter++] = g_byEOI;
    byaPublicBuffer[iCounter] = 0x00;

    DWORD lWritten;

    // 为了减少干扰，发送前清发送/接收缓冲区
    PurgeComm(hComm, PURGE_TXCLEAR);
    PurgeComm(hComm, PURGE_RXCLEAR);

    WriteFile(hComm, (char*)byaPublicBuffer, iCounter, &lWritten, NULL);


    time_t now;
    struct tm  *timenow;
    time(&now);
    timenow = localtime(&now);
    WriteAsc( HEX_FILE, "\r\n%s\r\n",asctime(timenow));
    WriteAsc( ASC_FILE, "\r\n%s\r\n",asctime(timenow));

    // 如果是跟踪测试，且非OCI－5方案(nExtOci5ID缺省为-1)，则记录调试信息。
    //if( bTestFlag && nExtOci5ID<0 )
    //{
        //MessageBox( NULL, (char*)sSendStr, "发送信息", MB_OK );    // 显示发送信息。
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
函数名称:ReadPacket
函数说明:从串口上读出一帧数据，并校验帧的正确性
输入说明:主要的输入参数是缓冲区指针及长度
输出说明:一切正常返回TRUE,否则返回FALSE
返回的版本号存于 byVer,设备地址存于byAdr,
从机类型存放于byCID1, 返回码存放于byRet
返回的信息存放于pbyInfo,返回的信息长度存放于wLen(解包后长度)
*******************************************************************/
BOOL ReadPacket(HANDLE hComm, BYTE * byaPublicBuffer, BYTE& byVer, BYTE& byAdr,
                BYTE& byCID1, BYTE& byRet, BYTE* pbyInfo, WORD& wLen,
                const WORD wInfoBufLen)
{
    DWORD lRead;
    //清空接收缓冲区
    memset(byaPublicBuffer, 0, sizeof(byaPublicBuffer));
    Sleep(500);
    //读取帧头，若尝试了BOITRYTIMES次没有找到帧头，认为失败。
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
            WriteAsc(ASC_FILE,"\r\n没接收到帧头\r\n");
            WriteAsc(HEX_FILE,"\r\n没接收到帧头\r\n");
            return FALSE; //没找到帧头，失败
        }
    }

    if (iTryTimes >= BOITRYTIMES)
    {
        wLen = 0;
#ifndef _TSR_
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n未收到包头信息\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n未收到包头信息\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n未收到包头信息\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("Find Head char Error!", 1, 23, 0x1f);
#endif

        return FALSE;   //没找到帧头，失败
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
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
           // MessageBox(NULL, "\r\n包结构信息错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n包结构信息错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n包结构信息错误\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("Data Struct Error!", 1, 23, 0x1f);
#endif

        return FALSE; //没有收齐字节，失败
    }

    WORD wInfoLen;

    if (!MakeLength(byaPublicBuffer + 9,&wInfoLen))
    {
#ifndef _TSR_
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n包结构中长度信息错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s","\r\n包结构中长度信息错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s","\r\n包结构中长度信息错误\r\n");  // 写入到文本文件中
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
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n数据包长度错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n数据包长度错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n数据包长度错误\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("Data Len Error!", 1, 23, 0x1f);
#endif

        return FALSE;//要收的数据包比缓冲区长
    }

    //下面加13因为数据信息前面有13字节的其他内容
    //下面加5因为数据信息后面有4个字节的校验和1个字节的帧尾
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
        //MessageBox( NULL, (char*)sRecStr, "接收信息", MB_OK );  // 显示接收信息。
    //}   // end of if( bTestFlag )

    WriteAsc( ASC_FILE, "\r\nRecv:%d char", (int)lRead+13 );
    WriteAsc( HEX_FILE, "\r\nRecv:%d char", (int)lRead+13 );
    WriteAsc( ASC_FILE,"%s", (char*)byaPublicBuffer );
    WriteHex( HEX_FILE, (char*)byaPublicBuffer, (int)lRead+13 );
#endif    // end of #ifdef _DLL_

    if (wInfoLen + 5 != (WORD)lRead)
    {
#ifndef _TSR_
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n尾数据包长度错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n尾数据包长度错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n尾数据包长度错误\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("Next Data Len Error!", 1, 23, 0x1f);
#endif

        return FALSE;    //数据包不齐
    }

    if (g_byEOI != byaPublicBuffer[13 + wInfoLen + 4])
    {
#ifndef _TSR_
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n包尾错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n包尾错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n包尾错误\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("End char Error!", 1, 23, 0x1f);
#endif

        return FALSE; // 帧尾错
    }

    WORD wCalcSum = CheckSum(byaPublicBuffer + 1, wInfoLen + 12);

    BYTE bySumTmp[4];   // 获取接收包中的校验码
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
        if (bTestFlag)    // 如果是跟踪测试，则记录调试信息。
        {
            //MessageBox(NULL, "\r\n数据包校验错误\r\n", "告警信息", MB_OK);
            WriteAsc( ASC_FILE,"%s", "\r\n数据包校验错误\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n数据包校验错误\r\n");  // 写入到文本文件中
        }
#endif

#ifdef _TSR_
        WriteString("Check Sum Error!", 1, 23, 0x1f);
#endif

        return FALSE;  //校验和错
    }

    ChangeRecvToValue(byVer,  byaPublicBuffer + 1);   // 获取接收包中的版本号
    ChangeRecvToValue(byAdr,  byaPublicBuffer + 3);   // 获取接收包中的地址
    ChangeRecvToValue(byCID1, byaPublicBuffer + 5);   // 获取接收包中的CID1
    ChangeRecvToValue(byRet,  byaPublicBuffer + 7);   // 获取接收包中的返回码
     i = 0;
    for (i = 0; i < wLen; i ++)                         // 获取接收包中的:有效数据段
    {
        BYTE byTmp1, byTmp2;
        AsciiToValue(byaPublicBuffer[13 + 2*i], byTmp1);
        AsciiToValue(byaPublicBuffer[13 + 2*i + 1], byTmp2);
        pbyInfo[i] = (byTmp1 << 4) | (byTmp2);
    }

    return TRUE;
}


/*******************************************************************
函数名称:GetResponeData
函数说明:
输入说明:串口句柄,所用协议版本,目标设备地址,存放数据缓冲区指针,
所采集到数据长度,存放数据缓冲区的大小.
//输出说明:采集成功,返回TRUE,否则返回FALSE.
返回:按电总协议规定。0－成功，其它－错误信息。
*******************************************************************/
int GetResponeData(HANDLE hComm,       // 串口句柄
                   const BYTE byVer,   // 通讯协议版本号
                   const BYTE byAdr,   // 设备地址
                   const BYTE byCID1,  // 设备类型标识控制码
                   const BYTE byCID2,  // 命令类型字CID2
                   int   nSendLen,     // 发送命令参数长度(包括CmdGroup+CmdType+参数)
                   BYTE *pbyData,      // 处理后接收数据指针
                   WORD& wRecvLen,     // 接收数据长度
                   // int nLoop,          // 命令号
                   BYTE*pSendTmp)     // 数据参数指针
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

        if ( ((byAdr != 255) && (byAdr != byReadAdr))    // qilei 20101221 地址255不做检查
            || (0x80 != byReadCID1)
            || (g_byVersion != byReadVer)
            )
        {
#ifndef _TSR_
            WriteAsc(ASC_FILE, "\r\n包头信息错误:Addr: %d, CID1: %d, Version: %d\r\n", byReadAdr, byReadCID1, byReadVer);
            WriteAsc(HEX_FILE, "\r\n包头信息错误:Addr: %d, CID1: %d, Version: %d\r\n", byReadAdr, byReadCID1, byReadVer);
#endif    // _TSR_

            continue;
        }
        //张辉杰 北京联通  增加 2010.09.17
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
            WriteAsc(ASC_FILE, "\r\n错误:%s\r\n", caErrMessage[byRet-0xd8]);
            WriteAsc(HEX_FILE, "\r\n错误:%s\r\n", caErrMessage[byRet-0xd8]);
        }
        else if (byRet>0 && byRet <= 7)
        {
            WriteAsc(ASC_FILE, "\r\n错误:%s\r\n", caErrMessage[byRet]);
            WriteAsc(HEX_FILE, "\r\n错误:%s\r\n", caErrMessage[byRet]);
            return nErrorED;  //张辉杰增加  2010.09.15
        }
        else
        {
            WriteAsc(ASC_FILE, "\r\n错误:未定义错误。\r\n");
            WriteAsc(HEX_FILE, "\r\n错误:未定义错误。\r\n");
            return nErrorEC;  //张辉杰增加  2010.09.15
        }
#endif  // _TSR_
    }

    return byRet;
}

// 处理SM门禁实时时钟数据:返回7个数据(年、月、日、星期、时、分、秒)
void FixSMClock(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)((byData[0] >> 4)*10+ (byData[0]&0x0f))*100
        + (byData[1] >> 4)*10 + (byData[1]&0x0f);  // 门禁日期:年
    int i = 0;
    for ( i = 1; i < 7; i++)    // 门禁日期、时间:月、日、星期、时、分、秒
    {
        pfRecvData[i] = (float)(byData[i + 1] >> 4)*10 + (byData[i + 1]&0x0f);
    }
}

// 处理读取记录数据E2命令:返回34个数据
void FixRecord(float* pfRecvData, BYTE* byData)
{
    // 数据处理后顺序:高0、低卡号1，记录时间年2、月3、日4、时5、分6、秒7
    //       事件状态8-15，线路状态16-23，报警事件24,REMARK字25,参数修改标志26-33

    // 处理卡号:通道nReadE2+0为高位，通道nReadE2+1为低位
    unsigned long lTempCardNo=0l;

    int i = 0;
    for ( i = 0; i < 4; i++)
    {
        lTempCardNo += (unsigned long)(byData[4-i] * pow(256, i));
    }
    //pfRecvData[0] = (float)byData[0];  // 卡号高位 2002.01.23 CMQ修改
    unsigned long lHiCardNo = (unsigned long)byData[0];
    memcpy( &pfRecvData[1], &lTempCardNo, 4);    // zhx    19991130
    memcpy( &pfRecvData[0], &lHiCardNo, 4);      // cmq 2002.01.23

    // 为了调测小卡号方便，特编写下面代码:2000.08.02
    char szTemp[5]={0};
    memcpy( &szTemp, &pfRecvData[1], 4 );

    pfRecvData[2] = (float)((byData[5] >> 4)*10 + (byData[5]&0x0f))*100
        + (float)(byData[6] >> 4)*10 + (byData[6] & 0x0f);  // 年
     i = 0;
    for (i = 0; i < 5; i++)    // 刷卡记录日期、时间:月、日、时、分、秒
    {
        pfRecvData[3 + i] = (float)(byData[i + 7] >> 4)*10 + (byData[i + 7] & 0x0f);
    }

    // 状态字节:STATUS根据REMARK字有多类定义:0－4，7，5+6+8-11。

    // REMARK的D7＝1表示该信号已读取过
    if ((byData[13]&0x7f) >= 0 && (byData[13]&0x7f) <= 3)
    {
        // 事件状态(8-15)状态字:低－>高
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[8 + i] = (float)((byData[12] >> i) & 0x01);
        }

        // 线路状态16-23，报警事件24，REMARK字25，参数修改标志26-33
         i = 0;
        for (i = 0; i < 18; i++)
        {
            pfRecvData[16 + i] = 20.0f; // 未定义(REMARK字在本函数最后有更新)
        }
    }
    else if ((byData[13]&0x7f) == 7) // 内部控制参数被修改的记录
    {
        // 事件状态8-15，线路状态16-23，报警事件24
        i = 0;
        for (i = 0; i < 17; i++)
        {
            pfRecvData[8 + i] = 20.0f; // 状态未定义
        }

        // 因修改数据含义字存储在卡号位置，故需将卡号(低位)清零
        pfRecvData[1] = 0.0f;
         i = 0;
        for (i = 0; i < 8; i++)    // 参数修改标志26-33状态字:低－>高
        {
            pfRecvData[26 + i] = (float)((byData[4] >> i) & 0x01);  // 卡号最低字节
        }
    }
    else if ((byData[13]&0x7f) == 4)//超时未关门记录
    {
        // 因报警数据含义字存储在卡号位置，故需将卡号(低位)清零
        pfRecvData[0] = 0.0f;//刷卡超时的时候,事件是卡号
        pfRecvData[1] = 0.0f;
        for(i=0; i<8; i++)
        {
            pfRecvData[8 + i]  = 20.0f;
            pfRecvData[16 + i] = 20.0f;
            pfRecvData[26 + i] = 20.0f;
        }
        pfRecvData[24] = 20.0f;//报警源AS无效
    }
    else    // 告警记录:报警、SM掉电、无效卡、过期卡、非时段、密码确认错误
    {
        if ((byData[13]&0x7f) == 5)  // 报警事件24
        {
            // 因报警数据含义字存储在卡号位置，故需将卡号(低位)清零
            pfRecvData[1] = 0.0f;
            pfRecvData[24] = (float)byData[4];    // 卡号最低字节
        }
        else
        {
            pfRecvData[24] = 20.0f;   // 状态未定义
        }
         i = 0;
        for (i = 0; i < 8; i++)    // 线路状态(16-23)状态字:低－>高
        {
            pfRecvData[16 + i] = (float)((byData[12] >> i) & 0x01);
        }

        // 事件状态8-15
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[8 + i] = 20.0f;    // 状态未定义
        }

        // 参数修改标志26-33
         i = 0;
        for (i = 0; i < 8; i++)
        {
            pfRecvData[26 + i] = 20.0f;   // 状态未定义
        }
    }

    pfRecvData[25] = (float)(byData[13]);   // 备注字

    //2004-03-26,LYG,ES2100新增Remark=34,紧急联动事件，当"事件来源＝5字节全0时"，联动开始，
    //当"事件来源＝4字节全0时+第五字节＝1"，联动结束，为方便起见，修改如下，34代表联动开始，
    //35代表联动结束
//    if(pfRecvData[25]==34)
//        pfRecvData[25] += (float)byData[4];   // 备注字
}

/////L&L ADD/////
// 处理门禁SM监控状态E7命令数据:(比2100多的那个字节) 返回1个数据
void FixSMMonitorStatus_add(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)byData[2];
}

// 处理门禁SM监控状态E7命令数据:返回12个数据
void FixSMMonitorStatus(float* pfRecvData, BYTE* byData)
{
    int i = 0;
    for ( i = 0; i < 8; i++)    // SM的工作状态:低－>高
    {
        pfRecvData[i] = (float)((byData[0] >> i) & 0x01);
    }


    //lyg,2005-12-8
    // SM的监控线路状态:低－>高D0门锁继电器动作，D1手动开门钮，D3门开关
     i = 0;
    for (i = 0; i < 4; i++)
    {
        pfRecvData[8 + i] = (float)((byData[1] >> i) & 0x01);
    }
}

// 处理门禁SM特性参数E8命令数据:返回12个数据
void FixSMSpecialty(float*pfRecvData, BYTE* byData)
{
    int i = 0;
    for ( i = 0; i < 8; i++)         // 门碰开关、红外状态控制字:低－>高
    {
        pfRecvData[i] = (float)((byData[0] >> i) & 0x01);
    }
    pfRecvData[8] = (float) byData[1];  // 门锁继电器执行时间
    pfRecvData[9] = (float) byData[2];  // 开门等待进入延时时间

    //pfRecvData[10] = (float) byData[3]; // 红外报警发生至确认的延时时间
    //pfRecvData[11] = (float) byData[4]; // 感应卡阅读器型号及卡号获取控制字节
    pfRecvData[10] = (float)0.0;             //Es2100没有此项
    pfRecvData[11] = (float)0.0;
}

void FixCarNum(float* pfRecvData, BYTE* byData)
{
    pfRecvData[0] = (float)((*(byData)<<8)+(*(byData+1)));
}

// 功能:根据协议内容读取有关的采集数据，并处理，数据流未整理为PSMS门禁接口
BOOL GetData(HANDLE hComm, const BYTE byVer, const BYTE byAdr, const BYTE byCID1,
             float* pfRecvData)// , WORD& wRecvLen)
{
    BYTE byData[MAXINFOLEN/2]={0};    // 采集有效数据缓冲区
    BYTE pSendTmp[50]={0};  // 命令参数:Command Group + Command Type + DataF
    int  nRet = 0;      // 采集返回标识
    int  nLoop = 0;     // 采集数据包标号
    int  i = 0;

    // 读取SM的实时钟E0命令后数据的位置，共7个
#define nReadE0 0
    // 读取一条记录E2命令后数据的位置，共34个数据
#define nReadE2 7
    // 读取SM的监控状态E7命令后数据的位置，共12个数据
#define nReadE7 41
    // 读取SM控制的门之特性参数E8命令后数据的位置，共12个数据
#define nReadE8 53

    // 读取SM控制的加卡的数量，共1个数据
#define nReadE5 66

    /////L&L ADD/////
    // 读取SM的监控状态E7命令 EDM返回了3个字节,原来的ES2100只有2个字节
    // 只有1个数据
#define nReadE7add 66   //55号通道没有用

    do
    {
        //v5.40第四条命令，只用每天采集一次

        int j = 0;
        for (j = 0; j < 3; j++)    // 重试次数
        {
            pSendTmp[0] = strRead[nLoop].byCmdGroup;    // 命令组:command group
            pSendTmp[1] = strRead[nLoop].byCmdType;     // 命令类型:command type
            for (i = 0; i < strRead[nLoop].nDataLen; i++)
            {
                pSendTmp[2 + i] = strRead[nLoop].byData[i];
            }
            nRet = GetResponeData(hComm, byVer, byAdr, byCID1, strRead[nLoop].byCID2,
                // strRead[nLoop].nDataLen+2, byData, nLoop, pSendTmp);
                strRead[nLoop].nDataLen + 2, byData,strRead[nLoop].nRetLen, pSendTmp);

            // 当采集有返回，即表示采集成功，否则:发送失败、无响应、校验不正确等则重试

            //张辉杰增加 2010.09.15
            if (nRet==nErrorED || nRet==nErrorEC)
            {
                continue;
            }

            //lyg,2004-03-26
            //if (nLoop == 1 && (nRet != 0 && nRet != nNoRecord) )
            //张辉杰修改  2010.09.15
            if (nLoop == 1 && (nRet != 0 && nRet != nNoRecord) && nRet!= 0xE4 )
            {
                if (j==2)
                {
                    return FALSE;   // 采集记录命令中非有记录或有非无记录错误码时的情况
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
                    return FALSE;   // 其它命令中有错误码情况
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

        switch (nLoop)  // 根据命令序号，进行数据处理
        {
        case 3: // 读取SM的实时钟:7个
            FixSMClock(pfRecvData + nReadE0, byData);
            break;
        case 1: // 读取SM的一条历史记录:34个
            // 数据处理后顺序:高0、低卡号1，记录时间年2、月3、日4、时5、分6、秒7
            //   事件状态8-15，线路状态16-23，报警事件24，参数修改标志25-32,REMARK字33

            if (nRet == nNoRecord || nRet==0xE4 )          // 无记录
            {
                i = 0;
                for (i = 0; i < 8; i++)     // 数据清零
                {
                    pfRecvData[nReadE2 + i] = 0.0f;  // 模拟量数据清零
                }
               // pfRecvData[nReadE2 + 33] = 0.0f;
                i = 0;
                for (i = 0; i < 26; i++)    // 数据清零
                {
                    // 开关量数据和REMARK字置未定义
                    pfRecvData[nReadE2 + 8 + i] = 20.0f;
                }
            }
            else
            {
                FixRecord(pfRecvData + nReadE2,  byData);
            }
            break;
        case 2: // 读取SM的监控状态:12个
            FixSMMonitorStatus(pfRecvData + nReadE7, byData);
            /////L&L ADD/////
            FixSMMonitorStatus_add(pfRecvData + nReadE7add, byData);
            break;
        case 0: // 读取SM的门/门锁特性参数
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

// 根据新门禁PSMS数据接口规范整理数据。
void TurnToPsmsForm(float* fpreData,   // 转换前数据
                    float* fData)      // 转换后数据
{
    int i = 0;
    for ( i = 0; i < 8; i++)
    {
        fData[i] = fpreData[7 + i];   // 刷卡记录的卡号+日期+时间
    }

    //2000.08.02 cmq 为OCE小卡号无法显示修改
    memcpy(&fData[1],&fpreData[8],4);   // 低位卡号
    memcpy(&fData[0],&fpreData[7],4);   // 高位卡号 2002.01.23 CMQ增加


    if( fpreData[32]==1.0f )
    {
        fData[8] = 1.0f;                // 585进出门标志:双向出
    }
    else
    {
        fData[8] = 0.0f;                // 505进出门标志:单向进
    }

    // 上报:0-无标志,1-有效卡,2-无权进入,3-非法时区,4-密码错误,5-非法门区,6-非法卡
    if (((BYTE)fpreData[32] & 0x7f) == 0)    // 备注REMARK=0:正常刷卡开门记录
    {
        fData[9] = 1.0f;
    }

    else if (((BYTE)fpreData[32]&0x7f)>0 && ((BYTE)fpreData[32]&0x7f) < 8)
    {
        // 11-用户号+密码开门,12-远程开门,13-手动开门,14-联动开门,
        // 15-报警,16-SM掉电,17-内部控制参数被修改
        fData[9] = 10.0f + ((BYTE)fpreData[32] & 0x7f);
    }


    else if (((BYTE)fpreData[32]&0x7f) == 8)    // 无效卡刷卡记录
    {
        fData[9] = 6.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 9)    // 用户卡有效期已过
    {
        fData[9] = 2.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 10)   // 当前时间该用户无权进入
    {
        fData[9] = 3.0f;
    }

    else
    {
        fData[9] = 20.0f;  // 未知定义
    }

    fData[10] = 1.0f;   // 门号

    // added by zhx    19991124
    if (fData[1] == 0)
    {
        fData[10] = 0.0f;   // 门号
    }

    fData[11] = fpreData[32];   // REMARK字(注:在接口规范中未定义此值)
    i = 0;
    for (i = 0; i < 3; i++)
    {
        fData[13 + i] = fpreData[i];      // 门禁日期:年、月、日
    }
    i = 0;
    for (i = 0; i < 3; i++)
    {
        fData[16 + i] = fpreData[i + 4];    // 门禁时间:时、分、秒
    }

    fData[19] = fpreData[3];    // 门禁星期

    // 20无权卡告警状态0:正确,1:错误,2:无标志
    if (((BYTE)fpreData[32]&0x7f) == 8 )
    {
        fData[20] = 1.0f;   // 无效
    }
    else if (((BYTE)fpreData[32]&0x7f) < 1)
    {
        fData[20] = 0.0f;   // 正确刷卡、用户号开门
    }
    else
    {
        fData[20] = 2.0f;   // 无标志
    }

    if ( ( (BYTE)fpreData[32]&0x7f ) == 9 )
    {
        fData[21] = 1.0f;   // 过期
    }
    else if (((BYTE)fpreData[32]&0x7f) < 1)
    {
        fData[21] = 0.0f;   // 正确刷卡、用户号开门
    }
    else
    {
        fData[21] = 2.0f;   // 无标志
    }

    // 22非法时区告警状态0:正确,1:错误,2:无标志
    if (((BYTE)fpreData[32]&0x7f) == 10.0f)
    {
        fData[22] = 1.0f;
    }
    else if (((BYTE)fpreData[32]&0x7f) == 0.0f || ((BYTE)fpreData[32]&0x7f) == 1.0f)
    {
        fData[22] = 0.0f;   // 正确刷卡、用户号＋密码
    }
    else
    {
        fData[22] = 2.0f;
    }

    // 23非法门区告警状态0:正确,1:错误,2:无标志
    fData[23] = 2.0f;

    // 24开门超时告警0:正常,1:告警,2:无标志
    if (fpreData[19] == 20.0f)
    {
        fData[24] = 2.0f;
    }
    else
    {
        fData[24] = fpreData[19];
    }

   /* // 25刷卡未开门告警0:正常,1:告警,2:无标志
    if (fpreData[20] == 20.0f)
    {
        fData[25] = 2.0f;
    }
    else
    {
        fData[25] = fpreData[20];
    }
    */
    // 门开记录 0:未动作,1:动作,2:无标志
    if (fpreData[31] == 2.0f) //Remark=5,时，AS=2
    {
        fData[26] = 1.0f;
    }
    else
    {
        fData[26] = 2.0f;
    }

    // 27红外告警0:未动作,1:动作,2:无标志
    //fData[27] = fpreData[51];   // E7命令

    // 28门磁开关状态0:关,1:开,2:无状态
    fData[28] = fpreData[52];   // E7命令

    // 29红外布防撤防状态0:撤防,1:布防,2:无状态
    //fData[29] = fpreData[44];   // E7命令

    // 30门锁开关状态0:关,1:开,2:无状态
    fData[30] = fpreData[42];   // E7命令

    // 4.39只用39通道
    fData[39] = 4.0f;       // 门禁类型(DC505 = 1, ES2100 = 2,ES2000 = 3,EDM30E=4)
	fData[40] = 250;       //门禁最大存卡数量 EDM30E为250张卡
	fData[41] = fpreData[66];  //已加卡数量
    //fData[39] = 16.0f;  // 16进制显示卡号
    //fData[59] = 10.0f;  // 最大卡号长度10位
    //fData[79] = 1.0f;   // 最大门号:单向门
    i = 0;
    for (i = 0; i < 50; i++) // 将原数据拷贝到100以后的区域
    {
        fData[100 + i] = fpreData[15 + i];
    }
    /////L&L ADD/////由于E7多了一个数据
    fData[100 + 51] = fpreData[15 + 51];

}

/*******************************************************************
函数名称:Read
函数说明:动态库的Read函数,此函数仅用于psms4.0和TSR中
此函数采集设备上的数据,并进行处理,结果存放于给定的缓冲区
输入说明:串口句柄,设备地址,存放采集数据的指针
输出说明:采集和处理成功,返回TRUE,并将采集的数据放于缓冲区
采集或处理失败,返回FALSE.
********************************************************************/
DLLExport BOOL Read(HANDLE hComm, int nUnitNo, void* pvData)
{

    float* fData = (float*)pvData;
    float fpreData[160] = {0.0f};
    ASSERT(nUnitNo >= 0 && nUnitNo < 256);
    if (nUnitNo < 0 || nUnitNo > 255)
    {
        return FALSE; //地址不对
    }

    BYTE byCID1 = 0x80;     // 对门禁控制器

    if (!GetData(hComm, g_byVersion, (BYTE)nUnitNo, byCID1, fpreData))
        // 原始采集时用,即不转换为PSMS接口格式
        // if(!GetData(hComm, g_byVersion, (BYTE)nUnitNo, byCID1, fData))
    {
        return FALSE;
    }

    //add by liugang ,20070801,根据事件寄存器，修改相应的
    if( fpreData[66] >=0 &&  fpreData[66] <=3 )
    {
        if( fpreData[52] !=0)
        {
            fpreData[52] = 168 ; //门开关状态 & 门磁开关状态，对应通道号[137]，[28]
        }
        if( fpreData[42] ==1 )
        {
            fpreData[42] = 168 ; //门锁开关状态，对应通道号[30]
        }

        fpreData[19] = 0 ; //开门超时告警状态，对应通道号[24]
        fpreData[41] = 0 ; //门禁告警状态修改，对应通道号[126]
    }

    //add by liugang, 20071205,修改门禁告警状态信息
    if( fpreData[66] == 12 ) //超时未关门
    {
        fpreData[41] = 2 ; //门禁告警状态修改，对应通道号[126]
        fpreData[52] = 3 ; //门开关状态 & 门磁开关状态，对应通道号[137]，[28]
    }
    if( fpreData[66] == 11 )//超时未开门
    {
        fpreData[41] = 3 ; //门禁告警状态修改，对应通道号[126]
    }
    if( fpreData[66] == 9 )//非法开门
    {
        fpreData[41] = 4 ; //门禁告警状态修改，对应通道号[126]
    }

    // 将本门禁数据转换为PSMS门禁规范数据
    TurnToPsmsForm(fpreData, fData);

    //5.40门禁校验时间:大于10s就自动
#ifdef _LINUX_
    char pCmdStr[60];
    memset(pCmdStr, 0, 60);

    //获取时间
    struct tm *local;
    time_t t;
    t=time(NULL);
    local=localtime(&t); //这个必须要，否则local就是一个指针，没有分配地址。



    //WriteAsc( ASC_FILE,"IDU当前的时间点数:%d\r\n", t );
    //WriteAsc( HEX_FILE,"IDU当前的时间点数:%d\r\n", t );


    int IDU_year = local->tm_year + 1900;//IDU的时间年
    int IDU_month= local->tm_mon + 1;//IDU的时间月
    int IDU_day  = local->tm_mday;
    int IDU_hour = local->tm_hour;
    int IDU_min  = local->tm_min;
    int IDU_sec  = local->tm_sec;
    int IDU_wday = local->tm_wday;

    //WriteAsc( ASC_FILE, "IDU当前的时间:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
    //WriteAsc( HEX_FILE, "IDU当前的时间:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
    struct tm cardtime;
    memset(&cardtime, 0, sizeof(cardtime));
    cardtime.tm_year = (int)fData[13] - 1900;
    cardtime.tm_mon  = (int)fData[14] - 1;
    cardtime.tm_mday = (int)fData[15] ;
    cardtime.tm_hour = (int)fData[16] ;
    cardtime.tm_min  = (int)fData[17] ;
    cardtime.tm_sec  = (int)fData[18] ;

    /*
    WriteAsc( ASC_FILE, "门禁 当前的时间:%d-%d-%d %d:%d:%d\r\n", \
                                         (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
    WriteAsc( HEX_FILE, "门禁 当前的时间:%d-%d-%d %d:%d:%d\r\n", \
                                         (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
    */

    time_t t_Door=mktime(&cardtime);
    //WriteAsc( ASC_FILE,"门禁 当前的时间点数:%d\r\n", t_Door );
    //WriteAsc( HEX_FILE,"门禁 当前的时间点数:%d\r\n", t_Door );


    if ( abs( t_Door-t )>30 ) //若是时间超过30秒，自动校验时间
    {

        WriteAsc( ASC_FILE,"IDU当前的时间点数:%d\r\n", t );
        WriteAsc( HEX_FILE,"IDU当前的时间点数:%d\r\n", t );

        WriteAsc( ASC_FILE, "IDU当前的时间:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );
        WriteAsc( HEX_FILE, "IDU当前的时间:%d-%d-%d %d:%d:%d\r\n", IDU_year, IDU_month,IDU_day,IDU_hour,IDU_min,IDU_sec );

        WriteAsc( ASC_FILE, "门禁 当前的时间:%d-%d-%d %d:%d:%d\r\n", \
            (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );
        WriteAsc( HEX_FILE, "门禁 当前的时间:%d-%d-%d %d:%d:%d\r\n", \
            (int)fData[13], (int)fData[14],(int)fData[15],(int)fData[16],(int)fData[17],(int)fData[18] );

        WriteAsc( ASC_FILE,"门禁 当前的时间点数:%d\r\n", t_Door );
        WriteAsc( HEX_FILE,"门禁 当前的时间点数:%d\r\n", t_Door );


        char DataStr[50];
        memset(DataStr, 0, 50);
        sprintf(DataStr, "%04d%02d%02d%02d%02d%02d%d\0x00", IDU_year, IDU_month, \
            IDU_day, IDU_hour, IDU_min,  IDU_sec, IDU_wday );
        //获得密码
        FILE *fp;
        if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"rb")) == NULL)
        {
            //printf("密码文件不存在！\n");
            WriteAsc( ASC_FILE,"%s", "\r\n密码文件不存在！\r\n");  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"%s", "\r\n密码文件不存在！\r\n");

            sprintf(pCmdStr, "32,%s,%s", "00000", DataStr);//密码文件不存在，密码为000000
        }
        else
        {
            char str[20];
            memset(str, 0, 20);
            fgets(str,6,fp);
            //printf("str = %s\n", str);

            WriteAsc( ASC_FILE,"\r\n打开密码文件:%s！\r\n",str);  // 写入到二进制文件中
            WriteAsc( HEX_FILE,"\r\n打开密码文件:%s！\r\n",str);

            fclose(fp);//关闭文件

            sprintf(pCmdStr, "32,%s,%s", str, DataStr);//密码从文件中读出

        }

        Sleep(200);
        //调用控制接口
    //    bTestFlag = TRUE;
        WriteAsc( ASC_FILE,"%s", "\r\n门禁自动校验时间\r\n");  // 写入到二进制文件中
        WriteAsc( HEX_FILE,"%s", "\r\n门禁自动校验时间\r\n");
    //    bTestFlag = FALSE;


        Control( hComm, nUnitNo, pCmdStr);
    }

#endif

    return TRUE;
}

/**********************************************************************
函数名称:Query
函数说明:此函数是动态库的主要出口函数之一,
用来采集和处理设备上的数据
输入说明:输入为串口句柄,设备地址,数据处理函数,及一个本函数不使用的指针
输出说明:当数据采集成功时返回TRUE,并由数据处理函数完成数据处理
当数据采集失败时返回FALSE.
***********************************************************************/
#ifdef _DLL_
// V4.1以上4.x版/V5.0版 的数据采集上报函数。
DLLExport BOOL Query(
                     HANDLE hComm,              // 通讯口句柄
                     int nUnitNo,               // 采集器单元地址
                     ENUMSIGNALPROC EnumProc,   // 枚举函数
                     LPVOID lpvoid              // 空类型指针
                     )
{

/*
// 原始接口
float p[nMaxChanelNo] = {0.0f};

  // 数据采集
  if (!Read(hComm, nUnitNo, p))
  {
  return FALSE;
  }

    // 数据上报
    for (int i = 0; i < nMaxChanelNo; i++)
    // for(int i = 0; i < 65; i++)   // 原始采集时用,即不转换为PSMS接口格式
    {
    EnumProc(i, p[i], lpvoid);
    }

      return TRUE;

    //*/


    float p[nMaxChanelNo] =  {0.0f};

    // 数据采集
    if (!Read(hComm, nUnitNo, p))
    {
        return FALSE;
    }

#define DT_FLOAT        0            //浮点类型
#define DT_INT            1            //整数类型（暂不使用）
#define DT_DOOR            100            //嘉讯通门禁数据类型
#define DT_LONGDOORID    101            //长门禁卡号高位
#define DT_CARDIDLOW    102            //长门禁卡号低位


    struct CCardData
    {
        short            iDoorID;            //门ID
        UINT            nCardID;            //卡号
        time_t            tmFreshTime;        //刷卡时间
        char            chEntryFlag;        //进出标志,0进入。1离开
    };
    struct CNewCardData
    {
        short            iDoorID;            //门ID
        UINT            nCardIDHigh;        //卡号高位
        UINT            nCardIDLow;        //卡号低位
        UINT            nSpunitID;            //由前置机填写
        UINT            nParam_One;            //保留
        UINT            nParam_Two;            //保留
        BYTE            byteTemp;            //保留
        time_t            tmFreshTime;        //刷卡时间
        char            szCardID[50];        //保留未用
        char            chEntryFlag;        //进出标志,0进入。1离开
        char            chValidFlag;        //是否非法刷卡标志
    };

    // struct CCardData;
    //复合数据类型
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

    //将数据组合在结构里
    _DATA_VARIANT.VarData.newcarddata.iDoorID = (char)((int)p[10]);    // 门号

    //卡号p[0]和[1]是DWORD变量
    union   DATA_UNION
    {
        UINT    uTmp;
        float    fTmp;
    };
    DATA_UNION    _DATA_UNION;

    _DATA_UNION.fTmp = p[0];  // 卡高位
    _DATA_VARIANT.VarData.newcarddata.nCardIDHigh = _DATA_UNION.uTmp;

    unsigned short int    uSglFlag = (unsigned short int)_DATA_UNION.uTmp;
    _DATA_UNION.fTmp = p[1];  // 卡低位
    _DATA_VARIANT.VarData.newcarddata.nCardIDLow = _DATA_UNION.uTmp;
    uSglFlag += (unsigned short int)_DATA_UNION.uTmp;

    // 刷卡时间:年、月、日、时、分、秒
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

    _DATA_VARIANT.VarData.newcarddata.chEntryFlag =    (char)((int)p[8]);    // 进出标志
    _DATA_VARIANT.VarData.newcarddata.chValidFlag = (char)((int)p[9]);    // 非法刷卡标志

    // 数据上报
    union    DATA_ADDR_UNION
    {
        float           fAddr;
        DATA_VARIANT*   pAddr;
    };
    DATA_ADDR_UNION     _DATA_ADDR_UNION;
    _DATA_ADDR_UNION.pAddr = &_DATA_VARIANT;

    if (uSglFlag == 0 && p[10] == 0.0f)
    {       // 为0卡号时，从2通道开始传送
        for (int nChannel = 13; nChannel <= nMaxChanelNo; nChannel++)
        {
            EnumProc(nChannel, p[nChannel], lpvoid);
        }
    }
    else    // 只有不为0卡号时才传送
    {
        // 传卡号高位
        _DATA_VARIANT.dwDataType = DT_LONGDOORID;   // 长门禁标志
        EnumProc(0x04000000, _DATA_ADDR_UNION.fAddr, lpvoid);

        // 传卡号低位
        _DATA_VARIANT.dwDataType = DT_CARDIDLOW;    // 长门禁低位标志
        _DATA_UNION.fTmp = p[1];                    // 卡低位
        _DATA_VARIANT.VarData.nVal = _DATA_UNION.uTmp;
        EnumProc(0x04000001, _DATA_ADDR_UNION.fAddr, lpvoid);

        // 其它通道正常传送
        for (int nChannel = 2; nChannel <= nMaxChanelNo; nChannel++)
        {
            EnumProc(nChannel, p[nChannel], lpvoid);
        }
    }

    return TRUE;
}

// 跟踪测试入口函数
// 功能:在测试前，将全局标志 bTestFlag 置位；测试后，复位标志。
//*****************************************************************
// 函数名称:Test
// 功能描述:跟踪测试入口函数，在测试前，将全局标志 bTestFlag 置位；
//           测试后，复位标志。
// 输入参数:hComm - 通信句柄,
//           nUnitNo - 采集器单元地址,
//           pData - 上报数据缓冲区指针
// 输出参数:
// 返    回:TRUE－成功；FALSE－失败。
// 其    他:
//*****************************************************************
DLLExport BOOL Test(HANDLE hComm,               // 通讯口句柄
                    int nUnitNo,                // 采集器单元地址
                    ENUMSIGNALPROC EnumProc,    // 枚举函数
                    LPVOID lpvoid)              // 空类型指针
{
    // 将调试标志置位
    bTestFlag = TRUE;

    // 调用采集函数采集数据，其中会因调试标志的置位而显示调试信息。
    BOOL bFlag = Query( hComm, nUnitNo, EnumProc, lpvoid );

    WriteAsc( ASC_FILE, "\r\n本次采集结束\r\n", 16 );
    WriteAsc( HEX_FILE, "\r\n本次采集结束\r\n", 16 );

    // 将调试标志复位
    bTestFlag = FALSE;

    return bFlag;
}
#endif //_DLL_


// 将下发设置日期、时间命令串转换为命令参数。
BOOL PrepareDateTime(char* szValue,    // 下发的命令串
                     BYTE *bySendTmp)  // 转换好的命令参数
{
    // 下发命令串格式:199912312359595＝>1999年12月31日23点59分59秒周5
    BYTE byTmp[50]={0};     // 临时缓冲区
    int i = 0;
    for (i = 0; i < 4; i++)            // 世纪、年、月、日
    {
        memcpy(byTmp, szValue + 2*i, 2);
        byTmp[2] = 0;
        if( (byTmp == NULL)||(*byTmp == 0x00) )
        {
            return FALSE;
        }
        bySendTmp[2 + i] = (BYTE)strtol((char*)byTmp, NULL, 16);
    }

    memcpy(byTmp, szValue + 14, 1);         // 星期
    byTmp[1] = 0;
    if( (byTmp == NULL)||(*byTmp == 0x00) )
    {
        return FALSE;
    }
    bySendTmp[6] = (BYTE)strtol((char*)byTmp, NULL, 16);
    i = 0;
    for (i = 0; i < 3; i++)                 // 时、分、秒
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

// 将下发设置增加用户命令串转换为命令参数。
BOOL PrepareAddUser(char* szValue,    // 下发的命令串
                    BYTE *bySendTmp,  // 转换好的命令参数
                    char *ADDCardID )
{
    // 下发命令串格式:(卡号),(用户号),(4位密码)(有效期)(用户权限'字符串')
    BYTE byTmp[50]={0};     // 临时缓冲区
    BYTE byBuf[5]={0};      // 临时缓冲区
    char c[10]={0};
    int i=0;
    int nValue=0;

    // 卡号
    int nPoint = StrToK(szValue, (char*)byTmp, ',');
    if( (byTmp == NULL)||(*byTmp == 0x00) )
    {
        return FALSE;
    }

    // 因PSMS4.357卡号下发为16进制字符串形式，特注释
    //nValue = atoi((char*)byTmp);        // 获取卡号
    //sprintf((char*)byTmp, "%010X", nValue); // 5位卡号
    //sprintf((char*)byTmp, "%010S", byTmp); // 5位卡号
    if (strlen((char*)byTmp)>10)
    {
        return FALSE;   // 卡号太长
    }

    strcpy(ADDCardID,(char*)byTmp); //保存卡号

    i = 0;
    for (i = 0; i < 10; i = i + 2)
    {
        strncpy(c, (char*)byTmp + i, 2);
        c[2] = 0;
        bySendTmp[2 + i/2] = (BYTE)strtol(c, NULL, 16);
    }

    // 用户号:十进制
    nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
    if (byTmp == NULL || *byTmp == 0x00 || strlen((char*)byTmp)>4)
    {
        return FALSE;
    }
    nValue = atoi((char*)byTmp);              // 或取用户编号ID
    sprintf((char*)byTmp, "%08d", nValue);    // 4位用户编号
    i = 0;
    for (i = 0; i < 8; i = i + 2)
    {
        strncpy(c, (char*)byTmp + i, 2);
        c[2] = 0;
        bySendTmp[7 + i/2] = (BYTE)strtol(c, NULL, 16);
    }

    // 2字节用户密码+4字节有效期+1字节用户权限
    nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');

    if (byTmp == NULL || *byTmp == 0x00 || strlen((char*)byTmp) != 14)
    {
        return FALSE;
    }
    i = 0;
    for (i = 0; i < 14; i += 2)    // 2字节用户密码+4字节有效期+1字节用户权限
    {
        memcpy(byBuf, byTmp + i, 2);
        byBuf[2] = 0;

        bySendTmp[11 + i/2] = (BYTE)strtol((char*)byBuf, NULL, 16);
    }

    if( bySendTmp[17]< 0x10)
    {
        bySendTmp[17]=bySendTmp[17]+0x40;//默认加卡D6=1,一般用户
    }


    return TRUE;
}

// 将下发设置删除用户命令串转换为命令参数。
BOOL PrepareDelUser(char* szValue,    // 下发的命令串
                    BYTE *bySendTmp)  // 转换好的命令参数
{
    // 下发命令串格式:(删除方法0-2),删除参数(卡号/用户号)
    BYTE byTmp[50]={0};     // 临时缓冲区
    int nValue = 0;
    int i = 0;

    int nPoint = StrToK(szValue, (char*)byTmp, ',');
    int nParam = atoi((char*)byTmp);    // 方式字
    bySendTmp[2] = nParam;
    if (nParam == 0)                    // 按卡号删除
    {
        // 获取卡号
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        // PSMS4.357下发的为字符串，可直接使用
        //nValue = atoi((char*)byTmp);
        //sprintf((char*)byTmp, "%010X", nValue); // 卡号
        if (strlen((char*)byTmp)>10)            // 卡号超长
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
    else if (nParam == 1) // 按用户编号删除
    {
        bySendTmp[3] = 0x00;
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);              // 获取用户编号
        sprintf((char*)byTmp, "%08d", nValue);    // 4位用户编号

        char c[3];
        i = 0;
        for (i = 0; i < 8; i = i + 2)
        {
            strncpy(c, (char*)byTmp + i, 2);
            c[2] = 0;
            bySendTmp[4 + i/2] = (BYTE)strtol(c, NULL, 16);
        }
    }
    else if (nParam == 2) // 全部删除
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


// 将下发设置读数据命令串转换为命令参数。
BOOL PrepareReadCmd(char* szValue,      // 下发的命令串
                    int nCmdNo,         // 命令号
                    BYTE *bySendTmp)    // 转换好的命令参数
{
    // 下发命令串格式:(删除方法0-2),删除参数(卡号/用户号)
    BYTE byTmp[50]={0};     // 临时缓冲区
    int nValue = 0;
    int nPoint = 0;
    int i = 0;
 //   WriteString("Prepaire step 1",1,24,0x1f);
//    Sleep(1000);
    if (nCmdNo < 36 && nCmdNo>30)    // 读取参数命令，不用参数
    {
        for (i = 0; i < strSet[nCmdNo].nDataLen; i++)
        {
            bySendTmp[i+2] = strSet[nCmdNo].byData[i];
        }
    }
    else if (nCmdNo == 36)    // 读取SM的一组工作日准进时段
    {
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)nValue;        // 读取记录指针低位
    }
    else if (nCmdNo>36 && nCmdNo < 40)      // 读历史记录位置
    {
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)(nValue % 256);  // 读取记录指针低位
        bySendTmp[3] = (BYTE)(nValue / 256);  // 读取记录指针高位
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************
函数名称:SetDoor
函数说明:根据命令进行设备控制
输入说明:串口句柄，设备地址，命令号，命令参数
输出说明:控制成功返回TRUE；否则返回FALSE
*********************************************************************/
BOOL SetDoor(HANDLE hComm,  // 通讯口句柄
             int nUnitNo,   // 采集器单元地址
             int nCmdNo,    // 命令号
             char* szValue, // 命令参数
             BYTE*byData    // 控制后的数据(主要用于读命令)
             )
{
    BYTE bySendTmp[50]={0}; // 命令参数:Command Group + Command Type + DataF
    BYTE byTmp[50]={0};     // 临时缓冲区
    int  i = 0;
    int  nPoint = 0;    // 分离参数用
    float fValue = 0;    // 设置值
    int nValue=0;
    int NN;//实际卡号长度
    int start_add;//卡号小于5位时候的开始位置.
    char ADDCardID[50]={0}; //卡号

    //char *p;            // zhx    19991125
 //   WriteString(" setdoor step1",1,24,0x1f);
//        Sleep(1000);
    if (nCmdNo < 0 || nCmdNo>nMaxCmdNo)
    {
        return FALSE;   // 命令超界
    }

    switch (nCmdNo)
    {
    case 0:     // 设置权限确认

        // 参数:5个字节的密码

        //p = szValue;
        //for (; p; )
        //{
        //    if (*p != ' ')
        //        break;
        //    p++;
       // }
        // strcpy(szValue, p);
        //assert(szValue = p);


        if (strlen(szValue) > 6) // 因一位可写两个密码，为保证统一，故可使用6位密码
        {
            return FALSE;   // 密码太长(用键盘操作时为6位，可在第一位补0)
            //              szValue[6] = 0; // zhx  19991125    对"  0.000"(4357)的处理
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
        memset(bySendTmp,0x00,12);//将发送的5字节清0
        NN=strlen(szValue);//实际密码的长度.若是123,则发送00123.补齐5位.
        if (NN==0)//密码没有
        {
            return FALSE;
        }
        if(NN>5)//6位密码取前5位
        {
            NN=5;
        }
        start_add= 5-NN;

        int no;
        for( no=0;no<NN;no++)//ASCII直接转换成数据
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
    case 1:    // SU主动取消对SM的设置权限:无参数
        break;
    case 2:    // 修改对SM的访问密码
        // 参数:5位的密码+1位的校验码(密码的异或)

        // 删除门号
        nPoint = StrToK( szValue, (char*)byTmp, ',');
        if (byTmp== NULL || *byTmp == 0x00)
        {
            return FALSE;
        }

        if (strlen( (char*)byTmp ) > 6)
        {
            return FALSE;   // 密码太长(用键盘操作时为6位，可在第一位补0)
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
            return FALSE;   // 密码太长(用键盘操作时为6位，可在第一位补0)
        }
        bySendTmp[7] = 0;

        memset(bySendTmp,0x00,10);//将发送的5字节清0
        NN=strlen((char*)byTmp);//实际密码的长度.若是123,则发送00123.补齐5位.
        if(NN>5)//6位密码取前5位
        {
            NN=5;
        }
        start_add= 5-NN;


        for( no=0;no<NN;no++)//ASCII直接转换成数据
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
    case 3:    // 设置SM的日期时间
        // 参数:年(2)＋月＋日＋星期＋时＋分＋秒
        if (!PrepareDateTime(szValue, bySendTmp))
        {
            return FALSE;
        }
        break;
    case 4:    // 设置工作日准进时段
    case 5:    // 设置非工作日准进时段
        // 参数:时段组号＋时段1开始时＋时段1开始分＋时段1结束时＋时段1结束分，共4段
        if (nCmdNo == 14)
        {
            bySendTmp[2] = 1;           // 非工作日只有一组
        }
        else
        {
            memcpy(byTmp, szValue, 1);  // 组号
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
    case 6:    // 增加用户
        // 参数:5位卡号＋4位用户号＋2位密码＋有效期(YYYY,MM,DD)＋权限(VIP,Group)
        if (!PrepareAddUser(szValue, bySendTmp, ADDCardID))
        {
            return FALSE;
        }
        break;
    case 7:    // 删除用户
        // 参数:方式字(0-2)+5位卡号/00+4位用户ID
        if (!PrepareDelUser(szValue, bySendTmp))
        {
            return FALSE;
        }
        break;
    case 8:     // 设定门控器特性参数
    case 9:     // 设定对门锁继电器执行时间
    case 10:    // 设定开门等待进入时间
    case 11:    // 设定对红外报警的确认时间
    case 17:    // 设置系统支持感应卡的种类及卡片编号的获取方法command type=EE
    case 12:    // 设定对异地报警的延时时间:50(十进制)表示5秒
    case 21:    // 设定系统使用的感应卡种类:26、36、44、64
    case 22:    // 设定系统感应卡编号的获取方法command type=F3
    case 23:    // 设定联动输入I2的有效电平
    case 24:    // 设定刷卡开门时，是否输入用户密码:4个0－9
    case 25:    // 设定红外感应器在报警时的有效输出电平
    case 26:    // 设定门开关感应器在开门状态时的有效输出电平
    case 27:    // 设定是否监视联动输入I2的状态
    case 28:    // 设定门控电磁锁的(种类)特性
    case 29:    // 设定是否监视红外感应器的状态
    case 30:    // 设定是否监视门开关感应器的状态
        // 为了兼容其它门禁，特取走门号参数。
        //MessageBox(NULL, (char*)szValue, "参数", MB_OK);
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)atoi(szValue+nPoint);
        break;
    case 13:    // 设定SM的一星期内休息日:1－6为星期一至六，7为星期日，其它为不休息
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = atoi((char*)byTmp);   // 第一个休息日
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = atoi((char*)byTmp);   // 第二个休息日
        break;
    case 14:    // 设定SM的节假日(不包括星期内休息日)
    case 15:    // 删除SM内的一组节假日(两字节均为0表示全部删除)
        nPoint = StrToK(szValue, (char*)byTmp, ',');            // 月
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');  // 日
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = (BYTE)strtol((char*)byTmp, NULL, 16);
        break;
    case 16:    // SU远程开门
        bySendTmp[2] = 1;  // 操作方式:1－开门，0－不操作
        break;
    case 18:    // 设定SM红外监控开启/关闭方法
        nPoint = StrToK(szValue, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)atoi((char*)byTmp);  // 开启/关闭方法控制字
        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[3] = (BYTE)atoi((char*)byTmp);  // 开启延时时间
        break;
    case 19:    // 设定SM内记录区的指针:(使用三字节方式:读取记录指针,标志)
        nPoint = StrToK(szValue, (char*)byTmp, ',');  // 读取记录指针2字节
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        nValue = atoi((char*)byTmp);
        bySendTmp[2] = (BYTE)(nValue % 256);  // 读取记录指针低位
        bySendTmp[3] = (BYTE)(nValue / 256);  // 读取记录指针高位

        nPoint += StrToK(szValue + nPoint, (char*)byTmp, ',');
        if (byTmp == NULL || *byTmp == 0x00)
        {
            return FALSE;
        }
        bySendTmp[4] = (BYTE)atoi((char*)byTmp);      // 标志
        break;
    case 20:    // 设定按星期准进的时间段列表
        memcpy(byTmp, szValue, 1);    // 表号
        byTmp[1] = 0;
        //if (byTmp == NULL || *byTmp == 0x00)
		if (byTmp == NULL )
        {
            return FALSE;
        }
        bySendTmp[2] = (BYTE)strtol((char*)byTmp, NULL, 16);
        memcpy(byTmp, szValue + 1, 1);    // 星期号
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
    default:            // 读内部数据
#ifndef _TSR_
        //bTestFlag = TRUE;   // 读参数时显示收发信息
#endif

        if (!PrepareReadCmd(szValue, nCmdNo, bySendTmp))
        {
            return FALSE;
        }
        break;
    }

    bySendTmp[0] = strSet[nCmdNo].byCmdGroup;    // 命令组:command group
    bySendTmp[1] = strSet[nCmdNo].byCmdType;     // 命令类型:command type
 //       WriteString(" setdoor step2",1,24,0x1f);
//        Sleep(1000);
    i = 0;
    int nRet = 1;
    for (i = 0; i < 3; i++)
    {
        nRet = GetResponeData(hComm, 0x10, nUnitNo, 0x80, strSet[nCmdNo].byCID2,
            strSet[nCmdNo].nDataLen + 2, byData, strSet[nCmdNo].nRetLen,bySendTmp);

        // 显示返回码:
        //char s[100]={0};
        //sprintf( s, "%X", nRet );
        //MessageBox(NULL, s, "控制结果", MB_OK);

        // 当删除卡时，如果存储空间已空，会出现0xE4错误信息。若是没有此卡，则出现E5错误信息
        if( nCmdNo==7 && (nRet==0xE5||nRet==0xE4) )
        {
            nRet = 0;
        }

#ifndef _TSR_
        //bTestFlag = FALSE;  // 显示收发信息
#endif
//        WriteString(" setdoor step3",1,24,0x1f);
//        Sleep(1000);

        Sleep(100);         // 在两条命令之间加点延时,否则容易失败
        if (!nRet)          // 成功
        {
            return TRUE;
        }
        else if (nRet != nErrorEF && nRet != nErrorEE)
        {
            break;
        }
    }

    //5.40   当增加卡的时候，发现已经有卡，则删除卡，在增加卡
    //add by L&L 2010.03.18
    if ( nCmdNo==6 && nRet==0xE7 )
    {
        //保存增加卡的szValue，byData,重szValue中获得卡号:ADDCardID
        //先删除卡
        char ReAddszValue[50]={0};
        sprintf(ReAddszValue, "0,%s", ADDCardID);    // 按根据卡号删除协议方式


        WriteAsc(ASC_FILE, "\r\n已有卡号，先删除卡号:%s \r\n", ReAddszValue);
        WriteAsc(HEX_FILE, "\r\n已有卡号，先删除卡号:%s \r\n", ReAddszValue);


        nRet = SetDoor(hComm, nUnitNo, 7, ReAddszValue, byData);



        if (nRet)  //删除卡成功//再增加卡
        {
            //bTestFlag = TRUE;
            WriteAsc(ASC_FILE, "\r\n再增加卡:%s \r\n", szValue);
            WriteAsc(HEX_FILE, "\r\n再增加卡:%s \r\n", szValue);
            //bTestFlag = FALSE;

            nRet = SetDoor(hComm, nUnitNo, 6, szValue, byData);
            return nRet;
        }


    }

    return FALSE;
}


//*****************************************************************
// 函数名称：Write
// 功能描述：V4.0等版的控制函数。
// 输入参数：hComm - 通信句柄, nUnitNo - 采集器单元地址,
//           pCmdStr - 命令串，参数在前，命令号在后，用“,”分割。
// 输出参数：
// 返    回：TRUE－成功；FALSE－失败。
// 其    他：
//*****************************************************************
DLLExport BOOL Write(
                     HANDLE hComm,      // 通讯口句柄
                     int nUnitNo,       // 采集器单元地址
                     char* pCmdStr )    // 控制命令串
{
    // 考虑到目前的所有系统版本和 Write接口的特点，故不再支持本功能。
    //hComm;
    //nUnitNo;
    //pCmdStr;
    return FALSE;
}

/**********************************************************************
函数名称:Control_old，相对于PSMS4.357而言
函数说明:此函数为控制命令接口，用于4.x的DLL和TSR中
输出说明:控制成功,返回TRUE,否则返回FALSE.
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
        //MessageBox(0, pCmdStr, "控制命令", MB_OK);
        //WriteAsc(ASC_FILE,"%s","\r\n控制命令\r\n");
        //WriteAsc(HEX_FILE,"%s","\r\n控制命令\r\n");
    }
#endif
//    WriteString(" controlold step1",1,24,0x1f);
    char sTarget[100]={ "0x00" };
    char szValue[100]={ "0x00" };
    BYTE byData[MAXINFOLEN/2]={0};    // 控制返回参数

    if (strlen(pCmdStr) > 100)
    {
#ifndef _TSR_
//        fcloseall();
#endif
        return FALSE;   // 命令串太长
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
    int nCmdNo = (int)atoi(pCmdStr);  // 获取命令号


    if (nPoint)
    {
        strcpy(szValue, pCmdStr + nPoint);  // 获取命令参数
    }

    if (nCmdNo == 16 || nCmdNo == 17)  // 红外参数布防＝1/撤防＝0
    {
        sprintf(szValue, "%c", 65-nCmdNo);  // 参数
        nCmdNo = 89;                        // 命令号
//        WriteString(" controlold step3",1,24,0x1f);
//        Sleep(1000);
    }
    else if (nCmdNo == 31)        // 刷卡开门是否需要密码
    {
        nCmdNo = 84;
    }
    else if (nCmdNo == 36)        // 增加
    {
        // 卡号、用户号、密码、群组等
        char sCardID[100], sCardNo[100], sPassWord[100], sGroup[100], sDate[20];
        nPoint = StrToK(szValue, sTarget, '+');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardNo, sTarget);  // 用户号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardID, sTarget);  // 卡号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sGroup, sTarget);  // 群组(10进制)
        //int nGroup = atoi( sGroup );特权用户CO,用此函数会失效
        int nGroup = (BYTE)strtol(sGroup, NULL, 16);

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy( sPassWord, sTarget);  // 密码

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy( sDate, sTarget);  // 有效期

        // 协议的格式:(卡号),(用户号),(4位密码)(有效期)(用户权限)
        //sprintf(szValue, "%s,%s,%04s99991231%02X", sCardID, sCardNo, sPassWord, nGroup);
        //MessageBox(0, sDate, "控制命令", MB_OK);
        sprintf(szValue, "%s,%s,%04s%s%02X", sCardID, sCardNo, sPassWord, sDate, nGroup);
    }
    else if (nCmdNo == 37)    // 删除卡
    {
        char sCardID[50]={0}, sCardNo[50]={0};   // 卡号、用户号
        nPoint = StrToK(szValue, sTarget, '+');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardNo, sTarget);   // 用户号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        strcpy(sCardID, sTarget);   // 卡号
        sprintf(szValue, "0,%s", sCardID);    // 按根据卡号删除协议方式
    }
    else if (nCmdNo == 42)          // 按星期设置时间组
    {
        nCmdNo = 80;
        // 取门号
        nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
        if (pCmdStr == NULL || *pCmdStr == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        // 取时间组号:0-15
        nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
        if (pCmdStr == NULL || *pCmdStr == 0x00)
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        int nGroup = (int)atoi(sTarget) - 1;  // 获取时间组号
        char cGroup;    // 时间表号
        if (nGroup < 10)
        {
            cGroup = nGroup + 0x30; // 转换为0－9
        }
        else
        {
            cGroup = nGroup + 0x37; // 转换为A－F
        }

        // 星期号
        char cWeek = pCmdStr[nPoint + 24];
        if( '7' == cWeek)
        {
            //cWeek=0;这个是字符，0x37
            cWeek=0x30;
        }

        if (nPoint)
        {
            // 获取命令参数:时间组号星期时间段
            sprintf(szValue, "%c%c%s", cGroup, cWeek, pCmdStr + nPoint);
        }
    }
    else if (nCmdNo == 45)           // 删除所有卡
    {
        sprintf(szValue, "2, 0");    // 按根据卡号删除协议方式
        nCmdNo = 37;
    }
    else if (nCmdNo == 55)           // 清除所有刷卡记录
    {
        // 读取当前历史记录柜数据:命令号不用经后面程序索引
        if (!SetDoor(hComm, nUnitNo, 31, "", byData))
        {
#ifndef _TSR_
//            fcloseall();
#endif
            return FALSE;
        }
        // 将当前指针移动到即将存储数据处
        sprintf(szValue, "%d,%d", byData[2] + byData[3]*256, byData[6]);
        nCmdNo = 79;    // 设置指针:命令号需经下面程序索引。
    }


    // 因为门禁的上层接口总是变化，命令号不定，故使用本查找方式，使以后修改方便。
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
char sTmp[10]={0};  // 临时缓冲区
char sTime[50]={0}; // 处理后时间组
#endif

/**********************************************************************
函数名称:Control，针对于PSMS4.357
函数说明:此函数为控制命令接口
输出说明:控制成功,返回TRUE,否则返回FALSE.
注意事项:只要密码验证成功后，不管控制是否成功，均需取消密码
*********************************************************************/
DLLExport BOOL Control(HANDLE hComm, int nUnitNo, char *pCmdStr)
{
    int nRet=0;
    //bTestFlag = TRUE;

    //MessageBox(0, pCmdStr, "控制命令", MB_OK);
/*    if( hComm==0)
    {
        WriteString("Control hComm is NULL", 1, 24, 0x1f);
        return TRUE;
    }
  */

//#ifdef _DLL_

    WriteAsc(ASC_FILE,"6.00控制命令串:%s\r\n",pCmdStr);
    WriteAsc(HEX_FILE,"6.00控制命令串:%s\r\n",pCmdStr);

//#endif

#ifndef _TSR_
    char sTarget[100]={0};
#endif

    //  char sTarget[50]={0};
    if (strlen(pCmdStr) > 100)
    {
        return FALSE;   // 命令串太长
    }

    // Get nCmdNo
    int nPoint = StrToK(pCmdStr, sTarget, ',');
    if (pCmdStr == NULL || *pCmdStr == 0x00)
    {
        return FALSE;
    }
    int nCmdNo = (int)atoi(pCmdStr);  // 获取命令号

    // 获取第一个参数
    // Get Password:根据PSMS4.357定义每条控制命令第一个参数均为密码



      nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }


    //WriteString("step1",1,24,0x1f);
    //Sleep(1000);
    // 验证密码


#ifndef _TSR_
    char sStr[120]={0};
#endif




        sprintf(sStr, "60,%s", sTarget);
        if (!Control_old(hComm, nUnitNo, sStr))
        {

            WriteAsc(ASC_FILE,"权限确认失败:密码 %s\r\n",sTarget);
            WriteAsc(HEX_FILE,"权限确认失败:密码 %s\r\n",sTarget);
            return FALSE;
        }

        WriteAsc(ASC_FILE,"权限确认成功:密码 %s\r\n",sTarget);
        WriteAsc(HEX_FILE,"权限确认成功:密码 %s\r\n",sTarget);



    ////add 5.40/////////////////////////////////////////////////////////////////////////
    //密码权限确认OK后，如果密码更改，则保存新密码

#ifdef _LINUX_
    FILE *fp;
    char opwd[8] = { 0 };
    if ((fp = fopen("/home/idu/SO/EDM30e.ini", "rb")) != NULL)
    {
        fgets(opwd, 7, fp);
        fclose(fp);
    }

    if( strcmp(opwd, sTarget) != 0 )    // 密码不同 5.50
    {
        if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"wb+")) == NULL)
        {
            printf("条码文件不存在或无法获取条码！\n");
            return FALSE;
        }

        WriteAsc(ASC_FILE,"权限确认成功写入密码 %s\r\n",sTarget);
        WriteAsc(HEX_FILE,"权限确认成功写入密码 %s\r\n",sTarget);


        fprintf(fp, "%s\n", sTarget);
        fclose(fp);
    }
#endif


    //  WriteString("Error2",1,24,0x1f);
    //    Sleep(1000);
    // 获取第一个以后的参数:
#ifndef _TSR_
    char sCmdStr[120]={0};
#endif



    sprintf(sCmdStr,"%d,%s", nCmdNo, pCmdStr + nPoint);

    //42,mmmmmm,d,02,00012C1902582BC3E94B05145786406A47D06
    //42,mmmmmm,dd,tt,6个时间段1字节星期.
    //mmmmmm:密码，dd:门号，tt:时间组号（取值[0,15]）    6个时间段36位（每三位16进制为一个时间）星期1位
    // 特殊处理:因PSMS4.357下发星期表超长，需将时间段压缩了:3个16进值＝>4个十进制
    if( nCmdNo==42 )    // 按星期设置时间组
    {
        // 获取命令号
        nPoint = StrToK(sCmdStr, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");

            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // 获取门号
        nPoint += StrToK(sCmdStr+nPoint, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // 获时间组号
        nPoint += StrToK(sCmdStr+nPoint, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
#ifndef _TSR_
        char sTmp[10]={0};  // 临时缓冲区
        char sTime[50]={0}; // 处理后时间组
#endif
        int nTime=0;        // 时间组
        // 时间组处理:
        int i=0;
        //cxd for(  i=0; i<12; i++ )???
		for(  i=0; i<6; i++ )
        {
            strncpy( sTmp, sCmdStr+nPoint+i*4, 4 );
            nTime = (int)strtol( sTmp, NULL, 16 );
            sprintf( sTime+i*4, "%04X", nTime );
        }
        // 取星期号
        char cWeek = sCmdStr[nPoint+i*4];
        sprintf( sCmdStr+nPoint, "%s%c", sTime, cWeek );
    }

    // 特殊处理:针对PSMS4.357修改卡
    if( nCmdNo==38 )    // 修改卡:先删除该卡，再增加该卡，按卡号处理
    {
        // UID+卡号,权限,密码

        // 获取命令号
        nPoint = StrToK(sCmdStr, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // 获取用户号
        int nPoint1 = StrToK(sCmdStr+nPoint, sTarget, '+');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }

        // 获取卡号
        nPoint1 += StrToK(sCmdStr+nPoint+nPoint1, sTarget, ',');
        if (sTarget== NULL || *sTarget == 0x00)
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
        // 先删除该卡:
#ifndef _TSR_
        char sStr[100]={0};
#endif
        sprintf( sStr, "37,0+%s", sTarget );
        if( !Control_old( hComm, nUnitNo, sStr ) )
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }

        // 增加卡:一般来说增加卡成功后密码取消也会成功，故不保留成功标志。
        // cmq 2000.04.04 保留标志
        sprintf( sStr, "36,%s", sCmdStr+nPoint );
        nRet = Control_old( hComm, nUnitNo, sStr );
        if( !nRet )
        {
            // 主动取消控制权限。
            sprintf(sStr, "61");
            if (!Control_old(hComm, nUnitNo, sStr))
            {
                return FALSE;
            }
        }
    }//if
    else
    {
        // 其它控制:一般来说增加卡成功后密码取消也会成功，故不保留成功标志。
        // 2000.4.4 cmq修改为保留
        nRet = Control_old(hComm, nUnitNo, sCmdStr);
        // 主动取消控制权限。
        sprintf(sStr, "61");
        Control_old(hComm, nUnitNo, sStr);

        if (nCmdNo==62)
        {
            // Get 命令
            int nPoint = StrToK(sCmdStr, sTarget, ',');
            if (pCmdStr == NULL || *pCmdStr == 0x00)
            {
                return FALSE;
            }

            // 门号
            nPoint += StrToK(sCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }

            // 新密码
            nPoint += StrToK(sCmdStr + nPoint, sTarget, ',');
            if (sTarget == NULL || *sTarget == 0x00)
            {
                return FALSE;
            }

            ////add 5.40/////////////////////////////////////////////////////////////////////////
            //密码权限确认OK后，保存密码
#ifdef _LINUX_
            FILE *fp;
            if ((fp = fopen("/home/idu/SO/EDM30e.ini" ,"wb+")) == NULL)
            {
                printf("条码文件不存在或无法获取条码！\n");
                return FALSE;
            }

            WriteAsc(ASC_FILE,"修改密码成功，写入新密码 %s\r\n",sTarget);
            WriteAsc(HEX_FILE,"修改密码成功，写入新密码 %s\r\n",sTarget);


            fprintf(fp, "%s\n", sTarget);
            fclose(fp);
#endif

        }

        //bTestFlag=FALSE;
        return nRet;
    }
    // 主动取消控制权限。2000.04.04如果控制不成功，10分钟后，会自动取消权限，故控制结果效果长远些
    sprintf(sStr, "61");
    Control_old(hComm, nUnitNo, sStr);

    return TRUE;
}




////////////////////////////////////////////////////////////////////////////////
//
// 现场调测部分:初始化串口、采集和控制接口的调用等。
//
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG_
// 用于现场调试编程时，模拟系统调用函数。

#define nSetTimeOut 2000    // 定义串口超时参数:2S，可根据需要修改。

// 初始化串口:ComPort－串口号，szSetting－通讯格式:波特率、校验、数据位、停止位。
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

    // 用于通过调制解调器远程测试用。
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
    //Control    (hComm, 1, "42,123456,1,02,0000 0300 0400 0600 0700 1000 1200 1300 1400 1600 1700 2000 6");    // 设置时间段
    //Control(hComm, 1, "42,123456,1,02,000 12C 190 258 2BC 3E9 4B0 514 578 640 6A4 7D0 6");    // 设置时间段
    Control(hComm, 1, "42,123456,1,02,00012C1902582BC3E94B05145786406A47D06");    // 设置时间段

    Control(hComm, 0, "45,000000");    // 删除所有卡
    Control(hComm, 0, "10,000000,1");  // 开门
    Control(hComm, 4, "31,000000,1,0");
    Control(hComm, 1, "30,0,1,20");
    Control(hComm, 4, "62,000000,1,111111");    // 修改密码
    Control(hComm, 4, "36,000000,11+000000BCDF,C0,1111");   // 3.357
    Control(hComm, 4, "36,000000,01+000000BCDF,C0,1111,19991231");   // 3.39
    Control(hComm, 4, "38,000000,11+000000BCDF,12,1111");
    Control(hComm, 1, "32,111111,199909141625002");  // 设置日期

    Control_old(hComm, 1, "38, 0, 0");
    Control_old(hComm, 1, "300");
    Control_old(hComm, 1, "61");       // 取消密码
    Control_old(hComm, 1, "60,222222");   // 校验密码
    Control_old(hComm, 1, "62,1,222222"); // 修改密码

    Control_old(hComm, 1, "55");    // 清除所有记录
    Control_old(hComm, 1, "32,199909141625002");  // 设置日期
    Control_old(hComm, 1, "64,100001200160018001900200022002359");    // 设置时间段

    Control_old(hComm, 1, "37,0+5074664");  // 删除卡4D6EE8

    // Control_old(hComm, 1, "36, 5077815, 3, 000019991231C0");  // 增加卡
    // Control_old(hComm, 1, "36, 5074664, 1, 00000010010101");  // 增加卡4D6EE8
    // Control_old(hComm, 1, "36, 38379688, 800, 000019991231C0");  // 增加卡249a0a8
    Control_old(hComm, 1, "36, 800 + 38379688, C0, 0000");  // 增加卡249a0a8
    Control_old(hComm, 1, "16, 1");    // 红外布防

    Control_old(hComm, 1, "68, 0");    // 设定SM门控器特性参数
    Control_old(hComm, 1, "69, 81");   // 14－17时间
    Control_old(hComm, 1, "73, 6, 7");  // 星期休息日
    Control_old(hComm, 1, "74, 1, 15"); // 节假日
    Control_old(hComm, 1, "75, 10, 1"); // 删除节假日
    Control_old(hComm, 1, "10, 1");    // 开门

    Control_old(hComm, 1, "79, 390, 0");  // 设定记录区指针

    // 设置时间段
    Control_old(hComm, 1, "80, 05000003000400060007001000120013001400160017002000");

    char sStr[50]={0};  // 测试准进人员数
    for (int i = 801; i < 3000; i++)
    {
        sprintf(sStr,"36,%d,%d,%04d1999123100", i, i, i);
        if (!Control_old(hComm, 1, sStr))
            break;
    }
    // 用于通过调制解调器远程测试用。
    // sprintf(Buf,"%s\r\n","+++ATH");
    // WriteFile(hComm, Buf, 6, &lWritten, NULL);
    // ReadFile(hComm, Buf, 30, &lRead, NULL);

    fcloseall();

    CloseComm(hComm);
}
#endif
