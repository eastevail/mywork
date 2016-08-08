/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: setting.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#ifndef __SETTING_H__
#define __SETTING_H__

#include "type_common.h"
#include "msg_common.h"
#include "alarm_msg_drv.h"
#include "sys_msg_drv.h"
#include "msg_util.h"
#include "cw_pthread_mng.h"
#include "cw_util.h"

#include "abspage.h"

typedef enum
{
    SubPage_Begin = -1,
    SubPage_MicroPhone,
    SubPage_BrightNess,
    SubPage_Date,
    SubPage_Time,
    SubPage_Lang,
    SubPage_DeleteAll,
    SubPage_Version,
    //SubPage_Manufacture,
    SubPage_End,
} SubPage_E;

class CSettingPage : public CAbsPage
{
public:
    CSettingPage(CAbsPage *parentPage = NULL);

    ~CSettingPage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* ��ʾTitle���� */
    BOOL ShowTitleText(const char *szTitle);

    /*  */
    void Recover();

	void ShowSubPageByID(SubPage_E subPageId);

	void UpdateCurItem();

	void DeleteSubPage();

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

    /* ��ʾ�������� */
    BOOL ShowSettingFlag(BOOL bVisible);

    /* ��ʼ�������� */
    BOOL InitListBox();

    /**/
    void DestoryListBox();

    /* ��ʼ���±����� */
    BOOL InitSubStrFont();

    /* �����±����� */
    void DestorySubStrFont();

    /*  */
    int GetDateStr(char *szBuf, int bufLen);

    /*  */
    int GetTimeStr(char *szBuf, int bufLen);

    /* list ��Ļص����� */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /* */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

public:
	/*  */

private:

    /* ϵͳ����ͼ�� */
    LiteImage*                      m_settingImage;

    /* ϵͳ�������� */
    LiteLabel*                      m_settingLabel;

    /*  */
    LiteList*                       m_listBox;

    /*  */
    LiteFont*                       m_listSubFont;

    /*  */
	IDirectFBFont*                  m_listSubFontInterface;

    /*  */
    SubPage_E                       m_subPageId;

    /*  */
    CAbsPage*                       m_subPagePtr;

    /* ��ǰ��ѡ�е�select���index */
    int                             m_curIndex;
};

#endif

