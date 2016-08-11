/*
 * jpegDisplay.h
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_DISPLAYMANAGE_JPEGDISPLAY_H_
#define TBAPP_DISPLAYMANAGE_JPEGDISPLAY_H_
#include "../common/avDataPro.h"

class jpegDisplay: public avDataPro
{
public:
	jpegDisplay();
	virtual ~jpegDisplay();
	virtual int init();
	virtual int displayOneVideoFrame(uint8_t* start, uint32_t len);
};

#endif /* TBAPP_DISPLAYMANAGE_JPEGDISPLAY_H_ */
