/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: del_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	: 
	History		:
					Create by zyfan.2014.12.26
******************************************************************************/

#ifndef __DEL_PAGE_H__
#define __DEL_PAGE_H__

#include "abspage.h"
#include "modaldialog.h"

class CDeletePage : public CAbsPage
{
public:
    
    CDeletePage(CAbsPage *parentPage = NULL);
    ~CDeletePage();

private:
    
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
    static void OnDeleteAllCB(void *ctx);
	static void DeleteAllWaitingCB(void *ctx);

    /*  */
    void OnDeleteAll();

    /*  */
    static void OnCancelCB(void *ctx);

    /*  */
    void OnCancel();
    
private:
    
    CModalDialog*                   m_modalDialog;
    
};

#endif

