/*
 * avDataManage.cpp
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#include "avDataManage.h"
#include "jpegPro.h"
#include "audioPro.h"
#include <sys/time.h>
static uint64_t getus();
avDataManage::avDataManage()
{
	// TODO Auto-generated constructor stub
	m_pVideoPro=NULL;
	m_pcacheserver=NULL;
	m_pAudioPro=NULL;
	m_pAviOp=NULL;

}

avDataManage::~avDataManage()
{
	// TODO Auto-generated destructor stub
	if(m_pVideoPro)
		delete m_pVideoPro;
	if(m_pcacheserver){
		delete m_pcacheserver;
	}
	if(m_pAudioPro)
		delete m_pAudioPro;
	if(m_pAviOp)
		delete m_pAudioPro;
}

int avDataManage::init()
{
	m_pVideoPro = new jpegPro();
	m_pAudioPro = new audioPro();
	m_pAviOp = new aviFormatOp();

	m_pcacheserver = cacheManage::getinstance(server);
	if((m_pVideoPro->init()<0)||(m_pcacheserver->init()<0)||(m_pAudioPro->init()<0)||(m_pAviOp->init()<0)){
		return -1;
	}
	bzero(m_bufV,sizeof(m_bufV));
	bzero(m_bufA,sizeof(m_bufA));
	return 0;
}

int 	avDataManage::run()
{
	while(1){
		saveOneAvdataToCach();
	}
	return 0;
}

int avDataManage::getOneAvdataFromFile(char* filename,Avdata* av)
{

	int ret = m_pAviOp->decodeOneAvFile(av,filename);
	return ret;

}

int avDataManage::getOneFrame()
{
	av.lenV = VIDEOBUFFMAX;
	av.lenV = m_pAviOp->getOneFrame((char*) av.buffV);
	return av.lenV;
}
int avDataManage::palyOneFrame()
{
	m_pVideoPro->displayOneVideoFrame(av.buffV, av.lenV);
	return 0;
}
static void *playVideo(void *arg)
{
	uint64_t timebefore = 0, timeafter = 0, timestart = 0;
	int remain;
	avDataManage* pavM = (avDataManage*) arg;
	while (1) {

		if (pavM->getOneFrame() <= 0)
			break;
		if (0 == timestart) {
			timestart = getus();
		}
		pavM->palyOneFrame();
		timeafter = getus();
		if (0 == timebefore)
			remain = 40000 - (timeafter - timestart);
		else
			remain = 40000 - (timeafter - timebefore);
		if (remain > 0)
			usleep(remain);
		timebefore = getus();
	}
	pthread_exit((void *) 0);
}
int avDataManage::displayFile(char* filename)
{

	if (m_pAviOp->openAvFile(filename) < 0)
		return -1;

	pthread_t videoTid;
	if (pthread_create(&videoTid, NULL, playVideo, this) != 0) {
		DBG("Create thread playVideo error!\n");
		return -1;
	}
	uint64_t audiosleep;
	uint64_t timebefore = 0, timeafter = 0, timestart = 0;
	int remain;
	while (1) {

		av.lenA = AUDIOBUFFMAX;
		av.lenA=m_pAviOp->getOneAudio((char*)av.buffA,2048);
		if(av.lenA<=0)
			break;
		audiosleep=av.lenA*1000/32;
		if (0 == timestart) {
			timestart = getus();
		}
		m_pAudioPro->displayOneAudioFrame(av.buffA, av.lenA);

		timeafter = getus();
		if (0 == timebefore)
			remain = audiosleep - (timeafter - timestart);
		else
			remain = audiosleep - (timeafter - timebefore);
		if (remain > 0)
			usleep(remain);
		timebefore = getus();
	}
	DBG();
	if (pthread_join(videoTid, NULL) != 0) {
		printf("Join thread 2 error!\n");
	}
	DBG();
	m_pAviOp->closeAvFile(filename);
	return 0;
}

uint64_t getus()
{
    uint64_t cur_time = 0;
    struct timeval tm_now;
    gettimeofday(&tm_now, NULL);
    cur_time = (uint64_t)(tm_now.tv_sec*1000000 + tm_now.tv_usec);
    return cur_time;
}

int avDataManage::saveOneAvdataToCach()
{
	//Avdata av;
	static int msg = 2,count=1;
	bzero(m_bufV, sizeof(m_bufV));
	bzero(m_bufA, sizeof(m_bufA));
	av.buffA = m_bufA;
	av.lenA = AUDIOBUFFMAX;
	av.buffV = m_bufV;
	av.lenV = VIDEOBUFFMAX;
	m_pVideoPro->setDisplayZoom(NOZOOM);
	switch (msg) {
	case (0): {
		//normal
		//switchAudio++;
		count++;
		if(count%200==0)
			msg=1;
		m_pVideoPro->enableVideoForRec();
		m_pVideoPro->getOneVideoFrameFromDri(1, &(av.buffV), &av.lenV);
		m_pVideoPro->displayOneVideoFrame(0,0);
		m_pVideoPro->disableVideoForRec();

		m_pAudioPro->getOneAudioFrameFromDri(av.buffA, &av.lenA);

		m_pcacheserver->saveOneAvdata(&av);
		//DBG("audio len :%d",av.lenA);
	}
		break;
	case (1): {
		//photo
		m_pVideoPro->enableVideoForPhoto();
		m_pVideoPro->getOneVideoFrameFromDri(0, &(av.buffV), &av.lenV);
		m_pVideoPro->displayOneVideoFrame(0,0);
		m_pVideoPro->disableVideoForPhoto();
		msg++;
	}
		break;
	case (2): {
		//avi codec
		m_pVideoPro->enableVideoForCodec();
		displayFile("/mnt/tmp/DCIM/REC046.AVI");
		msg = 0;
/*		while (1) {
			timebefore = getms();
			av.lenA=AUDIOBUFFMAX;
			av.lenV = VIDEOBUFFMAX;
			if (0 == getOneAvdataFromFile("/mnt/tmp/DCIM/REC046.AVI", &av)) {
				msg = 0;
				break;
			}

			m_pVideoPro->displayOneVideoFrame(av.buffV, av.lenV);
			m_pAudioPro->displayOneAudioFrame(av.buffA, av.lenA);
			timeafter = getms();
			int remain = 40 - (timeafter - timebefore);
					if(remain>0)
			 usleep(remain*100);

		}*/

		m_pVideoPro->disableVideoForCodec();
	}

		break;
	default:
		break;
	}
	//m_pAudioPro
	return 0;
}
