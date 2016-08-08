/*
 * recordManage.h
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_RECORDMANAGE_RECORDMANAGE_H_
#define TBAPP_RECORDMANAGE_RECORDMANAGE_H_
#include "aviFormatOp.h"
// dev record const
typedef enum
{
	RECORD_DISK_STRATEGY_BEGIN = -1,
	RECORD_DISK_STRATEGY_STOP,		// ��������ʱֹͣ¼��
	RECORD_DISK_STRATEGY_DELETE,	// ��������ʱɾ�������ļ�
	RECORD_DISK_STRATEGY_END
} RECORD_DISK_STRATEGY_E;

typedef enum
{
	RECORD_FILE_STRATEGY_BEGIN = -1,
	RECORD_FILE_STRATEGY_STOP,		// ���ļ��ﵽ����ʱֹͣ¼��
	RECORD_FILE_STRATEGY_SWITCH,	// ���ļ��ﵽ����ʱ�л��ļ�
	RECORD_FILE_STRATEGY_END
} RECORD_FILE_STRATEGY_E;

typedef enum
{
	RECORD_STATUS_BEGIN = -1,
	RECORD_STATUS_STOP,			// ֹͣ¼��
	RECORD_STATUS_MANUAL,		// �����ֶ�¼��
	RECORD_STATUS_ALARM,		// ���ڱ���¼��
	RECORD_STATUS_MD,			// �����ƶ�¼��
	RECORD_STATUS_SCHEDULE,		// ���ڼƻ�����¼��
	RECORD_STATUS_JPEG_MANUAL,	// �����ֶ�ץͼ
	RECORD_STATUS_JPEG_ALARM,	// ���ڱ������뱨��ץͼ
	RECORD_STATUS_JPEG_MD,		// �����ƶ���ⱨ��ץͼ
	RECORD_STATUS_EXIT,			// �����˳�¼�񣬼�ֹͣ��ǰ���񲢲��ٿ�ʼ�µ�¼������
	RECORD_STATUS_DEL,          // ɾ������¼�Ƶ��ļ���ֹͣ
	RECORD_STATUS_END
} RECORD_STATUS_E;


#define ROOT_SAVE_PATH "/mnt/mmcblk0p5"
#define SD_DEVNAME "/dev/mmcblk0p5"
#define CUR_SAVE_PATH ROOT_SAVE_PATH"/DCIM"
#define MAX_INDEX       999
#define MAX_AVFILE_LEN (1*1024*1024)
class recordManage
{
public:
	recordManage();
	virtual ~recordManage();
	int init();
	int run();
private:
	int getOneAvdata();
	int saveOneAvToFile();
	int getFileName();
	int checkSdMount();
	int checkFileSizeExceed();
	int generate_record_dir(char *inPath);
	int rename_IndexFileName();
	int mountSd();
	int initcurFileIndex();
	uint32_t m_curMaxFileIndex;
	uint32_t m_curMaxDCIMIndex;
	unsigned char m_FileName[128];
	aviFormatOp* m_pAviOp;
	cacheManage* m_pCacheClient;
	Avdata m_Avdata;
	char first;
	RECORD_STATUS_E m_rectype;
};

#endif /* TBAPP_RECORDMANAGE_RECORDMANAGE_H_ */
