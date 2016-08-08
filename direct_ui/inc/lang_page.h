/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: lang_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.25
******************************************************************************/

#ifndef __LANG_PAGE_H__
#define __LANG_PAGE_H__

#include "abspage.h"

class CLangPage : public CAbsPage
{
public:

    CLangPage(CAbsPage *parentPage = NULL);

    virtual ~CLangPage();

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* 跳转到父页面 */
    void JumpToParentPage();

private:

    /*  */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /*  */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

    /*  */
    BOOL InitListBox();

    /*  */
    void DestoryListBox();

    /* 初始化背景, listbox默认没有背景颜色 */
    BOOL InitBackground();

    /*  */
    void DestoryBackground();

    /* 上键按下事件 */
    void OnUpKeyEvent(void);

    /* 下键按下事件 */
    void OnDownKeyEvent(void);

    /* OK键按下事件 */
    void OnOKKeyEvent(void);

private:

    LiteList*                   m_listBox;
    LiteLabel*                  m_bgLabel;
};

#endif

