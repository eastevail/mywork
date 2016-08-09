/*
 * CIpcEvt.h
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CIPCEVT_H_
#define TCPRELAYMODULE_CIPCEVT_H_
#include "CEventBase.h"
#include "CMangEVT.h"
#include "CSkyRtcpAnaly.h"
namespace myselfstock
{
#define MAXLEN 1500
class CIpcEvt: public CEventBase
{
public:
	CIpcEvt ( int acceptsockt,char* sn,int Kbps);
	virtual ~CIpcEvt ( );
    static void CallbackAcept(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int CreateEvt();
    virtual int CloseEvt();
    enum { FIRSTREAD,AWAITING_DOLLAR, AWAITING_STREAM_CHANNEL_ID, AWAITING_SIZE1, AWAITING_SIZE2, AWAITING_PACKET_DATA } ;
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual int SetEvtRorWorT();
    virtual event_callback_fn GetCallbackAddr();
    int ReadData();
    int WriteData();
    int SendToIpc();
    int m_Kbps;//ipc
    CMangEVT* m_pMangEVT;
private:
    bool m_IsLastTimeWr;
    int SendToIpcOnly();
    int m_RtpAlreadySend;
    int m_conut ;
    CRtpQueue* m_pRtpQue;
    int GetRtpInfo(uint8_t* prtpdata,uint64_t &rtpseq,bool &isIframe);
    int SendToRtpQue(unsigned short len,uint8_t* rtpdata);//len and rtpdata include 4 bytes head
    uint8_t m_recvbuf[MAXLEN];
    int m_GetRTPdataErrNum;
    RTP_INFO* m_pRtpInfo;
    RTCP_INFO* m_pRtcp;
    CSkyRtcpAnaly* m_pSRtcpAna;
    struct timeval m_Last;
    struct timeval m_Now;
    struct timeval m_OneSc;
    unsigned short m_fNextTCPReadSize;
    unsigned short m_SendTCPReadSize;
    int m_ReadNumOneSc;
    int m_AlreadyNum;
    uint8_t m_fStreamChannelId;//0x00 rtp;0x01 rtcp
    uint8_t m_fSizeByte1;
    int volatile  fTCPReadingState;
    uint64_t m_RtpSeq;

};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CIPCEVT_H_ */
