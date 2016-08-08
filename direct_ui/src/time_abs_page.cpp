/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: time_abs_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.23
******************************************************************************/

#include "time_abs_page.h"
#include "setting.h"
#include "desktop.h"
#include "cw_timechip.h"

CTimeAbsPage::CTimeAbsPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_bgLabel = NULL;
	m_leftRightText = NULL;
	m_leftRightArrow = NULL;
	m_upDownText = NULL;
	m_upDownArrow = NULL;

	m_labelSubFont = NULL;
	m_labelSubFontInterface = NULL;

	m_doneText = NULL;
	m_doOkImage = NULL;

	m_lblExit = NULL;
	m_imgExit = NULL;
}

CTimeAbsPage::~CTimeAbsPage()
{

}

void CTimeAbsPage::OnPageInit()
{
#if 0
	struct tm *pTime;
	struct tm oTime;

	memset(&oTime, 0, sizeof(oTime));

	if(	CDesktop::GetDesktop()->isFirstTimeSetting() )
	{
		oTime.tm_year = 115; //2015.01.01
		oTime.tm_mon = 0;
		oTime.tm_mday = 1;

	}
	else
	{
		oTime.tm_year = 112; //2012.01.01
		oTime.tm_mon = 0;
		oTime.tm_mday = 1;
	}

	DBG("OnPageInit, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+oTime.tm_year), oTime.tm_mon, oTime.tm_mday, \
			oTime.tm_hour, oTime.tm_min, oTime.tm_sec);

	time_t timebase = mktime(&oTime);
	time_t t = time(NULL);

	if( t > timebase )
	{
		pTime = localtime(&t);
	}
	else
	{
		pTime = localtime(&timebase);
	}

	DBG("OnPageInit, %04d-%02d-%02d\n", (1900+pTime->tm_year),pTime->tm_mon,pTime->tm_mday);

	g_curLocalTime = (struct tm*)malloc(sizeof(struct tm));
	memcpy(g_curLocalTime, pTime, sizeof(struct tm));
#else
	InitDateTime();
#endif

	if( g_isFirstTimeSetting )
	//if( 1 ) // xjf, for debug
		InitBGAndBottom();
	else
		InitSaveExit();

	InitSubStrFont();
	OnSubPageInit();
}

void CTimeAbsPage::OnPageExit()
{
	DestoryBGAndBottom();
	DestorySubStrFont();
	OnSubPageExit();
}

void CTimeAbsPage::JumpToParentPage()
{
	CSettingPage*			pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		if (pSetting)
		{
			pSetting->Recover();
		}
	}
}

void CTimeAbsPage::FillDoubleTriangle(IDirectFBSurface* surface, DFBRectangle *dfbRect)
{
	if ((surface == NULL) || (dfbRect == NULL))
	{
		DBG("[DirectFBUI-TimeAbsPage] parameter can not be null\n");
		return;
	}

	int space				= 2;	// pixel
	int	width				= 16;	// pixel
	int height				= 10;
	DFBPoint				p1, p2, p3;

	// ��������
	p1.x					= dfbRect->x + dfbRect->w / 2;
	p1.y					= dfbRect->h - height - space;
	p2.x					= (dfbRect->w - width) / 2;
	p2.y					= dfbRect->h - space;
	p3.x					= (dfbRect->w + width) / 2;
	p3.y					= dfbRect->h - space;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	// ��������
	p1.x					= dfbRect->x + dfbRect->w / 2;
	p1.y					= dfbRect->y + dfbRect->h + height + space;
	p2.x					= (dfbRect->w - width) / 2;
	p2.y					= dfbRect->y + dfbRect->h + space;
	p3.x					= (dfbRect->w + width) / 2;
	p3.y					= dfbRect->y + dfbRect->h + space;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void CTimeAbsPage::InitSaveExit()
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	DFBColor			bgColor;
	CDesktop*			pDesktop 	= NULL;
	LiteWindow*			liteWin 	= NULL;
	int 				screenWidth = 0;
	int 				screenHeight = 0;

	pDesktop 		= CDesktop::GetDesktop();
	liteWin			= pDesktop->GetLiTEWindow();

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	// ��ʼ������
	bgColor.a				= 0xFF;
	bgColor.r				= 0x3f;
	bgColor.g				= 0x3f;
	bgColor.b				= 0x3f;

	dfbRect.x				= 0;
	dfbRect.y				= TOP_LABEL_H;
	dfbRect.w				= screenWidth;
	dfbRect.h				= screenHeight - TOP_LABEL_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 12, NULL, &m_bgLabel))
	{
		DBG("[DirectFBUI-TimeAbsPage] create bg label failure\n");
		return FALSE;
	}

	// ��ʼ���ײ�
	fontColor.a				= DAT_FONT_COLOR_A;
	fontColor.r				= DAT_FONT_COLOR_R;
	fontColor.g				= DAT_FONT_COLOR_G;
	fontColor.b				= DAT_FONT_COLOR_B;

	int xx = 100;
	int xoff = 8;
	int cx = 46;

	//--------------------------------------------------------
	// Left/Right
	//--------------------------------------------------------
	// Save
	//DBG("xx = %d\n", xx);
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM) + 2;
	dfbRect.w 				= cx;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_doneText))
	{
		DBG("[DirectFBUI-TimeAbsPage] create done text label failure\n");
		return FALSE;
	}
	lite_set_label_text(m_doneText, g_CurLangDesc.Public.DateTimeSave.c_str());
	lite_set_label_alignment(m_doneText, LITE_LABEL_LEFT);

	// OK image
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= DAT_DONE_IMAGE_Y;
	dfbRect.w				= DAT_DONE_IMAGE_W;
	dfbRect.h				= DAT_DONE_IMAGE_H;
	if (DFB_OK != CreateLiTEImage(&dfbRect, g_CurLangDesc.Public.DateTimeSavePict.c_str(), &m_doOkImage) )
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_doOkImage failure\n");
		return FALSE;
	}

	//--------------------------------------------------------
	// Exit
	xx = dfbRect.x + dfbRect.w + xoff;
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM) + 2;
	dfbRect.w 				= cx;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_lblExit))
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_lblExit failure\n");
		return FALSE;
	}
	lite_set_label_text(m_lblExit, g_CurLangDesc.Public.DateTimeExit.c_str());
	lite_set_label_alignment(m_lblExit, LITE_LABEL_LEFT);

	// Exit image
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= DAT_DONE_IMAGE_Y;
	dfbRect.w				= DAT_DONE_IMAGE_W;
	dfbRect.h				= DAT_DONE_IMAGE_H;
	if (DFB_OK != CreateLiTEImage(&dfbRect, g_CurLangDesc.Public.DateTimeExitPict.c_str(), &m_imgExit) )
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_imgExit failure\n");
		return FALSE;
	}
}

BOOL CTimeAbsPage::InitBGAndBottom()
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	DFBColor			bgColor;
	CDesktop*			pDesktop 	= NULL;
	LiteWindow*			liteWin 	= NULL;
	int 				screenWidth = 0;
	int 				screenHeight = 0;
	char 				szText[MAX_LANG_LEN];

	pDesktop 		= CDesktop::GetDesktop();
	liteWin			= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-TimeAbsPage] lite window must init first\n");
		return FALSE;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	// ��ʼ������
	bgColor.a				= 0xFF;
	bgColor.r				= 0x3f;
	bgColor.g				= 0x3f;
	bgColor.b				= 0x3f;

	dfbRect.x				= 0;
	dfbRect.y				= TOP_LABEL_H;
	dfbRect.w				= screenWidth;
	dfbRect.h				= screenHeight - TOP_LABEL_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 12, NULL, &m_bgLabel))
	{
		DBG("[DirectFBUI-TimeAbsPage] create bg label failure\n");
		return FALSE;
	}

	// ��ʼ���ײ�
	fontColor.a				= DAT_FONT_COLOR_A;
	fontColor.r				= DAT_FONT_COLOR_R;
	fontColor.g				= DAT_FONT_COLOR_G;
	fontColor.b				= DAT_FONT_COLOR_B;

	int xx = 6;
	int xoff = 6;

	//--------------------------------------------------------
	// Left/Right
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM);
	dfbRect.w 				= 84;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_leftRightText))
	{
		DBG("[DirectFBUI-TimeAbsPage] create left/right text label failure\n");
		return FALSE;
	}
	lite_set_label_text(m_leftRightText, g_CurLangDesc.Public.DateTimeLeftRight.c_str());
	lite_set_label_alignment(m_leftRightText, LITE_LABEL_CENTER);

	// Left/Right arrow
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= screenHeight - (DAT_H + DAT_BOTTOM) + (DAT_H - DAT_LR_ARROW_H) / 2 +1;
	dfbRect.w				= DAT_LR_ARROW_W;
	dfbRect.h				= DAT_LR_ARROW_H;
	m_leftRightArrow 		= new CArrow(dfbRect, Arrow_Horizontal);
	// Active arrow, white
	m_leftRightArrow->setArrowActive();

	//--------------------------------------------------------
	// Up/Down
	xx = dfbRect.x + dfbRect.w + xoff;
	//DBG("xx1: %d\n", xx);
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM);
	dfbRect.w 				= 72;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_upDownText))
	{
		DBG("[DirectFBUI-TimeAbsPage] create up/down text label failure\n");
		return FALSE;
	}
	lite_set_label_text(m_upDownText, g_CurLangDesc.Public.DateTimeUpDown.c_str());
	lite_set_label_alignment(m_upDownText, LITE_LABEL_CENTER);

	// Up/Down arrow
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= screenHeight - (DAT_H + DAT_BOTTOM) + (DAT_H - DAT_UD_ARROW_H) / 2;
	dfbRect.w				= DAT_UD_ARROW_W;
	dfbRect.h				= DAT_UD_ARROW_H;
	m_upDownArrow			= new CArrow(dfbRect, Arrow_Vertical);
	// Active arrow, white
	m_upDownArrow->setArrowActive();

	//--------------------------------------------------------
	// Save
	xx = dfbRect.x + dfbRect.w + xoff;
	//DBG("xx = %d\n", xx);
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM) + 2;
	dfbRect.w 				= 46;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_doneText))
	{
		DBG("[DirectFBUI-TimeAbsPage] create done text label failure\n");
		return FALSE;
	}
	lite_set_label_text(m_doneText, g_CurLangDesc.Public.DateTimeSave.c_str());
	lite_set_label_alignment(m_doneText, LITE_LABEL_LEFT);

	// OK image
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= DAT_DONE_IMAGE_Y;
	dfbRect.w				= DAT_DONE_IMAGE_W;
	dfbRect.h				= DAT_DONE_IMAGE_H;
	if (DFB_OK != CreateLiTEImage(&dfbRect, g_CurLangDesc.Public.DateTimeSavePict.c_str(), &m_doOkImage) )
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_doOkImage failure\n");
		return FALSE;
	}

	//--------------------------------------------------------
	// Exit
	xx = dfbRect.x + dfbRect.w + xoff;
	dfbRect.x 				= xx;
	dfbRect.y 				= screenHeight - (DAT_H + DAT_BOTTOM) + 2;
	dfbRect.w 				= 44;
	dfbRect.h 				= DAT_H;
	if (DFB_OK != CreateLiTELabel(&dfbRect, NULL, DAT_FONT_SIZE, &fontColor, &m_lblExit))
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_lblExit failure\n");
		return FALSE;
	}
	lite_set_label_text(m_lblExit, g_CurLangDesc.Public.DateTimeExit.c_str());
	lite_set_label_alignment(m_lblExit, LITE_LABEL_LEFT);

	// Exit image
	xx = dfbRect.x + dfbRect.w;
	dfbRect.x				= xx;
	dfbRect.y				= DAT_DONE_IMAGE_Y;
	dfbRect.w				= DAT_DONE_IMAGE_W;
	dfbRect.h				= DAT_DONE_IMAGE_H;
	if (DFB_OK != CreateLiTEImage(&dfbRect, g_CurLangDesc.Public.DateTimeExitPict.c_str(), &m_imgExit) )
	{
		DBG("[DirectFBUI-TimeAbsPage] create m_imgExit failure\n");
		return FALSE;
	}

	return TRUE;
}

void CTimeAbsPage::DestoryBGAndBottom()
{
	SAFE_DEL_LITE(m_bgLabel);
	SAFE_DEL_LITE(m_leftRightText);
	SAFE_DELETE(m_leftRightArrow);
	SAFE_DEL_LITE(m_upDownText);
	SAFE_DELETE(m_upDownArrow);

	SAFE_DEL_LITE(m_doneText);
	SAFE_DEL_LITE(m_doOkImage);

	SAFE_DEL_LITE(m_lblExit);
	SAFE_DEL_LITE(m_imgExit);
}

BOOL CTimeAbsPage::InitSubStrFont()
{
	if (DFB_OK != lite_get_font("default", LITE_FONT_BOLD, DAT_TIME_SUB_FONT_SIZE, DEFAULT_FONT_ATTRIBUTE, &m_labelSubFont))
	{
		DBG("[DirectFBUI-TimeAbsPage] get lite font failure\n");
		return FALSE;
	}

	if (DFB_OK != lite_font(m_labelSubFont, &m_labelSubFontInterface))
	{
		DBG("[DirectFBUI-TimeAbsPage] get lit font object\n");
		return FALSE;
	}

	return TRUE;
}

void CTimeAbsPage::DestorySubStrFont()
{
	if (DFB_OK != lite_release_font(m_labelSubFont))
	{
		DBG("[DirectFBUI-TimeAbsPage] release lite font failure\n");
	}

	m_labelSubFont = NULL;
	m_labelSubFontInterface = NULL;
}

int CTimeAbsPage::GetMaxDayOfMonth(struct tm* localTime)
{
	int days = 30;
	int year = 1900;

	if (localTime == NULL)
	{
		return -1;
	}

	switch (localTime->tm_mon + 1)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{
			days = 31;
			break;
		}
		case 2:
		{
			year = localTime->tm_year + 1900;
			if((year % 4 == 0 && year % 100 !=0 ) || (year % 400 == 0))
			{
				days = 29;
			}
			else
			{
				days = 28;
			}
			break;
		}
		case 4:
		case 6:
		case 9:
		case 11:
		{
			days = 30;
			break;
		}
	}

	return days;
}

void CTimeAbsPage::SaveDateTime(void)
{
	time_t tv;

	//char szJcpCmd[256];
	//memset(szJcpCmd, 0, sizeof(szJcpCmd));

#if 0
	// ����cw_server��ʱ���������������Ĭ�ϼ���8Сʱ, �����ȥ8Сʱ.
	if (g_curLocalTime.tm_hour < 8)
	{
		//
		g_curLocalTime.tm_mday -= 1;
		g_curLocalTime.tm_hour += (23 - 8);
	}
	else
	{
		//
		g_curLocalTime.tm_hour -= 8;
	}
	printf("%s %d, year = %d, month = %d, mday = %d, hour = %d, minute = %d\n", __FUNCTION__, __LINE__,
			g_curLocalTime.tm_year + 1900, g_curLocalTime.tm_mon + 1, g_curLocalTime.tm_mday,
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min);
#endif

	tv = mktime(&g_curLocalTime);
//change by lixiang
	//CWTimeChipSet(tv);
	//done

	if (0 > stime(&tv))
	{
		return FAILURE;
	}

//	sprintf(szJcpCmd, "timecfg -act set -time %ld \r\n", mktime(g_curLocalTime));

//	SendJCPCmd(szJcpCmd);
}

BOOL CTimeAbsPage::InitFirstLabel(LiteLabel **label)
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	DFBColor			bgColor;

	dfbRect.x				= DAT_FIRST_L_X;
	dfbRect.y				= DAT_L_Y;
	dfbRect.w				= DAT_L_W;
	dfbRect.h				= DAT_L_H * 3;

	bgColor.a				= DAT_BG_COLOR_A;
	bgColor.r				= DAT_BG_COLOR_R;
	bgColor.g				= DAT_BG_COLOR_G;
	bgColor.b				= DAT_BG_COLOR_B;

	fontColor.a				= DAT_FONT_COLOR_A;
	fontColor.r				= DAT_FONT_COLOR_R;
	fontColor.g				= DAT_FONT_COLOR_G;
	fontColor.b				= DAT_FONT_COLOR_B;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, DAT_TIME_FONT_SIZE, &fontColor, label))
	{
		DBG("[DirectFBUI-TimeAbsPage] create first label failure\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CTimeAbsPage::InitSecondLabel(LiteLabel **label)
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	DFBColor			bgColor;

	dfbRect.x				= DAT_SECOND_L_X;
	dfbRect.y				= DAT_L_Y;
	dfbRect.w				= DAT_L_W;
	dfbRect.h				= DAT_L_H * 3;

	bgColor.a				= DAT_BG_COLOR_A;
	bgColor.r				= DAT_BG_COLOR_R;
	bgColor.g				= DAT_BG_COLOR_G;
	bgColor.b				= DAT_BG_COLOR_B;

	fontColor.a				= DAT_FONT_COLOR_A;
	fontColor.r				= DAT_FONT_COLOR_R;
	fontColor.g				= DAT_FONT_COLOR_G;
	fontColor.b				= DAT_FONT_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, DAT_TIME_FONT_SIZE, &fontColor, label))
	{
		DBG("[DirectFBUI-TimeAbsPage] create second label failure\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CTimeAbsPage::InitThirdLabel(LiteLabel **label)
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	DFBColor			bgColor;

	dfbRect.x				= DAT_THIRD_L_X;
	dfbRect.y				= DAT_L_Y;
	dfbRect.w				= DAT_L_W;
	dfbRect.h				= DAT_L_H * 3;

	bgColor.a				= DAT_BG_COLOR_A;
	bgColor.r				= DAT_BG_COLOR_R;
	bgColor.g				= DAT_BG_COLOR_G;
	bgColor.b				= DAT_BG_COLOR_B;

	fontColor.a				= DAT_FONT_COLOR_A;
	fontColor.r				= DAT_FONT_COLOR_R;
	fontColor.g				= DAT_FONT_COLOR_G;
	fontColor.b				= DAT_FONT_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, DAT_TIME_FONT_SIZE, &fontColor, label))
	{
		DBG("[DirectFBUI-DatePage] create third label failure\n");
		return FALSE;
	}

	return TRUE;
}




