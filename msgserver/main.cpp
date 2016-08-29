/*
 * stockcheck.cpp
 *
 *  Created on: 2015年10月22日
 *      Author: lixiang
 */

#include "../common/core/CMangEVT.h"
#include "CTcpListenEvt.h"
#include"tinyxml2.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

myselfstock::CMangEVT* pMevt=NULL;

int main()
{
    char buf[1024]={0};
    printf("lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
    pMevt = myselfstock::CMangEVT::GetInstance();
    myselfstock::CEventBase* tmp=NULL;
	int i , ret;
    tmp = new myselfstock::CTcpListenEvt(9999);
    tmp->Init();
    pMevt->CreatEvtStock(tmp);
    pMevt->DispatchEvt();
//    pthread_join(id,NULL);
    myselfstock::CMangEVT::Destroy();
    return 0;
}


