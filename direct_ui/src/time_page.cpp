/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: time_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#include "time_page.h"
#include "setting.h"
#include "desktop.h"
#include "cw_timechip.h"

CTimePage::CTimePage(CAbsPage *parentPage)
	: CTimeAbsPage(parentPage)
{
	m_hourLabel = NULL;
	m_minuteLabel = NULL;
	m_ampmLabel = NULL;

	m_curTimeStatus = Time_Status_Begin;
}

CTimePage::~CTimePage()
{
}

int CTimePage::GetHour12(int CurHr)
{
	int iHour = 12;

	if( CurHr >= 12 )
	{
		if( CurHr == 12 )
			iHour = 12;
		else
			iHour = CurHr - 12;
	}
	else
	{
		if( CurHr == 0 )
			iHour = 12;
		else
			iHour = CurHr;
	}

	return iHour;
}

void CTimePage::OnSubPageInit()
{
	CSettingPage *pSetting = NULL;

	DBG("OnSubPageInit\n");

	if( HasParentPage() )
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		if( pSetting != NULL )
		{
			tsSubTime *pSubTime = &g_CurLangDesc.Setting.MainTime.SubTime;
			if( g_isFirstTimeSetting )
				pSetting->ShowTitleText(pSubTime->TitleFirst.c_str());
			else
				pSetting->ShowTitleText(pSubTime->Title.c_str());
		}
	}

	InitHourLabel();
	InitMinuteLabel();
	InitAMPMLabel();

	SetActiveLabel(Time_Status_Hour);
}

void CTimePage::OnSubPageExit()
{
	SAFE_DEL_LITE(m_hourLabel);
	SAFE_DEL_LITE(m_minuteLabel);
	SAFE_DEL_LITE(m_ampmLabel);

	m_curTimeStatus = Time_Status_Begin;
}

void CTimePage::OnKeyEvent(DFBWindowEvent *dfbEvent)
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

void CTimePage::SetActiveLabel(Time_Status_E timeStatus)
{
	LiteBox*			liteHourBox		= NULL;
	LiteBox*			liteMinuteBox	= NULL;
	LiteBox*			liteAMPMBox		= NULL;

	DBG("SetActiveLabel, timeStatus: %d\n", timeStatus);

	if ((timeStatus <= Time_Status_Begin) || (timeStatus >= Time_Status_End))
	{
		return;
	}

	liteHourBox					= LITE_BOX(m_hourLabel);
	liteMinuteBox				= LITE_BOX(m_minuteLabel);
	liteAMPMBox					= LITE_BOX(m_ampmLabel);

	if (timeStatus == Time_Status_Hour)
	{
		liteHourBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteHourBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteHourBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteHourBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteMinuteBox->background->a	= DAT_BG_COLOR_A;
		liteMinuteBox->background->r	= DAT_BG_COLOR_R;
		liteMinuteBox->background->g	= DAT_BG_COLOR_G;
		liteMinuteBox->background->b	= DAT_BG_COLOR_B;

		liteAMPMBox->background->a	= DAT_BG_COLOR_A;
		liteAMPMBox->background->r	= DAT_BG_COLOR_R;
		liteAMPMBox->background->g	= DAT_BG_COLOR_G;
		liteAMPMBox->background->b	= DAT_BG_COLOR_B;
	}
	else if (timeStatus == Time_Status_Minute)
	{
		liteMinuteBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteMinuteBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteMinuteBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteMinuteBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteHourBox->background->a	= DAT_BG_COLOR_A;
		liteHourBox->background->r	= DAT_BG_COLOR_R;
		liteHourBox->background->g	= DAT_BG_COLOR_G;
		liteHourBox->background->b	= DAT_BG_COLOR_B;

		liteAMPMBox->background->a	= DAT_BG_COLOR_A;
		liteAMPMBox->background->r	= DAT_BG_COLOR_R;
		liteAMPMBox->background->g	= DAT_BG_COLOR_G;
		liteAMPMBox->background->b	= DAT_BG_COLOR_B;
	}
	else if (timeStatus == Time_Status_AMPM)
	{
		liteAMPMBox->background->a	= DAT_BG_ACTIVE_COLOR_A;
		liteAMPMBox->background->r	= DAT_BG_ACTIVE_COLOR_R;
		liteAMPMBox->background->g	= DAT_BG_ACTIVE_COLOR_G;
		liteAMPMBox->background->b	= DAT_BG_ACTIVE_COLOR_B;

		liteHourBox->background->a	= DAT_BG_COLOR_A;
		liteHourBox->background->r	= DAT_BG_COLOR_R;
		liteHourBox->background->g	= DAT_BG_COLOR_G;
		liteHourBox->background->b	= DAT_BG_COLOR_B;

		liteMinuteBox->background->a	= DAT_BG_COLOR_A;
		liteMinuteBox->background->r	= DAT_BG_COLOR_R;
		liteMinuteBox->background->g	= DAT_BG_COLOR_G;
		liteMinuteBox->background->b	= DAT_BG_COLOR_B;
	}

	m_curTimeStatus = timeStatus;

	lite_update_box(liteHourBox, NULL);
	lite_update_box(liteMinuteBox, NULL);
	lite_update_box(liteAMPMBox, NULL);
}

void CTimePage::JumpToParentPage()
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

void CTimePage::OnLeftKeyEvent(void)
{
	int timeStatus = (int)m_curTimeStatus;

	timeStatus -= 1;
	timeStatus = (timeStatus < (int)Time_Status_Hour) ? (int)Time_Status_AMPM : timeStatus;

	SetActiveLabel((Time_Status_E)timeStatus);
}

void CTimePage::OnRightKeyEvent(void)
{
	int timeStatus = (int)m_curTimeStatus;

	timeStatus += 1;
	timeStatus = (timeStatus > (int)Time_Status_AMPM) ? (int)Time_Status_Hour : timeStatus;

	SetActiveLabel((Time_Status_E)timeStatus);
}

void CTimePage::OnDownKeyEvent(void)
{
	LiteBox*			liteBox		= NULL;
	int					curHour		= 0;
	int					curMinute	= 0;

	if (m_curTimeStatus == Time_Status_Hour)
	{
		curHour					= g_curLocalTime.tm_hour;
		curHour					+= 1;
		curHour					= (curHour > 23) ? 0 : curHour;
		g_curLocalTime.tm_hour	= curHour;
		liteBox					= LITE_BOX(m_hourLabel);
	}
	else if (m_curTimeStatus == Time_Status_Minute)
	{
		curMinute				= g_curLocalTime.tm_min;
		curMinute				+= 1;
		curMinute				= (curMinute > 59) ? 0 : curMinute;
		g_curLocalTime.tm_min 	= curMinute;
		liteBox					= LITE_BOX(m_minuteLabel);
	}
	else if (m_curTimeStatus == Time_Status_AMPM)
	{
	#if 0 // AM/PM not loop
		curHour					= g_curLocalTime.tm_hour;
		curHour					= (curHour < 12) ? (curHour+12) : (curHour);
		g_curLocalTime.tm_hour	= curHour;
		liteBox					= LITE_BOX(m_ampmLabel);
	#else
		if( m_timeAMPM24 == TIME_PM )
		{
			m_timeAMPM24 = TIME_24;
		}
		else if( m_timeAMPM24 == TIME_AM )
		{
			m_timeAMPM24 = TIME_PM;

			curHour	= g_curLocalTime.tm_hour;
			curHour	= (curHour < 12) ? (curHour+12) : (curHour);
			g_curLocalTime.tm_hour = curHour;
		}
		else
		{
			m_timeAMPM24 = TIME_AM;

			curHour	= g_curLocalTime.tm_hour;
			curHour	= (curHour >= 12) ? (curHour-12) : (curHour);
			g_curLocalTime.tm_hour = curHour;
		}

		liteBox					= LITE_BOX(m_ampmLabel);
		lite_update_box(LITE_BOX(m_hourLabel),NULL);
	#endif
	}

	lite_update_box(liteBox, NULL);
}

void CTimePage::OnUpKeyEvent(void)
{
	LiteBox*			liteBox		= NULL;
	int					curHour		= 0;
	int					curMinute	= 0;

	if (m_curTimeStatus == Time_Status_Hour)
	{
		curHour					= g_curLocalTime.tm_hour;
		curHour					-= 1;
		curHour					= (curHour < 0) ? 23 : curHour;
		g_curLocalTime.tm_hour	= curHour;
		liteBox					= LITE_BOX(m_hourLabel);
	}
	else if (m_curTimeStatus == Time_Status_Minute)
	{
		curMinute				= g_curLocalTime.tm_min;
		curMinute				-= 1;
		curMinute				= (curMinute < 0) ? 59 : curMinute;
		g_curLocalTime.tm_min 	= curMinute;
		liteBox					= LITE_BOX(m_minuteLabel);
	}
	else if (m_curTimeStatus == Time_Status_AMPM)
	{
	#if 0 // AM/PM not loop
		curHour					= g_curLocalTime.tm_hour;
		curHour					= (curHour >= 12) ? (curHour - 12) : curHour;
		g_curLocalTime.tm_hour	= curHour;
		liteBox					= LITE_BOX(m_ampmLabel);
	#else
		if( m_timeAMPM24 == TIME_PM )
		{
			m_timeAMPM24 = TIME_AM;

			curHour	= g_curLocalTime.tm_hour;
			curHour	= (curHour >= 12) ? (curHour-12) : (curHour);
			g_curLocalTime.tm_hour = curHour;
		}
		else if( m_timeAMPM24 == TIME_AM )
		{
			m_timeAMPM24 = TIME_24;
		}
		else
		{
			m_timeAMPM24 = TIME_PM;

			curHour	= g_curLocalTime.tm_hour;
			curHour	= (curHour < 12) ? (curHour+12) : (curHour);
			g_curLocalTime.tm_hour = curHour;
		}

		liteBox					= LITE_BOX(m_ampmLabel);
		lite_update_box(LITE_BOX(m_hourLabel),NULL);
	#endif
	}

	lite_update_box(liteBox, NULL);
}

void CTimePage::OnOKKeyEvent(void)
{
#if 0
	SaveDateTime();

	// sleep 1��, ��Ȼ����settingҳ��ʱ, �ᵼ�»�ȡ�����޸ĺ��ʱ��
	usleep(1000 * 1000);

	JumpToParentPage();
#else
	time_t now = time(NULL);
	struct tm* ptm = localtime(&now);

	g_curLocalTime.tm_year = ptm->tm_year;
	g_curLocalTime.tm_mon = ptm->tm_mon;
	g_curLocalTime.tm_mday = ptm->tm_mday;

	//g_curLocalTime.tm_sec = 0;

	time_t tv = mktime(&g_curLocalTime);

/*change by lixiang
 * 	CWTimeChipSet(tv);*/

	if (0 > stime(&tv))
	{
		return FAILURE;
	}

	if( m_timeAMPM24 == TIME_24 )
		g_RunParam.tuobangPara.TimeFmt = TIME_FMT_24HR;
	else
		g_RunParam.tuobangPara.TimeFmt = TIME_FMT_12HR;

	// Save time formate, 12HR/24
	sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

	DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);

	DBG("OnOKKeyEvent, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d, AMPM24: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec, m_timeAMPM24);

	JumpToParentPage();
#endif
}

void CTimePage::OnSettingKeyEvent(void)
{
	if(!g_isFirstTimeSetting)
		JumpToParentPage();
}

void CTimePage::OnReplayKeyEvent(void)
{
}

void CTimePage::OnVideoCaptureKeyEvent(void)
{
}

BOOL CTimePage::InitHourLabel()
{
	if (FALSE == InitFirstLabel(&m_hourLabel))
	{
		DBG("[DirectFBUI-TimePage] create new hour label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_hourLabel, AfterDrawHourLabelCB, this);

	return TRUE;
}

BOOL CTimePage::InitMinuteLabel()
{
	if (FALSE == InitSecondLabel(&m_minuteLabel))
	{
		DBG("[DirectFBUI-TimePage] create new minute label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_minuteLabel, AfterDrawMinuteLabelCB, this);

	return TRUE;
}

BOOL CTimePage::InitAMPMLabel()
{
	if (FALSE == InitThirdLabel(&m_ampmLabel))
	{
		DBG("[DirectFBUI-TimePage] create new AM/PM label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_ampmLabel, AfterDrawAMPMLabelCB, this);

	if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
	{
		if( g_curLocalTime.tm_hour < 12 )
			m_timeAMPM24 = TIME_AM;
		else
			m_timeAMPM24 = TIME_PM;
	}
	else
	{
		m_timeAMPM24 = TIME_24;
	}

	return TRUE;
}

void CTimePage::AfterDrawHourLabelCB(void* data)
{
	CTimePage *pPage = (CTimePage *)data;

	if (pPage)
	{
		pPage->AfterDrawHourLabel();
	}
}

void CTimePage::AfterDrawHourLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_hourLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	char					szHourText[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iHour			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;
	char *pItem = NULL;

	DBG("AfterDrawHourLabel\n");

	surface->GetFont(surface, &fontInterface);

	// cur hour
	if( m_timeAMPM24 == TIME_AM || m_timeAMPM24 == TIME_PM )
	{
		iHour = GetHour12(g_curLocalTime.tm_hour);
	}
	else
	{
		iHour = g_curLocalTime.tm_hour;
	}

	// Draw main text
	sprintf(szHourText, "%02d", iHour);
	fontInterface->GetStringWidth(fontInterface, szHourText, strlen(szHourText), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, szHourText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	// Draw sub text
	surface->SetFont(surface, m_labelSubFontInterface);
	pItem = g_CurLangDesc.Public.TIME_HR.c_str();
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

	if (m_curTimeStatus == Time_Status_Hour)
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		// Draw up hour
		if( m_timeAMPM24 == TIME_AM || m_timeAMPM24 == TIME_PM )
		{
			iHour = g_curLocalTime.tm_hour - 1;
			iHour = (iHour < 0) ? 23 : iHour;
			iHour = GetHour12(iHour);
			sprintf(szHourText, "%02d", iHour);
			fontInterface->GetStringWidth(fontInterface, szHourText, strlen(szHourText), &fontWidth);
			fontInterface->GetHeight(fontInterface, &fontHeight);
			surface->DrawString(surface, szHourText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

			// Draw down hour
			iHour = g_curLocalTime.tm_hour + 1;
			iHour = (iHour > 23) ? 0 : iHour;
			iHour = GetHour12(iHour);
			sprintf(szHourText, "%02d", iHour);
			fontInterface->GetStringWidth(fontInterface, szHourText, strlen(szHourText), &fontWidth);
			fontInterface->GetHeight(fontInterface, &fontHeight);
			surface->DrawString(surface, szHourText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

			if( g_curLocalTime.tm_hour < 12 )
				m_timeAMPM24 = TIME_AM;
			else
				m_timeAMPM24 = TIME_PM;
		}
		else
		{
			iHour			= g_curLocalTime.tm_hour;
			iHour			-= 1;
			iHour 			= (iHour < 0) ? 23 : iHour;
			sprintf(szHourText, "%02d", iHour);
			fontInterface->GetStringWidth(fontInterface, szHourText, strlen(szHourText), &fontWidth);
			fontInterface->GetHeight(fontInterface, &fontHeight);
			surface->DrawString(surface, szHourText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

			// Draw down hour
			iHour			= g_curLocalTime.tm_hour;
			iHour			+= 1;
			iHour 			= (iHour > 23) ? 0 : iHour;
			sprintf(szHourText, "%02d", iHour);
			fontInterface->GetStringWidth(fontInterface, szHourText, strlen(szHourText), &fontWidth);
			fontInterface->GetHeight(fontInterface, &fontHeight);
			surface->DrawString(surface, szHourText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);
		}

		lite_update_box(LITE_BOX(m_ampmLabel),NULL);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);
	}
}

void CTimePage::AfterDrawMinuteLabelCB(void* data)
{
	CTimePage *pPage = (CTimePage *)data;

	if (pPage)
	{
		pPage->AfterDrawMinuteLabel();
	}
}

void CTimePage::AfterDrawMinuteLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_minuteLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	char					szMinuteText[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iMinute			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;
	char *pItem = NULL;

	DBG("AfterDrawMinuteLabel\n");

	surface->GetFont(surface, &fontInterface);

	// cur minute
	// Draw main text
	iMinute					= g_curLocalTime.tm_min;
	sprintf(szMinuteText, "%02d", iMinute);
	fontInterface->GetStringWidth(fontInterface, szMinuteText, strlen(szMinuteText), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, szMinuteText, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	// Draw sub text
	surface->SetFont(surface, m_labelSubFontInterface);
	pItem = g_CurLangDesc.Public.TIME_MIN.c_str();
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

	if (m_curTimeStatus == Time_Status_Minute)
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		// prev
		iMinute					= g_curLocalTime.tm_min;
		iMinute					-= 1;
		iMinute					= (iMinute < 0) ? 59 : iMinute;
		sprintf(szMinuteText, "%02d", iMinute);
		fontInterface->GetStringWidth(fontInterface, szMinuteText, strlen(szMinuteText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szMinuteText, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

		// next
		iMinute					= g_curLocalTime.tm_min;
		iMinute					+= 1;
		iMinute					= (iMinute > 59) ? 0 : iMinute;
		sprintf(szMinuteText, "%02d", iMinute);
		fontInterface->GetStringWidth(fontInterface, szMinuteText, strlen(szMinuteText), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, szMinuteText, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);
	}
}

void CTimePage::AfterDrawAMPMLabelCB(void* data)
{
	CTimePage *pPage = (CTimePage *)data;

	if (pPage)
	{
		pPage->AfterDrawAMPMLabel();
	}
}

void CTimePage::AfterDrawAMPMLabel()
{
	LiteBox*				liteBox			= LITE_BOX(m_ampmLabel);
	IDirectFBSurface*		surface			= liteBox->surface;
	DFBRectangle 			dfbRect;
	IDirectFBFont*			fontInterface 	= NULL;
	int						fontWidth 		= 0;
	int 					fontHeight		= 0;
	int						iHour			= 0;
	DFBSurfaceTextFlags  	flags 			= DSTF_TOP;
	char *pItemCur = NULL;
	char *pItemUp = NULL;
	char *pItemDown = NULL;
	char *pItemSub = NULL;

	DBG("AfterDrawAMPMLabel, g_curLocalTime.tm_hour: %d\n", g_curLocalTime.tm_hour);

	surface->GetFont(surface, &fontInterface);

	if( m_timeAMPM24 == TIME_AM )
	{
		pItemCur = g_CurLangDesc.Public.AM.c_str();
		pItemUp = g_CurLangDesc.Public.HOUR24.c_str();
		pItemDown = g_CurLangDesc.Public.PM.c_str();
		pItemSub = g_CurLangDesc.Public.SUB_12.c_str();
	}
	else if( m_timeAMPM24 == TIME_PM )
	{
		pItemCur = g_CurLangDesc.Public.PM.c_str();
		pItemUp = g_CurLangDesc.Public.AM.c_str();
		pItemDown = g_CurLangDesc.Public.HOUR24.c_str();
		pItemSub = g_CurLangDesc.Public.SUB_12.c_str();
	}
	else
	{
		pItemCur = g_CurLangDesc.Public.HOUR24.c_str();
		pItemUp = g_CurLangDesc.Public.PM.c_str();
		pItemDown = g_CurLangDesc.Public.AM.c_str();
		pItemSub = g_CurLangDesc.Public.SUB_24.c_str();
	}

	// cur
	// Draw main text
	fontInterface->GetStringWidth(fontInterface, pItemCur, strlen(pItemCur), &fontWidth);
	fontInterface->GetHeight(fontInterface, &fontHeight);
	surface->DrawString(surface, pItemCur, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + 1, flags);

	// Draw sub text
	surface->SetFont(surface, m_labelSubFontInterface);
	m_labelSubFontInterface->GetStringWidth(m_labelSubFontInterface, pItemSub, strlen(pItemSub), &fontWidth);
	surface->DrawString(surface, pItemSub, -1, (DAT_L_W - fontWidth) / 2, DAT_L_H + fontHeight + 3, flags);
	surface->SetFont(surface, fontInterface);

	// Draw up text to bk
	surface->SetColor(surface, 0x3f, 0x3f, 0x3f, 0xFF);
	dfbRect.x			= 0;
	dfbRect.y			= 0;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	// Draw down text to bk
	surface->FillRectangle(surface, 0, 0, DAT_L_W, DAT_L_H);
	dfbRect.x			= 0;
	dfbRect.y			= 2 * DAT_L_H;
	dfbRect.w			= DAT_L_W;
	dfbRect.h			= DAT_L_H;
	surface->FillRectangle(surface, 0, 2 * DAT_L_H, DAT_L_W, DAT_L_H);

	if (m_curTimeStatus == Time_Status_AMPM)
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		// Up text
		fontInterface->GetStringWidth(fontInterface, pItemUp, strlen(pItemUp), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, pItemUp, -1, (DAT_L_W - fontWidth) / 2, (DAT_L_H - fontHeight) / 2, flags);

		// Down text
		fontInterface->GetStringWidth(fontInterface, pItemDown, strlen(pItemDown), &fontWidth);
		fontInterface->GetHeight(fontInterface, &fontHeight);
		surface->DrawString(surface, pItemDown, -1, (DAT_L_W - fontWidth) / 2, 2 * DAT_L_H + (DAT_L_H - fontHeight) / 2, flags);

		// draw triangles
		dfbRect.x				= 0;
		dfbRect.y				= DAT_L_H;
		dfbRect.w				= DAT_L_W;
		dfbRect.h				= DAT_L_H;
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
		FillDoubleTriangle(surface, &dfbRect);

		//lite_update_box(LITE_BOX(m_hourLabel),NULL);
	}
}

void CTimePage::InitDateTime()
{
	if(	g_isFirstTimeSetting )
	{
		g_curLocalTime.tm_hour = 0; //00:00:00
		g_curLocalTime.tm_min = 0;
		g_curLocalTime.tm_sec = 0;

		DBG("aaa, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
	}
	else
	{
		time_t t = time(NULL);
		struct tm *pTime = localtime(&t);

		g_curLocalTime.tm_hour = pTime->tm_hour;
		g_curLocalTime.tm_min = pTime->tm_min;
		g_curLocalTime.tm_sec = pTime->tm_sec;

		DBG("bbb, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);
	}
}



