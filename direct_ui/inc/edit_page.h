/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: play_page.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.09
	Description	:
	History		:
					Create by hul .2015.06.01
******************************************************************************/

#ifndef __EDIT_PAGE_H__
#define __EDIT_PAGE_H__

#include "abspage.h"
#include "arrow.h"

enum{
	EDIT_BEGIN = -1,
	EDIT_ROTATE,
	EDIT_ZOOM,

};

class CEditPage : public CAbsPage
{
public:
    CEditPage(CAbsPage *parentPage = NULL);

    ~CEditPage();

	virtual void OnUpdateArrow();

private:

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* �����Զ����¼� */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);


	static void AfterDrawFileImageCB(void *data);

	void AfterDrawFileImage(void);

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

	BOOL InitSubStrFont();
	void CEditPage::DestorySubStrFont();

private:

    char                                m_fileToEdit[MAX_FILE_LEN];
	LiteImage*							m_FileImage;
    LiteLabel*                          m_topLabel;
	LiteImage*                          m_ReviewImage;

	int 	                            m_RotateDegree;
	int	                                m_ZoomDegree;

	LiteFont*                           m_pLiteFont;
	IDirectFBFont*                      m_pFontBoldInterface;

	int m_iRotateLeft;
	int m_iRotateRight;
	int m_iZoomUp;
	int m_iZoomDown;
};

#endif



