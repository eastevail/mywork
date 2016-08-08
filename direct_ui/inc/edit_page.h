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

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* 窗口自定义事件 */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);


	static void AfterDrawFileImageCB(void *data);

	void AfterDrawFileImage(void);

    /* 左键按下事件 */
    void OnLeftKeyEvent(void);

    /* 右键按下事件 */
    void OnRightKeyEvent(void);

    /* 上键按下事件 */
    void OnUpKeyEvent(void);

    /* 下键按下事件 */
    void OnDownKeyEvent(void);

    /* OK键按下事件 */
    void OnOKKeyEvent(void);

    /* SETTING键按下事件 */
    void OnSettingKeyEvent(void);

    /* REPLAY键按下事件 */
    void OnReplayKeyEvent(void);

    /* video/capture键按下事件 */
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



