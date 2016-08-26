/*
 * audioPro.h
 *
 *  Created on: 2016年8月22日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_AUDIOPRO_H_
#define TBAPP_AVDATACACHE_AUDIOPRO_H_
#include "../common/avDataPro.h"
#include"audioEncSrc/AACEncoder.h"

#define AACRECORDER_BIT_RATE	64000	// 64K bps
#define AACRECORDER_CHANNEL_NUM	1		// Mono
#define AACRECORDER_QUALITY		90		// 1 ~ 999
#define AACRECORDER_SAMPLE_RATE	16000	// 16K Hz
#define AACRECORDER_SAMPLE_RATE_8	8000	// 8K Hz
class audioPro: public avDataPro
{
public:
	audioPro();
	virtual ~audioPro();
	virtual int init();
	virtual int getOneAudioFrameFromDri(uint8_t* start, uint32_t *len); //change len to the real lenth
	virtual int displayOneAudioFrame(uint8_t* start, uint32_t len);
private:
	int getOnePcmData(uint8_t* start, uint32_t *len);
	int getOneAacData(uint8_t* start, uint32_t *len);
	int r_dsp, r_mixer,	speed;
	int p_dsp, p_mixer;
	volatile int rec_volume, play_volume;
};

#endif /* TBAPP_AVDATACACHE_AUDIOPRO_H_ */
