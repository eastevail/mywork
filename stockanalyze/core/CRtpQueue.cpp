/*
 * CRtpQueue.cpp
 *
 *  Created on: 2015年10月30日
 *      Author: lixiang
 */

#include "CRtpQueue.h"
#include "nedmalloc.h"
#include <stdlib.h>
#include <event2/event.h>
#include <assert.h>
#include <string.h>
using nedalloc::nedcalloc;
using nedalloc::nedfree;
namespace myselfstock
{

CRtpQueue::CRtpQueue(char* sn)
{
    // TODO Auto-generated constructor stub
	strncpy(m_Sn,sn,SNSIZE);
    INIT_TS_LOG;
    m_curRtpSeq=ERRORLENTH;
    evutil_timerclear(&m_Last);
    evutil_timerclear(&m_Now);
    evutil_timerclear(&m_OneSc);
    m_OneSc.tv_sec = 3;
    m_ReferCount=0;
    m_IsLastTimeWr=false;
    m_FirstApend=true;
    m_FirstGet=true;

}

CRtpQueue::~CRtpQueue()
{
    // TODO Auto-generated destructor stub
	map<uint64_t, RTP_INFO*>::iterator iterData;
	for (iterData = m_RtpQue.begin(); iterData != m_RtpQue.end(); ++iterData)
	{
			nedfree((void*)(iterData->second));
			m_RtpQue.erase(iterData);

	}
	m_RtpQue.clear();
	CLOSE_TS_LOG;
}
int CRtpQueue::DelOneSecData()
{
	//TSever_LOG_Error();
	map<uint64_t, uint64_t>::iterator iter;
	uint64_t firsRtpSeq = ERRORLENTH , secondRtpSeq = ERRORLENTH,thirdRtpSeq=ERRORLENTH;
	int i = 0;
	int ret=0;
	for (iter = m_IframeRtpSeq.begin(); iter != m_IframeRtpSeq.end(); ++iter)
	{
		if (0 == i)
		{
			firsRtpSeq = iter->first;
		}
		else if (1 == i)
		{
			secondRtpSeq = iter->first;
		}
		else if(2==i)
		{
			thirdRtpSeq=iter->first;
		}
		else
		{
			break;
		}
		i++;
	}
	if ((ERRORLENTH == secondRtpSeq)||(ERRORLENTH == thirdRtpSeq))
	{
		TSever_LOG_Error("have only one second data");
		return -1;
	}
	else
	{
		iter = m_IframeRtpSeq.begin();
		m_IframeRtpSeq.erase(iter);
		map<uint64_t, RTP_INFO*>::iterator iterData;
		for (iterData = m_RtpQue.begin(); iterData != m_RtpQue.end(); ++iterData)
		{
			if (secondRtpSeq == iterData->first)
			{
				break;
			}
			else
			{
				nedfree((void*)(iterData->second));
				m_RtpQue.erase(iterData);
			}
		}
		m_curRtpSeq=thirdRtpSeq;
//		TSever_LOG_Error("m_curRtpSeq=%d thirdRtpSeq=%d",m_curRtpSeq,thirdRtpSeq);
/*		for (iter = m_IframeRtpSeq.begin(); iter != m_IframeRtpSeq.end(); ++iter)
		{
			TSever_LOG_Error("iter->first=%lld,m_curRtpSeq=%lld",iter->first,m_curRtpSeq);
		}
		TSever_LOG_Error("down!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
	}
	return 0;
}
int CRtpQueue::RemoveRtpEXHeadAndCopy(RTP_INFO* dest,unsigned char* rtpdata,uint32_t rtplen)
{
	dest->rtplen=rtplen-20;
	memcpy(dest->rtpdata, rtpdata, 16);
	memcpy(dest->rtpdata+16, rtpdata+36, rtplen-36);
	dest->rtpdata[4]&=0xef;
	return 0;
}
int CRtpQueue::ApendRtpData(uint64_t rtpseq,unsigned char* rtpdata,uint32_t rtplen,bool isIframe)
{
    struct timeval diff;
//    RTP_INFO* tmp=NULL;
	RTP_INFO* tmp=(RTP_INFO*)nedcalloc(1,sizeof(RTP_INFO));
//	RTP_INFO* tmp=(RTP_INFO*)malloc(sizeof(RTP_INFO));
	if (tmp)
	{
		if (rtplen <= MAXMTU)
		{
//			TSever_LOG_Error("rtplen=%d",rtplen);
			//old
//			memcpy(tmp->rtpdata, rtpdata, rtplen);
//			tmp->rtplen = rtplen;
			//down
			//now
			RemoveRtpEXHeadAndCopy(tmp, rtpdata, rtplen);
			//down
	//		TSever_LOG_Error("rtpseq=%llu",rtpseq);
			m_RtpQue.insert(pair<uint64_t, RTP_INFO*>((uint64_t)rtpseq, tmp));
		}
		else
		{
			TSever_LOG_Error("rtplen error > MAXMTU,rtplen=%d",rtplen);
			return -1;
		}

//		TSever_LOG_Error();
	}
	else
	{
		TSever_LOG_Error("ngx_pcalloc ApendRtpData error");
		return -1;
	}
	if(isIframe){
		if(m_FirstApend){
			m_curRtpSeq=rtpseq;
			TSever_LOG_Error("m_curRtpSeq=%llu rtpseq=%llu",m_curRtpSeq,rtpseq);
			TSever_LOG_Error("first add rtpseq=%llu************************************************************",rtpseq);
			m_FirstApend=false;
		}
		m_IframeRtpSeq.insert(pair<uint64_t,uint64_t>(rtpseq, rtpseq));
	}
    if (!m_IsLastTimeWr)
    {
        evutil_gettimeofday(&m_Last, NULL);
        m_IsLastTimeWr=true;
    }
    evutil_gettimeofday(&m_Now, NULL);
    evutil_timersub(&m_Now,&m_Last,&diff);
    if (evutil_timercmp(&diff, &m_OneSc, >=))
    {
    	DelOneSecData();
        m_IsLastTimeWr=false;
        m_OneSc.tv_sec = 1;
    }
//    TSever_LOG_Error();
	return 0;
}
RTP_INFO* CRtpQueue::GetRtpData(uint64_t &RtpSeq)
{
	map<uint64_t, RTP_INFO*>::iterator iterData;
//	TSever_LOG_Error("RtpSeq=%llu,m_curRtpSeq=%llu",RtpSeq,m_curRtpSeq);
	if (ERRORLENTH == RtpSeq)
	{
//		TSever_LOG_Error();
		iterData = m_RtpQue.find(m_curRtpSeq);
		if (iterData != m_RtpQue.end())
		{
			if(m_FirstGet){
				TSever_LOG_Error("first get rtpseq=%llu****************************************************",m_curRtpSeq);
				m_FirstGet=false;
			}
			RtpSeq = m_curRtpSeq + 1;
			return iterData->second;
		}
		else
		{
//			TSever_LOG_Error("m_RtpQue.find(m_curRtpSeq); error");
			return NULL;
		}
	}
	else
	{
		iterData = m_RtpQue.find(RtpSeq);
		if (iterData != m_RtpQue.end())
		{
			RtpSeq=RtpSeq+1;
			return iterData->second;
		}
		else
		{
			if (RtpSeq < m_curRtpSeq)
			{
				TSever_LOG_Error("send slowly,server buf has clear");
				iterData = m_RtpQue.find(m_curRtpSeq);
				if (iterData != m_RtpQue.end())
				{
					RtpSeq = m_curRtpSeq + 1;
					return iterData->second;
				}
			}
			else
			{
//				TSever_LOG_Error("m_RtpQue.find(RtpSeq) ; error  RtpSeq=%ulld",RtpSeq);
				return NULL;
			}
		}

	}
	return NULL;
}
/*unsigned char* CRtpQueue::CallocRtpBuf(int len)
{
	return (unsigned char*)ngx_pcalloc(m_pool,len);
}*/















} /* namespace myselfstock */
