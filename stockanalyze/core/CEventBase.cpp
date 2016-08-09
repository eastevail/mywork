/*
 * CEventBase.cpp
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#include "CEventBase.h"
#include "com_log.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
namespace myselfstock
{

CEventBase::CEventBase()
{
    // TODO Auto-generated constructor stub
    INIT_TS_LOG;
    m_SendToApp=0;
    m_del=false;
    m_firstnotify=true;
    m_Type=0;
    bzero(m_Sn,sizeof(m_Sn));
}

CEventBase::~CEventBase()
{
    // TODO Auto-generated destructor stub

    CLOSE_TS_LOG;
}
int CEventBase::SaveRctpToFile(uint8_t* rctpdata,unsigned short len)
{
	return 0;
	char filename[32] = { 0 };

	snprintf(filename, sizeof(filename), "/home/lixiang/ipc/RTCP_frame");

	FILE* fp = fopen(filename, "a");
	if (NULL == fp)
	{
		TSever_LOG_Error("fopen file %s error  error:%s", filename, strerror(errno));
		return -1;
	}
	size_t ret = fwrite(rctpdata, 1, len, fp);
	if (ret != len)
	{
		TSever_LOG_Error("fwrite error ret=%d", ret);
	}
	fclose(fp);
	return 0;
}
int CEventBase::SaveRtpToFile(uint8_t* rtpdata,unsigned short len,uint64_t &rtpseq,bool &isIframe)
{
	return 0;
	char filename[32]={0};
/*	if(isrtcp)
	{
		snprintf(filename, sizeof(filename),"/home/lixiang/ipcdata/RTCP_frame");
	}
	else */if (isIframe)
	{
		snprintf(filename, sizeof(filename),"/home/lixiang/ipc/I_frame_%d", rtpseq);
	}
	else
	{
		snprintf(filename, sizeof(filename),"/home/lixiang/ipc/P_frame_%d", rtpseq);
	}
	FILE* fp = fopen(filename, "a");
	if (NULL == fp)
	{
		TSever_LOG_Error("fopen file %s error  error:%s", filename,strerror(errno));
		return -1;
	}
	size_t ret=fwrite(rtpdata,1,len,fp);
	if(ret!=len)
	{
		TSever_LOG_Error("fwrite error ret=%d",ret);
	}
	fclose(fp);
	return 0;
}
int CEventBase::SetTimeOut(int second)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = second;
    return 0;
}
} /* namespace myselfstock */
