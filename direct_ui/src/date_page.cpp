/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: date_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#include "date_page.h"
#include "desktop.h"
#include "setting.h"
#include "cw_timechip.h"

CDatePage::CDatePage(CAbsPage *parentPage)
	: CTimeAbsPage(parentPage)
{
	m_monthLabel 	= NULL;
	m_dayLabel 		= NULL;
	m_yearLabel 	= NULL;

	m_curDateStatus = Date_Status_Month;
}

CDatePage::~CDatePage()
{
}

void CDatePage::OnSubPageInit()
{
	CSettingPage*			pSetting = NULL;

	if( HasParentPage() )
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		if( pSetting != NULL )
		{
			tsSubDate *pSubDate = &g_CurLangDesc.Setting.MainDate.SubDate;
			if( g_isFirstTimeSetting )
				pSetting->ShowTitleText(pSubDate->TitleFirst.c_str());
			else
				pSetting->ShowTitleText(pSubDate->Title.c_str());
		}
	}

	InitMonthLabel();
	InitDayLabel();
	InitYearLabel();

	SetActiveLabel(Date_Status_Month);
}

void CDatePage::OnSubPageExit()
{
	SAFE_DEL_LITE(m_monthLabel);
	SAFE_DEL_LITE(m_dayLabel);
	SAFE_DEL_LITE(m_yearLabel);
}

void CDatePage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (dfbEvent->type == DWET_KEYDOWN)
	{
		switch (dfbEvent->key_code)
		{
			case LEFT_BTN:
			{
				OnLeftKeyEvent();
				break;
			}

			case RIGHT_BTN:
			{
				OnRightKeyEvent();
				break;
			}

			case UP_BTN:
			{
				OnUpKeyEvent();
				break;
			}

			case DOWN_BTN:
			{
				OnDownKeyEvent();
				break;
			}

			case OK_BTN:
			{
				OnOKKeyEvent();
				break;
			}

			case SETTING_BTN:
			{
				//OnSettingKeyEvent();
				break;
			}

			case REPLAY_BTN:
			{
				OnReplayKeyEvent();
				break;
			}

			case VIDEO_CAP_BTN:
			{
				OnVideoCaptureKeyEvent();
				break;
			}

			default:
			{
				DBG("[DirectFBUI-VideoPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CDatePage::SetActiveLabel(Date_Status_E dateStatus)
{
	LiteBox*			liteMonthBox	= NULL;
	LiteBox*			liteDayBox		= NULL;
	LiteBox*			liteYearBox		= NULL;

	if ((dateStatus < Date_Status_Begin) || (dateStatus > Date_Status_End))
	{
		return;
	}

	liteMonthBox				= LITE_BOX(m_monthLabel);
	liteDayBox					= LITE_BOX(m_dayLabel);
	liteYearBox					= LITE_BOX(m_yearLabel);

	if (dateStatus == Date_Status_Month)
	{
		liteMonthBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteMonthBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteMonthBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteMonthBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteDayBox->background->a	= DAT_BG_COLOR_A;
		liteDayBox->background->r	= DAT_BG_COLOR_R;
		liteDayBox->background->g	= DAT_BG_COLOR_G;
		liteDayBox->background->b	= DAT_BG_COLOR_B;

		liteYearBox->background->a	= DAT_BG_COLOR_A;
		liteYearBox->background->r	= DAT_BG_COLOR_R;
		liteYearBox->background->g	= DAT_BG_COLOR_G;
		liteYearBox->background->b	= DAT_BG_COLOR_B;
	}
	else if (dateStatus == Date_Status_Day)
	{
		liteDayBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteDayBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteDayBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteDayBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteMonthBox->background->a	= DAT_BG_COLOR_A;
		liteMonthBox->background->r	= DAT_BG_COLOR_R;
		liteMonthBox->background->g	= DAT_BG_COLOR_G;
		liteMonthBox->background->b	= DAT_BG_COLOR_B;

		liteYearBox->background->a	= DAT_BG_COLOR_A;
		liteYearBox->background->r	= DAT_BG_COLOR_R;
		liteYearBox->background->g	= DAT_BG_COLOR_G;
		liteYearBox->background->b	= DAT_BG_COLOR_B;
	}
	else if (dateStatus == Date_Status_Year)
	{
		liteYearBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteYearBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteYearBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteYearBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteDayBox->background->a	= DAT_BG_COLOR_A;
		liteDayBox->background->r	= DAT_BG_COLOR_R;
		liteDayBox->background->g	= DAT_BG_COLOR_G;
		liteDayBox->background->b	= DAT_BG_COLOR_B;

		liteMonthBox->background->a	= DAT_BG_COLOR_A;
		liteMonthBox->background->r	= DAT_BG_COLOR_R;
		liteMonthBox->background->g	= DAT_BG_COLOR_G;
		liteMonthBox->background->b	= DAT_BG_COLOR_B;
	}

	m_curDateStatus 		= dateStatus;

	lite_update_box(liteMonthBox, NULL);
	lite_update_box(liteDayBox, NULL);
	lite_update_box(liteYearBox, NULL);
}

void CDatePage::JumpToParentPage()
{
	CSettingPage* pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		if (pSetting)
		{
			pSetting->Recover();
		}
	}
}

void CDatePage::AfterDrawMonthLabelCB(void* data)
{
	CDatePage *pPage = (CDatePage *)data;

	if (pPage)
	{
		pPage->AfterDrawMonthLabel();
	}
}

void CDatePage::AfterDrawMonthLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_monthLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	char					szMonthText[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iMonth			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;
	char *pItem = NULL;

	surface->GetFont(surface, &fontInterface);

	// cur month
	// Draw main text
	iMonth = g_curLocalTime.tm_mon + 1;
	sprintf(szMonthText, "%02d", iMonth);
	fontInterface->GetStringWidth(fontInterface, szMonthText, strlen(szMonthText), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, szMonthText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	// Draw sub text
	surface->SetFont(surface, m_labelSubFontInterface);
	pItem = g_CurLangDesc.Month[iMonth-1].ShortDesc.c_str();
	m_labelSubFontInterface->GetStringWidth(m_labelSubFontInterface, pItem, strlen(pItem), &fontWidth);
	surface->DrawString(surface, pItem, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + fontHeight + 3, flags);
	surface->SetFont(surface, fontInterface);

	// Draw up text to bk
	surface->SetColor(surface, 0x3f, 0x3f, 0x3f, 0xFF);
	dfbRect.x			= 0;
	dfbRect.y			= 0;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 0, DAT_L_W, DAT_L_H);
	// Draw down text to bk
	dfbRect.x			= 0;
	dfbRect.y			= 2 * DAT_L_H;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 2 * DAT_L_H, DAT_L_W, DAT_L_H);

	if (m_curDateStatus == Date_Status_Month)
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		// prev month
		iMonth			= (g_curLocalTime.tm_mon + 1) - 1;
		iMonth 			= (iMonth < 1) ? 12 : iMonth;
		sprintf(szMonthText, "%02d", iMonth);
		fontInterface->GetStringWidth(fontInterface, szMonthText, strlen(szMonthText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szMonthText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

		// next month
		iMonth			= (g_curLocalTime.tm_mon + 1) + 1;
		iMonth			= (iMonth > 12) ? 1 : iMonth;
		sprintf(szMonthText, "%02d", iMonth);
		fontInterface->GetStringWidth(fontInterface, szMonthText, strlen(szMonthText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szMonthText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);
	}

	lite_update_box(LITE_BOX(m_dayLabel), NULL);
}

BOOL CDatePage::InitMonthLabel()
{
	if (FALSE == InitFirstLabel(&m_monthLabel))
	{
		DBG("[DirectFBUI-DatePage] create new month label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_monthLabel, AfterDrawMonthLabelCB, this);

	return TRUE;
}

BOOL CDatePage::InitDayLabel()
{
	if (FALSE == InitSecondLabel(&m_dayLabel))
	{
		DBG("[DirectFBUI-DatePage] create new day label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_dayLabel, AfterDrawDayLabelCB, this);

	return TRUE;
}

void CDatePage::AfterDrawDayLabelCB(void* data)
{
	CDatePage *pPage = (CDatePage *)data;

	if (pPage)
	{
		pPage->AfterDrawDayLabel();
	}
}

void CDatePage::AfterDrawDayLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_dayLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	char					szDayText[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iDay			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;
	int						iMaxDays 		= 30;
	char *pItem = NULL;

	surface->GetFont(surface, &fontInterface);

	// cur day
	// Draw main text
	iDay = g_curLocalTime.tm_mday;
	sprintf(szDayText, "%02d", iDay);
	fontInterface->GetStringWidth(fontInterface, szDayText, strlen(szDayText), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, szDayText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	// Draw sub text
	time_t timep = mktime(&g_curLocalTime);
	struct tm* p = localtime(&timep);

	surface->SetFont(surface, m_labelSubFontInterface);
	pItem = g_CurLangDesc.Week[p->tm_wday].ShortDesc.c_str();
	m_labelSubFontInterface->GetStringWidth(m_labelSubFontInterface, pItem, strlen(pItem), &fontWidth);
	surface->DrawString(surface, pItem, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + fontHeight + 3, flags);
	surface->SetFont(surface, fontInterface);

	surface->SetColor(surface, 0x3f, 0x3f, 0x3f, 0xFF);
	dfbRect.x			= 0;
	dfbRect.y			= 0;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 0, DAT_L_W, DAT_L_H);
	dfbRect.x			= 0;
	dfbRect.y			= 2 * DAT_L_H;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 2 * DAT_L_H, DAT_L_W, DAT_L_H);

	if (m_curDateStatus == Date_Status_Day)
	{
		iMaxDays		= GetMaxDayOfMonth(&g_curLocalTime);

		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
		// prev
		iDay			= g_curLocalTime.tm_mday - 1;
		iDay 			= (iDay < 1) ? iMaxDays : iDay;
		sprintf(szDayText, "%02d", iDay);
		fontInterface->GetStringWidth(fontInterface, szDayText, strlen(szDayText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szDayText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

		// next
		iDay			= g_curLocalTime.tm_mday + 1;
		iDay			= (iDay > iMaxDays) ? 1 : iDay;
		sprintf(szDayText, "%02d", iDay);
		fontInterface->GetStringWidth(fontInterface, szDayText, strlen(szDayText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szDayText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);
	}
}

BOOL CDatePage::InitYearLabel()
{
	if (FALSE == InitThirdLabel(&m_yearLabel))
	{
		DBG("[DirectFBUI-DatePage] create new year label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_yearLabel, AfterDrawYearLabelCB, this);

	return TRUE;
}

void CDatePage::AfterDrawYearLabelCB(void* data)
{
	CDatePage *pPage = (CDatePage *)data;

	if (pPage)
	{
		pPage->AfterDrawYearLabel();
	}
}

void CDatePage::AfterDrawYearLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_yearLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	char					szYearText[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iYear			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;

	surface->GetFont(surface, &fontInterface);

	// cur year
	iYear 				= (g_curLocalTime.tm_year + 1900);
	memset(szYearText, 0, MAX_LANG_LEN);
	if ((iYear - BASE_YEAR) < 10)
	{
		sprintf(szYearText, "0%d", (iYear - BASE_YEAR));
	}
	else
	{
		sprintf(szYearText, "%d", (iYear - BASE_YEAR));
	}

	fontInterface->GetStringWidth(fontInterface, szYearText, strlen(szYearText), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, szYearText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	surface->SetFont(surface, m_labelSubFontInterface);
	memset(szYearText, 0, MAX_LANG_LEN);
	sprintf(szYearText, "%d", iYear);
	m_labelSubFontInterface->GetStringWidth(m_labelSubFontInterface, szYearText, strlen(szYearText), &fontWidth);
	surface->DrawString(surface, szYearText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + fontHeight + 3, flags);
	surface->SetFont(surface, fontInterface);

	surface->SetColor(surface, 0x3f, 0x3f, 0x3f, 0xFF);
	dfbRect.x			= 0;
	dfbRect.y			= 0;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 0, DAT_L_W, DAT_L_H);
	dfbRect.x			= 0;
	dfbRect.y			= 2 * DAT_L_H;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 2 * DAT_L_H, DAT_L_W, DAT_L_H);

	if (m_curDateStatus == Date_Status_Year)
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		// prev year
		iYear			= (g_curLocalTime.tm_year + 1900) - 1;
		iYear 			= (iYear < MIN_YEAR) ? MAX_YEAR : iYear;
		memset(szYearText, 0, MAX_LANG_LEN);
		if ((iYear - BASE_YEAR) < 10)
		{
			sprintf(szYearText, "0%d", (iYear - BASE_YEAR));
		}
		else
		{
			sprintf(szYearText, "%d", (iYear - BASE_YEAR));
		}
		fontInterface->GetStringWidth(fontInterface, szYearText, strlen(szYearText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szYearText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

		// next year
		iYear			= (g_curLocalTime.tm_year + 1900) + 1;
		iYear			= (iYear > MAX_YEAR) ? MIN_YEAR : iYear;
		memset(szYearText, 0, MAX_LANG_LEN);
		if ((iYear - BASE_YEAR) < 10)
		{
			sprintf(szYearText, "0%d", (iYear - BASE_YEAR));
		}
		else
		{
			sprintf(szYearText, "%d", (iYear - BASE_YEAR));
		}
		fontInterface->GetStringWidth(fontInterface, szYearText, strlen(szYearText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szYearText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);
	}

	surface->SetFont(surface, fontInterface);

	lite_update_box(LITE_BOX(m_dayLabel), NULL);

#if 1
	DBG("AfterDrawYearLabel, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif
}

void CDatePage::OnLeftKeyEvent(void)
{
	int dateStatus = (int)m_curDateStatus;

	dateStatus -= 1;
	dateStatus = (dateStatus < (int)Date_Status_Month) ? (int)Date_Status_Year : dateStatus;
	dateStatus = (dateStatus > (int)Date_Status_Year) ? (int)Date_Status_Month : dateStatus;
	SetActiveLabel((Date_Status_E)dateStatus);
}

void CDatePage::OnRightKeyEvent(void)
{
	int dateStatus = (int)m_curDateStatus;

	dateStatus += 1;
	dateStatus = (dateStatus < (int)Date_Status_Month) ? (int)Date_Status_Year : dateStatus;
	dateStatus = (dateStatus > (int)Date_Status_Year) ? (int)Date_Status_Month : dateStatus;
	SetActiveLabel((Date_Status_E)dateStatus);
}

void CDatePage::OnDownKeyEvent(void)
{
	LiteBox*			liteBox		= NULL;
	int					curMonth 	= 0;
	int					curDay		= 0;
	int 				maxDays		= 30;
	int					curYear		= BASE_YEAR;

	if (m_curDateStatus == Date_Status_Month)
	{
		curMonth				= g_curLocalTime.tm_mon + 1;
		curMonth				+= 1;
		curMonth				= (curMonth > 12) ? 1 : curMonth;
		g_curLocalTime.tm_mon	= curMonth - 1;
		liteBox					= LITE_BOX(m_monthLabel);

		maxDays 				= GetMaxDayOfMonth(&g_curLocalTime);

		if(g_curLocalTime.tm_mday > maxDays)
			g_curLocalTime.tm_mday = maxDays;
	}
	else if (m_curDateStatus == Date_Status_Day)
	{
		maxDays				= GetMaxDayOfMonth(&g_curLocalTime);
		curDay				= g_curLocalTime.tm_mday;
		curDay				+= 1;
		curDay				= (curDay > maxDays) ? 1 : curDay;
		g_curLocalTime.tm_mday = curDay;
		liteBox					= LITE_BOX(m_dayLabel);
	}
	else if (m_curDateStatus == Date_Status_Year)
	{
		curYear 			= (g_curLocalTime.tm_year + 1900);
		curYear				+= 1;
		curYear				= (curYear > MAX_YEAR) ? MIN_YEAR : curYear;
		g_curLocalTime.tm_year	= curYear - 1900;
		liteBox					= LITE_BOX(m_yearLabel);

		maxDays 				= GetMaxDayOfMonth(&g_curLocalTime);

		if(g_curLocalTime.tm_mday > maxDays)
			g_curLocalTime.tm_mday = maxDays;
	}

#if 1
	DBG("OnDownKeyEvent, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif

	lite_update_box(liteBox, NULL);
}

void CDatePage::OnUpKeyEvent(void)
{
	LiteBox*			liteBox		= NULL;
	int					curMonth 	= 0;
	int					curDay		= 0;
	int 				maxDays		= 30;
	int					curYear		= BASE_YEAR;

	if (m_curDateStatus == Date_Status_Month)
	{
		curMonth				= g_curLocalTime.tm_mon + 1;
		curMonth				-= 1;
		curMonth				= (curMonth < 1) ? 12 : curMonth;
		g_curLocalTime.tm_mon	= curMonth - 1;
		liteBox					= LITE_BOX(m_monthLabel);

		maxDays 				= GetMaxDayOfMonth(&g_curLocalTime);

		if(g_curLocalTime.tm_mday > maxDays)
			g_curLocalTime.tm_mday = maxDays;
	}
	else if (m_curDateStatus == Date_Status_Day)
	{
		maxDays					= GetMaxDayOfMonth(&g_curLocalTime);
		curDay					= g_curLocalTime.tm_mday;
		curDay					-= 1;
		curDay					= (curDay < 1) ? maxDays : curDay;
		g_curLocalTime.tm_mday = curDay;
		liteBox					= LITE_BOX(m_dayLabel);
	}
	else if (m_curDateStatus == Date_Status_Year)
	{
		curYear 				= (g_curLocalTime.tm_year + 1900);
		curYear					-= 1;
		curYear					= (curYear < MIN_YEAR) ? MAX_YEAR : curYear;
		g_curLocalTime.tm_year	= curYear - 1900;
		liteBox					= LITE_BOX(m_yearLabel);

		maxDays 				= GetMaxDayOfMonth(&g_curLocalTime);

		if(g_curLocalTime.tm_mday > maxDays)
			g_curLocalTime.tm_mday = maxDays;
	}

#if 1
	DBG("OnUpKeyEvent, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif

	lite_update_box(liteBox, NULL);
}

void CDatePage::OnOKKeyEvent(void)
{
#if 0
	SaveDateTime();

	// sleep 1��, ��Ȼ����settingҳ��ʱ, �ᵼ�»�ȡ�����޸ĺ��ʱ��
	usleep(1000 * 1000);

	JumpToParentPage();
#else
#if 1
	DBG("OnOKKeyEvent 1111, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif

	time_t now = time(NULL);
	struct tm* ptm = localtime(&now);

	g_curLocalTime.tm_hour = ptm->tm_hour;
	g_curLocalTime.tm_min = ptm->tm_min;
	g_curLocalTime.tm_sec = ptm->tm_sec;

#if 1
	DBG("OnOKKeyEvent 2222, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif

	time_t tv = mktime(&g_curLocalTime);

#if 1
	DBG("OnOKKeyEvent 3333, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
#endif

/*change by lixiang
 * CWTimeChipSet(tv);*/

	if (0 > stime(&tv))
	{
		return FAILURE;
	}

	DBG("OnOKKeyEvent, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);

	JumpToParentPage();
#endif
}

void CDatePage::OnSettingKeyEvent(void)
{
	if(!g_isFirstTimeSetting)
		JumpToParentPage();
}

void CDatePage::OnReplayKeyEvent(void)
{
}

void CDatePage::OnVideoCaptureKeyEvent(void)
{

}

//-------------------------------------------------------
void CDatePage::InitDateTime()
{
	if(	g_isFirstTimeSetting )
	{
		g_curLocalTime.tm_year = 115; //2015.01.01
		g_curLocalTime.tm_mon = 0;
		g_curLocalTime.tm_mday = 1;

		DBG("aaa, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
	}
	else
	{
		time_t t = time(NULL);
		struct tm *pTime = localtime(&t);

		g_curLocalTime.tm_year = pTime->tm_year;
		g_curLocalTime.tm_mon = pTime->tm_mon;
		g_curLocalTime.tm_mday = pTime->tm_mday;

		DBG("bbb, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
	}
}



