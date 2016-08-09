/*
 * CTcpRelay.h
 *
 *  Created on: 2015年10月23日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CTCPLISTENEVT_H_
#define TCPRELAYMODULE_CTCPLISTENEVT_H_
#include "CEventBase.h"
#include "CMangEVT.h"
namespace myselfstock
{

class CTcpListenEvt: public CEventBase
{
public:
    CTcpListenEvt(int port);
    ~CTcpListenEvt();
    static void CallbackListen(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int CreateEvt();
    virtual int CloseEvt();
    virtual int SetEvtRorWorT();
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int Accept();
    int ReadData();
    int WritedData();
private:
    int m_port;
    CMangEVT* m_pMangEVT;;
};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CTCPLISTENEVT_H_ */
