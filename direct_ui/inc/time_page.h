/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: time_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#ifndef __TIME_PAGE_H__
#define __TIME_PAGE_H__

#include "time_abs_page.h"

typedef enum
{
    Time_Status_Begin = -1,
    Time_Status_Hour,
    Time_Status_Minute,
    Time_Status_AMPM,
    Time_Status_End
} Time_Status_E;

typedef enum
{
	TIME_AM = 0,
	TIME_PM,
	TIME_24
} teAMPM24;

class CTimePage : public CTimeAbsPage
{
public:
    CTimePage(CAbsPage *parentPage = NULL);

    virtual ~CTimePage();

    /*进入页面的初始化事件*/
    virtual void OnSubPageInit();

    /*退出页面事件*/
    virtual void OnSubPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	/* Init time */
	virtual void InitDateTime();

    /*  */
    void SetActiveLabel(Time_Status_E timeStatus);

    /* 跳转到父页面 */
    void JumpToParentPage();

private:

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

    /*  */
    BOOL InitHourLabel();

    /*  */
    BOOL InitMinuteLabel();

    /*  */
    BOOL InitAMPMLabel();

    /*  */
    static void AfterDrawHourLabelCB(void* data);

    /*  */
    void AfterDrawHourLabel();

    /*  */
    static void AfterDrawMinuteLabelCB(void* data);

    /*  */
    void AfterDrawMinuteLabel();

    /*  */
    static void AfterDrawAMPMLabelCB(void* data);

    /*  */
    void AfterDrawAMPMLabel();

private:

    /* 月份 */
    LiteLabel*                      m_hourLabel;

    /* 天 */
    LiteLabel*                      m_minuteLabel;

    /* 年 */
    LiteLabel*                      m_ampmLabel;

    /* */
    Time_Status_E                   m_curTimeStatus;

	int m_timeAMPM24;
	int GetHour12(int CurHr);

};

#endif

