/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: play_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.09
	Description	:
	History		:
					Create by zyfan.2015.04.09
******************************************************************************/

#ifndef __PLAY_PAGE_H__
#define __PLAY_PAGE_H__

#include "abspage.h"

#define     MAX_FILE_LEN        128

typedef enum
{
    Play_State_Begin = -1,
    Play_State_Play,
    Play_State_Stop,
    Play_State_Replay,
    Play_State_End,
} Play_State_E;

typedef enum
{
    Play_Dir_Begin = -1,
    Play_Dir_Normal,
    Play_Dir_Forward,
    Play_Dir_Back,
    Play_Dir_End,
} Play_Dir_E;

class CPlayPage : public CAbsPage
{
public:
    CPlayPage(CAbsPage *parentPage = NULL);

    ~CPlayPage();

	virtual void OnUpdateArrow();

private:

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* 窗口自定义事件 */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

	void OnWaitingEvent();

	static void OnWaitingEventCb(void *data);


    /* ?????? */
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

    /*  */
    void InitProgreeBar();

    int ComTimToStr(double curtime, char *szValue);

    BOOL InitSubStrFont();

    void DestorySubStrFont();

private:

    char                                    m_fileToPlay[MAX_FILE_LEN];

    Play_State_E                            m_playState;

    Play_Dir_E                              m_playDir;

    LiteLabel*                              m_topLabel;

    /* 播放进度条 */
    LiteProgressBar*                        m_progress;

    /* 显示录像时间 */
    LiteLabel*                              m_timeLabel;

        /*  */
    LiteFont*                               m_listSubFont;

    /*  */
	IDirectFBFont*                          m_listSubFontInterface;


	LiteImage*                          m_ReviewImage;

	LiteImage*                          m_PauseOKImage;

	LiteImage*                          m_ExitImage;

	double                              m_fileTimeTotal;

	int m_iRWArrow;
	int m_iFFArrow;

	int m_curTime;
	long m_afterFileDuration;
};

#endif

