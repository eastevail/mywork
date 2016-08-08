/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: microphone.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	: 
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#ifndef __MICROPHONE_H__
#define __MICROPHONE_H__

#include "abspage.h"
#include "modaldialog.h"

class CMicroPhonePage : public CAbsPage
{
public:
    CMicroPhonePage(CAbsPage *parentPage = NULL);
    
    virtual ~CMicroPhonePage();

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
    static void OnSaveCB(void *ctx);

    /*  */
    void OnSave();

    /*  */
    static void OnCancelCB(void *ctx);

    /*  */
    void OnCancel();
    
private:
    
    CModalDialog*                   m_modalDialog;
};

#endif

