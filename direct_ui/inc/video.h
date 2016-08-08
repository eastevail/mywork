/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: video.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "abspage.h"
#include "arrow.h"
#include "modaldialog.h"

typedef enum
{
    Record_Mode_Record = 0,
    Record_Mode_Snap,
} Record_Mode_E;

#define     UPDATE_BLOCK_LEN        1 * 1024
#define     UPDATE_TOTAL_TIME       120         // ��

#define     UPDATE_ROCKBRIGHT_TIME      1 * 1000        // microseconds
#define     fifo_name              "/tmp/top_fifo"

class CVideoPage : public CAbsPage
{
public:
    CVideoPage(CAbsPage *parentPage = NULL);
    ~CVideoPage();

	BOOL abnormalStopRecord();

	void UpdateRockLed();
	void UpdateMsg();
	void InitRecTimeMic();
	void UpdateRecTimeMic();
	void UpdateRecTimeStamp();

	virtual void OnUpdateArrow();

private:
    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	virtual void OnWindowEvent(Window_Msg_S *winMsg);

private:

	friend class CDesktop;

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

    /* ��ʾ¼��ʱ�� */
    BOOL BeginShowRecordTimeText(const char *szTimeText);

    void ShowRecordTimeText(const char *szTimeText);

    BOOL CancelShowRecordTimeText();

    /* ����ʱ����ʾ */
    void RecordTimerEvent();

    /* ��ʾ�������ʵʱ��Ƶ��ʶ */
    BOOL ShowVideoFlag(BOOL bVisible);

    /* ��ʾ/����¼��״̬ͼ�� */
    BOOL ShowRecordFlag(BOOL bVisible);

	// Save video file
    /* ���水ť�ص����� */
    static void OnSaveCB(void *ctx);
    /* ���溯�� */
    void OnSave();

    /* ȥ����ť�ص����� */
    static void OnCancelCB(void *ctx);
    /* ȡ������ */
    void OnCancel();


	// Save photo file
	/* ���水ť�ص����� */
	static void OnPhotoSaveCB(void *ctx);
    /* ���溯�� */
    void OnPhotoSave();

    /* ȥ����ť�ص����� */
    static void OnPhotoCancelCB(void *ctx);
	/* ȡ������ */
    void OnPhotoCancel();

    static void OnActionOverCB(void *ctx);
    void OnActionOver();

    /*  */
    void ShowNoStorageMsg(void);
	void ReleaseStorageMsg(void);

    /*  */
    void CheckUpdate();
    int ReadFully(int fd, void* buf, size_t nbytes);
    int WriteFully(int fd, const void* buf, size_t nbytes);
    static void* UpdateThreadCB(void *arg);
    void DoUpdateThread(void);

    static DFBResult OnUpdateTimerEvent(void *data);
    void UpdateTimerEvent(void);

	void saveAction();

	void UpdateFont(const char * pFontFile);

	void HideModalDlg();
	BOOL InitSubStrFont();
	void DestorySubStrFont();
	void InitTopLabel();
	static void AfterDrawTopLabelCB(void *data);
	void AfterDrawTopLabel(void);

private:
    /* �Ŵ��� */
    static  int                         s_zoomNum;

    /* ͼ����ת״̬ */
    static Rotate_E                     m_rotateState;

    /**/
    CModalDialog*                       m_modalDialog;

    /**/
    Record_E                            m_recordStatus;

    /* ¼��ʼʱ�� */
    time_t                              m_recordBeginTime;

    int                                 m_updatIng;     // 1 : ��������, 0:

    static Record_Mode_E                m_mode;         //

    char                                m_szUpdateFile[256];
	char                                m_szUpdateFileOk[256];
    /* ���������߳� ID */
    pthread_t                           m_updateThread;
    /* ���������� */
    LiteProgressBar*                    m_updateBar;
    /* ��ʾ�������� */
    LiteLabel*                          m_updateText;
    /* ��ʱ��ID, �������������� */
    int                                 m_updateTimerID;
    /**/
    float                               m_updateBarValue;


	char 								m_TimeText[16];
	int                                 m_pipefd;
	char                                m_photoName[64];

	/* ʵʱ��Ƶͼ�� */
    LiteImage*                          m_videoImage;
    /* ¼��ͼ�� */
    LiteImage*                          m_recordImage;
	/* ���²���ʾ¼��ʱ�� */
    LiteLabel*                          m_recordTimeLabel;
    // ��˷�ͼ��
    LiteImage*                          m_micImage;
	/* ����ͼƬ */
    LiteImage*                          m_noStorageMsg;
	/* ��ʾ��Ϣ */
	LiteLabel*							m_lblStorageMsg;
	/* ҡ��LED������ */
    LiteImage*                          m_rockBrightImage;

	LiteLabel*                          m_topLabel;
	LiteFont*                           m_pLiteFont;
	IDirectFBFont*                      m_pFontBoldInterface;

	int m_iRotateLeft;
	int m_iRotateRight;
	int m_iZoomUp;
	int m_iZoomDown;
	BOOL m_bLedShow;
	long m_lCurVideoRec;
};

#endif

