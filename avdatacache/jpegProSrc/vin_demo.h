/*
 * vin_demo.h
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_JPEGPROSRC_VIN_DEMO_H_
#define TBAPP_AVDATACACHE_JPEGPROSRC_VIN_DEMO_H_
#include<stdint.h>

int initJpegPro();
int getOneJpeg(int rot_num, uint8_t* buff, uint32_t* buflen);
int unInitJpecPro();



#endif /* TBAPP_AVDATACACHE_JPEGPROSRC_VIN_DEMO_H_ */
