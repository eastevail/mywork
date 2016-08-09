/*
 * stockcheck.cpp
 *
 *  Created on: 2015年10月22日
 *      Author: lixiang
 */

#include "CMangEVT.h"
#include "CWowzaEvt.h"
#include "CTcpListenEvt.h"
#include"tinyxml2.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

myselfstock::CMangEVT* pMevt=NULL;
int Creat_wowza_from_cfg(const char* configfile)
{
	myselfstock::CEventBase* tmp = NULL;
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError res = doc.LoadFile(configfile);
	if (tinyxml2::XML_SUCCESS != res)
	{
		TSever_LOG_Error("open %s error  %s\n", configfile, doc.ErrorName());
		return -1;
	}
	tinyxml2::XMLNode* root = doc.RootElement();
	// trace every items below root.
	for (tinyxml2::XMLNode* item = root->FirstChild(); item; item = item->NextSibling())
	{
		// read name.
		const char* ip;
		const char* sn;
		const char*port;
		tinyxml2::XMLNode* child = item->FirstChildElement("ip");
		if (child)
		{
			ip = child->ToElement()->GetText();
		}
		child = item->FirstChildElement("sn");
		if (child)
		{
			sn = child->ToElement()->GetText();
		}
		child = item->FirstChildElement("port");
		if (child)
		{
			port = child->ToElement()->GetText();
		}
		tmp = new myselfstock::CWowzaEvt((char*)sn, (char*)ip, atoi(port));
		tmp->Init();
		pMevt->CreatEvtStock(tmp);
	}

	return 0;

}
void* thread(void* arg)
{
	sleep(5);
//	Creat_wowza_from_cfg("./output.xml");
	myselfstock::CEventBase* tmp = NULL;

	for (int i = 0; i < 50; i ++)
	{
		printf("Create pthread !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!i\n");
		tmp = new myselfstock::CWowzaEvt("sky123456789", "10.188.188.3", 8760 + i);
		tmp->Init();
		pMevt->CreatEvtStock(tmp);
	}
	sleep(2);
/*	sleep(2);
	pMevt->DelOneFlowFromUp(tmp);*/
	return NULL;
}
int main()
{
    char buf[1024]={0};
    printf("lllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllll\n");
    pMevt = myselfstock::CMangEVT::GetInstance();
    myselfstock::CEventBase* tmp=NULL;
    pthread_t id;
	int i , ret;
	ret = pthread_create(&id, NULL, thread, NULL);
	if (ret != 0)
	{
		printf("Create pthread error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!n");
//		exit(1);
	}
    tmp = new myselfstock::CTcpListenEvt(9999);
    tmp->Init();
    pMevt->CreatEvtStock(tmp);

/*    tmp = new myselfstock::CWowzaEvt("sky123456789","10.0.0.109",8765);
    tmp->Init();
    pMevt->CreatEvtStock(tmp);*/

    pMevt->DispatchEvt();
//    pthread_join(id,NULL);
    myselfstock::CMangEVT::Destroy();
    return 0;
}


