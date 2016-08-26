/*
 * msgClient.h
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#ifndef STOCKSRC_CSTOCKEVT_H_
#define STOCKSRC_CSTOCKEVT_H_

#include "CEventBase.h"
#include<map>
#include <string>
#include"tb_tpye.h"
#include "ngx_config.h"
#include "ngx_conf_file.h"
#include "nginx.h"
#include "ngx_core.h"
#include "ngx_string.h"
#include "ngx_palloc.h"
using std::map;
using std::pair;
using std::string;
namespace myselfstock
{
#define  MAXDATASIZE 1024*8
class msgClient: public CEventBase
{
public:
	msgClient(FUNC_PRO_TYPE typePro);
    ~msgClient();
    static void CallbackStock(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int SetEvtRorWorT();
    virtual int CreateEvt();
    virtual int CloseEvt();
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int SetTimeOut ( struct timeval* ptime);
    enum { AWAITING_HEAD, AWAITING_PACKET_DATA } ;
    int createMsg(char* pmsg,int msglen,FUNC_PRO_TYPE msgdest ,TB_MSG_TYPE msgtype);
    int ReadData();
    int WritedData();
private:
    map<int,MsgCommuni*> m_recvQue;
    map<int,MsgCommuni*> m_sendQue;
    MsgCommuni m_recvbuf;
    int m_AlreadyNum;
    int ReadMsgData(const MsgCommuni* pmsg);
    int m_DeviceTyp;
    int headlen;
    int volatile  fTCPReadingState;
    ngx_pool_t *pool;
    int sendkey,recvkey;
};

} /* namespace myselfstock */

#endif /* STOCKSRC_CSTOCKEVT_H_ */
