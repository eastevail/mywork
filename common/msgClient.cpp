/*
 * msgClient.cpp
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#include "msgClient.h"

#include "com_log.h"
#include <event2/event.h>
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
extern int errno;
namespace myselfstock
{
#define  PORT 9999

msgClient::msgClient(FUNC_PRO_TYPE typePro):fTCPReadingState(AWAITING_HEAD)
{
    // TODO Auto-generated constructor stub
    m_Type=STOCKEVT;
    struct timeval tim;
    tim.tv_sec=0;
    tim.tv_usec=5000;
    SetTimeOut(&tim);
    SetEvtRorWorT();
    m_DeviceTyp=typePro;
    sendkey=0;
    m_AlreadyNum=0;
    recvkey=0;
}

msgClient::~msgClient()
{
    // TODO Auto-generated destructor stub

    ngx_destroy_pool(pool);
    CloseEvt();
}
int msgClient::Init()
{
    pool = ngx_create_pool(1024*2, NULL);
    headlen=sizeof(m_recvbuf);
    return 0;
}
int msgClient::SetTimeOut ( struct timeval* ptime)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = ptime->tv_sec;
    m_TimeOut.tv_usec = ptime->tv_usec;
    return 0;
}

int msgClient::createMsg(char* pmsg,int msglen,FUNC_PRO_TYPE msgdest ,TB_MSG_TYPE msgtype)
{
    MsgCommuni* pmsgw=(MsgCommuni*)ngx_pcalloc(pool,sizeof(MsgCommuni));
    pmsgw->destPro=msgdest;
    pmsgw->srcPro=m_DeviceTyp;
    pmsgw->msglen=msglen;
    pmsgw->msg=pmsg;
    pmsgw->msgType=msgtype;
	m_sendQue.insert(pair<int, MsgCommuni*>(sendkey++, pmsgw));
}

int msgClient::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ;
	return 0;
}
int msgClient::CreateEvt()
{
    struct sockaddr_in server;

    if ((m_ReadFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        TSever_LOG_Error("socket()error\n");
        return -1;
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(m_ReadFd, (struct sockaddr *) &server, sizeof(server)) == -1)
    {
        TSever_LOG_Error("connect()error\n");
        return -1;
    }
    createMsg(NULL,0,FUNC_PRO_TOTAL,REGISTER_TO_MSGSERVER);
    return 0;
}
void msgClient::CallbackStock(evutil_socket_t fd, short event, void *arg)
{
    msgClient* pCStock=(msgClient*)arg;
    switch (event)
    {
    case EV_READ:
        pCStock->ReadData();
        break;
    case EV_TIMEOUT:
        pCStock->WritedData();
        //        event_del(&ServerEvent);
 //       close(fd);
        break;
    default:
        break;
    }
    return ;
}

int msgClient::ReadMsgData(const MsgCommuni* pmsg)
{
	int num;
	static MsgCommuni* tmpmsg=NULL;
	static int alreadyReadNum=0;
	if(!tmpmsg){
		tmpmsg=(MsgCommuni*)ngx_pcalloc(pool,sizeof(MsgCommuni));
		tmpmsg->msg=(char*)ngx_pcalloc(pool,pmsg->msglen);
		memcpy(tmpmsg,pmsg,sizeof(MsgCommuni)-4);
	}
	if (pmsg->msglen) {
		num = recv(m_ReadFd, tmpmsg->msg, tmpmsg->msglen - alreadyReadNum, 0);
		if (0 == num) {
			return -2;
		} else if (num < 0) {
			DBG("recv fTCPReadingState %d error:%s", AWAITING_PACKET_DATA, strerror(errno));
			return -1;
		}
		alreadyReadNum = alreadyReadNum + num;
	}
	if( tmpmsg->msglen == alreadyReadNum){
		alreadyReadNum=0;
		m_recvQue.insert(pair<int, MsgCommuni*>(recvkey++, tmpmsg));
		tmpmsg=NULL;
		return 1;
	}
	return 0;
}

int msgClient::ReadData()
{
	int num;

	switch (fTCPReadingState) {
	case AWAITING_HEAD: {
		num = recv(m_ReadFd, (char*)&m_recvbuf + m_AlreadyNum, headlen - m_AlreadyNum, 0);
		if (0 == num) {
			return -2;
		} else if (num < 0) {
			DBG("recv fTCPReadingState %d error:%s", AWAITING_HEAD, strerror(errno));
			return -1;
		}
		m_AlreadyNum = m_AlreadyNum + num;
		//    TSever_LOG_Error("revalread=%d num=%d", m_AlreadyNum, num);
		if (headlen == m_AlreadyNum) {
			m_AlreadyNum = 0;
			m_recvbuf.srcPro = ntohl(m_recvbuf.srcPro);
			m_recvbuf.destPro = ntohl(m_recvbuf.destPro);
			if(m_DeviceTyp != m_recvbuf.destPro){
				DBG("process %d RECIVE error msg DEST process is %d",m_DeviceTyp,m_recvbuf.destPro);
			}else if(m_recvbuf.srcPro>=FUNC_PRO_TOTAL){
				DBG("process %d RECIVE error msg SRC process is %d",m_DeviceTyp,m_recvbuf.srcPro);
			}else{
				fTCPReadingState = AWAITING_PACKET_DATA;

				m_recvbuf.msgType = ntohl(m_recvbuf.msgType);
				m_recvbuf.msglen = ntohl(m_recvbuf.msglen);
				m_recvbuf.magic = ntohl(m_recvbuf.magic);
			}
		}
		break;
	}
	case AWAITING_PACKET_DATA: {
		if (1== ReadMsgData(&m_recvbuf)) {
			fTCPReadingState = AWAITING_HEAD;
		}
		break;
	}

	}
	return 0;
}
int msgClient::WritedData()
{
	static MsgCommuni* msgsend = NULL;
	static int alreadySendhead = 0, alreadySenddata = 0;
	map<int, MsgCommuni*>::iterator iter;
	int ret;
	if (!msgsend) {
		//first
		if (!m_sendQue.size()) {
			iter = m_sendQue.begin();
			msgsend = iter->second;
			msgsend->destPro = htonl(msgsend->destPro);
			msgsend->srcPro = htonl(msgsend->srcPro);
			msgsend->msgType = htonl(msgsend->msgType);
			msgsend->msglen = htonl(msgsend->msglen);
			msgsend->magic = htonl(msgsend->magic);
		}
	} else {
		if (alreadySendhead != headlen) {
			if ((ret = write(m_ReadFd, (char*) msgsend + alreadySendhead, headlen - alreadySendhead))
					== -1) {
				if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
					DBG("sento() error:%s", strerror(errno));
					return -1;
				} else if (errno == EBADF) {
					DBG("sento() error:%s", strerror(errno));
					return -2;
				} else {
					DBG("sento() error:%s", strerror(errno));
					return -2;
				}
			} else {
				alreadySendhead = alreadySendhead + ret;
			}
		} else {
			int datalen = ntohl(msgsend->msglen);
			//send data
			if (!datalen) {
				if ((ret = write(m_ReadFd, (char*) msgsend->msg + alreadySenddata,
						datalen - alreadySenddata)) == -1) {
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
						DBG("sento() error:%s", strerror(errno));
						return -1;
					} else if (errno == EBADF) {
						DBG("sento() error:%s", strerror(errno));
						return -2;
					} else {
						DBG("sento() error:%s", strerror(errno));
						return -2;
					}
				} else {
					alreadySenddata = alreadySenddata + ret;
				}
				if (alreadySenddata == datalen) {
					alreadySendhead = 0;
					alreadySenddata = 0;
					ngx_pfree(pool, msgsend->msg);
					ngx_pfree(pool, msgsend);
					m_sendQue.erase(iter);
					msgsend = NULL;
				}
			} else {
				alreadySendhead = 0;
				alreadySenddata = 0;
				ngx_pfree(pool, msgsend->msg);
				ngx_pfree(pool, msgsend);
				m_sendQue.erase(iter);
				msgsend = NULL;
			}
		}
	}
	return 0;
}

int msgClient::CloseEvt()
{
    if(evutil_closesocket(m_ReadFd)<0){
        TSever_LOG_Error("CloseEvt() error:%s",strerror(errno));
    }
    TSever_LOG_Error();
    return 0;
}
/*int msgClient::SetTimeOut(int second)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = second;
    return 0;
}*/
event_callback_fn msgClient::GetCallbackAddr()
{
    return CallbackStock;
}
int msgClient::GetType()
{
    return m_Type;
}
} /* namespace myselfstock */
