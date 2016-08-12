/*
 * jpegPro.cpp
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#include "jpegPro.h"
#include "../common/tb_tpye.h"
extern "C" {
#include"jpegEncSrc/vin_demo.h"
}
jpegPro::jpegPro()
{
	// TODO Auto-generated constructor stub

}

jpegPro::~jpegPro()
{
	// TODO Auto-generated destructor stub
	unInitJpecPro();
}

int jpegPro::getOneVideoFrameFromDri(int rot_num,uint8_t** start, uint32_t *len)
{
	return getOneJpeg(rot_num,start,len);
}

int jpegPro::init()
{
	return initJpegPro(eIMAGE_QVGA);
}
