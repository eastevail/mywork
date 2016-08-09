/*
 * CMangEVT.cpp
 *
 *  Created on: 2015年10月13日
 *      Author: lixiang
 */

#include "CMangEVT.h"
#include "com_log.h"
//#include "CStockEvt.h"
//#include "COperaMaridb.h"
#include <event2/event.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
namespace myselfstock
{
CMangEVT* CMangEVT::m_pInstance=NULL;

int CMangEVT::m_ReferCount=0;
CMangEVT::CMangEVT()
{
    // TODO Auto-generated constructor stub
    m_pBase = event_base_new();
    m_pEvtNotify=NULL;
    m_pEvtNotifyerr=NULL;
//    m_pEvtBase=NULL;
    INIT_TS_LOG;
    TSever_LOG_Error("*****CMangEVT*****CMangEVT******CMangEVT*******CMangEVT*******************CMangEVT********CMangEVT******");
    TSever_LOG_Error("VERSION   IS     STAE_CLOUD_V1.0.01.003");
}

CMangEVT::~CMangEVT()
{
    // TODO Auto-generated destructor stub

    map<CEventBase*,CEventBase*>::iterator iter;
/*    for (iter = m_EvtMapNew.begin(); iter != m_EvtMapNew.end(); ++iter){
//        event_del(&(iter->first->m_ServerEvent));
        delete iter->first;
    }*/
    for (iter = m_EvtMapAdded.begin(); iter != m_EvtMapAdded.end(); ++iter){
        event_del(&(iter->first->m_ServerEvent));
        if(ACCEPT==iter->first->m_Type){
        	iter->first->CloseEvt();
        }
        delete iter->first;
    }

//    m_EvtMapNew.clear();
    m_EvtMapAdded.clear();
	map<string, CRtpQueue*>::iterator iterData;
	for (iterData = m_ManageRtpQueue.begin(); iterData != m_ManageRtpQueue.end(); ++iterData){
		delete iterData->second;
	}
	m_ManageRtpQueue.clear();
//    event_del(&m_signal_int);

    TSever_LOG_Error();
    CLOSE_TS_LOG;

}
int CMangEVT::DelRtpQue(char *sn)
{
	string str(sn);
	map<string,CRtpQueue*>::iterator iter;
	iter=m_ManageRtpQueue.find(str);
	if (iter != m_ManageRtpQueue.end()){
		(iter->second->m_ReferCount)--;
		if (0 == iter->second->m_ReferCount)
		{
		    TSever_LOG_Error();
			delete iter->second;
			m_ManageRtpQueue.erase(iter);
		}
		return 0;
	}
	return -1;

}
CRtpQueue* CMangEVT::GetInstanceRtpQue(char *sn)
{
	string str(sn);
	map<string,CRtpQueue*>::iterator iter;
	iter=m_ManageRtpQueue.find(str);
	if (iter != m_ManageRtpQueue.end()){
		(iter->second->m_ReferCount)++;
		return iter->second;
	}else{
		CRtpQueue* tmp=new CRtpQueue(sn);
		(tmp->m_ReferCount)++;
		m_ManageRtpQueue.insert(pair<string, CRtpQueue*>(str,tmp));
		return tmp;
	}
}
CMangEVT* CMangEVT::GetInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new CMangEVT();
    }
//    m_ReferCount++;
//    TSever_LOG_Error("start m_ReferCount=%d",m_ReferCount);
    return m_pInstance;
}
int CMangEVT::Destroy()
{
//    m_ReferCount--;
//    TSever_LOG_Error("end m_ReferCount=%d",m_ReferCount);
    if (0 == m_ReferCount)
    {
        TSever_LOG_Error("@@@@@@@@@CMangEVT@@@CMangEVT@@@@@@@@@@@@@@CMangEVT@@@CMangEVT@@@@@@CMangEVT@@@@@@CMangEVT@@@@@@@@@@");
        delete m_pInstance;
    }
    return 0;
}
/*int CMangEVT::AssignEvt()
{
    map<CEventBase*,CEventBase*>::iterator iter;
    for (iter = m_EvtMapAdded.begin(); iter != m_EvtMapAdded.end(); ++iter)
    {
        event_assign(&(iter->first->m_ServerEvent),m_pBase,iter->first->m_ReadFd,EV_PERSIST|EV_READ,
                iter->first->GetCallbackAddr(),iter->first);
        event_add(&(iter->first->m_ServerEvent), &(iter->first->m_TimeOut));

    }
    return 0;
}*/
int CMangEVT::AssignOneEvt(CEventBase* pdelevt)
{
    struct timeval tmp;
    evutil_timerclear(&tmp);
    event_assign(&(pdelevt->m_ServerEvent), m_pBase, pdelevt->m_ReadFd, pdelevt->m_event, pdelevt->GetCallbackAddr(), pdelevt);
    if (evutil_timercmp(&(pdelevt->m_TimeOut), &tmp, ==))
    {
        event_add(&(pdelevt->m_ServerEvent), NULL);
    }
    else
    {
    	TSever_LOG_Error("add event timeout=%d",pdelevt->m_TimeOut.tv_sec);
        event_add(&(pdelevt->m_ServerEvent), &(pdelevt->m_TimeOut));
    }
    m_EvtMapAdded.insert(pair<CEventBase*, CEventBase*>(pdelevt, pdelevt));
    return 0;
}
int CMangEVT::ChangToWriteReadEvt(CEventBase* pEvtB)
{
	event_del(&(pEvtB->m_ServerEvent));
    event_assign(&(pEvtB->m_ServerEvent), m_pBase, pEvtB->m_ReadFd, EV_PERSIST | EV_READ|EV_WRITE, pEvtB->GetCallbackAddr(), pEvtB);
	event_add(&(pEvtB->m_ServerEvent), NULL);
	return 0;
}

int CMangEVT::ChangToTimeOutReadWriteEvt(CEventBase* pEvtB,struct timeval* pTime)
{
	event_del(&(pEvtB->m_ServerEvent));
    event_assign(&(pEvtB->m_ServerEvent), m_pBase, pEvtB->m_ReadFd, EV_PERSIST | EV_READ|EV_WRITE, pEvtB->GetCallbackAddr(), pEvtB);
	event_add(&(pEvtB->m_ServerEvent), pTime);
	return 0;
}
int CMangEVT::ChangToAppEvt(CEventBase* pEvtBDel,CEventBase* pEvtAppAdd)
{
	int ret=event_del(&(pEvtBDel->m_ServerEvent));
	if(ret<0){
		TSever_LOG_Error("event_del in ChangToAppEvt error");
		return -1;
	}
	m_EvtMapAdded.erase(pEvtBDel);
	delete pEvtBDel;

    event_assign(&(pEvtAppAdd->m_ServerEvent), m_pBase, pEvtAppAdd->m_ReadFd, pEvtAppAdd->m_event, pEvtAppAdd->GetCallbackAddr(), pEvtAppAdd);
	struct timeval tmp;
    evutil_timerclear(&tmp);
    tmp.tv_sec = 5;
	event_add(&(pEvtAppAdd->m_ServerEvent), &tmp);
	m_EvtMapAdded.insert(pair<CEventBase*, CEventBase*>(pEvtAppAdd, pEvtAppAdd));
	return 0;
}
int CMangEVT::ChangToIpcEvt(CEventBase* pEvtBDel,CEventBase* pEvtIpcAdd)
{
	int ret=event_del(&(pEvtBDel->m_ServerEvent));
	if(ret<0){
		TSever_LOG_Error("event_del in ChangToAppEvt error");
		return -1;
	}
	m_EvtMapAdded.erase(pEvtBDel);
	delete pEvtBDel;
    ret=event_assign(&(pEvtIpcAdd->m_ServerEvent), m_pBase, pEvtIpcAdd->m_ReadFd, pEvtIpcAdd->m_event, pEvtIpcAdd->GetCallbackAddr(), pEvtIpcAdd);
	if(ret<0){
		TSever_LOG_Error("event_del in ChangToAppEvt error");
		return -1;
	}
	struct timeval tmp;
    evutil_timerclear(&tmp);
    tmp.tv_sec = 5;
	ret=event_add(&(pEvtIpcAdd->m_ServerEvent), &tmp);
	if(ret<0){
		TSever_LOG_Error("event_del in ChangToAppEvt error");
		return -1;
	}
	m_EvtMapAdded.insert(pair<CEventBase*, CEventBase*>(pEvtIpcAdd, pEvtIpcAdd));
	return 0;
}
int CMangEVT::ChangToTimeOutReadEvt(CEventBase* pEvtB,struct timeval* pTime)
{
	event_del(&(pEvtB->m_ServerEvent));
    event_assign(&(pEvtB->m_ServerEvent), m_pBase, pEvtB->m_ReadFd, EV_PERSIST | EV_READ, pEvtB->GetCallbackAddr(), pEvtB);
	event_add(&(pEvtB->m_ServerEvent), pTime);
	return 0;
}
int CMangEVT::DelOneFlowFromUp(CEventBase* pdelevt)
{
	pdelevt->m_del=true;
	return 0;
}
int CMangEVT::GetModuleVersion()
{
    TSever_LOG_Error("VERSION   IS     STAE_CLOUD_V1.0.01.003");
	return 0;
}
int CMangEVT::DelOneEvt(CEventBase* pdelevt,bool streamerr)
{

    map<CEventBase*,CEventBase*>::iterator iter;
    iter = m_EvtMapAdded.find(pdelevt);
    if (iter != m_EvtMapAdded.end())
    {
        event_del(&(iter->first->m_ServerEvent));
        if(ACCEPT==iter->first->m_Type){
        	iter->first->CloseEvt();
        }
        delete iter->first;
        m_EvtMapAdded.erase(iter);
    }
    if(streamerr){
    	if(m_pEvtNotifyerr)
    	{
    		m_pEvtNotifyerr(pdelevt);
    	}
    }

    return 0;
}
static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
    CMangEVT *pCMangEVT = (CMangEVT *)arg;
    map<CEventBase*,CEventBase*>::iterator iter;
 /*   for (iter = pCMangEVT->m_EvtMapNew.begin(); iter != pCMangEVT->m_EvtMapNew.end(); ++iter){
        event_del(&(iter->first->m_ServerEvent));
    }*/
    for (iter = pCMangEVT->m_EvtMapAdded.begin(); iter != pCMangEVT->m_EvtMapAdded.end(); ++iter){
        event_del(&(iter->first->m_ServerEvent));

        delete iter->first;
        pCMangEVT->m_EvtMapAdded.erase(iter);
    }
    event_del(&(pCMangEVT->m_signal_int));

/*    printf("%s: got signal %d\n", __func__, EVENT_SIGNAL(signal));

    if (called >= 2)
        event_del(signal);

    called++;*/
//    exit(1);
}


/*
static void
timeout_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event *timeout = arg;
	TSever_LOG_Error("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

	return;
}
*/




int CMangEVT::DispatchEvt()
{

    event_assign(&m_signal_int, m_pBase, SIGINT, EV_SIGNAL|EV_PERSIST, signal_cb,this);
    event_add(&m_signal_int, NULL);
    TSever_LOG_Error("########################event start######################");
    /************************************/
/*    struct event timeout;
    struct timeval tv;
    event_assign(&timeout, m_pBase, -1, EV_PERSIST, timeout_cb, (void*) &timeout);

	evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(&timeout, &tv);*/
    /************************************/

    event_base_dispatch(m_pBase);
    TSever_LOG_Error("########################event down######################");
    event_base_free(m_pBase);
    return 0;
}

/*CEventBase* CMangEVT::GetIpcAccordSN(char* sn)
{
    map<CEventBase*, CEventBase*>::iterator iter;

    for (iter = m_EvtMapAdded.begin(); iter != m_EvtMapAdded.end(); ++iter)
    {
        if (0 == strcmp(iter->first->m_Sn, sn))
        {
            return iter->first;
        }
    }
    return NULL;
}*/
/**
 *
 * @param tmp
 * @return -1 should delete tmp outside
 */
int CMangEVT::CreatEvtStock(CEventBase* tmp)
{
    TSever_LOG_Error("coming");
/*    m_pEvtBase=NULL;
    m_pEvtBase=new CStockevt();*/
    if (tmp)
    {
        if (tmp->CreateEvt() < 0)
        {
            TSever_LOG_Error("create stock event error");
//            delete m_pEvtBase;
//            m_pEvtBase=NULL;
            return -1;
        }
        else
        {
            AssignOneEvt(tmp);
        }
    }
//    m_pEvtBase=NULL;
    return 0;
}

} /* namespace myselfstock */
