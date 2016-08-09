/*
 * cacheManage.h
 *
 *  Created on: 2016年8月4日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_COMMON_CACHEMANAGE_H_
#define TBAPP_COMMON_CACHEMANAGE_H_
#include <unistd.h>
#include <linux/unistd.h>
#include<stdio.h>
#include<stdint.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "debug.h"
#define CACHEERRORNUM 0xffffffff
#define INDEXNUM 90
#define SHAREMEMKEY 666888
#define SEMSMEMKEY 888666
#define VIDEOBUFFMAX (26*1024)
#define AUDIOBUFFMAX (4*1024)
typedef struct{
	uint32_t offV;//correspond to shmptr
	uint32_t lenV;
	uint32_t offA;//correspond to shmptr
	uint32_t lenA;
	uint32_t id;
}AvdataIndex;

typedef struct{
	uint8_t* buffV;//correspond to shmptr
	uint32_t lenV;
	uint8_t* buffA;//correspond to shmptr
	uint32_t lenA;
	uint32_t id;
}Avdata;

typedef struct{
	uint32_t newestId;//will write id
	uint32_t oldestId;
	AvdataIndex index[INDEXNUM];
}AVIndex;

enum cacheType{
	client,
	server,
};

class cacheManage
{
public:
	virtual ~cacheManage();
	static cacheManage* getinstance(cacheType type);
	static int destroy();
	int saveOneAvdata(Avdata* pdata);
	int getOneAvdata(Avdata* pdata);//when first get,id is 0
	int init();
private:
	cacheManage(cacheType type);
	int saveOneVideoFrame(unsigned char* start,int len,int index);
	int saveOneAudioFrame(unsigned char* start, int len, int index);
	static cacheManage* pclient;
	static cacheManage* pserver;
	AVIndex* pindexInSh;
	int waitForNextAvFrame(); //call by getOneAvdata
	int setAvdataLen();
	int setOldestId();
	uint32_t getIndexFromeId(uint32_t id);
	int shInit();
	int semInit();
	int shmid;
	int semid;
	int semP(int index);
	int semV(int index);
	cacheType currentType;
	int indexlen;
	uint8_t* shmptr;
	uint32_t avdataLen;
	uint32_t sharememLen;
};

#endif /* TBAPP_COMMON_CACHEMANAGE_H_ */
