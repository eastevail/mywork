/*
 * funcProEvt.cpp
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#include "funcProEvt.h"

#include <event2/event.h>
extern "C"{
#include "com_log.h"
}
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace myselfstock
{

funcProEvt::funcProEvt ( int acceptsockt,FUNC_PRO_TYPE type):fTCPReadingState(AWAITING_HEAD)
{
	// TODO Auto-generated constructor stub
	INIT_TS_LOG;
    m_ReadFd=acceptsockt;
    struct timeval tim;
    tim.tv_sec=0;
    tim.tv_usec=5000;
    SetTimeOut(&tim);
    SetEvtRorWorT();
    m_pMangEVT=CMangEVT::GetInstance();
    m_DeviceTyp=type;
    pool=NULL;
    sendkey=0;
}

int funcProEvt::SetTimeOut ( struct timeval* ptime)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = ptime->tv_sec;
    m_TimeOut.tv_usec = ptime->tv_usec;
    return 0;
}
funcProEvt::~funcProEvt ( )
{
	// TODO Auto-generated destructor stub
	CloseEvt();
    ngx_destroy_pool(pool);
    m_pMangEVT->m_proEvt[m_DeviceTyp]=NULL;
//    CMangEVT::Destroy();
    CLOSE_TS_LOG;
}
int funcProEvt::CreateEvt()
{

	TSever_LOG_Error("funcProEvt evt coming,sn=%s",m_Sn);
	return 0;
}
int funcProEvt::Init ( )
{
    pool = ngx_create_pool(1024*2, NULL);
    m_pMangEVT->m_proEvt[m_DeviceTyp]=this;
    headlen=sizeof(m_recvbuf);
    return 0;
}
int funcProEvt::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ;
	return 0;
}

int funcProEvt::ReadOtherProComeData(const MsgCommuni* pmsg,int otherfd)
{
	int num;
	static MsgCommuni* tmpmsg=NULL;
	static int alreadyReadNum=0;
	if(!tmpmsg){
		tmpmsg=(MsgCommuni*)ngx_pcalloc(pool,sizeof(MsgCommuni));
		tmpmsg->msg=(char*)ngx_pcalloc(pool,pmsg->msglen);
		memcpy(tmpmsg,pmsg,sizeof(MsgCommuni)-4);
	}
	if (pmsg->msglen) {
		num = recv(otherfd, tmpmsg->msg, tmpmsg->msglen - alreadyReadNum, 0);
		if (0 == num) {
			return -2;
		} else if (num < 0) {
			DBG("recv fTCPReadingState %d error:%s", AWAITING_PACKET_DATA, strerror(errno));
			return -1;
		}
		alreadyReadNum = alreadyReadNum + num;
	}
	if( tmpmsg->msglen == alreadyReadNum){
		alreadyReadNum=0;
		m_sendQue.insert(pair<int, MsgCommuni*>(sendkey++, tmpmsg));
		tmpmsg=NULL;
		return 1;
	}
	return 0;
}

int funcProEvt::ReadData ( )
{
	int num;

	switch (fTCPReadingState) {
	case AWAITING_HEAD: {
		num = recv(m_ReadFd, (char*)&m_recvbuf + m_AlreadyNum, headlen - m_AlreadyNum, 0);
		if (0 == num) {
			return -2;
		} else if (num < 0) {
			DBG("recv fTCPReadingState %d error:%s", AWAITING_HEAD, strerror(errno));
			return -1;
		}
		m_AlreadyNum = m_AlreadyNum + num;
		//    TSever_LOG_Error("revalread=%d num=%d", m_AlreadyNum, num);
		if (headlen == m_AlreadyNum) {
			m_AlreadyNum = 0;
			m_recvbuf.srcPro = ntohl(m_recvbuf.srcPro);
			m_recvbuf.destPro = ntohl(m_recvbuf.destPro);
			if(m_DeviceTyp != m_recvbuf.srcPro){
				DBG("process %d send error msg src process is %d",m_DeviceTyp,m_recvbuf.srcPro);
			}else if(m_recvbuf.destPro>=FUNC_PRO_TOTAL){
				DBG("process %d send error msg dest process is %d",m_DeviceTyp,m_recvbuf.destPro);
			}else if(!m_pMangEVT->m_proEvt[m_recvbuf.destPro]){
				DBG("dest process %d do not run",m_recvbuf.destPro);
			}else{
				fTCPReadingState = AWAITING_PACKET_DATA;

				m_recvbuf.msgType = ntohl(m_recvbuf.msgType);
				m_recvbuf.msglen = ntohl(m_recvbuf.msglen);
				m_recvbuf.magic = ntohl(m_recvbuf.magic);
			}
		}
		break;
	}
	case AWAITING_PACKET_DATA: {
		if (1== (m_pMangEVT->m_proEvt[m_recvbuf.destPro]->ReadOtherProComeData(&m_recvbuf,m_ReadFd))) {
			fTCPReadingState = AWAITING_HEAD;
		}
		break;
	}

	}
	return 0;
}

int funcProEvt::WriteData()
{
	static MsgCommuni* msgsend = NULL;
	static int alreadySendhead = 0, alreadySenddata = 0;
	map<int, MsgCommuni*>::iterator iter;
	int ret;
	if (!msgsend) {
		//first
		if (!m_sendQue.size()) {
			iter = m_sendQue.begin();
			msgsend = iter->second;
			msgsend->destPro = htonl(msgsend->destPro);
			msgsend->srcPro = htonl(msgsend->srcPro);
			msgsend->msgType = htonl(msgsend->msgType);
			msgsend->msglen = htonl(msgsend->msglen);
			msgsend->magic = htonl(msgsend->magic);
		}
	} else {
		if (alreadySendhead != headlen) {
			if ((ret = write(m_ReadFd, (char*) msgsend + alreadySendhead, headlen - alreadySendhead))
					== -1) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					DBG("sento() error:%s", strerror(errno));
					return -1;
				} else if (errno == EBADF) {
					DBG("sento() error:%s", strerror(errno));
					return -2;
				} else {
					DBG("sento() error:%s", strerror(errno));
					return -2;
				}
			} else {
				alreadySendhead = alreadySendhead + ret;
			}
		} else {
			int datalen = ntohl(msgsend->msglen);
			//send data
			if (!datalen) {
				if ((ret = write(m_ReadFd, (char*) msgsend->msg + alreadySenddata,
						datalen - alreadySenddata)) == -1) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
						DBG("sento() error:%s", strerror(errno));
						return -1;
					} else if (errno == EBADF) {
						DBG("sento() error:%s", strerror(errno));
						return -2;
					} else {
						DBG("sento() error:%s", strerror(errno));
						return -2;
					}
				} else {
					alreadySenddata = alreadySenddata + ret;
				}
				if (alreadySenddata == datalen) {
					alreadySendhead = 0;
					alreadySenddata = 0;
					ngx_pfree(pool, msgsend->msg);
					ngx_pfree(pool, msgsend);
					m_sendQue.erase(iter);
					msgsend = NULL;
				}
			} else {
				alreadySendhead = 0;
				alreadySenddata = 0;
				ngx_pfree(pool, msgsend->msg);
				ngx_pfree(pool, msgsend);
				m_sendQue.erase(iter);
				msgsend = NULL;
			}
		}
	}
	return 0;
}
event_callback_fn funcProEvt::GetCallbackAddr()
{
    return CallbackAcept;
}
int funcProEvt::GetType()
{
    return m_Type;
}
void funcProEvt::CallbackAcept(evutil_socket_t fd, short event, void *arg)
{
	funcProEvt* pCStock=(funcProEvt*)arg;
	if(pCStock->m_del){
		pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg,false);
		return;
	}
    int ret;
    switch (event)
    {
    case EV_READ:
        ret=pCStock->ReadData();
        if (-2 == ret)
        {
            pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg);
        }
        break;
	case EV_WRITE:
		ret = pCStock->WriteData();
		if (-2 == ret)
		{
			pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg);
		}
		break;
    case EV_TIMEOUT:
		ret = pCStock->WriteData();
		if (-2 == ret)
		{
			pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg);
		}
//        TSever_LOG_Error();
//        event_del(&ServerEvent);
//       close(fd);
        break;
    default:
        break;
    }
    return;
}
int funcProEvt::CloseEvt()
{
    if (0 != m_ReadFd)
    {
        if (evutil_closesocket(m_ReadFd) < 0)
        {
            TSever_LOG_Error("CloseEvt() m_ReadFd error:%s", strerror(errno));
            return -1;
        }
    }

    if (0 != m_SendToApp)
    {
        if (evutil_closesocket(m_SendToApp) < 0)
        {
            TSever_LOG_Error("CloseEvt() m_SendToApp error:%s", strerror(errno));
            return -1;
        }
    }

    TSever_LOG_Error();
    return 0;
}
} /* namespace myselfstock */
