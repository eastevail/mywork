/*
 * CStockevt.h
 *
 *  Created on: 2015年10月7日
 *      Author: lixiang
 */

#ifndef STOCKSRC_CSTOCKEVT_H_
#define STOCKSRC_CSTOCKEVT_H_

#include "CEventBase.h"
#include "CStockOperation.h"
#include<map>
#include <string>
using std::map;
using std::pair;
using std::string;
namespace myselfstock
{
#define  MAXDATASIZE 1024*8
class CStockevt: public CEventBase
{
public:
    CStockevt();
    CStockevt(const char* stocknames,bool check);
    CStockevt(int stocknum,const char** stocknames,const char* addr);//default stocknum=5
    ~CStockevt();
    static void CallbackStock(evutil_socket_t fd, short event, void *arg);
    virtual int Init();
    virtual int SetEvtRorWorT();
    virtual int CreateEvt();
    virtual int CloseEvt();
//    virtual int SetTimeOut(int second);
    virtual int GetType();
    virtual event_callback_fn GetCallbackAddr();
    int ReadData();
    int WritedData();
private:
    int ProcessData(char* body);
    int GetBodyLen(char* buf);
    int ProcessDataOne(string &str,bool islast);
    char m_RecvBuf[MAXDATASIZE];
    unsigned int m_ReqNumSuc;//Success RequestNum
    unsigned int m_StockNum;
    unsigned int m_BodyLen;
    unsigned int m_BodyRemain;
    bool m_IsReadDone;
    bool m_IsCheck;
    char* m_pbody;
    unsigned int m_BodyLastRd;
    map<string,CStockOperation*> m_StockOpMap;
    CStockOperation* m_pStockOp;
    char m_RequstLine[MAXDATASIZE];
};

} /* namespace myselfstock */

#endif /* STOCKSRC_CSTOCKEVT_H_ */
