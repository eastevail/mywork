/*
 * avDataPro.h
 *
 *  Created on: 2016年8月5日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_AVDATACACHE_AVDATAMANAGE_H_
#define TBAPP_AVDATACACHE_AVDATAMANAGE_H_
#include "../common/avDataPro.h"
#include "../common/cacheManage.h"
#include "../common/aviFormatOp.h"
class avDataManage
{
public:
	avDataManage();
	virtual ~avDataManage();
	int init();
	int run();
	int getOneFrame();
	int palyOneFrame();
private:
	avDataPro* m_pVideoPro;
	avDataPro* m_pAudioPro;
	aviFormatOp* m_pAviOp;
	cacheManage* m_pcacheserver;
	int saveOneAvdataToCach();
	int getOneAvdataFromFile(char* filename,Avdata* av);
	int displayFile(char* filename);
	uint8_t m_bufV[VIDEOBUFFMAX];
	uint8_t m_bufA[AUDIOBUFFMAX];
	Avdata av;
};

#endif /* TBAPP_AVDATACACHE_AVDATAMANAGE_H_ */
