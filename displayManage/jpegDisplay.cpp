/*
 * jpegDisplay.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#include "jpegDisplay.h"
#include "../common/debug.h"
#include<stdio.h>
#include <string.h>
#include <linux/unistd.h>
#include <unistd.h>
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
/*	FILE* fp;
	int ret;
	fp=fopen("/mnt/tmp/test.jpg","w+");
	if(fp){
		ret=fwrite(start,1,len,fp);
		if(ret==len){
			fclose(fp);
			sync();
		}else{
			DBGERROR("fwrite error");
		}
	}else{
		DBGERROR("fopen error:%s",strerror(errno));
	}*/

/*	FILE* fp;
	int ret;
	uint8_t buff[30*1024]={0};
	fp=fopen("/mnt/tmp/thumb0.jpg","r");
	if(fp){
		ret=fread(buff,1,30*1024,fp);
		if(ret>0){
			fclose(fp);
			sync();
		}else{
			DBGERROR("fread error");
		}
	}else{
		DBGERROR("fopen error:%s",strerror(errno));
	}
	DBG("len is %d",ret);*/
	return dispalyOneJpeg(start,len);
}
