/*
 * CIpcEvt.cpp
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#include "CIpcEvt.h"
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
using nedalloc::nedcalloc;
using nedalloc::nedfree;
namespace myselfstock
{

CIpcEvt::CIpcEvt (int acceptsockt ,char* sn,int Kbps):fTCPReadingState(AWAITING_DOLLAR),m_fNextTCPReadSize(0),m_SendTCPReadSize(0),m_GetRTPdataErrNum(0)
{
	// TODO Auto-generated constructor stub
	INIT_TS_LOG;
    m_ReadFd=acceptsockt;
    strncpy(m_Sn,sn,sizeof(m_Sn));
    m_conut=0;
    SetTimeOut(5);
    m_pRtpInfo=NULL;
    m_pRtpQue=NULL;
    m_Kbps=Kbps;
    evutil_timerclear(&m_Last);
    evutil_timerclear(&m_Now);
    evutil_timerclear(&m_OneSc);
    m_ReadNumOneSc=0;
    m_AlreadyNum=0;
    m_IsLastTimeWr=false;
    m_OneSc.tv_sec = 1;
    m_pRtcp=NULL;
    SetEvtRorWorT();
    m_pMangEVT=CMangEVT::GetInstance();
    m_pSRtcpAna=new CSkyRtcpAnaly();

}

CIpcEvt::~CIpcEvt ( )
{
	// TODO Auto-generated destructor stub
	CloseEvt();
    m_pMangEVT->DelRtpQue(m_Sn);
    TSever_LOG_Error();
    delete m_pSRtcpAna;
    TSever_LOG_Error();
//    CMangEVT::Destroy();
    CLOSE_TS_LOG;
}
int CIpcEvt::Init ( )
{
    m_pRtpQue=m_pMangEVT->GetInstanceRtpQue(m_Sn);
    if(!m_pRtpQue){
    	return -1;
    }
    return 0;
}
static unsigned long long ntohll(unsigned long long val)
{
         return (((unsigned long long )ntohl((int)((val << 32) >> 32))) << 32) | (unsigned int)ntohl((int)(val >> 32));
}
int CIpcEvt::GetType()
{
    return m_Type;
}
int CIpcEvt::CreateEvt ( )
{
	if(m_pMangEVT->m_pEvtNotify)
	{
		m_pMangEVT->m_pEvtNotify(this);
	}
	TSever_LOG_Error("CIpcEvt evt coming,sn=%s", m_Sn);
	return 0;

}
int CIpcEvt::GetRtpInfo(uint8_t * prtpdata,uint64_t &rtpseq,bool &isIframe)
{
//	rtpseq = prtpdata[2] << 8 | prtpdata[3];
//	rtpseq=prtpdata[16]<<56|prtpdata[17]<<48|prtpdata[18]<<40|prtpdata[19]<<32|prtpdata[20]<<24|prtpdata[21]<<16|prtpdata[22]<<8|prtpdata[23];
	uint64_t tmp;
	memcpy(&tmp, prtpdata + 16, 8);
	rtpseq = ntohll(tmp);
//	TSever_LOG_Error("rtpseq=%ulld 0x%x",rtpseq,prtpdata[1]);
//	if (prtpdata[1] & 0x80)//old
	if (prtpdata[24] & 0x01) //new
	{
//		TSever_LOG_Error("I frame come rtp num=%ulld",rtpseq);
		isIframe = true;
	}
	else
	{
		isIframe = false;
	}
	return 0;
}
void CIpcEvt::CallbackAcept(evutil_socket_t fd, short event, void *arg)
{
    CIpcEvt* pCStock=(CIpcEvt*)arg;
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

		TSever_LOG_Error("stream error,ipc offline maybe");
		pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg);

//        TSever_LOG_Error();
        //        event_del(&ServerEvent);
 //       close(fd);
        break;
    default:
        break;
    }
    return;
}
int CIpcEvt::WriteData()
{
	return SendToIpc(); //所有数据send完后改变成可读与超时事件
}

int CIpcEvt::SendToRtpQue(unsigned short len,uint8_t* rtpdata)
{
	uint64_t rtpseq; //extension head 8 Bytes
	bool isIframe;
	GetRtpInfo(rtpdata + 4, rtpseq, isIframe);
	if (isIframe)
	{
		m_conut++;
	}
	if (m_conut == 1)
	{
		SaveRtpToFile(rtpdata, len, rtpseq, isIframe);
	}
	if (m_pRtpQue)
	{
//		TSever_LOG_Error("rtpseq=%lld len=%d isIframe=%d",rtpseq,len,isIframe);
		m_pRtpQue->ApendRtpData(rtpseq, rtpdata, len, isIframe);
//		TSever_LOG_Error();
	}
	else
	{
		TSever_LOG_Error("can not get m_pRtpQue");
	}
	return 0;
}
int CIpcEvt::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ;
	return 0;
}
int CIpcEvt::ReadData()
{
	int num;
	char* tmp = NULL;
	u_int8_t c;
	struct timeval diff;
	if (!m_IsLastTimeWr)
	{
		evutil_gettimeofday(&m_Last, NULL);
		m_IsLastTimeWr = true;
	}

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
				SendToRtpQue(m_SendTCPReadSize, m_recvbuf);
				break;
			case 0x01: //rtcp
				m_pSRtcpAna->ParseRtcpPackApp(m_recvbuf, m_SendTCPReadSize);
				SaveRctpToFile(m_recvbuf, m_SendTCPReadSize);
				TSever_LOG_Error();
				break;
			default:
				TSever_LOG_Error("error streamchannel tpye");
				break;
			}

			/*			int ret = sendto(m_UdpCliSckt, m_recvbuf+4, m_fNextTCPReadSize, 0, (struct sockaddr *) &m_WzaIp,sizeof(struct sockaddr_in));
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
	m_ReadNumOneSc = m_ReadNumOneSc + num;
	evutil_gettimeofday(&m_Now, NULL);
	evutil_timersub(&m_Now, &m_Last, &diff);
	if (evutil_timercmp(&diff, &m_OneSc, >=))
	{
		TSever_LOG_Error(".......................%d kbit/s,m_ReadNumOneSc=%dB,m_AlreadyNum=%d",
				m_ReadNumOneSc * 8 / 1024, m_ReadNumOneSc, m_AlreadyNum);
		m_ReadNumOneSc = 0;
		m_IsLastTimeWr = false;
	}

	return 0;
}
event_callback_fn CIpcEvt::GetCallbackAddr()
{
    return CallbackAcept;
}
int CIpcEvt::SendToIpcOnly()
{
	int ret = 0;
	if (m_pRtcp)
	{
		if ((ret = write(m_ReadFd, m_pRtcp->rtcpdata + m_pRtcp->curpos, m_pRtcp->rtcplen - m_pRtcp->curpos)) == -1)
		{
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
			{
				TSever_LOG_Error("sento() error:%s", strerror(errno));
				return -1;
			}
			else if (errno == EBADF)
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
			m_pRtcp->curpos = m_pRtcp->curpos + ret;
		}
		if (m_pRtcp->curpos == m_pRtcp->rtcplen)
		{
			nedfree((void*)m_pRtcp);
			m_pRtcp = NULL;
		}
		return 0;
	}
	return -1;
}
int CIpcEvt::SendToIpc()
{

	if (!m_pRtcp)
	{
		m_pRtcp = m_pSRtcpAna->GetRtcpData();
		if (!m_pRtcp)
		{
			struct timeval tmp;
			evutil_timerclear(&tmp);
			tmp.tv_sec = 5;
			m_pMangEVT->ChangToTimeOutReadEvt((CEventBase*) this, &tmp);
			return -1;
		}
		else
		{
			return SendToIpcOnly();
		}
	}
	else
	{
		return SendToIpcOnly();
	}
	return 0;
}
int CIpcEvt::CloseEvt()
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
