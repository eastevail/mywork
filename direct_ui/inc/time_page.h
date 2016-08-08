/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: time_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#ifndef __TIME_PAGE_H__
#define __TIME_PAGE_H__

#include "time_abs_page.h"

typedef enum
{
    Time_Status_Begin = -1,
    Time_Status_Hour,
    Time_Status_Minute,
    Time_Status_AMPM,
    Time_Status_End
} Time_Status_E;

typedef enum
{
	TIME_AM = 0,
	TIME_PM,
	TIME_24
} teAMPM24;

class CTimePage : public CTimeAbsPage
{
public:
    CTimePage(CAbsPage *parentPage = NULL);

    virtual ~CTimePage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnSubPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnSubPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	/* Init time */
	virtual void InitDateTime();

    /*  */
    void SetActiveLabel(Time_Status_E timeStatus);

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
    BOOL InitHourLabel();

    /*  */
    BOOL InitMinuteLabel();

    /*  */
    BOOL InitAMPMLabel();

    /*  */
    static void AfterDrawHourLabelCB(void* data);

    /*  */
    void AfterDrawHourLabel();

    /*  */
    static void AfterDrawMinuteLabelCB(void* data);

    /*  */
    void AfterDrawMinuteLabel();

    /*  */
    static void AfterDrawAMPMLabelCB(void* data);

    /*  */
    void AfterDrawAMPMLabel();

private:

    /* �·� */
    LiteLabel*                      m_hourLabel;

    /* �� */
    LiteLabel*                      m_minuteLabel;

    /* �� */
    LiteLabel*                      m_ampmLabel;

    /* */
    Time_Status_E                   m_curTimeStatus;

	int m_timeAMPM24;
	int GetHour12(int CurHr);

};

#endif

