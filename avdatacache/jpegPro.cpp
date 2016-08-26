/*
 * jpegPro.cpp
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#include "jpegPro.h"
#include "debug.h"
#include<stdio.h>
extern "C" {
#include"jpegEncSrc/vin_demo.h"
}
static const int FIRSTSWITCH =0x1234;
jpegPro::jpegPro()
{
	// TODO Auto-generated constructor stub
	mZoomLev=NOZOOM;
	mIsVideoForPhoto=0;
	needSwitchRes=FIRSTSWITCH;
	curStatus=videoforrec;
}

jpegPro::~jpegPro()
{
	// TODO Auto-generated destructor stub
	unInitJpecPro();
}

int jpegPro::getOneVideoFrameFromDri(int rot_num,uint8_t** start, uint32_t *len)
{
	return getOneJpeg((ROTATEDIREC)rot_num,start,len,mZoomLev,mIsVideoForPhoto,needSwitchRes);
}

int jpegPro::displayOneVideoFrame(uint8_t* start, uint32_t len)
{
	if (curStatus != videoforCodec) {
		displayVideoFrame();
	}else{
		dispalyOneJpeg(start,len);
	}
	return 0;
}

int jpegPro::init()
{
	return initJpegPro(eIMAGE_VGA);
}

int jpegPro::setDisplayZoom(ZOOMLEVEL level)
{
	mZoomLev=level;
	return 0;
}
int jpegPro::clearFb()
{
	DBG();
	return __clearFb();
}
int jpegPro::enableVideoForPhoto()
{
	mIsVideoForPhoto = 1;
	if ((FIRSTSWITCH == needSwitchRes)||(curStatus!=videoforpho)){
		clearFb();
		needSwitchRes = 1;
		curStatus=videoforpho;
	}else if(curStatus==videoforpho){
		needSwitchRes = 0;
		curStatus=videoforpho;
	}
	return 0;
}
int jpegPro::disableVideoForPhoto()
{
	return 0;
}
int jpegPro::enableVideoForRec()
{
	mIsVideoForPhoto = 0;
	if ((FIRSTSWITCH == needSwitchRes)||(curStatus!=videoforrec)){
		clearFb();
		needSwitchRes = 1;
		curStatus=videoforrec;
	}else if(curStatus==videoforrec){
		needSwitchRes = 0;
		curStatus=videoforrec;
	}
	return 0;
}
int jpegPro::disableVideoForRec()
{
	return 0;
}
int jpegPro::enableVideoForCodec()
{
	mIsVideoForPhoto = 0;
	if ((FIRSTSWITCH == needSwitchRes)||(curStatus!=videoforCodec)){
		clearFb();
		needSwitchRes = 1;
		curStatus=videoforCodec;
	}else if(curStatus==videoforCodec){
		needSwitchRes = 0;
		curStatus=videoforCodec;
	}
	return 0;
}
int jpegPro::disableVideoForCodec()
{
	return 0;
}
