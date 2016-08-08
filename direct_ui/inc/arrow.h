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
    Arrow_Horizontal,               // 水平箭头
    Arrow_Vertical,                 // 垂直箭头
    Arrow_End
} ArrowDir_E;


#define BLINK_TIME                  150
#define ARROW_COUNT                 2

class CArrow
{
public:
    CArrow(DFBRectangle &dfbRect, ArrowDir_E dir);
    ~CArrow();

    /* 闪烁左箭头 */
    void BlinkLeftArrow();

    /* 闪烁右箭头 */
    void BlinkRightArrow();

    /* 闪烁上箭头 */
    void BlinkUpArrow();

    /* 闪烁下箭头 */
    void BlinkDownArrow();
	
    /*  */
	int setArrowNOActive();
	
	int setArrowActive();

private:
    /* 定时器回调函数 */
    static DFBResult BlinkArrowCB(void *data);

    void BlinkArrow();
    
    /* 显示箭头的颜色 */
    void ShowArrowActive(BOOL active);
    
    
    /*  */
    void InitArrow();

    /*  */
    void UnInitArrow();
    
private:
    
    /* 箭头方向 */
    ArrowDir_E                      m_arrowDir;

    /* 显示的位置 */
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

