/*
 * funcProEvt.h
 *
 *  Created on: 2015年11月10日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CAPPEVT_H_
#define TCPRELAYMODULE_CAPPEVT_H_
#include "CEventBase.h"
#include "CMangEVT.h"
#include "tb_tpye.h"
#include "ngx_config.h"
#include "ngx_conf_file.h"
#include "nginx.h"
#include "ngx_core.h"
#include "ngx_string.h"
#include "ngx_palloc.h"
namespace myselfstock
{
class funcProEvt : public CEventBase
{
public:
	funcProEvt ( int acceptsockt,FUNC_PRO_TYPE type);
	virtual ~funcProEvt ( );
    static void CallbackAcept(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int SetEvtRorWorT();
    virtual int CreateEvt();
    virtual int CloseEvt();
    int SetTimeOut ( struct timeval* ptime);
    enum { AWAITING_HEAD, AWAITING_PACKET_DATA } ;
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int ReadData();
    int ReadOtherProComeData(const MsgCommuni* pmsg,int otherfd);
    int WriteData();
    CMangEVT* m_pMangEVT;
    int m_DeviceTyp;
private:
    map<int,MsgCommuni*> m_recvQue;
    map<int,MsgCommuni*> m_sendQue;
    MsgCommuni m_recvbuf;
    int m_AlreadyNum;
    int headlen;
    int volatile  fTCPReadingState;
    ngx_pool_t *pool;
    int sendkey;
};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CAPPEVT_H_ */
