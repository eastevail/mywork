/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: brightness.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#ifndef __MANUFACTURE_H__
#define __MANUFACTURE_H__

#include "abspage.h"

class CManufacturePage : public CAbsPage
{
public:
    CManufacturePage(CAbsPage *parentPage = NULL);

    virtual ~CManufacturePage();

    /*����ҳ��ĳ�ʼ���¼�*/
    virtual void OnPageInit();

    /*�˳�ҳ���¼�*/
    virtual void OnPageExit();

    /*ҳ�水���¼�*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* ��ת����ҳ�� */
    void JumpToParentPage();

private:

    /*  */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /*  */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

    /*  */
    BOOL InitListBox();

    /*  */
    void DestoryListBox();

    /* ��ʼ������, listboxĬ��û�б�����ɫ */
    BOOL InitBackground();

    /*  */
    void DestoryBackground();

    /* �ϼ������¼� */
    void OnUpKeyEvent(void);

    /* �¼������¼� */
    void OnDownKeyEvent(void);

    /* OK�������¼� */
    void OnOKKeyEvent(void);

	void InitTestColor();
	void TestColorChange(int dir);
	static void AfterDrawColorBoxCB(void *data);
	void AfterDrawColorBox(void);

	void RestoreDefParam();

private:

    LiteList*                   m_listBox;
    LiteLabel*                  m_bgLabel;

	LiteLabel *m_plblColorBox;
};

#endif // __MANUFACTURE_H__

