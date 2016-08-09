/*
 * CTcpAccepEvtOnly.h
 *
 *  Created on: 2015年10月23日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CTCPACCEPEVTONLY_H_
#define TCPRELAYMODULE_CTCPACCEPEVTONLY_H_
#include "CEventBase.h"
#include "CMangEVT.h"

namespace myselfstock
{
#define MAXLEN 1500
#define FIRSTRDLEN 1344
#define GETRTPDATAERRNUMMAX 1000
class CSkyRtcpAnaly;

class CTcpAccepEvtOnly: public CEventBase
{
public:
    CTcpAccepEvtOnly(int acceptsockt);
    ~CTcpAccepEvtOnly();
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
    int WritedData();
    int WriteData();
    int GetDeviceInfo();
    int m_Kbps;//ipc
    char m_DeviceTyp;//0x01表示是app，0x02表示ipc
    CMangEVT* m_pMangEVT;
private:
    char m_recvfirst[FIRSTRDLEN];
    CSkyRtcpAnaly* m_pSRtcpAna;
    uint8_t m_recvbuf[MAXLEN];
    int m_AlreadyNum;
    int m_AlreadyNumFirst;
    unsigned short m_fNextTCPReadSize;
    unsigned short m_SendTCPReadSize;
    u_int8_t m_fStreamChannelId;//0x00 rtp;0x01 rtcp
    u_int8_t m_fSizeByte1;
    int volatile  fTCPReadingState;

};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CTCPACCEPEVTONLY_H_ */
