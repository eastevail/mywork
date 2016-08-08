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
#define     UPDATE_TOTAL_TIME       120         // 秒

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
    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	virtual void OnWindowEvent(Window_Msg_S *winMsg);

private:

	friend class CDesktop;

    /* 左键按下事件 */
    void OnLeftKeyEvent(void);

    /* 右键按下事件 */
    void OnRightKeyEvent(void);

    /* 上键按下事件 */
    void OnUpKeyEvent(void);

    /* 下键按下事件 */
    void OnDownKeyEvent(void);

    /* OK键按下事件 */
    void OnOKKeyEvent(void);

    /* SETTING键按下事件 */
    void OnSettingKeyEvent(void);

    /* REPLAY键按下事件 */
    void OnReplayKeyEvent(void);

    /* video/capture键按下事件 */
    void OnVideoCaptureKeyEvent(void);

    /* 显示录像时间 */
    BOOL BeginShowRecordTimeText(const char *szTimeText);

    void ShowRecordTimeText(const char *szTimeText);

    BOOL CancelShowRecordTimeText();

    /* 更新时间显示 */
    void RecordTimerEvent();

    /* 显示上条框的实时视频标识 */
    BOOL ShowVideoFlag(BOOL bVisible);

    /* 显示/隐藏录像状态图标 */
    BOOL ShowRecordFlag(BOOL bVisible);

	// Save video file
    /* 保存按钮回调函数 */
    static void OnSaveCB(void *ctx);
    /* 保存函数 */
    void OnSave();

    /* 去掉按钮回调函数 */
    static void OnCancelCB(void *ctx);
    /* 取消函数 */
    void OnCancel();


	// Save photo file
	/* 保存按钮回调函数 */
	static void OnPhotoSaveCB(void *ctx);
    /* 保存函数 */
    void OnPhotoSave();

    /* 去掉按钮回调函数 */
    static void OnPhotoCancelCB(void *ctx);
	/* 取消函数 */
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
    /* 放大倍数 */
    static  int                         s_zoomNum;

    /* 图像旋转状态 */
    static Rotate_E                     m_rotateState;

    /**/
    CModalDialog*                       m_modalDialog;

    /**/
    Record_E                            m_recordStatus;

    /* 录像开始时间 */
    time_t                              m_recordBeginTime;

    int                                 m_updatIng;     // 1 : 正在升级, 0:

    static Record_Mode_E                m_mode;         //

    char                                m_szUpdateFile[256];
	char                                m_szUpdateFileOk[256];
    /* 升级处理线程 ID */
    pthread_t                           m_updateThread;
    /* 升级进度条 */
    LiteProgressBar*                    m_updateBar;
    /* 显示升级文字 */
    LiteLabel*                          m_updateText;
    /* 定时器ID, 更新升级进度条 */
    int                                 m_updateTimerID;
    /**/
    float                               m_updateBarValue;


	char 								m_TimeText[16];
	int                                 m_pipefd;
	char                                m_photoName[64];

	/* 实时视频图标 */
    LiteImage*                          m_videoImage;
    /* 录像图标 */
    LiteImage*                          m_recordImage;
	/* 更新并显示录像时间 */
    LiteLabel*                          m_recordTimeLabel;
    // 麦克风图标
    LiteImage*                          m_micImage;
	/* 背景图片 */
    LiteImage*                          m_noStorageMsg;
	/* 提示信息 */
	LiteLabel*							m_lblStorageMsg;
	/* 摇杆LED灯亮度 */
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

