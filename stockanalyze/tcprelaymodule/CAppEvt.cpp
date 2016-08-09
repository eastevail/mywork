/*
 * CAppEvt.cpp
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#include "CAppEvt.h"
#include <event2/event.h>
#include "com_log.h"
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
#include "nedmalloc.h"
namespace myselfstock
{

CAppEvt::CAppEvt ( int acceptsockt,char* sn):m_GetRTPdataErrNum(0)
{
	// TODO Auto-generated constructor stub
	INIT_TS_LOG;
    m_ReadFd=acceptsockt;
    strncpy(m_Sn,sn,sizeof(m_Sn));
    SetTimeOut(5);
    m_RtpSeq=ERRORLENTH;
    m_pRtpInfo=NULL;
    m_RtpAlreadySend=0;
    m_pRtpQue=NULL;
    SetEvtRorWorT();
    m_pMangEVT=CMangEVT::GetInstance();
}

CAppEvt::~CAppEvt ( )
{
	// TODO Auto-generated destructor stub
	CloseEvt();
    m_pMangEVT->DelRtpQue(m_Sn);
//    CMangEVT::Destroy();
    CLOSE_TS_LOG;
}
int CAppEvt::CreateEvt()
{

	TSever_LOG_Error("CAppEvt evt coming,sn=%s",m_Sn);
	return 0;
}
int CAppEvt::Init ( )
{
    m_pRtpQue=m_pMangEVT->GetInstanceRtpQue(m_Sn);
    if(!m_pRtpQue){
    	return -1;
    }
    return 0;
}
int CAppEvt::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ|EV_WRITE;
	return 0;
}
int CAppEvt::ReadData ( )
{
	return 0;
}
int CAppEvt::SendToApp()
{
	if (!m_pRtpInfo)
	{

		m_pRtpInfo = m_pRtpQue->GetRtpData(m_RtpSeq);
		if (!m_pRtpInfo)
		{
			TSever_LOG_Error("GetRtpData error");
			m_GetRTPdataErrNum++;
			if (GETRTPDATAERRNUMMAX < m_GetRTPdataErrNum)
			{
				return -2;
//				m_pMangEVT->DelOneEvt((CEventBase*) this);//失败10次表明没有流上来了
			}
			else
			{
				struct timeval tmp;
				evutil_timerclear(&tmp);
				tmp.tv_usec = 1000 * 5;
				m_pMangEVT->ChangToTimeOutReadEvt((CEventBase*) this, &tmp);
			}
			return -1;
		}
		else
		{
			m_GetRTPdataErrNum = 0;
			return SendToAppOnly();
		}
	}
	else
	{
		return SendToAppOnly();
	}
}
int CAppEvt::SendToAppOnly()
{
	int ret;
	if (m_pRtpInfo)
	{
		if (m_firstnotify)
		{
			if (m_pMangEVT->m_pEvtNotify)
			{
				m_pMangEVT->m_pEvtNotify(this);
				m_firstnotify = false;
			}
		}
		m_pMangEVT->ChangToWriteReadEvt((CEventBase*) this);
        if ((ret = write(m_ReadFd, m_pRtpInfo->rtpdata + m_RtpAlreadySend, m_pRtpInfo->rtplen-m_RtpAlreadySend)) == -1)
        {
            if((errno==EAGAIN)||(errno==EWOULDBLOCK))
            {
                TSever_LOG_Error("sento() error:%s", strerror(errno));
                return -1;
            }
            else if(errno==EBADF)
            {
                TSever_LOG_Error("sento() error:%s", strerror(errno));
                return -2;
            }
            else
            {
                TSever_LOG_Error("sento() error:%s", strerror(errno));
                return -2;
            }
		}
		else
		{
			m_RtpAlreadySend = m_RtpAlreadySend + ret;
		}
        if(m_RtpAlreadySend==m_pRtpInfo->rtplen){
        	m_RtpAlreadySend=0;
        	m_pRtpInfo=NULL;
        }
	}
	return 0;
}
int CAppEvt::WriteData()
{
	return SendToApp(); //判断返回值
}
event_callback_fn CAppEvt::GetCallbackAddr()
{
    return CallbackAcept;
}
int CAppEvt::GetType()
{
    return m_Type;
}
void CAppEvt::CallbackAcept(evutil_socket_t fd, short event, void *arg)
{
	CAppEvt* pCStock=(CAppEvt*)arg;
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
int CAppEvt::CloseEvt()
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
