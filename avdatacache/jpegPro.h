/*
 * jpegPro.h
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_JPEGPRO_H_
#define TBAPP_AVDATACACHE_JPEGPRO_H_

#include "../common/avDataPro.h"
#include "../common/tb_tpye.h"
typedef enum{
	videoforpho=0,
	videoforrec,
}CURSTATUS;
class jpegPro: public avDataPro
{
public:
	jpegPro();
	virtual ~jpegPro();
	int init();
	int getOneVideoFrameFromDri(int rot_num,uint8_t** start, uint32_t *len);//rot_num :rotate angle
	int setDisplayZoom(ZOOMLEVEL level);
	int enableVideoForPhoto();
	int disableVideoForPhoto();
	int enableVideoForRec();
	int disableVideoForRec();
private:
	ZOOMLEVEL mZoomLev;
	int mIsVideoForPhoto;
	int needSwitchRes;
	CURSTATUS curStatus;

};

#endif /* TBAPP_AVDATACACHE_JPEGPRO_H_ */
