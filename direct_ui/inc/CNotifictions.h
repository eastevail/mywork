/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: CNotifictions.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.05.13
	Description	:
	History		:
					Create by hul.2015.05.13
******************************************************************************/


#ifndef __CNOTIFICTION_H__
#define __CNOTIFICTION_H__

#include "abspage.h"

class CNotifictions : public CAbsPage
{
public:
    CNotifictions(CAbsPage *parentPage = NULL);

    virtual ~CNotifictions();

    virtual void OnPageInit();

    virtual void OnPageExit();

    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    void JumpToParentPage();


	void InitDispWnd();
	void SetNotification(NotifictionId_E id, BOOL show);
	void DrawNotification();
	void ShowNotification(BOOL bShow);
	void SetAllNotification(BOOL bShow);

private:

	LiteImage *m_NotifictionImage;
	LiteLabel *m_lblMainText[2];
	LiteLabel *m_lblSubText[3];
};

#endif
