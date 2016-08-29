/*
 * CTcpAccepEvtOnly.cpp
 *
 *  Created on: 2015年10月23日
 *      Author: lixiang
 */

#include "CTcpAccepEvtOnly.h"
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
#include "funcProEvt.h"
namespace myselfstock
{

CTcpAccepEvtOnly::CTcpAccepEvtOnly(int acceptsockt):fTCPReadingState(AWAITING_HEAD)
{
    // TODO Auto-generated constructor stub
	INIT_TS_LOG;
    m_ReadFd=acceptsockt;
    m_DeviceTyp=FUNC_PRO_TOTAL;
    m_Type=ACCEPT;
    m_Kbps=0;
    SetTimeOut(50);
    bzero(&m_recvbuf,sizeof(m_recvbuf));
    m_AlreadyNum=0;
    SetEvtRorWorT();
    m_pMangEVT=CMangEVT::GetInstance();

}

CTcpAccepEvtOnly::~CTcpAccepEvtOnly()
{
    // TODO Auto-generated destructor stub
    //CloseEvt();
//    m_pMangEVT->DelRtpQue(m_Sn);
//    CMangEVT::Destroy();
    CLOSE_TS_LOG;
}
void CTcpAccepEvtOnly::CallbackAcept(evutil_socket_t fd, short event, void *arg)
{
    CTcpAccepEvtOnly* pCStock=(CTcpAccepEvtOnly*)arg;
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
        else
        {
        	if(FUNC_PRO_TOTAL!=pCStock->m_DeviceTyp){
				TSever_LOG_Error("coming process %d",pCStock->m_DeviceTyp);
				CEventBase* tmp = NULL;
				tmp = new funcProEvt(pCStock->m_ReadFd, pCStock->m_DeviceTyp);
				tmp->Init();
				pCStock->m_pMangEVT->ChangToAppEvt((CEventBase*) arg, tmp);
        	}
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
		TSever_LOG_Error("accept error,accept maybe offline");
		pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg);
        break;
    default:
        break;
    }
    return;
}
int CTcpAccepEvtOnly::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ;
	return 0;
}
int CTcpAccepEvtOnly::Init()
{

    evutil_make_socket_nonblocking(m_ReadFd);
    headlen=sizeof(m_recvbuf);

    return 0;
}
int CTcpAccepEvtOnly::CreateEvt()
{
	TSever_LOG_Error("CTcpAccepEvtOnly evt coming");
	return 0;
}
int CTcpAccepEvtOnly::GetDeviceInfo()
{
    return 0;
}
int CTcpAccepEvtOnly::CloseEvt()
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
/*int CTcpAccepEvtOnly::SetTimeOut(int second)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = second;
    return 0;
}*/
int CTcpAccepEvtOnly::GetType()
{
    return m_Type;
}
static unsigned long long ntohll(unsigned long long val)
{
         return (((unsigned long long )ntohl((int)((val << 32) >> 32))) << 32) | (unsigned int)ntohl((int)(val >> 32));
}



event_callback_fn CTcpAccepEvtOnly::GetCallbackAddr()
{
    return CallbackAcept;
}




int CTcpAccepEvtOnly::WriteData()
{

	return 0;
}

int CTcpAccepEvtOnly::ReadData()
{
	int num;
	char* tmp = NULL;
	u_int8_t c;
	struct timeval diff;

//	 if (FIRSTREAD == fTCPReadingState)
//	 {
//	TSever_LOG_Error();
/*	num = recv(m_ReadFd, m_recvfirst + m_AlreadyNumFirst, FIRSTRDLEN - m_AlreadyNumFirst, 0);
	if (0 == num)
	{
		TSever_LOG_Error("recv num =00000000000000000000000000000000000,FIRSTRDLEN - m_AlreadyNumFirst=%d",
				FIRSTRDLEN - m_AlreadyNumFirst);
		return -2;
	}
	else if (num < 0)
	{
		TSever_LOG_Error("recv fTCPReadingState %d error:%s", FIRSTREAD, strerror(errno));
		return -1;
	}
	m_AlreadyNumFirst = m_AlreadyNumFirst + num;
	//    TSever_LOG_Error("revalread=%d num=%d", m_AlreadyNum, num);
	if (FIRSTRDLEN == m_AlreadyNumFirst)
	{
		if (GetDeviceInfo() < 0)
		{
			return -2;
		}
		int ret = send(m_ReadFd, "ok", 2, 0);
		if (ret < 0)
		{
			TSever_LOG_Error("sento(ok) error:%s", strerror(errno));
			return -2;
		}
		m_AlreadyNumFirst = 0;
		fTCPReadingState = AWAITING_DOLLAR;
	}*/

//	 }

	switch (fTCPReadingState)
	{
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
			fTCPReadingState = AWAITING_PACKET_DATA;
			m_recvbuf.srcPro=ntohl(m_recvbuf.srcPro);
			m_DeviceTyp=m_recvbuf.srcPro;
		}
		break;
	}
	case AWAITING_PACKET_DATA: {/*
	 num = recv(m_ReadFd, m_recvbuf + 4 + m_AlreadyNum, m_fNextTCPReadSize - m_AlreadyNum, 0);
	 TSever_LOG_Error("num=%d,m_fNextTCPReadSize=%d",num,m_fNextTCPReadSize);
	 if (0 == num)
	 {
	 TSever_LOG_Error("recv num =00000000000000000000000000000000000,m_fNextTCPReadSize - m_AlreadyNum=%d",
	 m_fNextTCPReadSize - m_AlreadyNum);
	 return -2;
	 }
	 else if (num < 0)
	 {
	 TSever_LOG_Error("recv fTCPReadingState %d error:%s", AWAITING_PACKET_DATA, strerror(errno));
	 return -1;
	 }
	 m_AlreadyNum = m_AlreadyNum + num;
	 //    TSever_LOG_Error("revalread=%d num=%d", m_AlreadyNum, num);
	 if (m_fNextTCPReadSize == m_AlreadyNum)
	 {

	 }
	 break;
	 */
	}
	}
//	TSever_LOG_Error();
	return 0;
}

int CTcpAccepEvtOnly::WritedData()
{
	return 0;
}
} /* namespace myselfstock */
