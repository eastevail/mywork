/*
 * CRtpQueue.h
 *
 *  Created on: 2015年10月30日
 *      Author: lixiang
 */

#ifndef TCPRELAYMODULE_CRTPQUEUE_H_
#define TCPRELAYMODULE_CRTPQUEUE_H_
#include<map>
#include <string>
#include <stdint.h>
#include "com_log.h"

//#include <boost/pool/pool.hpp>
//using boost::pool;
using std::map;
using std::pair;
using std::string;
namespace myselfstock
{

/*typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned   uint32_t;
typedef unsigned long long   uint64_t;*/

#define POOLSIZE 1024*1024*1
#define SNSIZE 32
#define ERRORLENTH 0xffffffffffffffff
#define MAXMTU 1446
typedef struct{
	unsigned char rtpdata[MAXMTU];//rtpdata include 4bytes head like $, , ,
	uint32_t rtplen;
}RTP_INFO;
const unsigned int RTCP_MAXMTU = 1500;
typedef struct{
	unsigned char rtcpdata[RTCP_MAXMTU];//rtcpdata include 4bytes head like $, , ,
	uint32_t rtcplen;
	uint32_t curpos;
}RTCP_INFO;
class CRtpQueue
{
public:
    CRtpQueue(char* sn);//根据sn创建一个
    virtual ~CRtpQueue();
//    unsigned char* CallocRtpBuf(int len);
    RTP_INFO* GetRtpData(uint64_t &RtpSeq);//根据RtpSeq获得队列里的相应的一个rtp包
    int m_ReferCount;
    //rtpdata include 4bytes head like $, , ;追加一个rtp包到该对列*/
    int ApendRtpData(uint64_t rtpseq,unsigned char* rtpdata,uint32_t rtplen,bool isIframe);
private:
    int DelOneSecData();
    int RemoveRtpEXHeadAndCopy(RTP_INFO* dest,unsigned char* rtpdata,uint32_t rtplen);
    uint64_t m_curRtpSeq;
//    pool<> m_Rtp_pool(sizeof(RTP_INFO));
//    ngx_pool_t *m_pool;
    char m_Sn[SNSIZE];
    char* m_rtpbuf;
    bool m_IsLastTimeWr;
    bool m_FirstApend;
    bool m_FirstGet;
    struct timeval m_Last;
    struct timeval m_Now;
    struct timeval m_OneSc;
    map<uint64_t,RTP_INFO*> m_RtpQue;//map<rtpseq,rtpdata(包括4个字节识别头)>
    map<uint64_t,uint64_t> m_IframeRtpSeq;

};

} /* namespace myselfstock */

#endif /* TCPRELAYMODULE_CRTPQUEUE_H_ */
