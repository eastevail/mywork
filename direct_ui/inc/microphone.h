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

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* ��ת����ҳ�� */
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

