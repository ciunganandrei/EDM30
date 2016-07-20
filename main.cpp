

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
            return FALSE;
        }
        strcpy(sCardNo, sTarget);  // 用户号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
            return FALSE;
        }
        strcpy(sCardID, sTarget);  // 卡号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
            return FALSE;
        }
        strcpy(sGroup, sTarget);  // 群组(10进制)
        //int nGroup = atoi( sGroup );特权用户CO,用此函数会失效
        int nGroup = (BYTE)strtol(sGroup, NULL, 16);

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
            return FALSE;
        }
        strcpy( sPassWord, sTarget);  // 密码

        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {
            return FALSE;
        }
        strcpy(sDate,szDay);
        strcat(sDate,szMonth);
        strcat(sDate,szYear);
        strcpy(sCardID, szCardID);

        cout << "SDate" <<sDate << endl;
        //strcpy( sDate, sTarget);  // 有效期

        // 协议的格式:(卡号),(用户号),(4位密码)(有效期)(用户权限)
        //sprintf(szValue, "%s,%s,%04s99991231%02X", sCardID, sCardNo, sPassWord, nGroup);
        //MessageBox(0, sDate, "控制命令", MB_OK);
        sprintf(szValue, "%s,%s,%04s%s%02X", sCardID, sCardNo, sPassWord, sDate, nGroup);
        cout << "szValue" << szValue << endl;
        cout << "sCardID" << sCardID << endl;
        cout << "sCardNo" << sCardNo << endl;
        cout << "sDate" << sDate << endl;
        cout << "nGroup" << nGroup << endl;
    }
    else if (nCmdNo == 37)    // 删除卡
    {
        char sCardID[50]={0}, sCardNo[50]={0};   // 卡号、用户号
        nPoint = StrToK(szValue, sTarget, '+');
        if (sTarget == NULL || *sTarget == 0x00)
        {

            return FALSE;
        }
        strcpy(sCardNo, sTarget);   // 用户号
        nPoint += StrToK(szValue + nPoint, sTarget, ',');
        if (sTarget == NULL || *sTarget == 0x00)
        {

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

            return FALSE;
        }
        // 取时间组号:0-15
        nPoint += StrToK(pCmdStr + nPoint, sTarget, ',');
        if (pCmdStr == NULL || *pCmdStr == 0x00)
        {

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
    /*    if (!SetDoor(hComm, nUnitNo, 31, "", byData))
        {

            return FALSE;
        } */
        // 将当前指针移动到即将存储数据处
        sprintf(szValue, "%d,%d", byData[2] + byData[3]*256, byData[6]);
        nCmdNo = 79;    // 设置指针:命令号需经下面程序索引。
    }
       else if (nCmdNo == 100)
    {
        strcpy(szYear,pCmdStr+nPoint);
    }
           else if (nCmdNo == 101)
    {
        strcpy(szMonth,pCmdStr+nPoint);
    }
           else if (nCmdNo == 102)
    {
        strcpy(szDay,pCmdStr+nPoint);
    }
          else if (nCmdNo == 103)
    {
        strcpy(szCardID,pCmdStr+nPoint);
    }


    // 因为门禁的上层接口总是变化，命令号不定，故使用本查找方式，使以后修改方便。
    for (int i = 0; i < nMaxCmdNo; i++)
    {
        if (nCmdNo == strSet[i].nCmdNo)
        {
            nCmdNo = i;
            int nRet = 1;//SetDoor(hComm, nUnitNo, nCmdNo, szValue, byData);


            return nRet;
        }
    }
    return FALSE;
}



int main()
{
    cout << "Hello world!" << endl;
    char* s= "Ana are mere cat cuprinde";
    char* d;

    int rez;
    rez=StrToK(s,d,' ');
    cout << rez << d << endl;
    Control_old(1, 4, "102,17");
    Control_old(1, 4, "101,12");
    Control_old(1, 4, "100,2001");
    Control_old(1, 4, "103,0000DFG");
    Control_old(1, 4, "36,000000,01+000000BCDF,C0,1111,19991231");

    cout << szYear << endl;
    Control_old(1, 4, "102,258");
    cout << szYear << endl;
    //Control_old(1, 1, "36,01+0000080080,C0,0000,20200101");
    return 0;
}
