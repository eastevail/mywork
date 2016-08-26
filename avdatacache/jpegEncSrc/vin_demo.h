/*
 * vin_demo.h
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_JPEGENCSRC_VIN_DEMO_H_
#define TBAPP_AVDATACACHE_JPEGENCSRC_VIN_DEMO_H_
#include<stdint.h>
#include "../../common/tb_tpye.h"
int initJpegPro(E_IMAGE_RESOL resol);
int getOneJpeg(ROTATEDIREC rot_num, uint8_t** buff, uint32_t* buflen, ZOOMLEVEL zoom, int takephoto,int SwitchRes);
int unInitJpecPro();
int displayVideoFrame();
int dispalyOneJpeg(uint8_t* buff, uint32_t buflen);
int __clearFb();


#endif /* TBAPP_AVDATACACHE_JPEGENCSRC_VIN_DEMO_H_ */
