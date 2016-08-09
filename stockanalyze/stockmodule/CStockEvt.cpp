/*
 * CStockevt.cpp
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#include "CStockEvt.h"
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
#define  PORT 80

CStockevt::CStockevt()
{
    // TODO Auto-generated constructor stub
    m_Type=STOCKEVT;
    m_BodyLen=0;
    m_BodyLastRd=0;
    m_BodyRemain=0;
    m_IsReadDone=false;
    m_IsCheck=false;
    SetEvtRorWorT();
    m_pbody=NULL;
    bzero(m_RequstLine,sizeof(m_RequstLine));
    bzero(m_RecvBuf,sizeof(m_RecvBuf));
    SetTimeOut(1);
    m_ReqNumSuc=0;
    INIT_TS_LOG;
}
CStockevt::CStockevt(const char* stocknames,bool check)
{
    INIT_TS_LOG;
    m_pStockOp = NULL;
    m_StockNum=0;
    m_BodyLen=0;
    m_BodyLastRd=0;
    m_BodyRemain=0;
    m_IsReadDone=false;
    m_IsCheck=check;
    m_pbody=NULL;
    SetEvtRorWorT();
    char* tmp=NULL;
    const char*start=stocknames;
    bzero(m_RequstLine,sizeof(m_RequstLine));
    bzero(m_RecvBuf,sizeof(m_RecvBuf));
    while(1){
        tmp=strstr((char*)start,",");
        if(!tmp)
            break;
        string str(start,tmp-start);
//        TSever_LOG_Error("%s",str.c_str());
        m_StockOpMap.insert(pair<string, CStockOperation*>(str, NULL));
        start=tmp+1;
    }

    snprintf(m_RequstLine, sizeof(m_RequstLine), "GET /list=%s HTTP/1.1\r\nHost: hq.sinajs.cn\r\nConnection: "
            "Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.3.1\r\n\r\n", stocknames);
    TSever_LOG_Error("m_RequstLine=%s", m_RequstLine);
/*    m_pStockOp = new CStockOperation[stocknum]();
    if (m_pStockOp)
    {
        for (int i = 0; i < stocknum; i++)
        {

            m_pStockOp[i].SetStockName(stocknames[i]);
            strcat(tmp, "sh");
            strcat(tmp, stocknames[i]);
//            if ((stocknum - 1) != i)
//            {
                strcat(tmp, ",");
 //           }
        }
        snprintf(m_RequstLine,sizeof(m_RequstLine),"GET /list=%s HTTP/1.1\r\nHost: hq.sinajs.cn\r\nConnection: "
                "Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.3.1\r\n\r\n",tmp);
        TSever_LOG_Error("tmp=%s",tmp);
        TSever_LOG_Error("m_RequstLine=%s",m_RequstLine);
    }*/
    m_Type=STOCKEVT;

    SetTimeOut(1);
    m_ReqNumSuc=0;

}
CStockevt::CStockevt(int stocknum,const char** stocknames,const char* addr)
{
    INIT_TS_LOG;
    char newname[10]={0};
    m_pStockOp = NULL;
    m_StockNum=stocknum;
    m_BodyLen=0;
    m_BodyLastRd=0;
    m_BodyRemain=0;
    m_IsReadDone=false;
    m_IsCheck=false;
    m_pbody=NULL;
    SetEvtRorWorT();
    char tmp[MAXDATASIZE]={0};
    bzero(m_RequstLine,sizeof(m_RequstLine));
    bzero(m_RecvBuf,sizeof(m_RecvBuf));
    for (int i = 0; i < stocknum; i++)
    {
        bzero(newname,sizeof(newname));
        snprintf(newname, sizeof(newname),"%s%s",addr,stocknames[i]);
        m_StockOpMap.insert(pair<string, CStockOperation*>(newname, NULL));
//        strcat(tmp, addr);
        strcat(tmp, newname);
        strcat(tmp, ",");
    }
    snprintf(m_RequstLine, sizeof(m_RequstLine), "GET /list=%s HTTP/1.1\r\nHost: hq.sinajs.cn\r\nConnection: "
            "Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.3.1\r\n\r\n", tmp);
    TSever_LOG_Error("tmp=%s", tmp);
    TSever_LOG_Error("m_RequstLine=%s", m_RequstLine);
/*    m_pStockOp = new CStockOperation[stocknum]();
    if (m_pStockOp)
    {
        for (int i = 0; i < stocknum; i++)
        {

            m_pStockOp[i].SetStockName(stocknames[i]);
            strcat(tmp, "sh");
            strcat(tmp, stocknames[i]);
//            if ((stocknum - 1) != i)
//            {
                strcat(tmp, ",");
 //           }
        }
        snprintf(m_RequstLine,sizeof(m_RequstLine),"GET /list=%s HTTP/1.1\r\nHost: hq.sinajs.cn\r\nConnection: "
                "Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.3.1\r\n\r\n",tmp);
        TSever_LOG_Error("tmp=%s",tmp);
        TSever_LOG_Error("m_RequstLine=%s",m_RequstLine);
    }*/
    m_Type=STOCKEVT;

    SetTimeOut(1);
    m_ReqNumSuc=0;

}
CStockevt::~CStockevt()
{
    // TODO Auto-generated destructor stub
    CLOSE_TS_LOG;
    if (m_pStockOp)
    {
        delete[] m_pStockOp;
        m_pStockOp = NULL;
    }
    map<string,CStockOperation*>::iterator iter;
    for (iter = m_StockOpMap.begin(); iter != m_StockOpMap.end(); ++iter)
    {
        delete iter->second;
    }
    m_StockOpMap.clear();
    CloseEvt();
}
int CStockevt::Init()
{
    map<string, CStockOperation*>::iterator iter;
    for (iter = m_StockOpMap.begin(); iter != m_StockOpMap.end(); ++iter)
    {
        iter->second = new CStockOperation();
        if (iter->second)
        {
            iter->second->SetStockName((iter->first).c_str());
        }
        else
        {
            TSever_LOG_Error("new CStockOperation error");
            return -1;
        }
    }
    return 0;
}
int CStockevt::SetEvtRorWorT()
{
	m_event=EV_PERSIST | EV_READ|EV_WRITE;
	return 0;
}
int CStockevt::CreateEvt()
{
    struct hostent *he;
    struct sockaddr_in server;
/*    if (argc != 2)
    {
        TSever_LOG_Error("Usage:%s <IP Address>\n", argv[0]);
        exit(1);
    }*/
    TSever_LOG_Error();
    if ((he = gethostbyname("hq.sinajs.cn")) == NULL)
    {
        TSever_LOG_Error("gethostbyname()error\n");
        return -1;
    }
    TSever_LOG_Error();
    if ((m_ReadFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        TSever_LOG_Error("socket()error\n");
        return -1;
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr *) he->h_addr);
    if (connect(m_ReadFd, (struct sockaddr *) &server, sizeof(server)) == -1)
    {
        TSever_LOG_Error("connect()error\n");
        return -1;
    }
    return 0;
}
void CStockevt::CallbackStock(evutil_socket_t fd, short event, void *arg)
{
    CStockevt* pCStock=(CStockevt*)arg;
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
/**
 *
 * @param buf
 * @return -1 error
 */
int CStockevt::GetBodyLen ( char* buf )
{
    char*tmp = strstr(buf, "Content-Length:");
    if (tmp)
    {
        char* tmp1 = strstr(tmp, "\r\n");
        if (tmp1)
        {
            char BodyLen[8] = { 0 };
            strncpy(BodyLen, tmp + 15, tmp1 - tmp - 15);
            m_BodyLen=atoi(BodyLen);
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}
int CStockevt::ReadData()
{
    int num;
    char* tmp = NULL;
//    char buf[MAXDATASIZE] = { 0 };
    if ((num = recv(m_ReadFd, m_RecvBuf + m_BodyLastRd, MAXDATASIZE-m_BodyLastRd, 0)) == -1)
    {
        TSever_LOG_Error("m_BodyLastRd=%d recv() error:%s",m_BodyLastRd,strerror(errno));
        return -1;
    }
    if (0 == m_BodyLen)
    {
        GetBodyLen(m_RecvBuf);

    }

//        buf[num - 1] = '\0';
//        m_ReqNumSuc++;
    m_BodyLastRd += num;
    tmp = strstr(m_RecvBuf, "\r\n\r\n");
    if (tmp)
    {
        m_BodyRemain = m_BodyLen - (m_RecvBuf + m_BodyLastRd - tmp - 4);
        if (0 == m_BodyRemain)
        {
            ProcessData(tmp + 4);
//            TSever_LOG_Error("response: %s", m_RecvBuf);
            bzero(m_RecvBuf,sizeof(m_RecvBuf));
            m_BodyLen = 0;
            m_BodyLastRd=0;
            m_ReqNumSuc++;
        }
    }

//    TSever_LOG_Error("m_ReqNumSuc: %d", m_ReqNumSuc);

    return 0;
}
int CStockevt::WritedData()
{
    if (!m_BodyLastRd)
    {
        int num = strlen(m_RequstLine);
        int ret = 0, pos = 0;
//    char *mg = "GET /list=sh601006,sh601007 HTTP/1.1\r\nHost: hq.sinajs.cn\r\nConnection: Keep-Alive\r\nUser-Agent: Apache-HttpClient/4.3.1\r\n\r\n";
        while (num)
        {
            if ((ret = send(m_ReadFd, m_RequstLine + pos, num, 0)) == -1)
            {
                TSever_LOG_Error("m_BodyLastRd=%d send() error:%s",m_BodyLastRd,strerror(errno));
                return -1;
            }
            num = num - ret;
            pos = pos + ret;
        }
    }
//    TSever_LOG_Error("num: %d", num);
    return 0;
}
int CStockevt::ProcessDataOne(string &str,bool islast)
{
//    TSever_LOG_Error("%s", str.c_str());
//    std::size_t pos_sh = str.find("sz");
    std::size_t pos = str.find("=");
//    string stockname = str.substr(pos_sh + 2, pos - pos_sh - 2);
    string stockname = str.substr(pos-8, 8);
//    TSever_LOG_Error("%s", stockname.c_str());
    map<string, CStockOperation*>::iterator iter;
    iter = m_StockOpMap.find(stockname);
    if (iter != m_StockOpMap.end())
    {
        iter->second->SetValue(str);
        if (m_IsCheck)
        {
            //可以优化这里，否则每次都更新一次
            iter->second->CheckYerDayHammerOK(islast);
        }
        else
        {
            iter->second->CheckHammerEnd();
        }
    }
    return 0;
}
int CStockevt::ProcessData(char* body)
{
    char* tmp = NULL, *start = body;
    char* last=NULL;
    last=start+strlen(start);
    while (1)
    {
        tmp = strstr(start, "\n");
        if (tmp)
        {
//            TSever_LOG_Error();
            string str(start, tmp - start);
            start = tmp + 1;
            if (last == start)
            {
                ProcessDataOne(str,true);
            }
            else
            {
                ProcessDataOne(str,false);
            }

        }else{
/*            TSever_LOG_Error();
            string str(start,last-start);
            ProcessDataOne(str);*/
            break;
        }
    }

    return 0;
}
int CStockevt::CloseEvt()
{
    if(evutil_closesocket(m_ReadFd)<0){
        TSever_LOG_Error("CloseEvt() error:%s",strerror(errno));
    }
    TSever_LOG_Error();
    return 0;
}
/*int CStockevt::SetTimeOut(int second)
{
    evutil_timerclear(&m_TimeOut);
    m_TimeOut.tv_sec = second;
    return 0;
}*/
event_callback_fn CStockevt::GetCallbackAddr()
{
    return CallbackStock;
}
int CStockevt::GetType()
{
    return m_Type;
}
} /* namespace myselfstock */
