/*
 * recordManage.cpp
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#include "recordManage.h"
#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>

recordManage::recordManage()
{
	// TODO Auto-generated constructor stub
	m_pAviOp=NULL;
	m_pCacheClient =NULL;
	bzero(&m_Avdata,sizeof(m_Avdata));
	bzero(m_FileName,sizeof(m_FileName));
	first = 1;
	m_curMaxFileIndex =0;
}

recordManage::~recordManage()
{
	// TODO Auto-generated destructor stub
	if(m_pAviOp)
		delete m_pAviOp;
	if(m_pCacheClient)
		delete m_pCacheClient;
	if(m_Avdata.buffV)
		delete m_Avdata.buffV;
	if(m_Avdata.buffA)
		delete m_Avdata.buffA;
}

static bool StorageMountCheck(char *devName)
{
	FILE *fp = NULL;
    char line[512] = {0};
    char dev[64] = {0};
    char point[64] = {0};
    char fsType[64] = {0};
    bool bRet = false;

	//DBG("StorageMountCheck, devName: %s\n", devName);

    if (NULL == (fp = fopen("/proc/mounts", "r")))
    {
    	DBGERROR("fopen /proc/mounts  errmsg:%s", strerror(errno));
		goto ERR_EXIT;
    }

    while( fgets(line, sizeof(line)-1, fp) )
    {
    	DBG();
        sscanf(line, "%s %s %s", dev, point, fsType);
		DBG("dev: %s, point: %s, fsType: %s\n", dev, point, fsType);
        DBG();
        if (strstr(dev, devName))
        {
        	DBG("StorageMountCheck 111, dev: %s\n", dev);
            bRet =  true;
			break;
        }
        else
        {
            char *ptr = strrchr(point, '/');
            DBG();
            if (ptr && !strcmp(ptr + 1, devName))
            {
            	DBG("StorageMountCheck 222, ptr: %s\n", ptr);
                bRet = true;
				break;
            }
        }
    }

    fclose(fp);

ERR_EXIT:
	DBG("StorageMountCheck, bRet = %d\n", bRet);
    return bRet;
}
int recordManage::checkSdMount()
{
	int ret;
	ret=StorageMountCheck(SD_DEVNAME);
	if(ret==true){
		return 0;
	}else
		return -1;
}
int recordManage::mountSd()
{
	return 0;
}

int recordManage::init()
{
	int ret;
	m_pAviOp = new aviFormatOp();
	m_pCacheClient = cacheManage::getinstance(client);
	if(m_pAviOp->init()<0)
		return -1;
	if(m_pCacheClient->init()<0)
		return -1;
	ret = checkSdMount();
	CHECKRET(ret);
	ret =initcurFileIndex();
	CHECKRET(ret);
	ret=generate_record_dir(CUR_SAVE_PATH);
	CHECKRET(ret);
	m_rectype = RECORD_STATUS_MANUAL;
	m_Avdata.buffV=(uint8_t*)malloc(VIDEOBUFFMAX);
	m_Avdata.buffA=(uint8_t*)malloc(AUDIOBUFFMAX);
	m_Avdata.lenA=AUDIOBUFFMAX;
	m_Avdata.lenV=VIDEOBUFFMAX;
	return 0;
}
int recordManage::getOneAvdata()
{

	m_Avdata.lenA = AUDIOBUFFMAX;
	m_Avdata.lenV = VIDEOBUFFMAX;
	if (first) {
		m_Avdata.id = CACHEERRORNUM;
		first = 0;
	}
	 int ret=m_pCacheClient->getOneAvdata(&m_Avdata);
	 if(ret==0){
		 m_Avdata.id++;
	 }
	 return ret;
}

int 	recordManage::run()
{
	int ret;

	while(1){
		ret =getOneAvdata();
		if(-1==ret){
			usleep(10000);//10ms
			continue;
		}else{
			CHECKRET(ret);
		}
		ret=saveOneAvToFile();
		CHECKRET(ret);
	}
	return 0;
}

static int searchRECFileNumber(char *path)
{
	DIR *dirptr = NULL;
	struct dirent *entry;
	char szFileName[64] = { 0 };

	char szPrefix[16];
	char szExten[8];
	char szVal[5];
	int val = 0;
	int curnum = 0;

	int count = 1;

	if (path == NULL)
		return -1;

	do {
		if ((dirptr = opendir(path)) == NULL) {
			DBGERROR("open dir %s ,%d fail!\ns", path, count);
			count++;
			usleep(100000);
			continue;
		} else
			break;
	} while (count < 4);

	if (count >= 4) {
		return 1;
	}

	while (entry = readdir(dirptr)) {
		//	printf("%s\n", entry->d_name);

		memset(szFileName, 0, sizeof(szFileName));

		strcpy(szFileName, entry->d_name);

		memset(szPrefix, 0, sizeof(szPrefix));
		memset(szExten, 0, sizeof(szExten));
		sscanf(szFileName, "%[^.].%s", szPrefix, szExten);

		if (strncasecmp(szExten, "AVI", 3) && strncasecmp(szExten, "JPG", 3)) {
			continue;
		}

		if (strncasecmp(szPrefix, "REC", 3) && strncasecmp(szPrefix, "IMG", 3)
				&& strncasecmp(szPrefix, "FREC", 4) && strncasecmp(szPrefix, "FIMG", 4)) {
			continue;
		}

		memset(szVal, 0, sizeof(szVal));

		sscanf(szPrefix, "%*[^0-9]%[0-9]", szVal);
		curnum = atoi(szVal);

		if (curnum > val)
			val = curnum;

	}

	closedir(dirptr);

	return val;

}

static int searchDCIMNumber(char *path)
{
	DIR *dirptr = NULL;
	struct dirent *entry;
	char curName[64];
	char *ptr = NULL;
	int ret = 0;
	int curnum = 0;
	int count = 1;

	if (path == NULL)
		return 0;

	do {
		if ((dirptr = opendir(path)) == NULL) {
			DBGERROR("open dir %s ,%d fail!\ns",path,count);
			DBGERROR("opendir :%s", strerror(errno));
			count++;

			usleep(100000);
			continue;
		} else
			break;
	} while (count < 4);
	if (count >= 4) {
		return -1;
	}
	while (entry = readdir(dirptr)) {
		//printf("%s\n", entry->d_name);
		memset(curName, 0, sizeof(curName));
		strcpy(curName, entry->d_name);
		if ((ptr = strcasestr(curName, "DCIM")) != NULL) {
			curnum = atoi(ptr + 4);
			if (ret < curnum)
				ret = curnum;
		}
	}
	closedir(dirptr);
	return ret;
}

int recordManage::initcurFileIndex()
{
	int ret;
	ret=generate_record_dir(CUR_SAVE_PATH);
	CHECKRET(ret);
	m_curMaxFileIndex = searchRECFileNumber(CUR_SAVE_PATH);
	CHECKRET(m_curMaxFileIndex);
	m_curMaxDCIMIndex = searchDCIMNumber(ROOT_SAVE_PATH);
	return 0;
}

int recordManage::rename_IndexFileName()
{
	int ret = 0;
	char otherFilePath[256] = { 0, };

	memset(otherFilePath, 0, sizeof(otherFilePath));
	sprintf(otherFilePath, "%s/DCIM%03d", ROOT_SAVE_PATH, ++m_curMaxDCIMIndex);
	rename(CUR_SAVE_PATH, otherFilePath);

	DBG("rename devpath! %s\n", otherFilePath);
	return generate_record_dir(CUR_SAVE_PATH);
}

int recordManage::generate_record_dir(char *inPath)
{
    struct stat theStatBuffer;
    if (stat(inPath, &theStatBuffer) == -1)
    {
        //this directory doesn't exist, so let's try to create it
        if (mkdir(inPath, S_IRWXU |S_IRGRP | S_IXGRP|S_IROTH | S_IXOTH) == -1)
            return -1;
    }

    //directory exists
    return 0;
}

int recordManage::checkFileSizeExceed()
{
	return (m_pAviOp->getCurWriAvFileLen())>MAX_AVFILE_LEN?1:0;
}

int 	recordManage::getFileName()
{
	char szExt[4];
	char szPrefix[32] = { 0 };
	if (strlen((const char*) m_FileName)) {
		if(!checkFileSizeExceed()){
			return 0;
		}
	}
	DBG("size is %d",m_pAviOp->getCurWriAvFileLen());
	bzero(m_FileName, sizeof(m_FileName));
	m_curMaxFileIndex++;
	if (m_curMaxFileIndex < 1 || m_curMaxFileIndex > MAX_INDEX) {
		rename_IndexFileName();
		m_curMaxFileIndex = 1;
	}

	memset(szExt, 0, sizeof(szExt));
	memset(szPrefix, 0, sizeof(szPrefix));
	if (RECORD_STATUS_MANUAL == m_rectype || RECORD_STATUS_ALARM == m_rectype
			|| RECORD_STATUS_MD == m_rectype || RECORD_STATUS_SCHEDULE == m_rectype) {
		sprintf(szExt, "AVI");
		sprintf(szPrefix, "REC");
	} else if (RECORD_STATUS_JPEG_MANUAL == m_rectype || RECORD_STATUS_JPEG_ALARM == m_rectype
			|| RECORD_STATUS_JPEG_MD == m_rectype) {
		sprintf(szExt, "JPG");
		sprintf(szPrefix, "IMG");
	}
	sprintf((char*) m_FileName, CUR_SAVE_PATH"/%s%03d.%s", szPrefix, m_curMaxFileIndex, szExt);
	return 0;
}
int recordManage::saveOneAvToFile()
{
	int ret=0;
	ret = getFileName();
	CHECKRET(ret);
	ret=m_pAviOp->createOneAvFile(&m_Avdata,m_FileName);
	CHECKRET(ret);
	return 0;
}
