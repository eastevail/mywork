/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: brightness.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#ifndef __MANUFACTURE_H__
#define __MANUFACTURE_H__

#include "abspage.h"

class CManufacturePage : public CAbsPage
{
public:
    CManufacturePage(CAbsPage *parentPage = NULL);

    virtual ~CManufacturePage();

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

	void InitTestColor();
	void TestColorChange(int dir);
	static void AfterDrawColorBoxCB(void *data);
	void AfterDrawColorBox(void);

	void RestoreDefParam();

private:

    LiteList*                   m_listBox;
    LiteLabel*                  m_bgLabel;

	LiteLabel *m_plblColorBox;
};

#endif // __MANUFACTURE_H__

