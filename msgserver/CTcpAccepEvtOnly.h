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
#include "funcProEvt.h"
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
    enum { AWAITING_HEAD, AWAITING_PACKET_DATA } ;
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int ReadData();
    int WritedData();
    int WriteData();
    int GetDeviceInfo();
    int m_Kbps;//ipc
    int m_DeviceTyp;//0x01表示是app，0x02表示ipc
    CMangEVT* m_pMangEVT;
private:
    MsgCommuni m_recvbuf;
    int m_AlreadyNum;
    int headlen;
    int volatile  fTCPReadingState;

};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CTCPACCEPEVTONLY_H_ */
