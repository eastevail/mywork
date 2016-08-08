/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: date_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#ifndef __DATE_PAGE_H__
#define __DATE_PAGE_H__

#include "time_abs_page.h"

typedef enum
{
    Date_Status_Begin = -1,
    Date_Status_Month,
    Date_Status_Day,
    Date_Status_Year,
    Date_Status_End
} Date_Status_E;

#define     MIN_YEAR            2012
#define     MAX_YEAR            2030
#define     BASE_YEAR           2000

class CDatePage : public CTimeAbsPage
{
public:
    CDatePage(CAbsPage *parentPage = NULL);

    virtual ~CDatePage();

    /*进入页面的初始化事件*/
    virtual void OnSubPageInit();

    /*退出页面事件*/
    virtual void OnSubPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	/* Init date */
	virtual void InitDateTime();

    /*  */
    void SetActiveLabel(Date_Status_E dateStatus);

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
    static void AfterDrawMonthLabelCB(void* data);

    /*  */
    void AfterDrawMonthLabel();

    /*  */
    BOOL InitMonthLabel();

    /*  */
    BOOL InitDayLabel();

    /*  */
    static void AfterDrawDayLabelCB(void* data);

    /*  */
    void AfterDrawDayLabel();

    /*  */
    BOOL InitYearLabel();

    /*  */
    static void AfterDrawYearLabelCB(void* data);

    /*  */
    void AfterDrawYearLabel();

private:

    /* 月份 */
    LiteLabel*                      m_monthLabel;

    /* 天 */
    LiteLabel*                      m_dayLabel;

    /* 年 */
    LiteLabel*                      m_yearLabel;

    /*  */
    Date_Status_E                   m_curDateStatus;
};

#endif

