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

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* �����Զ����¼� */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

	void OnWaitingEvent();

	static void OnWaitingEventCb(void *data);


    /* ?????? */
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

    /* ���Ž����� */
    LiteProgressBar*                        m_progress;

    /* ��ʾ¼��ʱ�� */
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

