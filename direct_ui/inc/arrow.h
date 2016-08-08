/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: arrow.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	: 
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __ARROW_H__
#define __ARROW_H__

#include "common.h"

typedef enum
{
    Arrow_Begin = -1,
    Arrow_Horizontal,               // ˮƽ��ͷ
    Arrow_Vertical,                 // ��ֱ��ͷ
    Arrow_End
} ArrowDir_E;


#define BLINK_TIME                  150
#define ARROW_COUNT                 2

class CArrow
{
public:
    CArrow(DFBRectangle &dfbRect, ArrowDir_E dir);
    ~CArrow();

    /* ��˸���ͷ */
    void BlinkLeftArrow();

    /* ��˸�Ҽ�ͷ */
    void BlinkRightArrow();

    /* ��˸�ϼ�ͷ */
    void BlinkUpArrow();

    /* ��˸�¼�ͷ */
    void BlinkDownArrow();
	
    /*  */
	int setArrowNOActive();
	
	int setArrowActive();

private:
    /* ��ʱ���ص����� */
    static DFBResult BlinkArrowCB(void *data);

    void BlinkArrow();
    
    /* ��ʾ��ͷ����ɫ */
    void ShowArrowActive(BOOL active);
    
    
    /*  */
    void InitArrow();

    /*  */
    void UnInitArrow();
    
private:
    
    /* ��ͷ���� */
    ArrowDir_E                      m_arrowDir;

    /* ��ʾ��λ�� */
    DFBRectangle                    m_rectPos;

    /*  */
   // LiteLabel*                      m_arrowLabel;
	LiteImage*						m_ArrowImage1;

	LiteImage*						m_ArrowImage2;

    int                             m_triangleIndex;

    int                             m_blinkTimeID;

    int                             m_bBlinkFlag;

	int                             m_bNOActiveFlag;
};

#endif

