/*
 * displayManage.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#include "displayManage.h"
#include "jpegDisplay.h"
displayManage::displayManage()
{
	// TODO Auto-generated constructor stub
	m_pJpegPro=NULL;
	m_pcachcCient =NULL;
	m_pAviOp=NULL;
	bzero(&m_Avdata,sizeof(m_Avdata));
	first = 1;
}

displayManage::~displayManage()
{
	// TODO Auto-generated destructor stub
	if(m_pJpegPro)
		delete m_pJpegPro;
	if(m_pAviOp)
		delete m_pAviOp;
	if(m_pcachcCient){
		delete m_pcachcCient;
	}
	if(m_Avdata.buffV)
		delete m_Avdata.buffV;
	if(m_Avdata.buffA)
		delete m_Avdata.buffA;
}

int displayManage::init()
{
	m_pJpegPro = new jpegDisplay();
	if(m_pJpegPro->init()<0)
		return -1;
/*	m_pcachcCient = cacheManage::getinstance(client);
	if(m_pcachcCient->init()<0)
		return -1;*/
	m_pAviOp = new aviFormatOp();
	if(m_pAviOp->init()<0)
		return -1;
	m_Avdata.buffV=(uint8_t*)malloc(VIDEOBUFFMAX);
	m_Avdata.buffA=(uint8_t*)malloc(AUDIOBUFFMAX);
	m_Avdata.lenA=AUDIOBUFFMAX;
	m_Avdata.lenV=VIDEOBUFFMAX;
	return 0;
}

int displayManage::getOneAvdataFromFile(char* filename)
{
	m_Avdata.lenA = AUDIOBUFFMAX;
	m_Avdata.lenV = VIDEOBUFFMAX;
	int ret = m_pAviOp->decodeOneAvFile(&m_Avdata,filename);
	return ret;

}

int displayManage::getOneAvdataFromCache()
{
	m_Avdata.lenA = AUDIOBUFFMAX;
	m_Avdata.lenV = VIDEOBUFFMAX;
	if (first) {
		m_Avdata.id = CACHEERRORNUM;
		first = 0;
	}
	 int ret=m_pcachcCient->getOneAvdata(&m_Avdata);
	 if(ret==0){
		 m_Avdata.id++;
	 }
	 return ret;

}
int displayManage::getOneAvdata(char* filename)
{
	if (filename) {
		return getOneAvdataFromFile(filename);
	} else {
		return getOneAvdataFromCache();
	}
}

int displayManage::displayOneAvdata()
{
	int ret=m_pJpegPro->displayOneVideoFrame(m_Avdata.buffV,m_Avdata.lenV);
	CHECKRET(ret);
	return 0;
}
int 	displayManage::run()
{
	int ret;

	while(1){
		ret =getOneAvdata("/mnt/sdcard2/DCIM/REC010.AVI");
		if(-1==ret){
			usleep(10000);//10ms
			continue;
		}else{
			CHECKRET(ret);
		}
		ret = displayOneAvdata();
		CHECKRET(ret);
	}
	return 0;
}
