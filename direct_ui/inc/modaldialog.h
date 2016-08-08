/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: modaldialog.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __MODALDIALOG_H__
#define __MODALDIALOG_H__

#include "abspage.h"

#define DIALOG_TEXT_LEN             64

typedef void (*DialogBtnClick) (void *ctx);

typedef enum
{
    Dialog_OK = 0,
    Dialog_Cancel,
} Dialog_E;

class CModalDialog : public CAbsPage
{
public:
    CModalDialog(DFBRectangle &dfbRect, CAbsPage *parentPage = NULL);

    ~CModalDialog();

    /* ����ȷ���¼��Ļص����� */
    void SetOKCB(char *szText, DialogBtnClick okCB, void *data);

    /* ����ȡ���¼��Ļص����� */
    void SetCancelCB(char *szText, DialogBtnClick cancelCB, void *data);

    /* ��ʾ�Ի��� */
    void ShowDialog();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	void setDailogText(char *szLine1,char *szLine2);

	void setDailogBackground(int r,int g,int b);

	void setDailogBackground(char *filePath);

	void setRecordTime(char *sztimetxt);

	void setKeyHide();

	void SetCurSel(int sel);

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
    void ShowOKBtn(BOOL bVisible, BOOL bActive);

    /*  */
    void ShowCancelBtn(BOOL bVisible, BOOL bActive);

    void ToggleBtn(void);

private:
    /* ȷ���¼��Ļص����� */
    DialogBtnClick          m_okCB;

    void*                   m_okCBData;

    /* ȡ���¼��Ļص����� */
    DialogBtnClick          m_cancelCB;

    void*                   m_cancelCBData;

    char                    m_okText[DIALOG_TEXT_LEN];

    char                    m_cancelText[DIALOG_TEXT_LEN];

    Dialog_E                m_dialogStatus;

//hul add
	char*					m_pLineText1;

	char*					m_pLineText2;

	LiteLabel*				m_txtLine1;

	LiteLabel*				m_txtLine2;

	LiteLabel*				m_txtTime;         //for vedio record

	char*					m_pTimeText;		//for vedio record

	    // ��˷�ͼ��
 //   LiteImage*                          m_micImage;

	DFBColor		m_bgColor;

	char 	        m_bgimagePath[DIALOG_TEXT_LEN];

	LiteImage*              m_bgImage;

//hul add
    /* �Ի���ı��� */
    LiteLabel*              m_dialogBg;

    /* ȷ�ϰ�ť */
    LiteLabel*              m_okButton;

    /* ȡ����ť */
    LiteLabel*              m_cancelButton;

    /* �Ի�����ʾ��λ�� */
    DFBRectangle            m_rect;

	int                     m_isPressOK;
};

#endif

