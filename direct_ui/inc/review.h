/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: review.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.26
******************************************************************************/

#ifndef __REVIEW_H__
#define __REVIEW_H__

#include <map>

#include "abspage.h"
#include "label_bar.h"
#include "modaldialog.h"


#define MAX_FILE                2000            // ���֧���������ļ�����
#define MAX_FILE_PATH           128

#define FILE_IMG                0x0001      // ͼƬ�ļ�
#define FILE_REC                0x0002      // ¼���ļ�
#define FILE_FLAG               0x0004      // ����ļ�

typedef struct
{
    int                 fileType;
    time_t              lastAccesstime;           // time of last modification
    char                szFilePath[MAX_FILE_PATH];
	int					width;
	int 				height;
} File_Attr_S;

typedef enum
{
    List_Begin = -1,
    List_Play,
    List_Edit,
    List_Delete,
    List_DoFlag,
    List_UnDoFlag,
    List_Done,
    List_End,
} List_Cmd_E;

class CReviewPage : public CAbsPage
{
public:

    CReviewPage(CAbsPage *parentPage = NULL);

    ~CReviewPage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* �����Զ����¼� */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /* ��ȡ��ǰѡ�в��ŵ��ļ� */
    BOOL GetCurPlayFile(char *szFile, int iLen);

    /*  */
    void Recover();


	int GetJPEGFileSize(char *filePath,int *width ,int *height );


	static void OnDeleteOKCB(void *ctx);

	/* ���溯�� */
	void OnDeleteOK();

	/* ȥ����ť�ص����� */
	static void OnCancelCB(void *ctx);

	/* ȡ������ */
	void OnCancel();

	double GetCurFileduration(){return m_curFiledration;}
	long GetAfterFileduration(){return m_afterFileDuration;}

	void DeleteSubPage();
	void UpdateMsg();
	virtual void OnUpdateArrow();

private:

    /*  */
    BOOL InitFrame();

    /*  */
    void DestoryFrame();

    /* �����ļ�, ���ݹ�����Ŀ¼ */
   // int SearchFiles(const char *szPath = "/media/mmcblk0p1/DCIM001");
	int SearchFiles();

	void delFileList();

    /* �޴洢������ʾ��Ϣ */
    void ShowNoStorageMsg(int type);

    /* ��ȡ�ļ��ܵĸ��� */
    int GetTotalFileCount(void);

	int GetRecFileDuration(char *pFilePath,char *pTimeText);


    /* ���ݹ���������������ļ��� */
    BOOL FilterFiles(const char *szFileName);

    /* ����key��map�в����ļ� */
    BOOL GetFileByKey(int keyIndex, File_Attr_S *fileAttr);

	void ShowPreFileBG(File_Attr_S *fileAttr);

	void ShowNextFileBG(File_Attr_S *fileAttr);

    void ShowFileBG(File_Attr_S *fileAttr);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

    /*  */
    static void AfterDrawShowBGCB(void *data);

    /*  */
    void AfterDrawShowBG(void);

    /* ��������¼� */
    void OnLeftKeyEvent(void);

    /* �Ҽ������¼� */
    void OnRightKeyEvent(void);

    /* �ϼ������¼� */
    void OnUpKeyEvent(void);

    /* �¼������¼� */
    void OnDownKeyEvent(void);

    /* OK�������¼� */
    void OnOKKeyEvent(void);

    /* SETTING�������¼� */
    void OnSettingKeyEvent(void);

    /* REPLAY�������¼� */
    void OnReplayKeyEvent(void);

    /* video/capture�������¼� */
    void OnVideoCaptureKeyEvent(void);

    /* ��ʼ�������� */
    void InitListBox(void);

    /* list ��Ļص����� */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /*  */
    void DestoryListBox(void);

    /* */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

    /* */
    void DoCmdPlayFile(void);

    /* */
    void DoCmdEditFile(void);

    /*  */
    void DoCmdDeleteFile(void);

    /*  */
    void DoCmdFlagFile();

    /*  */
    void DoCmdDone(void);

private:

    /*  */
    CLabelBar*                          m_leftBar;

    /*  */
    CLabelBar*                          m_rightBar;

    /*  */
    LiteLabel*                          m_topLabel;

    /*  */
	LiteImage*                          m_showImage90;
    LiteImage*                          m_showImage;
	LiteImage*                          m_showPreImage;
	LiteImage*                          m_showNextImage;

	LiteImage*                          m_ReviewImage;

    /* �ļ�map */
  //  std::map<int,File_Attr_S>           m_fileMap;

	struct dirent **                    m_filelist;
	int                                 m_fileNum;

    /* ��ǰ��ʾ�ļ���map�е�index */
    int                                 m_curKeyIndex;

    /*  */
    LiteImage*                          m_noStorageMsg;
	LiteLabel*							m_lblStorageMsg;

    LiteLabel*                          m_bottomLabel;

    LiteLabel*                          m_fileNameLabel;

    LiteImage*                          m_fileTypeImage;

    LiteImage*                          m_fileFlagImage;

    LiteList*                           m_listBox;

	LiteLabel*							m_recdration;

	CModalDialog*                       m_modalDialog;

    /*  */
    CAbsPage*                           m_subPagePtr;

	double                              m_curFiledration;

	long m_afterFileDuration;
};

#endif

