/*
 * CAppEvt.h
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CAPPEVT_H_
#define TCPRELAYMODULE_CAPPEVT_H_
#include "CEventBase.h"
#include "CMangEVT.h"
namespace myselfstock
{
#define GETRTPDATAERRNUMMAX 1000
class CAppEvt : public CEventBase
{
public:
	CAppEvt ( int acceptsockt,char* sn);
	virtual ~CAppEvt ( );
    static void CallbackAcept(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int SetEvtRorWorT();
    virtual int CreateEvt();
    virtual int CloseEvt();
    enum { FIRSTREAD,AWAITING_DOLLAR, AWAITING_STREAM_CHANNEL_ID, AWAITING_SIZE1, AWAITING_SIZE2, AWAITING_PACKET_DATA } ;
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int ReadData();
    int WriteData();
    int SendToIpc();
    int SendToApp();
    CMangEVT* m_pMangEVT;
private:
    CRtpQueue* m_pRtpQue;
    int SendToAppOnly();
    int m_RtpAlreadySend;
    int m_GetRTPdataErrNum;
    uint64_t m_RtpSeq;
    RTP_INFO* m_pRtpInfo;
};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CAPPEVT_H_ */
