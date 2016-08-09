/*
 * CSkyRtcpAnaly.h
 *
 *  Created on: 2015年11月6日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CSKYRTCPANALY_H_
#define TCPRELAYMODULE_CSKYRTCPANALY_H_
#include<map>
#include <stdint.h>
#include "CRtpQueue.h"
#include "CTcpAccepEvtOnly.h"
using std::map;
using std::pair;
namespace myselfstock
{
// RTCP packet types:
const unsigned char RTCP_PT_SR = 200;
const unsigned char RTCP_PT_RR = 201;
const unsigned char RTCP_PT_SDES = 202;
const unsigned char RTCP_PT_BYE = 203;
const unsigned char RTCP_PT_APP = 204;

// SDES tags:
const unsigned char RTCP_SDES_END = 0;
const unsigned char RTCP_SDES_CNAME = 1;
const unsigned char RTCP_SDES_NAME = 2;
const unsigned char RTCP_SDES_EMAIL = 3;
const unsigned char RTCP_SDES_PHONE = 4;
const unsigned char RTCP_SDES_LOC = 5;
const unsigned char RTCP_SDES_TOOL = 6;
const unsigned char RTCP_SDES_NOTE = 7;
const unsigned char RTCP_SDES_PRIV = 8;
typedef struct
{
	unsigned char flags;					/// bit7=1 indicates that video is enabled
											/// bit6=1 indicates videoScanType is interlaced, otherwise, progressive
											/// bit5=1 indicates videoQualityControlType is "vbr", otherwise, "cbr"
											/// bit[4:0] reserved.
											/// skip videoInputChannelID
	unsigned char videoCodecType;			/// defined in enum ENUM_VIDEO_CODEC_TYPE;
	unsigned char videoResolutionWidth[2];	///
	unsigned char videoResolutionHeight[2];	///
	unsigned char constantBitRate[2];		/// only meaningful if videoQualityControlType is "cbr", in kbps
	unsigned char vbrUpperCap[2];			/// only meaningful if videoQualityControlType is "vbr", in kbps
	unsigned char vbrLowerCap[2];			/// only meaningful if videoQualityControlType is "vbr", in kbps
	unsigned char maxFrameRate[2];			/// maximum frame rate x100
}RTCP_INFO_VIDEO_FORMAT;
typedef enum
{
	RTCP_SDES_PRIVATE_TAG_TYPE				= 1,
	RTCP_SDES_PRIVATE_TAG_SN				= 2,
	RTCP_SDES_PRIVATE_TAG_USER_ID			= 3,
	RTCP_SDES_PRIVATE_TAG_USER_NAME			= 4,
	RTCP_SDES_PRIVATE_TAG_TOKEN				= 5,
	RTCP_SDES_PRIVATE_TAG_VIDEO_FORMAT		= 6,
	RTCP_SDES_PRIVATE_TAG_AUDIO_FORMAT		= 7,
	RTCP_SDES_PRIVATE_TAG_KBPS	= 8,
}ENUM_RTCP_SDES_PRIVATE_INFO_TAG;
typedef enum
{
	RTCP_APP_SELF_DEFINE_COMMAND	= 3,
	RTCP_APP_SELF_DEFINE_RESPOND	= 4,
	RTCP_APP_SELF_DEFINE_NOTIFY		= 5,

}ENUM_RTCP_APP_SELF_DEFINED;		// subtype;

typedef enum
{
	ENDPOINT_TYPE_IPC		= 1,
	ENDPOINT_TYPE_CLOUD		= 2,
	ENDPOINT_TYPE_APPS		= 3,
}ENUM_ENDPOINT_TYPE;

class CSkyRtcpAnaly
{
public:
	CSkyRtcpAnaly ( );
	virtual ~CSkyRtcpAnaly ( );
//	int ParseRtcpPack(uint8_t* rtcppack,unsigned short rtcplen,CTcpAccepEvtOnly* pAcceptEvt);//rtcppack include 4 bytes head
	int ParseRtcpPackSdes(uint8_t* rtcppack,unsigned short rtcplen,CTcpAccepEvtOnly* pAcceptEvt);//rtcppack include 4 bytes head
	int ParseRtcpPackApp(uint8_t* rtcppack,unsigned short rtcplen);
	int ParseSdeItemTlv(uint8_t* tlvdata,CTcpAccepEvtOnly* pAcceptEvt);
	int GetDeviceName(uint8_t* tlvdata,int &startpos,char &m_DeviceTyp);
	int GetDeviceSn(uint8_t* tlvdata,int &startpos,char* sn);
	int GetIPCKbps(uint8_t* tlvdata,int &startpos,int &kbps);
	int CreateAppOkMsg(RTCP_INFO* prtcp);
	int SendMsgToRtcpQue(RTCP_INFO* prtcp);
	RTCP_INFO* GetRtcpData();
	RTCP_INFO_VIDEO_FORMAT m_VideoInfo;
	uint64_t m_curmaxkey;
	map<uint64_t,RTCP_INFO*> m_RtCpQue;
};

class SDESItem {
public:
  SDESItem(unsigned char tag, unsigned char const* value);

  unsigned char const* data() const {return fData;}
  unsigned totalSize() const;

private:
  unsigned char fData[2 + 0xFF]; // first 2 bytes are tag and length
};


} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CSKYRTCPANALY_H_ */
