/*
 * CTcpRelay.cpp
 *
 *  Created on: 2015年10月23日
 *      Author: lixiang
 */

#include "CTcpListenEvt.h"
#include <event2/event.h>
#include "com_log.h"
#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CTcpAccepEvtOnly.h"
extern int errno;
namespace myselfstock
{

CTcpListenEvt::CTcpListenEvt(int port)
{
    // TODO Auto-generated constructor stub
    INIT_TS_LOG;
    m_Type = WOZAEVT;
    m_port=port;
    SetTimeOut(0);
    SetEvtRorWorT();
    m_pMangEVT=CMangEVT::GetInstance();

    TSever_LOG_Error("VERSION   IS     STAE_CLOUD_V1.0.01.003");
}

CTcpListenEvt::~CTcpListenEvt()
{
    // TODO Auto-generated destructor stub
    CloseEvt();
//    CMangEVT::Destroy();
    CLOSE_TS_LOG;
}
void CTcpListenEvt::CallbackListen(evutil_socket_t fd, short event, void *arg)
{
    CTcpListenEvt* pCStock=(CTcpListenEvt*)arg;
	if(pCStock->m_del){
		pCStock->m_pMangEVT->DelOneEvt((CEventBase*) arg,false);
		return;
	}
    switch (event)
    {
    case EV_READ:
        pCStock->Accept();
        break;
    case EV_TIMEOUT:
//        TSever_LOG_Error();
        //        event_del(&ServerEvent);
 //       close(fd);
        break;
    default:
        break;
    }
    return;
}
int CTcpListenEvt::Init()
{
	return 0;
}
int CTcpListenEvt::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ;
	return 0;
}
int CTcpListenEvt::CreateEvt()
{
    struct sockaddr_in sin;
    int n=0;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(m_port);

 //   signal(SIGPIPE, SIG_IGN);
 //   signal(SIGCHLD, SIG_IGN);

    m_ReadFd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_ReadFd)
    {
        TSever_LOG_Error("socket() error:%s",strerror(errno));
        return -1;
    }
    int on = 1;
    n = setsockopt(m_ReadFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    if (-1 == n)
    {
        TSever_LOG_Error("setsockopt() error:%s",strerror(errno));
        return -1;
    }
    n = bind(m_ReadFd, (struct sockaddr *) &sin, sizeof(sin));
    if (-1 == n)
    {
        TSever_LOG_Error("bind() error:%s",strerror(errno));
        return -1;
    }
    n = listen(m_ReadFd, 20);
    if (-1 == n)
    {
        TSever_LOG_Error("listen() error:%s",strerror(errno));
        return -1;
    }

	TSever_LOG_Error("CTcpListenEvt evt coming");
    TSever_LOG_Error("Accepting connections...\n");
    return 0;
}
int CTcpListenEvt::CloseEvt()
{
	if (evutil_closesocket(m_ReadFd) < 0)
	{
		TSever_LOG_Error("CloseEvt() error:%s", strerror(errno));
	}
	TSever_LOG_Error();
	return 0;
}
/*int CTcpListenEvt::SetTimeOut(int second)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = second;
    return 0;
}*/
int CTcpListenEvt::GetType()
{
    return m_Type;
}
event_callback_fn CTcpListenEvt::GetCallbackAddr()
{
    return CallbackListen;
}
int CTcpListenEvt::Accept()
{
    struct sockaddr_in pin;
    socklen_t address_size = sizeof(pin);
    int conn_fd = accept(m_ReadFd, (struct sockaddr *) &pin, &address_size);
    if (-1 == conn_fd)
    {
        TSever_LOG_Error("accept() error:%s",strerror(errno));
    }
    else
    {
        TSever_LOG_Error("far ip is %s at port %d",inet_ntoa(pin.sin_addr),ntohs(pin.sin_port));
        CEventBase* tmp=NULL;
        tmp = new CTcpAccepEvtOnly(conn_fd);
        tmp->Init();
        m_pMangEVT->CreatEvtStock(tmp);
    }
    return 0;
}
int CTcpListenEvt::ReadData()
{
	return 0;
}
int CTcpListenEvt::WritedData()
{
	return 0;
}
} /* namespace myselfstock */
