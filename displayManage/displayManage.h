/*
 * displayManage.h
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_DISPLAYMANAGE_DISPLAYMANAGE_H_
#define TBAPP_DISPLAYMANAGE_DISPLAYMANAGE_H_
#include "../common/avDataPro.h"
#include "../common/cacheManage.h"
#include "../common/aviFormatOp.h"
class displayManage
{
public:
	displayManage();
	virtual ~displayManage();
	int init();
	int run();
private:
	avDataPro* m_pJpegPro;
	cacheManage* m_pcachcCient;
	aviFormatOp* m_pAviOp;
	int getOneAvdata(char* filename);
	int getOneAvdataFromCache();
	int getOneAvdataFromFile(char* filename);
	int displayOneAvdata();
	Avdata m_Avdata;
	char first;
};

#endif /* TBAPP_DISPLAYMANAGE_DISPLAYMANAGE_H_ */
