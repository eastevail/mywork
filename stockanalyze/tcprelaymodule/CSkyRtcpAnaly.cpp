/*
 * CSkyRtcpAnaly.cpp
 *
 *  Created on: 2015年11月6日
 *      Author: lixiang
 */

#include "CSkyRtcpAnaly.h"
#include "com_log.h"
#include "nedmalloc.h"
#include <event2/event.h>
#include <string.h>
#include <stdlib.h>
using nedalloc::nedcalloc;
using nedalloc::nedfree;
namespace myselfstock
{

CSkyRtcpAnaly::CSkyRtcpAnaly ( )
{
	// TODO Auto-generated constructor stub
	INIT_TS_LOG
	m_curmaxkey=0;
}

CSkyRtcpAnaly::~CSkyRtcpAnaly ( )
{
	// TODO Auto-generated destructor stub
	map<uint64_t, RTCP_INFO*>::iterator iterData;
	for (iterData = m_RtCpQue.begin(); iterData != m_RtCpQue.end(); ++iterData)
	{
			nedfree((void*)(iterData->second));
//			m_RtCpQue.erase(iterData);

	}
	m_RtCpQue.clear();
	CLOSE_TS_LOG
}
RTCP_INFO* CSkyRtcpAnaly::GetRtcpData()
{
//	RTCP_INFO* tmp=NULL;
	map<uint64_t, RTCP_INFO*>::iterator iterData;
	iterData = m_RtCpQue.begin();
	if(iterData != m_RtCpQue.end())
	{
		return iterData->second;
	}
	return NULL;
}
int CSkyRtcpAnaly::GetDeviceName(uint8_t* tlvdata,int &startpos,char &m_DeviceTyp)
{
	switch (tlvdata[startpos])
	{
	case RTCP_SDES_PRIVATE_TAG_TYPE:
	{
		char tmpname[4] = { 0 };
		memcpy(tmpname, tlvdata + 2 + startpos, 3);
		if (0 == strncmp("APP", tmpname, 3))
		{
			m_DeviceTyp = 0x01;
		}
		else if (0 == strncmp("IPC", tmpname, 3))
		{
			m_DeviceTyp = 0x02;
		}
		else
		{
			TSever_LOG_Error("recv unsurpport device name");
			return -1;
		}
		break;
	}
	default:
		TSever_LOG_Error("recv not RTCP_SDES_PRIVATE_TAG_TYPE");
		return -1;
		break;

	}
	startpos = 5;
	return 0;
}
int CSkyRtcpAnaly::GetDeviceSn(uint8_t* tlvdata,int &startpos,char* sn)
{
	switch(tlvdata[startpos])
	{
	case RTCP_SDES_PRIVATE_TAG_SN:
		memcpy(sn,tlvdata+2+startpos,32);
		break;
	default:
		TSever_LOG_Error("recv not RTCP_SDES_PRIVATE_TAG_SN");
		return -1;
		break;
	}
	startpos=startpos+2+32;
	return 0;
}
int CSkyRtcpAnaly::GetIPCKbps(uint8_t* tlvdata,int &startpos,int &kbps)
{
	switch (tlvdata[startpos])
	{
//	case RTCP_SDES_PRIVATE_TAG_KBPS:
	case RTCP_SDES_PRIVATE_TAG_VIDEO_FORMAT:
	{
/*		char tmpname[9] = { 0 };
		memcpy(tmpname, tlvdata + 2 + startpos, 8);
		kbps = atoi(tmpname);*/
		memcpy(&m_VideoInfo, tlvdata + 2 + startpos, sizeof(m_VideoInfo));
//		kbps = atoi(m_VideoInfo.constantBitRate);
		kbps=((m_VideoInfo.constantBitRate[0]||0x00ff)<<8)||m_VideoInfo.constantBitRate[1];
		break;
	}
	default:
		TSever_LOG_Error("recv not RTCP_SDES_PRIVATE_TAG_KBPS");
		return -1;
		break;
	}
	startpos = startpos + 2 + 8;
	return 0;
}
int CSkyRtcpAnaly::ParseSdeItemTlv(uint8_t* tlvdata,CTcpAccepEvtOnly* pAcceptEvt)//0x01表示是app，0x02表示ipc
{
	int startpos = 0;
	int ret = GetDeviceName(tlvdata, startpos, pAcceptEvt->m_DeviceTyp);
	if (ret < 0)
		return ret;
	ret = GetDeviceSn(tlvdata, startpos, pAcceptEvt->m_Sn);
	TSever_LOG_Error("sn=%s",pAcceptEvt->m_Sn);
	if (ret < 0)
		return ret;
/*	if (0x02 == pAcceptEvt->m_DeviceTyp)
	{
		ret = GetIPCKbps(tlvdata, startpos, pAcceptEvt->m_Kbps);
		if (ret < 0)
			return ret;
	}*/
	return 0;
}
int CSkyRtcpAnaly::SendMsgToRtcpQue(RTCP_INFO* prtcp)
{
	m_curmaxkey++;
	m_RtCpQue.insert(pair<uint64_t, RTCP_INFO*>(m_curmaxkey, prtcp));
	return 0;
}
int CSkyRtcpAnaly::CreateAppOkMsg(RTCP_INFO* prtcp)
{
	const char* ok="OK";
	(prtcp->rtcpdata[0])='$';
	(prtcp->rtcpdata[1])=0x01;
	(prtcp->rtcpdata[4])=0x84;
	(prtcp->rtcpdata[5])=0xcc;//RTCP_PT_APP
	(prtcp->rtcpdata[16])=0x04;//RTCP_APP_SELF_DEFINE_RESPOND
	(prtcp->rtcpdata[17])=0x02;//2
	memcpy((prtcp->rtcpdata)+18,ok,2);
	unsigned numBytes = 12+2+2;
	unsigned num4ByteWords = (numBytes + 3) / 4;
	((prtcp->rtcpdata[6]))|=(num4ByteWords&0x0000ff00)>>8;
	((prtcp->rtcpdata[7]))|=(num4ByteWords&0x000000ff);
	((prtcp->rtcpdata[2]))|=((num4ByteWords*4)&0x0000ff00)>>8;
	((prtcp->rtcpdata[3]))|=((num4ByteWords*4)&0x0000ff00);
	prtcp->rtcplen=num4ByteWords*4+4;
	SendMsgToRtcpQue(prtcp);
	return 0;
/*	unsigned rtcpHdr = 0x84000000; // version 2, no padding, 1 SSRC chunk
	rtcpHdr |= (RTCP_PT_APP << 16);
	rtcpHdr |= num4ByteWords;*/
}
int CSkyRtcpAnaly::ParseRtcpPackSdes(uint8_t* rtcppack,unsigned short rtcplen,CTcpAccepEvtOnly* pAcceptEvt)
{
	switch (rtcppack[5])
	{
	case RTCP_PT_SDES:
		if (RTCP_SDES_PRIV == rtcppack[12])
		{
			int ret = ParseSdeItemTlv(rtcppack + 15,pAcceptEvt);
			if (ret < 0)
			{
				return ret;
			}
			else
			{
				struct timeval tmp;
			    evutil_timerclear(&tmp);
			    tmp.tv_sec = 5;
				pAcceptEvt->m_pMangEVT->ChangToTimeOutReadWriteEvt((CEventBase*)pAcceptEvt,&tmp);
				RTCP_INFO* tmprtcp=(RTCP_INFO*)nedcalloc(1,sizeof(RTCP_INFO));
				CreateAppOkMsg(tmprtcp);//包含4个字节的同步头
			}
		}
		else
		{
			TSever_LOG_Error("recv SDES Item not surport");
		}
		break;
/*	case RTCP_PT_APP:
		break;*/
	default:
		TSever_LOG_Error("recv rtcp pack not surport");
		return -1;
		break;
	}
	return 0;
}
int CSkyRtcpAnaly::ParseRtcpPackApp(uint8_t* rtcppack,unsigned short rtcplen)
{
	switch (rtcppack[5])
	{
	case RTCP_PT_APP:
		break;
	default:
		TSever_LOG_Error("recv rtcp pack not surport");
		return -1;
		break;
	}
	return 0;
}

SDESItem::SDESItem(unsigned char tag, unsigned char const* value) {
  unsigned length = strlen((char const*)value);
  if (length > 0xFF) length = 0xFF; // maximum data length for a SDES item

  fData[0] = tag;
  fData[1] = (unsigned char)length;
  memmove(&fData[2], value, length);
}

unsigned SDESItem::totalSize() const {
  return 2 + (unsigned)fData[1];
}
} /* namespace myselfstock */
