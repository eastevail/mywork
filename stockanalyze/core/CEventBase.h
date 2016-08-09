/*
 * CEventBase.h
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#ifndef STOCKSRC_CEVENTBASE_H_
#define STOCKSRC_CEVENTBASE_H_
#include <event2/event_struct.h>
#include <event2/event.h>
namespace myselfstock
{
enum
{
    WOZAEVT = 1,
    STOCKEVT,
	ACCEPT
};
class CEventBase
{
public:
    CEventBase();
    virtual ~CEventBase();

    virtual int Init()=0;//init some parameters according to different event
    virtual int CreateEvt()=0;//create event,the main work is to set the socket
    virtual int SetEvtRorWorT()=0;//set trigger mode ,eg read write
    virtual int CloseEvt()=0;//close socket
    virtual int SetTimeOut(int second);//set the timeout,default is 5s
    virtual int GetType()=0;//get current event type,eg WOZAEVT STOCKEVT
    virtual event_callback_fn GetCallbackAddr()=0;//get the addr of callback function
    int SaveRctpToFile(uint8_t* rctpdata,unsigned short len);
    int SaveRtpToFile(uint8_t* rtpdata,unsigned short len,uint64_t &rtpseq,bool &isIframe);
    struct event m_ServerEvent;//struct event about libevent
    struct timeval m_TimeOut;
    char m_Sn[32];
    int m_ReadFd;//ipc
    int m_SendToApp;
    int m_Type;
    bool m_del;
    short m_event;
    bool m_firstnotify;
private:

};

} /* namespace myselfstock */

#endif /* STOCKSRC_CEVENTBASE_H_ */
