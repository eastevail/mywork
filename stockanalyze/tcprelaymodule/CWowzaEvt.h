/*
 * CWowzaEvt.h
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CWOWZAEVT_H_
#define TCPRELAYMODULE_CWOWZAEVT_H_
#include "CEventBase.h"
#include "CMangEVT.h"
namespace myselfstock
{
#define GETRTPDATAERRNUMMAX 6000
class CWowzaEvt : public CEventBase
{
public:
	CWowzaEvt ( char* sn,char* ip,int port);
	virtual ~CWowzaEvt ( );
    static void CallbackAcept(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int CreateEvt();
    virtual int CloseEvt();
    virtual int SetEvtRorWorT();
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
    struct sockaddr_in m_wozaaddr;
    int SendToAppOnly();
    uint32_t m_RtpAlreadySend;
    int m_GetRTPdataErrNum;
    uint64_t m_RtpSeq;
    RTP_INFO* m_pRtpInfo;
    bool m_PrintfTime;
};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CWOWZAEVT_H_ */
