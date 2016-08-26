/*
 * audioPro.cpp
 *
 *  Created on: 2016年8月22日
 *      Author: lixiang 21588
 */

#include "audioPro.h"
#include <sys/soundcard.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 #include <sys/ioctl.h>
#include "../common/debug.h"
#include "../common/cacheManage.h"
audioPro::audioPro()
{
	// TODO Auto-generated constructor stub
	 rec_volume=60;
	 play_volume=60;
	 speed=AACRECORDER_SAMPLE_RATE;
}

audioPro::~audioPro()
{
	// TODO Auto-generated destructor stub
    close(r_dsp);
	close(r_mixer);
	  close(p_dsp);
	  close(p_mixer);
	AACEnc_Finalize();
}

int audioPro::init()
{
	// record from external Codec (for example NAU8822)

	if ((r_dsp = open("/dev/dsp1",O_RDONLY|O_NONBLOCK,0)) == -1)
	{
		perror("/dev/dsp1");
		return -1;
	}
	printf("open dsp1 successful\n");
	/*  Setting Sampling Parameters
		1. sample format
		2. number of channels
		3. sampling rate
	*/
	int format = AFMT_S16_LE;
	if (ioctl(r_dsp,SNDCTL_DSP_SETFMT, &format) == -1)
	{
		perror("SNDCTL_DSP_SETFMT");
		return -1;
	}
	if (ioctl(r_dsp, SNDCTL_DSP_SPEED, &speed) == -1)
	{
		perror("SNDCTL_DSP_SPEED");
		return -1;
	}
	else
		printf("Actual Speed : %d \n",speed);


	/* Volume control */
	if ((r_mixer = open("/dev/mixer1", O_WRONLY)) == -1)
    {
      	perror("open /dev/mixer1 error");
        return -1;
    }
	printf("open mixer successful\n");

	ioctl(r_mixer , MIXER_WRITE(SOUND_MIXER_PCM), &rec_volume);
	printf("set mic volume = 0x%x   !!!\n", rec_volume);


	//open output audio
	printf("*****\n");
		p_dsp = open("/dev/dsp", O_RDWR);
		if( p_dsp < 0 ){
			printf("Open dsp error\n");
			return -1;
		}
	printf("====\n");
		p_mixer = open("/dev/mixer", O_RDWR);
		if( p_mixer < 0 ){
			printf("Open mixer error\n");
			return -1;
		}
		int data, oss_format, channels, sample_rate;
		data = 0x5050;
		oss_format=AFMT_S16_LE;/*standard 16bit little endian format, support this format only*/
		sample_rate = AACRECORDER_SAMPLE_RATE;
		channels = 1;
		ioctl(p_dsp, SNDCTL_DSP_SETFMT, &oss_format);
		ioctl(p_mixer, MIXER_WRITE(SOUND_MIXER_PCM), &data);
		ioctl(p_dsp, SNDCTL_DSP_SPEED, &sample_rate);
		ioctl(p_dsp, SNDCTL_DSP_CHANNELS, &channels);
	// Initialize AAC Encoder
	S_AACENC sEncoder;

	sEncoder.m_u32SampleRate = AACRECORDER_SAMPLE_RATE;
	sEncoder.m_u32ChannelNum = AACRECORDER_CHANNEL_NUM;
	sEncoder.m_u32BitRate = AACRECORDER_BIT_RATE * sEncoder.m_u32ChannelNum;
	sEncoder.m_u32Quality = AACRECORDER_QUALITY;
	sEncoder.m_bUseAdts = true;
	sEncoder.m_bUseMidSide = false;
	sEncoder.m_bUseTns = false;

	E_AACENC_ERROR eAACEnc_Error = AACEnc_Initialize(&sEncoder);

	if (eAACEnc_Error != eAACENC_ERROR_NONE) {
		DBG("AAC Recorder: Fail to initialize AAC Encoder: Error code 0x%08x\n", eAACEnc_Error);
		return -1;
	}

	return 0;
}

int audioPro::getOnePcmData(uint8_t* start, uint32_t *len)
{
	int count = -1;
	count=read(r_dsp, start, 2048);
	*len=count;
	//DBG("count %d",count);
	return 0;
}

int audioPro::getOneAacData(uint8_t* start, uint32_t *len)
{
	int count = -1;
	uint8_t bufpcm[2*1024]={0};
	count=read(r_dsp, bufpcm, 2*1024);
	//DBG("time %d",time(NULL));
	E_AACENC_ERROR eAACEnc_Error = AACEnc_EncodeFrame((short*)bufpcm, start, 1024, len);
	//DBG("time %d",time(NULL));
	if (eAACEnc_Error != eAACENC_ERROR_NONE) {
		DBG("AAC Recorder: Fail to encode file: Error code 0x%08x\n", eAACEnc_Error);
		return -1;		// Auto stop recorder
	} // if
	return 0;
}

int audioPro::getOneAudioFrameFromDri(uint8_t* start, uint32_t *len)
{
	return getOnePcmData(start,len);
}
int audioPro::displayOneAudioFrame(uint8_t* start, uint32_t len)
{
	if(0==len)
		return 0;
	write(p_dsp, start, len);
	return 0;
}
