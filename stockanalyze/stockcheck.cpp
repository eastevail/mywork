/*
 * stockcheck.cpp
 *
 *  Created on: 2015年10月22日
 *      Author: lixiang
 */

#include "CMangEVT.h"
#include "CStockEvt.h"
#include "com_log.h"
#include <unistd.h>
#include<stdio.h>
#include <string.h>
static int CreateCStockevt(myselfstock::CEventBase* tmp,myselfstock::CMangEVT* pMevt,char* buf)
{
    char* pos = NULL;
    char* start=buf;
    int i=0;
    while (1)
    {
        pos = strstr(start, ",");
        if (pos)
        {
            start=pos+1;
            i++;
            if('\0'==(*start))
            {
                char name[128]={0};
                strncpy(name,buf,start-buf);
                TSever_LOG_Error("name111111111=%s",name);
                tmp = new myselfstock::CStockevt(name, 1);
                tmp->Init();
                pMevt->CreatEvtStock(tmp);
                break;
            }
            else if (10 == i)
            {
                char name[128]={0};
                strncpy(name,buf,start-buf);
                TSever_LOG_Error("name22222222222=%s",name);
                tmp = new myselfstock::CStockevt(name, 1);
                tmp->Init();
                pMevt->CreatEvtStock(tmp);
                return CreateCStockevt(tmp,pMevt,start);
            }
        }else{
            break;
        }
    }
    return 1;
}

int main()
{
    char buf[1024*5]={0};
    char* pos=NULL;
    myselfstock::CMangEVT* pMevt = myselfstock::CMangEVT::GetInstance();
    myselfstock::CEventBase* tmp=NULL;
    myselfstock::COperaMaridb* pOpMaridb=myselfstock::COperaMaridb::GetInstance("sfsdf");
    pOpMaridb->SelectDb("select stockname from yesterday_hammer_stock",buf,1024*5);
    CreateCStockevt(tmp,pMevt,buf);
    pMevt->DispatchEvt();
    myselfstock::CMangEVT::Destroy();
    myselfstock::COperaMaridb::Destroy("sfsdf");
    return 0;
}


