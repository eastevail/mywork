/*
 * aviFormatOp.h
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_RECORDMANAGE_AVIFORMATOP_H_
#define TBAPP_RECORDMANAGE_AVIFORMATOP_H_

#include "avFromatOp.h"

class aviFormatOp: public avFromatOp
{
public:
	aviFormatOp();
	virtual ~aviFormatOp();
	virtual int init();
	virtual int createOneAvFile(Avdata* pdata, unsigned char* filename);
	virtual unsigned int getCurWriAvFileLen();
private:
	char lastCallFilename[128];
};

#endif /* TBAPP_RECORDMANAGE_AVIFORMATOP_H_ */
