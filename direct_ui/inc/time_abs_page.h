/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: time_abs_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#ifndef __TIME_ABS_PAGE_H__
#define __TIME_ABS_PAGE_H__

#include "abspage.h"
#include "arrow.h"

class CTimeAbsPage : public CAbsPage
{
public:
    CTimeAbsPage(CAbsPage *parentPage = NULL);

    virtual ~CTimeAbsPage();

    /*进入页面的初始化事件*/
    virtual void OnSubPageInit() = 0;

    /*退出页面事件*/
    virtual void OnSubPageExit() = 0;

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent) = 0;

	/* Init date time */
	virtual void InitDateTime() = 0;

protected:

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /* 跳转到父页面 */
    void JumpToParentPage();

    /* 在矩形框上下画三角箭头 */
    void FillDoubleTriangle(IDirectFBSurface* surface, DFBRectangle *dfbRect);

    /* 获取一个月最大的天数 */
    int GetMaxDayOfMonth(struct tm* localTime);

    /* */
    void SaveDateTime(void);

    /*  */
    BOOL InitFirstLabel(LiteLabel **label);

    /*  */
    BOOL InitSecondLabel(LiteLabel **label);

    /*  */
    BOOL InitThirdLabel(LiteLabel **label);

private:

    /*  */
    BOOL InitBGAndBottom();
	void InitSaveExit();

    /*  */
    void DestoryBGAndBottom();

    BOOL InitSubStrFont();

    void DestorySubStrFont();

protected:

    /*  */
	IDirectFBFont*                  m_labelSubFontInterface;

private:

    /* 背景label */
    LiteLabel*                      m_bgLabel;

    /* 底部 左/右文字 */
    LiteLabel*                      m_leftRightText;

    /* 左右箭头 */
    CArrow*                         m_leftRightArrow;

    /* 底部 上/下文字 */
    LiteLabel*                      m_upDownText;

    /* 上下箭头 */
    CArrow*                         m_upDownArrow;

    /* done 文本 */
    LiteLabel*                      m_doneText;
	LiteImage*                      m_doOkImage;

	// Exit
	LiteLabel*                      m_lblExit;
	LiteImage*                      m_imgExit;

    /*  */
    LiteFont*                       m_labelSubFont;
};

#endif

