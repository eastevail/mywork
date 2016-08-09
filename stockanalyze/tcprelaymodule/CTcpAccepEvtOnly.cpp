/*
 * CTcpAccepEvtOnly.cpp
 *
 *  Created on: 2015年10月23日
 *      Author: lixiang
 */

#include "CTcpAccepEvtOnly.h"
#include "CSkyRtcpAnaly.h"
#include <event2/event.h>
#include "com_log.h"
#include "CIpcEvt.h"
#include "CAppEvt.h"
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

CTcpAccepEvtOnly::CTcpAccepEvtOnly(int acceptsockt):fTCPReadingState(AWAITING_DOLLAR),m_fNextTCPReadSize(0),m_SendTCPReadSize(0)
{
    // TODO Auto-generated constructor stub
	INIT_TS_LOG;
    m_ReadFd=acceptsockt;
    m_AlreadyNumFirst=0;
    m_DeviceTyp=0;
    m_Type=ACCEPT;
    m_Kbps=0;
    SetTimeOut(50);
    bzero(m_recvbuf,MAXLEN);
    m_AlreadyNum=0;
    SetEvtRorWorT();
    m_pSRtcpAna=new CSkyRtcpAnaly();
    m_pMangEVT=CMangEVT::GetInstance();

}

CTcpAccepEvtOnly::~CTcpAccepEvtOnly()
{
    // TODO Auto-generated destructor stub
    //CloseEvt();
//    m_pMangEVT->DelRtpQue(m_Sn);
    delete m_pSRtcpAna;
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
            switch (pCStock->m_DeviceTyp)
			{
			case 0x01:
			{
				TSever_LOG_Error("coming device app app");
				CEventBase* tmp = NULL;
				tmp = new CAppEvt(pCStock->m_ReadFd, pCStock->m_Sn);
				tmp->Init();
				pCStock->m_pMangEVT->ChangToAppEvt((CEventBase*) arg, tmp);
				break;
			}
			case 0x02:
			{
				TSever_LOG_Error("coming device ipc ipc");
				CEventBase* tmp = NULL;
				tmp = new CIpcEvt(pCStock->m_ReadFd, pCStock->m_Sn, 0);
				tmp->Init();
				pCStock->m_pMangEVT->ChangToIpcEvt((CEventBase*) arg, tmp);
				TSever_LOG_Error();
				break;
			}
			default:
//				TSever_LOG_Error("coming device donot know");
				break;
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

    return 0;
}
int CTcpAccepEvtOnly::CreateEvt()
{
	TSever_LOG_Error("CTcpAccepEvtOnly evt coming");
	return 0;
}
int CTcpAccepEvtOnly::GetDeviceInfo()
{
	m_DeviceTyp=m_recvfirst[0];
    if((0x01!=m_DeviceTyp)&&(0x02!=m_DeviceTyp)){
        TSever_LOG_Error("recv m_DeviceTyp error");
        return -1;
    }

    strncpy(m_Sn,m_recvfirst+1,sizeof(m_Sn));
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
	if (fTCPReadingState != AWAITING_PACKET_DATA)
	{
		num = recv(m_ReadFd, &c, 1, 0);
		if (0 == num)
		{
			TSever_LOG_Error("recv num =00000000000000000000000000000000000");
			return -2;
		}
		else if (num < 0)
		{
			TSever_LOG_Error("recv fTCPReadingState %d error:%s", fTCPReadingState, strerror(errno));
			return -1;
		}
	}
	switch (fTCPReadingState)
	{
	case AWAITING_DOLLAR:
	{
		if (c == '$')
		{
			fTCPReadingState = AWAITING_STREAM_CHANNEL_ID;
			m_recvbuf[0] = c;
		}
		break;
	}
	case AWAITING_STREAM_CHANNEL_ID:
	{
		m_fStreamChannelId = c;
		//        TSever_LOG_Error("m_fStreamChannelId=%d",m_fStreamChannelId);
		fTCPReadingState = AWAITING_SIZE1;
		m_recvbuf[1] = c;
		break;
	}
	case AWAITING_SIZE1:
	{
		// The byte that we read is the first (high) byte of the 16-bit RTP or RTCP packet 'size'.
		m_fSizeByte1 = c;
		m_recvbuf[2] = c;
		fTCPReadingState = AWAITING_SIZE2;
		break;
	}
	case AWAITING_SIZE2:
	{
		// The byte that we read is the second (low) byte of the 16-bit RTP or RTCP packet 'size'.
		unsigned short size = (m_fSizeByte1 << 8) | c;
		m_fNextTCPReadSize = size;
		m_recvbuf[3] = c;
		fTCPReadingState = AWAITING_PACKET_DATA;
		break;
	}
	case AWAITING_PACKET_DATA:
	{
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
			m_SendTCPReadSize = m_fNextTCPReadSize + 4;
			switch (m_fStreamChannelId)
			{
			case 0x00: //rtp
//				SendToRtpQue(m_SendTCPReadSize, m_recvbuf);
				TSever_LOG_Error("unsupported type");
				break;
			case 0x01: //rtcp
			{
				SaveRctpToFile(m_recvbuf, m_SendTCPReadSize);
				m_pSRtcpAna->ParseRtcpPackSdes(m_recvbuf, m_SendTCPReadSize, this);
				RTCP_INFO* tmp = m_pSRtcpAna->GetRtcpData();
				if (tmp)
				{
					int ret = 0;
					if ((ret = write(m_ReadFd, tmp->rtcpdata, tmp->rtcplen)) == -1)
					{
						TSever_LOG_Error("sento() error:%s", strerror(errno));
						return -2;
					}
					TSever_LOG_Error("send ret=%d,tmp->rtcplen=%d",ret,tmp->rtcplen);
				}
				TSever_LOG_Error();
				break;
			}
			default:
				TSever_LOG_Error("error streamchannel tpye");
				break;
			}

/*					int ret = sendto(m_UdpCliSckt, m_recvbuf+4, m_fNextTCPReadSize, 0, (struct sockaddr *) &m_WzaIp,sizeof(struct sockaddr_in));
			 if (ret < 0)
			 {
			 TSever_LOG_Error("sento() error:%s", strerror(errno));
			 }*/
			m_AlreadyNum = 0;
			fTCPReadingState = AWAITING_DOLLAR;
			m_SendTCPReadSize = 0;
			bzero(m_recvbuf, sizeof(m_recvbuf));
		}
		break;
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
