/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: label_bar.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.07
	Description	:
	History		:
					Create by zyfan.2015.04.07
******************************************************************************/
#include "desktop.h"
#include "label_bar.h"

CLabelBar::CLabelBar(DFBRectangle *dfbRect)
{
	m_outerLabel = NULL;
	m_innerLabel = NULL;

	InitLabel(dfbRect);
}

CLabelBar::CLabelBar(DFBRectangle *dfbRectOut,DFBRectangle *dfbRectIn)
{

	m_outerLabel = NULL;
	m_innerLabel = NULL;

	InitLabel(dfbRectOut,dfbRectIn);
}

CLabelBar::~CLabelBar()
{
	DestoryLabel();
}

BOOL CLabelBar::InitLabel(DFBRectangle *dfbRect)
{
	DFBColor bgColor;
	DFBRectangle rect;

	rect.x			= dfbRect->x;
	rect.y			= dfbRect->y;
	rect.w			= dfbRect->w;
	rect.h			= dfbRect->h;
	bgColor.r		= 0x3f;
	bgColor.g		= 0x3f;
	bgColor.b		= 0x3f;
	bgColor.a		= 0xFF;
	if (DFB_OK != CreateLiTELabel(&rect, &bgColor, 12, NULL, &m_outerLabel))
	{
		DBG("[DirectFBUI-LabelBar] create label error\n");
		return FALSE;
	}

	rect.x 			= dfbRect->x + BAR_SPACE;
	rect.y			= dfbRect->y;
	rect.w			= dfbRect->w - 2 * BAR_SPACE;
	rect.h			= dfbRect->h;

	bgColor.r		= 0x70;
	bgColor.g		= 0x70;
	bgColor.b		= 0x70;
	bgColor.a		= 0xFF;
	if (DFB_OK != CreateLiTELabel(&rect, &bgColor, 12, NULL, &m_innerLabel))
	{
		DBG("[DirectFBUI-LabelBar] create label error\n");
		return FALSE;
	}

	lite_set_label_alignment(m_innerLabel, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_innerLabel, TRUE);

	lite_set_label_alignment(m_outerLabel, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_outerLabel, TRUE);

	return TRUE;
}

BOOL CLabelBar::InitLabel(DFBRectangle *dfbRectOut,DFBRectangle *dfbRectIn)
{
	DFBColor bgColor;
	DFBRectangle rect;

	rect.x			= dfbRectOut->x;
	rect.y			= dfbRectOut->y;
	rect.w			= dfbRectOut->w;
	rect.h			= dfbRectOut->h;
	bgColor.r		= 0x95;
	bgColor.g		= 0x95;
	bgColor.b		= 0x95;
	bgColor.a		= 0xFF;
	if (DFB_OK != CreateLiTELabel(&rect, &bgColor, 12, NULL, &m_outerLabel))
	{
		DBG("[DirectFBUI-LabelBar] create label error\n");
		return FALSE;
	}

	rect.x 			= dfbRectIn->x;
	rect.y			= dfbRectIn->y;
	rect.w			= dfbRectIn->w;
	rect.h			= dfbRectIn->h;
	bgColor.r		= 0x0;
	bgColor.g		= 0x0;
	bgColor.b		= 0x0;
	bgColor.a		= 0x0;
	if (DFB_OK != CreateLiTELabel(&rect, &bgColor, 12, NULL, &m_innerLabel))
	{
		DBG("[DirectFBUI-LabelBar] create label error\n");
		return FALSE;
	}

	lite_set_label_alignment(m_innerLabel, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_innerLabel, TRUE);

	lite_set_label_alignment(m_outerLabel, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_outerLabel, TRUE);

	return TRUE;
}

void CLabelBar::DestoryLabel()
{
	SAFE_DEL_LITE(m_outerLabel);
	SAFE_DEL_LITE(m_innerLabel);
}

DFBResult CLabelBar::CreateLiTELabel(DFBRectangle *dfbRect, DFBColor *dfbBgColor, int fontSize, DFBColor *dfbFontColor, LiteLabel **liteLabel)
{
	if (dfbRect == NULL)
	{
		DBG("[DirectFBUI-LabelBar] paramter must not null\n");
		return DFB_INVARG;
	}

	CDesktop*		pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*		liteWin 	= pDesktop->GetLiTEWindow();
	LiteLabelTheme	theme;

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-LabelBar] lite window must init first\n");
		return DFB_INVARG;
	}

	memset(&theme, 0, sizeof(LiteLabelTheme));

	if (dfbBgColor == NULL)
	{
		lite_new_label(LITE_BOX(liteWin), dfbRect, NULL, fontSize, dfbFontColor, liteLabel);
	}
	else
	{
		theme.theme.bg_color = *dfbBgColor;
		lite_new_label(LITE_BOX(liteWin), dfbRect, &theme, fontSize, dfbFontColor, liteLabel);
	}

	DBG("[DirectFBUI-LabelBar] crate label at [%d %d %d %d]\n", dfbRect->x, dfbRect->y, dfbRect->w, dfbRect->h);

	return DFB_OK;
}


