/*
 * avFromatOp.h
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_COMMON_AVFROMATOP_H_
#define TBAPP_COMMON_AVFROMATOP_H_
#include "cacheManage.h"
class avFromatOp
{
public:
	avFromatOp();
	virtual ~avFromatOp();
	virtual int init()=0;
	virtual int createOneAvFile(Avdata* pdata,char* filename)=0;
	virtual int decodeOneAvFile(Avdata* pdata,char* filename)=0;
	virtual unsigned int getCurWriAvFileLen()=0;
	unsigned int m_curWriLen;
};

#endif /* TBAPP_COMMON_AVFROMATOP_H_ */
