/*
 * avDataPro.h
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_AVDATAPRO_H_
#define TBAPP_AVDATACACHE_AVDATAPRO_H_
#include<stdint.h>
class avDataPro
{
public:
	avDataPro();
	virtual ~avDataPro();
	virtual int init()=0;
	virtual int getOneVideoFrameFromDri(int rot_num,uint8_t* start, uint32_t *len);//change len to the real lenth
	virtual int getOneAudioFrameFromDri(uint8_t* start, uint32_t *len);//change len to the real lenth
};

#endif /* TBAPP_AVDATACACHE_AVDATAPRO_H_ */
