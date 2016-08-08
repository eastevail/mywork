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

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* 显示Title文字 */
    BOOL ShowTitleText(const char *szTitle);

    /*  */
    void Recover();

	void ShowSubPageByID(SubPage_E subPageId);

	void UpdateCurItem();

	void DeleteSubPage();

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

    /* 显示设置文字 */
    BOOL ShowSettingFlag(BOOL bVisible);

    /* 初始化下拉框 */
    BOOL InitListBox();

    /**/
    void DestoryListBox();

    /* 初始化下标字体 */
    BOOL InitSubStrFont();

    /* 销毁下标字体 */
    void DestorySubStrFont();

    /*  */
    int GetDateStr(char *szBuf, int bufLen);

    /*  */
    int GetTimeStr(char *szBuf, int bufLen);

    /* list 框的回调函数 */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /* */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

public:
	/*  */

private:

    /* 系统设置图标 */
    LiteImage*                      m_settingImage;

    /* 系统设置文字 */
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

    /* 当前被选中的select框的index */
    int                             m_curIndex;
};

#endif

