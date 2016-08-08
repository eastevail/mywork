/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: abspage.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __ABSPAGE_H__
#define __ABSPAGE_H__

#include "common.h"

#define     MAX_FILE_LEN        128

typedef    void (*UI_FUNCTIONCB)(void*);

#define MAX_SUB_PAGE            64

class CAbsPage
{
public:
    CAbsPage(CAbsPage *parentPage = NULL);

    virtual ~CAbsPage();

    /*进入页面的初始化事件*/
    virtual void OnPageInit() = 0;

    /*退出页面事件*/
    virtual void OnPageExit() = 0;

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent) = 0;

    /* 窗口自定义事件 */
    virtual void OnWindowEvent(Window_Msg_S *winMsg)
    {
    }

	virtual void OnUpdateArrow(){}

    BOOL HasParentPage();

    CAbsPage* GetParentPage();

    BOOL SendJCPCmd(const char *szJcpCmd);

protected:

    /* 在指定的位置创建并显示一张图片 */
    DFBResult CreateLiTEImage(DFBRectangle *dfbRect, const char *szImageFile, LiteImage **liteImage);

    /* 创建并显示label */
    DFBResult CreateLiTELabel(DFBRectangle *dfbRect, DFBColor *dfbBgColor, int fontSize, DFBColor *dfbFontColor, LiteLabel **liteLabel);

private:

    /* 子页面的个数 */
    int                     m_childCount;

    /* 直接父页面 */
    CAbsPage*               m_parentPage;

    /* 子页面数组 */
    CAbsPage*               m_childPageArray[MAX_SUB_PAGE];
};

#endif

