/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: usb_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2015.04.18
******************************************************************************/

#ifndef __USB_PAGE_H__
#define __USB_PAGE_H__

#include "abspage.h"

class CUSBPage : public CAbsPage
{
public:

    CUSBPage(CAbsPage *parentPage = NULL);

    virtual ~CUSBPage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

private:

private:

    LiteLabel*                  m_msgLabel;
	LiteLabel*               	m_topLabel;
	LiteImage *m_imgBk;
};

#endif

