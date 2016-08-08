/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: label_bar.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.07
	Description	: 
	History		:
					Create by zyfan.2015.04.07
******************************************************************************/

#ifndef __LABEL_BAR_H__
#define __LABEL_BAR_H__

#define         BAR_SPACE       6

#include "common.h"

class CLabelBar
{
public:
    CLabelBar(DFBRectangle *dfbRect);
	CLabelBar(DFBRectangle *dfbRectOut,DFBRectangle *dfbRectIn);
    
    ~CLabelBar();


private:
    
    /*  */
    BOOL InitLabel(DFBRectangle *dfbRect);
	BOOL InitLabel(DFBRectangle *dfbRectOut,DFBRectangle *dfbRectIn);

    /*  */
    void DestoryLabel();

    /*  */
    DFBResult CreateLiTELabel(DFBRectangle *dfbRect, DFBColor *dfbBgColor, int fontSize, DFBColor *dfbFontColor, LiteLabel **liteLabel);
    
private:

    /*  */
    LiteLabel*                  m_outerLabel;

    /*  */
    LiteLabel*                  m_innerLabel;
};

#endif

