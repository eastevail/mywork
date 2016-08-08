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

    /* 设置确认事件的回调函数 */
    void SetOKCB(char *szText, DialogBtnClick okCB, void *data);

    /* 设置取消事件的回调函数 */
    void SetCancelCB(char *szText, DialogBtnClick cancelCB, void *data);

    /* 显示对话框 */
    void ShowDialog();

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

	void setDailogText(char *szLine1,char *szLine2);

	void setDailogBackground(int r,int g,int b);

	void setDailogBackground(char *filePath);

	void setRecordTime(char *sztimetxt);

	void setKeyHide();

	void SetCurSel(int sel);

private:
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

    /*  */
    void ShowOKBtn(BOOL bVisible, BOOL bActive);

    /*  */
    void ShowCancelBtn(BOOL bVisible, BOOL bActive);

    void ToggleBtn(void);

private:
    /* 确认事件的回调函数 */
    DialogBtnClick          m_okCB;

    void*                   m_okCBData;

    /* 取消事件的回调函数 */
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

	    // 麦克风图标
 //   LiteImage*                          m_micImage;

	DFBColor		m_bgColor;

	char 	        m_bgimagePath[DIALOG_TEXT_LEN];

	LiteImage*              m_bgImage;

//hul add
    /* 对话框的背景 */
    LiteLabel*              m_dialogBg;

    /* 确认按钮 */
    LiteLabel*              m_okButton;

    /* 取消按钮 */
    LiteLabel*              m_cancelButton;

    /* 对话框显示的位置 */
    DFBRectangle            m_rect;

	int                     m_isPressOK;
};

#endif

