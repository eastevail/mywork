/*
 * avDataManage.cpp
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */

#include "avDataManage.h"
#include "jpegPro.h"
avDataManage::avDataManage()
{
	// TODO Auto-generated constructor stub

}

avDataManage::~avDataManage()
{
	// TODO Auto-generated destructor stub
	if(m_pVideoPro)
		delete m_pVideoPro;
	if(m_pcacheserver){
		delete m_pcacheserver;
	}

}

int avDataManage::init()
{
	m_pVideoPro = new jpegPro();
	m_pcacheserver = cacheManage::getinstance(server);
	if((m_pVideoPro->init()<0)||(m_pcacheserver->init()<0)){
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

int avDataManage::saveOneAvdataToCach()
{
	Avdata av;
	bzero(m_bufV,sizeof(m_bufV));
	bzero(m_bufA,sizeof(m_bufA));
	av.buffA=m_bufA;
	av.lenA=AUDIOBUFFMAX;
	av.buffV=m_bufV;
	av.lenV=VIDEOBUFFMAX;
	m_pVideoPro->getOneVideoFrameFromDri(0,av.buffV,&av.lenV);
	//m_pAudioPro
	m_pcacheserver->saveOneAvdata(&av);

	return 0;
}
