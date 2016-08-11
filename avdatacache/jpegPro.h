/*
 * jpegPro.h
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_JPEGPRO_H_
#define TBAPP_AVDATACACHE_JPEGPRO_H_

#include "../common/avDataPro.h"

class jpegPro: public avDataPro
{
public:
	jpegPro();
	virtual ~jpegPro();
	int init();
	int getOneVideoFrameFromDri(int rot_num,uint8_t* start, uint32_t *len);//rot_num :rotate angle
private:
};

#endif /* TBAPP_AVDATACACHE_JPEGPRO_H_ */
