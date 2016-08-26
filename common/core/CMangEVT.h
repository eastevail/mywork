/*
 * CMangEVT.h
 *
 *  Created on: 2015年10月13日
 *      Author: lixiang
 */

#ifndef STOCKSRC_CMANGEVT_H_
#define STOCKSRC_CMANGEVT_H_
#include<map>
#include "CEventBase.h"
#include"tb_tpye.h"
using std::map;
using std::pair;
namespace myselfstock
{
typedef void (*eventflowcreate_notify)(CEventBase* );//new
typedef void (*eventflowerror_notify)(CEventBase* );//new
class funcProEvt;
class CMangEVT
{
public:
    static CMangEVT* GetInstance();//get singleton class
    static int Destroy();//detroy a class CMangEVT
    int DelOneFlowFromUp(CEventBase* pdelevt);//new
    int GetModuleVersion();
    int DelOneEvt(CEventBase* pdelevt,bool streamerr=true);//delete a event
    eventflowcreate_notify m_pEvtNotify;//new
    eventflowerror_notify m_pEvtNotifyerr;//new
    int AssignOneEvt(CEventBase* pdelevt);//assign one event to libevent
    int DispatchEvt();//call DispatchEvt() of libevent,this operation will block current process
    int ChangToTimeOutReadEvt(CEventBase* pEvtB,struct timeval* pTime);//change the event to a read and timeout event
    int ChangToTimeOutReadWriteEvt(CEventBase* pEvtB,struct timeval* pTime);//change the event to a readwrite and timeout event
    int ChangToWriteReadEvt(CEventBase* pEvtB);//change the event to a readwrite event
    int ChangToAppEvt(CEventBase* pEvtBDel,CEventBase* pEvtAppAdd);//改变一个事件成为APP事件，第二个参数代表APP事件，APP派生类见扩展模块
    int ChangToIpcEvt(CEventBase* pEvtBDel,CEventBase* pEvtIpcAdd);//改变一个事件成为IPC事件，第二个参数代表IPC事件，IPC派生类见扩展模块
//    CEventBase* GetIpcAccordSN(char* sn);//
    int CreatEvtStock(CEventBase* tmp);//创建一个事件然后添加进基础事件
    map<CEventBase*,CEventBase*> m_EvtMapAdded;
    struct event m_signal_int;
    funcProEvt* m_proEvt[FUNC_PRO_TOTAL];
private:
    struct event_base *m_pBase;
    static CMangEVT* m_pInstance;
    static int m_ReferCount;

    CMangEVT();
    ~CMangEVT();
    int AddEvtToMap();
};

} /* namespace myselfstock */

#endif /* STOCKSRC_CMANGEVT_H_ */
