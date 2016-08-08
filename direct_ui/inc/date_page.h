/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: date_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#ifndef __DATE_PAGE_H__
#define __DATE_PAGE_H__

#include "time_abs_page.h"

typedef enum
{
    Date_Status_Begin = -1,
    Date_Status_Month,
    Date_Status_Day,
    Date_Status_Year,
    Date_Status_End
} Date_Status_E;

#define     MIN_YEAR            2012
#define     MAX_YEAR            2030
#define     BASE_YEAR           2000

class CDatePage : public CTimeAbsPage
{
public:
    CDatePage(CAbsPage *parentPage = NULL);

    virtual ~CDatePage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnSubPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnSubPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	/* Init date */
	virtual void InitDateTime();

    /*  */
    void SetActiveLabel(Date_Status_E dateStatus);

    /* ��ת����ҳ�� */
    void JumpToParentPage();

private:

    /* ��������¼� */
    void OnLeftKeyEvent(void);

    /* �Ҽ������¼� */
    void OnRightKeyEvent(void);

    /* �ϼ������¼� */
    void OnUpKeyEvent(void);

    /* �¼������¼� */
    void OnDownKeyEvent(void);

    /* OK�������¼� */
    void OnOKKeyEvent(void);

    /* SETTING�������¼� */
    void OnSettingKeyEvent(void);

    /* REPLAY�������¼� */
    void OnReplayKeyEvent(void);

    /* video/capture�������¼� */
    void OnVideoCaptureKeyEvent(void);

    /*  */
    static void AfterDrawMonthLabelCB(void* data);

    /*  */
    void AfterDrawMonthLabel();

    /*  */
    BOOL InitMonthLabel();

    /*  */
    BOOL InitDayLabel();

    /*  */
    static void AfterDrawDayLabelCB(void* data);

    /*  */
    void AfterDrawDayLabel();

    /*  */
    BOOL InitYearLabel();

    /*  */
    static void AfterDrawYearLabelCB(void* data);

    /*  */
    void AfterDrawYearLabel();

private:

    /* �·� */
    LiteLabel*                      m_monthLabel;

    /* �� */
    LiteLabel*                      m_dayLabel;

    /* �� */
    LiteLabel*                      m_yearLabel;

    /*  */
    Date_Status_E                   m_curDateStatus;
};

#endif

