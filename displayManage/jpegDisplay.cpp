/*
 * jpegDisplay.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#include "jpegDisplay.h"
#include "../common/debug.h"
#include<stdio.h>
extern "C" {
extern int dispalyOneJpeg(uint8_t* buff, uint32_t buflen);
extern int unInitJpegCodec();
extern int initJpegCodec();
}

jpegDisplay::jpegDisplay()
{
	// TODO Auto-generated constructor stub

}

jpegDisplay::~jpegDisplay()
{
	// TODO Auto-generated destructor stub
	unInitJpegCodec();
}

int jpegDisplay::init()
{
	return initJpegCodec();
}

int jpegDisplay::displayOneVideoFrame(uint8_t* start, uint32_t len)
{
	if(len<=0){
		DBGERROR("len is 0");
		return -1;
	}
	DBG("len is %d",len);
	return dispalyOneJpeg(start,len);
}
