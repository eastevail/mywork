/*
 * aviFormatOp.h
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_COMMON_AVIFORMATOP_H_
#define TBAPP_COMMON_AVIFORMATOP_H_

#include "avFromatOp.h"
extern "C" {
#include"avilib/avilib.h"
}
class aviFormatOp: public avFromatOp
{
public:
	aviFormatOp();
	virtual ~aviFormatOp();
	virtual int init();
	virtual int createOneAvFile(Avdata* pdata, char* filename);
	virtual int decodeOneAvFile(Avdata* pdata,char* filename);
	virtual unsigned int getCurWriAvFileLen();
private:
	char lastCallFilename[128];
	avi_t *out_fd;
	avi_t *read_fd;
};

#endif /* TBAPP_COMMON_AVIFORMATOP_H_ */
