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

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit() = 0;

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit() = 0;

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent) = 0;

    /* �����Զ����¼� */
    virtual void OnWindowEvent(Window_Msg_S *winMsg)
    {
    }

	virtual void OnUpdateArrow(){}

    BOOL HasParentPage();

    CAbsPage* GetParentPage();

    BOOL SendJCPCmd(const char *szJcpCmd);

protected:

    /* ��ָ����λ�ô�������ʾһ��ͼƬ */
    DFBResult CreateLiTEImage(DFBRectangle *dfbRect, const char *szImageFile, LiteImage **liteImage);

    /* ��������ʾlabel */
    DFBResult CreateLiTELabel(DFBRectangle *dfbRect, DFBColor *dfbBgColor, int fontSize, DFBColor *dfbFontColor, LiteLabel **liteLabel);

private:

    /* ��ҳ��ĸ��� */
    int                     m_childCount;

    /* ֱ�Ӹ�ҳ�� */
    CAbsPage*               m_parentPage;

    /* ��ҳ������ */
    CAbsPage*               m_childPageArray[MAX_SUB_PAGE];
};

#endif

