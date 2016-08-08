/*
 * cacheManage.cpp
 *
 *  Created on: 2016年8月6日
 *      Author: lixiang 21588
 */



#include "cacheManage.h"

#include<stdio.h>
cacheManage* cacheManage::pclient=NULL;
cacheManage* cacheManage::pserver=NULL;
union semun
{
     int                val;
     struct semid_ds    *buf;
     unsigned short     *array;
};
cacheManage::cacheManage(cacheType type)
{
	// TODO Auto-generated constructor stub
	currentType = type;
}

cacheManage::~cacheManage()
{
	// TODO Auto-generated destructor stub
	if (shmdt(shmptr) == -1) {
		DBGERROR("shmdt failed\n");
	}
	//删除共享内存
	if (currentType == server) {
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			DBGERROR("shmctl(IPC_RMID) failed\n");
		}

		union semun sem_union;
		if (semctl(semid, 0, IPC_RMID, sem_union) == -1) {
			perror("semctl IPC_RMID error:");
		}
	}
}

cacheManage* cacheManage::getinstance(cacheType type)
{
	switch (type) {
	case client:
		if (!pclient) {
			pclient = new cacheManage(client);
		}
		return pclient;
		break;
	case server:
		if (!pserver) {
			pserver = new cacheManage(server);
		}
		return pserver;
		break;
	default:
		return NULL;
		break;
	}
}

int cacheManage::destroy()
{
	if (pclient) {
		delete pclient;
	}
	if (pserver) {
		delete pserver;
	}
	return 0;
}

int cacheManage::semP(int index)
{
	struct sembuf sem_b;
	sem_b.sem_num = index;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(semid, &sem_b, 1) == -1) {
		perror("p operation error:");
		return -1;
	}
	return 0;
}

int cacheManage::semV(int index)
{
	struct sembuf sem_b;
	sem_b.sem_num = index;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if (semop(semid, &sem_b, 1) == -1) {
		perror("V operation error:");
		return -1;
	}
	return 0;
}

int cacheManage::saveOneVideoFrame(unsigned char* start,int len,int index)
{
	memcpy(shmptr+pindexInSh->index[index].offV,start,len);
	pindexInSh->index[index].lenV=len;
	return 0;
}
int cacheManage::saveOneAudioFrame(unsigned char* start, int len, int index)
{
	memcpy(shmptr+pindexInSh->index[index].offA,start,len);
	pindexInSh->index[index].lenA=len;
	return 0;
}

uint32_t cacheManage::getIndexFromeId(uint32_t id)
{
	return id%INDEXNUM;
}
int cacheManage::saveOneAvdata(Avdata* pdata)
{
	if (currentType == server) {
		if (pindexInSh->newestId == CACHEERRORNUM)
			pindexInSh->newestId = 0;
		uint32_t index = getIndexFromeId(pindexInSh->newestId);
		if ((pdata->lenA + pdata->lenV) > (VIDEOBUFFMAX + AUDIOBUFFMAX)) {
			DBGERROR("av length is big than cache len\n");
			return -1;
		}
		semP(index);
		saveOneVideoFrame(pdata->buffV, pdata->lenV, index);
		saveOneAudioFrame(pdata->buffA, pdata->lenA, index);
		pindexInSh->newestId++;
		semV(index);
		return 0;
	} else {
		return -1;
	}
}
int cacheManage::getOneAvdata(Avdata* pdata)
{
	if (currentType == client) {
		uint32_t index = getIndexFromeId(pdata->id);
		if(index>=pindexInSh->newestId){
			DBGERROR("no more cache ,wait a monment\n");
			return -1;
		}
		if ((pdata->lenA + pdata->lenV) < (VIDEOBUFFMAX + AUDIOBUFFMAX)) {
			DBGERROR("av length is small than cache len\n");
			return -2;
		}
		semP(index);
		memcpy(pdata->buffV,shmptr+pindexInSh->index[index].offV,pindexInSh->index[index].lenV);
		pdata->lenV=pindexInSh->index[index].lenV;
		memcpy(pdata->buffA,shmptr+pindexInSh->index[index].offA,pindexInSh->index[index].lenA);
		pdata->lenA=pindexInSh->index[index].lenA;
		semV(index);
		return 0;
	} else {
		return -3;
	}
	return 0;
}
int cacheManage::setAvdataLen()
{
	avdataLen = VIDEOBUFFMAX+AUDIOBUFFMAX;
	return 0;
}
int cacheManage::shInit()
{
	switch (currentType) {
	case client: {
	    shmid = shmget((key_t)SHAREMEMKEY,0, 0666);
	    if(shmid == -1)  {
	        perror("shmget error in client:");
	        return -1;
	    }
	    shmptr = shmat(shmid, 0, SHM_RDONLY);
	    if(shmptr == (void*)-1)
	    {
	    	perror( "shmat failed in client");
	        return -1;
	    }
	    pindexInSh = (AVIndex*)shmptr;
	}
		break;
	case server: {
		int pagesize = getpagesize();
		sharememLen = ((indexlen + avdataLen * INDEXNUM) / pagesize + 1) * pagesize;
	    shmid = shmget((key_t)SHAREMEMKEY,sharememLen, 0666|IPC_CREAT|IPC_EXCL);
	    if(shmid == -1)  {
	        perror("shmget error in server:");
	        return -1;
	    }
	    shmptr = shmat(shmid, 0, 0);
	    if(shmptr == (void*)-1)
	    {
	    	perror( "shmat failed in server");
	        return -1;
	    }
	    pindexInSh = (AVIndex*)shmptr;
	    bzero(pindexInSh,sizeof(AVIndex));
	    pindexInSh->newestId=CACHEERRORNUM;
	    pindexInSh->oldestId=CACHEERRORNUM;
	    for(int i=0;i<INDEXNUM;i++){
	    	pindexInSh->index[i].id=i;
	    	pindexInSh->index[i].offV=indexlen+i*avdataLen;
	    	pindexInSh->index[i].lenV=VIDEOBUFFMAX;
	    	pindexInSh->index[i].offA=pindexInSh->index[i].offV+pindexInSh->index[i].lenV;
	    	pindexInSh->index[i].lenA=AUDIOBUFFMAX;
	    }
	}
		break;
	default:
		return -2;
		break;
	}
	return 0;
}


int cacheManage::semInit()
{
	switch (currentType) {
	case client: {
		semid = semget((key_t) SEMSMEMKEY, 0, 0666 );
		if (semid == -1) {
			perror("semget error in client:");
			return -1;
		}
	}
		break;
	case server: {
		semid = semget((key_t) SEMSMEMKEY, INDEXNUM, 0666 | IPC_CREAT | IPC_EXCL);
		if (semid == -1) {
			perror("semget error in server:");
			return -1;
		}
		struct semid_ds seminfo;
		unsigned short ptr[INDEXNUM];
		union semun arg;
		arg.buf = &seminfo;
		//获取信号量集的相关信息
		if (semctl(semid, 0, IPC_STAT, arg) < 0) {
			perror("semctl stat error:");
			return -1;
		}

		arg.array = ptr;
		//初始化信号量的值
		for (int i = 0; i < INDEXNUM; i++) {
			ptr[i] = 1;
		}
		//通过arg设置信号量集合
		if (semctl(semid, 0, SETALL, arg) < 0) {
			perror("semctl stat error:");
			return -1;
		}

	}
		break;
	default:
		return -2;
		break;
	}
	return 0;
}

int cacheManage::init()
{
	indexlen = sizeof(AVIndex);
	setAvdataLen();
	if((shInit()<0)||(semInit()<0))
		return -1;
	return 0;
}
