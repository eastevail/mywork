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

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnSubPageInit() = 0;

    /*�˳�ҳ���¼�*/
    virtual void OnSubPageExit() = 0;

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent) = 0;

	/* Init date time */
	virtual void InitDateTime() = 0;

protected:

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /* ��ת����ҳ�� */
    void JumpToParentPage();

    /* �ھ��ο����»����Ǽ�ͷ */
    void FillDoubleTriangle(IDirectFBSurface* surface, DFBRectangle *dfbRect);

    /* ��ȡһ������������ */
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

    /* ����label */
    LiteLabel*                      m_bgLabel;

    /* �ײ� ��/������ */
    LiteLabel*                      m_leftRightText;

    /* ���Ҽ�ͷ */
    CArrow*                         m_leftRightArrow;

    /* �ײ� ��/������ */
    LiteLabel*                      m_upDownText;

    /* ���¼�ͷ */
    CArrow*                         m_upDownArrow;

    /* done �ı� */
    LiteLabel*                      m_doneText;
	LiteImage*                      m_doOkImage;

	// Exit
	LiteLabel*                      m_lblExit;
	LiteImage*                      m_imgExit;

    /*  */
    LiteFont*                       m_labelSubFont;
};

#endif

