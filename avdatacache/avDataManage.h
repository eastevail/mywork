/*
 * avDataPro.h
 *
 *  Created on: 2016年8月5日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_AVDATAMANAGE_H_
#define TBAPP_AVDATACACHE_AVDATAMANAGE_H_
#include "../common/avDataPro.h"
#include "../common/cacheManage.h"
class avDataManage
{
public:
	avDataManage();
	virtual ~avDataManage();
	int init();
	int run();
private:
	avDataPro* m_pVideoPro;
	cacheManage* m_pcacheserver;
	int saveOneAvdataToCach();
	uint8_t m_bufV[VIDEOBUFFMAX];
	uint8_t m_bufA[AUDIOBUFFMAX];
};

#endif /* TBAPP_AVDATACACHE_AVDATAMANAGE_H_ */
