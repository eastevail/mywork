/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: setting.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include "setting.h"
#include "desktop.h"
#include "microphone.h"
#include "brightness.h"
#include "date_page.h"
#include "time_page.h"
#include "lang_page.h"
#include "del_page.h"
#include "manufacture.h"

//------------------------------------------------------
CSettingPage::CSettingPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_settingImage = NULL;
	m_settingLabel = NULL;
	m_listBox = NULL;
	m_listSubFont = NULL;
	m_listSubFontInterface = NULL;

	m_subPageId = SubPage_Begin;
	m_subPagePtr = NULL;

	m_curIndex = 0;
}

CSettingPage::~CSettingPage()
{
}

void CSettingPage::OnPageInit()
{
	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_IDLE;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	/* 显示标题文字 */
	ShowTitleText(g_CurLangDesc.Setting.Title.c_str());

	/* 显示设置图标 */
	ShowSettingFlag(TRUE);

	if (FALSE == InitSubStrFont())
	{
		DBG("[DirectFBUI-SettingPage] init lite font for sub str failure\n");
		return;
	}

	InitListBox();
}

void CSettingPage::OnPageExit()
{
	if(m_subPagePtr != NULL)
	{
		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}

	SAFE_DEL_LITE(m_settingImage);
	SAFE_DEL_LITE(m_settingLabel);

	DestorySubStrFont();

	DestoryListBox();
}

void CSettingPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (m_subPageId != SubPage_Begin)
	{
		// Proc key event in sub page UI
		if( dfbEvent->type == DWET_KEYDOWN )
		{
			if( m_subPageId == SubPage_DeleteAll )
			{
				m_subPagePtr->OnKeyEvent(dfbEvent);
			}
			else
			{
				switch( dfbEvent->key_code )
				{
				case SETTING_BTN:
				//case OK_BTN: // xjf
					m_subPagePtr->OnKeyEvent(dfbEvent);
					Recover();
					break;
				default:
					m_subPagePtr->OnKeyEvent(dfbEvent);
					break;
				}
			}
		}
	}
	else
	{
		if (dfbEvent->type == DWET_KEYDOWN)
		{
			switch (dfbEvent->key_code)
			{
				case LEFT_BTN:
				{
					//OnLeftKeyEvent();
					break;
				}

				case RIGHT_BTN:
				{
					//OnRightKeyEvent();
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
					//g_RunParam.dlyKey = 1; // comment 2016.02.24
					break;
				}

				case SETTING_BTN:
				{
					OnSettingKeyEvent();
					//g_RunParam.dlyKey = 1; // comment 2016.02.24
					break;
				}

				case REPLAY_BTN:
				{
					//OnReplayKeyEvent();
					break;
				}

				case VIDEO_CAP_BTN:
				{
					//OnVideoCaptureKeyEvent();
					break;
				}

				default:
				{
					DBG("[DirectFBUI-SettingPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
					break;
				}
			}
		}
	}
}

BOOL CSettingPage::ShowTitleText(const char *szTitle)
{
	DFBRectangle 		dfbRect;
	CDesktop*			pDesktop 		= NULL;
	LiteWindow*			liteWin 		= NULL;
	int					screenWidth 	= 0;
	int					screenHeight 	= 0;
	LiteLabelTheme 		theme;
	DFBColor 			fontColor;

	if (m_settingLabel == NULL)
	{
		pDesktop 		= CDesktop::GetDesktop();
		liteWin 		= pDesktop->GetLiTEWindow();

		if (liteWin == NULL)
		{
			DBG("[DirectFBUI-SettingPage] lite window must init first\n");
			return FALSE;
		}

		lite_get_window_size(liteWin, &screenWidth, &screenHeight);
		//dfbRect.x 		= (screenWidth - SETTING_TXT_W) / 2;
		dfbRect.x 		= 60;
		dfbRect.y 		= 0;
		//dfbRect.w 		= SETTING_TXT_W;
		dfbRect.w 		= 200;
		dfbRect.h 		= SETTING_TXT_H;

		memset(&theme, 0, sizeof(LiteLabelTheme));
		theme.theme.bg_color.a = TOP_LABEL_COLOR_A;
		theme.theme.bg_color.r = TOP_LABEL_COLOR_R;
		//theme.theme.bg_color.r = 0xff;
		theme.theme.bg_color.g = TOP_LABEL_COLOR_G;
		theme.theme.bg_color.b = TOP_LABEL_COLOR_B;

		fontColor.a = SETTING_TEXT_COLOR_A;
		fontColor.r = SETTING_TEXT_COLOR_R;
		fontColor.g = SETTING_TEXT_COLOR_G;
		fontColor.b = SETTING_TEXT_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &dfbRect, &theme, SETTING_TXT_SIZE, &fontColor, &m_settingLabel))
		{
			DBG("[DirectFBUI-SettingPage] create lite label failure\n");
			return FALSE;
		}
	}

	if (m_settingLabel != NULL)
	{
		lite_set_label_text(m_settingLabel, szTitle);
		lite_set_label_alignment(m_settingLabel, LITE_LABEL_CENTER);
	}

	return TRUE;
}

void CSettingPage::Recover()
{
	if( g_isFirstTimeSetting )
	{
		if(m_subPageId == SubPage_Date && m_subPagePtr != NULL)
		{
			DBG("set Date Over: %d \n", m_subPageId);

			m_subPagePtr->OnPageExit();
			delete m_subPagePtr;
			m_subPagePtr = NULL;

			ShowSubPageByID(SubPage_Time);
		}
		else if(m_subPageId == SubPage_Time && m_subPagePtr != NULL)
		{
			DBG("set Time Over: %d \n", m_subPageId);

			m_subPagePtr->OnPageExit();
			delete m_subPagePtr;
			m_subPagePtr = NULL;

			ShowSubPageByID(SubPage_Lang);
		}
		else if(m_subPageId == SubPage_Lang && m_subPagePtr != NULL)
		{
			DBG("set Language Over: %d \n", m_subPageId);

			m_subPagePtr->OnPageExit();
			delete m_subPagePtr;
			m_subPagePtr = NULL;

			g_isFirstTimeSetting = FALSE;
			m_subPageId = SubPage_Begin;
			CDesktop::GetDesktop()->ShowPageByID(DEFAULT_PAGE);
		}
		else
		{
			ERR("NO-NORMAL\n");

			if( m_subPagePtr != NULL)
			{
				m_subPagePtr->OnPageExit();
				delete m_subPagePtr;
				m_subPagePtr = NULL;

				m_subPageId = SubPage_Begin;
			}

			g_isFirstTimeSetting = FALSE;
			CDesktop::GetDesktop()->ShowPageByID(DEFAULT_PAGE);
		}
	}
	else
	{
		if( m_subPagePtr != NULL )
		{
			m_subPagePtr->OnPageExit();
			delete m_subPagePtr;
			m_subPagePtr = NULL;

			m_subPageId = SubPage_Begin;
		}

		ShowTitleText(g_CurLangDesc.Setting.Title.c_str());

		InitListBox();
	}
}

void CSettingPage::OnLeftKeyEvent(void)
{
}

void CSettingPage::OnRightKeyEvent(void)
{
}

void CSettingPage::OnUpKeyEvent(void)
{
	int curSelIndex = 0;
	int totalCount = 0;

	if (m_listBox)
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
		{
			DBG("[DirectFBUI-SettingPage] get list index failure\n");
			return;
		}

		if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
		{
			DBG("[DirectFBUI-SettingPage] get list count failure\n");
			return;
		}

		curSelIndex -= 1;
		curSelIndex = (curSelIndex < 0) ? (totalCount - 1) : curSelIndex;

		lite_list_set_selected_item_index(m_listBox, curSelIndex);
	}
}

void CSettingPage::OnDownKeyEvent(void)
{
	int curSelIndex = 0;
	int totalCount = 0;

	if (m_listBox)
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
		{
			DBG("[DirectFBUI-SettingPage] get list index failure\n");
			return;
		}

		if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
		{
			DBG("[DirectFBUI-SettingPage] get list count failure\n");
			return;
		}

		curSelIndex += 1;
		curSelIndex = (curSelIndex >= totalCount) ? 0 : curSelIndex;

		lite_list_set_selected_item_index(m_listBox, curSelIndex);
	}
}

void CSettingPage::OnOKKeyEvent(void)
{
	int curIndex = 0;
	SubPage_E subPageId;
	unsigned long temp;

	if( m_listBox )
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curIndex))
		{
			return;
		}

		if (DFB_OK != lite_list_get_item(m_listBox, curIndex, (LiteListItemData *)&temp))
		{
			return;
		}

		subPageId = (SubPage_E)temp;
		m_curIndex = curIndex;

		ShowSubPageByID(subPageId);
	}
}

void CSettingPage::OnSettingKeyEvent(void)
{
	CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
	pDesktop->ShowPageByID(DEFAULT_PAGE);
}

void CSettingPage::OnReplayKeyEvent(void)
{
}

void CSettingPage::OnVideoCaptureKeyEvent(void)
{

}

BOOL CSettingPage::ShowSettingFlag(BOOL bVisible)
{
	DFBRectangle 		dfbRect;

	if (m_settingImage == NULL)
	{
		// 创建并显示录像图标
		dfbRect.x 			= SETTING_IMG_X;
		dfbRect.y 			= SETTING_IMG_Y;
		dfbRect.w 			= SETTING_IMG_W;
		dfbRect.h 			= SETTING_IMG_H;
		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"SETTING_FILE, &m_settingImage))
		{
			return FALSE;
		}
	}

	if (m_settingImage != NULL)
	{
		lite_set_image_visible(m_settingImage, bVisible);
	}

	return TRUE;
}

BOOL CSettingPage::InitListBox()
{
	CDesktop*			pDesktop 		= NULL;
	LiteWindow*			liteWin 		= NULL;
	DFBRectangle		dfbRect;
	int					subPageId		= 0;
	int					count			= 0;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-SettingPage] lite window must init first\n");
		return FALSE;
	}

	if (liteDefaultListTheme == NULL)
	{
		if (DFB_OK != lite_new_list_theme(PIC_PATH"/"SCROLL_BAR_FILE, 3, &liteDefaultListTheme))
		{
			D_INFO("DirectFB_UI-LanguagePage: Load list theme fail\n");
			return FALSE;
		}
	}

	dfbRect.x 			= 0;
	dfbRect.y 			= SETTING_LIST_TOP;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= g_RunParam.screenHeight - SETTING_LIST_TOP;
	if (DFB_OK != lite_new_list(LITE_BOX(liteWin), &dfbRect, liteDefaultListTheme, LIST_MAIN_FONT_SIZE, &m_listBox))
	{
		DBG("DirectFB_UI-settingPage: Create list fail\n");
		return FALSE;
	}

	if (DFB_OK != lite_list_set_row_height(m_listBox, (g_RunParam.screenHeight - SETTING_LIST_TOP) / SHOW_LIST_COUNT))
	{
		DBG("DirectFB_UI-settingPage: set list row height failure\n");
		return FALSE;
	}

	lite_list_on_draw_item(m_listBox, OnListDrawItem, this);

	for( subPageId = (int)SubPage_MicroPhone; subPageId < (int)SubPage_End; subPageId++ )
	{
		lite_list_get_item_count(m_listBox, &count);
		lite_list_insert_item(m_listBox, count, (LiteListItemData)subPageId);
	}

	lite_list_set_selected_item_index(m_listBox, m_curIndex);

	return TRUE;
}

void CSettingPage::UpdateCurItem()
{
	lite_update_box(LITE_BOX(m_listBox), NULL);
}


void CSettingPage::DestoryListBox()
{
	lite_destroy_list_theme(liteDefaultListTheme);

	liteDefaultListTheme = NULL;

	SAFE_DEL_LITE(m_listBox);
}

BOOL CSettingPage::InitSubStrFont()
{
	if (DFB_OK != lite_get_font("default", LITE_FONT_BOLD, LIST_SUB_FONT_SIZE, DEFAULT_FONT_ATTRIBUTE, &m_listSubFont))
	{
		DBG("[DirectFBUI-SettingPage] get lite font failure\n");
		return FALSE;
	}

	if (DFB_OK != lite_font(m_listSubFont, &m_listSubFontInterface))
	{
		DBG("[DirectFBUI-SettingPage] get lit font object\n");
		return FALSE;
	}

	return TRUE;
}

void CSettingPage::DestorySubStrFont()
{
	if (DFB_OK != lite_release_font(m_listSubFont))
	{
		DBG("[DirectFBUI-SettingPage] release lite font failure\n");
	}

	m_listSubFont = NULL;
	m_listSubFontInterface = NULL;
}

int CSettingPage::GetDateStr(char *szBuf, int bufLen)
{
	if ((szBuf == NULL) || (bufLen <= 0))
	{
		return 0;
	}

	time_t now;
	struct tm  *timenow;
	int len = 0;

	time(&now);
	timenow  = localtime(&now);

	if ((1 + timenow->tm_mon) < 10)
	{
		len += sprintf(szBuf + len, "0%d/", (1 + timenow->tm_mon));
	}
	else
	{
		len += sprintf(szBuf + len, "%d/", (1 + timenow->tm_mon));
	}

	if (timenow->tm_mday < 10)
	{
		len += sprintf(szBuf + len, "0%d/", timenow->tm_mday);
	}
	else
	{
		len += sprintf(szBuf + len, "%d/", timenow->tm_mday);
	}

	len += sprintf(szBuf + len, "%d", (1900 + timenow->tm_year));

	return len;
}

int CSettingPage::GetTimeStr(char *szBuf, int bufLen)
{
	if ((szBuf == NULL) || (bufLen <= 0))
	{
		return 0;
	}

	time_t timep;
	struct tm *p;
	time(&timep);
	p = gmtime(&timep);
	int len = 0;
	int iHour = 0;

	iHour = (p->tm_hour < 12) ? (p->tm_hour) : (p->tm_hour - 12);

	if (iHour < 10)
	{
		len += sprintf(szBuf + len, "0%d:", iHour);
	}
	else
	{
		len += sprintf(szBuf + len, "%d:", iHour);
	}

	if (p->tm_min < 10)
	{
		len += sprintf(szBuf + len, "0%d", p->tm_min);
	}
	else
	{
		len += sprintf(szBuf + len, "%d", p->tm_min);
	}

	if (p->tm_hour < 12)
	{
		len += sprintf(szBuf + len, " %s", g_CurLangDesc.Public.AM.c_str());
	}
	else
	{
		len += sprintf(szBuf + len, " %s", g_CurLangDesc.Public.PM.c_str());
	}

	return len;
}

void CSettingPage::OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx)
{
	CSettingPage *pPage = (CSettingPage *)ctx;

	if (pPage)
	{
		pPage->DrawListItem(l, draw_item);
	}
}

void CSettingPage::DrawListItem(LiteList *l, ListDrawItem *draw_item)
{
	IDirectFBSurface*		surface 					= draw_item->surface;
	SubPage_E 				subPageId 					= SubPage_Begin;
	char 					szSubStr[MAX_LANG_LEN];
	IDirectFBFont*			fontInterface 			= NULL;
	int 					height 						= 0;
	char *pMainMsg = NULL;
	char *pSubMsg = NULL;

	if (surface == NULL)
	{
		return;
	}

	subPageId = (SubPage_E)draw_item->item_data;
	if (draw_item->selected)
	{
		surface->SetColor(surface, LIST_BG_ACTIVE_COLOR_R, LIST_BG_ACTIVE_COLOR_G,
							LIST_BG_ACTIVE_COLOR_B, LIST_BG_ACTIVE_COLOR_A);
	}
	else
	{
		surface->SetColor(surface, LIST_BG_COLOR_R, LIST_BG_COLOR_G, LIST_BG_COLOR_B, LIST_BG_COLOR_A);
	}

	surface->FillRectangle(surface, draw_item->rc_item.x, draw_item->rc_item.y, draw_item->rc_item.w, draw_item->rc_item.h);

	memset(szSubStr, 0, sizeof(szSubStr));

	switch (subPageId)
	{
		case SubPage_MicroPhone:
		{
			pMainMsg = g_CurLangDesc.Setting.MainMicrophone.MainMsg.c_str();

			if( g_RunParam.tuobangPara.bMicroPhone )
				pSubMsg = g_CurLangDesc.Setting.MainMicrophone.SubMsgOn.c_str();
			else
				pSubMsg = g_CurLangDesc.Setting.MainMicrophone.SubMsgOff.c_str();
			break;
		}

		case SubPage_BrightNess:
		{
			pMainMsg = g_CurLangDesc.Setting.MainLcdBrightness.MainMsg.c_str();

			if( g_RunParam.tuobangPara.brightNess == TP_BRIGHTNESS_LOW )
				pSubMsg = g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow.c_str();
			else if( g_RunParam.tuobangPara.brightNess == TP_BRIGHTNESS_MEDIUM )
				pSubMsg = g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium.c_str();
			else
				pSubMsg = g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh.c_str();
			break;
		}

		case SubPage_Date:
		{
			pMainMsg = g_CurLangDesc.Setting.MainDate.MainMsg.c_str();

			//GetDateStr(szSubStr, MAX_LANG_LEN);
			LoadCurDay(szSubStr);
			pSubMsg = szSubStr;
			break;
		}

		case SubPage_Time:
		{
			pMainMsg = g_CurLangDesc.Setting.MainTime.MainMsg.c_str();

			//GetTimeStr(szSubStr, MAX_LANG_LEN);
			LoadCurTime(szSubStr);
			pSubMsg = szSubStr;
			break;
		}

		case SubPage_Lang:
		{
			pMainMsg = g_CurLangDesc.Setting.MainLang.MainMsg.c_str();
			pSubMsg = g_CurLangDesc.Setting.MainLang.SubMsg.c_str();
			break;
		}

		case SubPage_DeleteAll:
		{
			pMainMsg = g_CurLangDesc.Setting.MainDeleteAll.MainMsg.c_str();
			pSubMsg = g_CurLangDesc.Setting.MainDeleteAll.SubMsg.c_str();
			break;
		}

		case SubPage_Version:
		{
			pMainMsg = g_CurLangDesc.Setting.MainVersion.MainMsg.c_str();
			pSubMsg = g_CurLangDesc.Setting.MainVersion.SubMsg.c_str();
			break;
		}

	#if 0
		case SubPage_Manufacture: // xjf
		{
			pMainMsg = g_CurLangDesc.Setting.MainManufacture.MainMsg.c_str();
			pSubMsg = g_CurLangDesc.Setting.MainManufacture.SubMsg.c_str();
			break;
		}
	#endif

		default:
		{
			DBG("DirectFB_UI-settingPage: do not support this page\n");
			break;
		}
	}

	DBG("subPageId: %d, pMainMsg: %s, pSubMsg: %s\n", subPageId, pMainMsg, pSubMsg);

	if( pMainMsg )
	{
		// draw main string
		surface->SetColor(surface, LIST_STR_COLOR_R, LIST_STR_COLOR_G, LIST_STR_COLOR_B, LIST_STR_COLOR_A);
		surface->DrawString(surface, pMainMsg, strlen(pMainMsg), draw_item->rc_item.x + LIST_LEFT_MARGIN, draw_item->rc_item.y + 2, DSTF_LEFT|DSTF_TOP);
	}

	if( pSubMsg )
	{
		// draw sub string
		surface->GetFont(surface, &fontInterface);
		surface->SetFont(surface, m_listSubFontInterface);
		surface->SetColor(surface, LIST_STR_COLOR_R, LIST_STR_COLOR_G, LIST_STR_COLOR_B, LIST_STR_COLOR_A);
		m_listSubFontInterface->GetHeight(m_listSubFontInterface, &height);
		surface->DrawString(surface, pSubMsg, strlen(pSubMsg), draw_item->rc_item.x + LIST_LEFT_MARGIN, draw_item->rc_item.y + draw_item->rc_item.h - height, DSTF_LEFT|DSTF_TOP);
		surface->SetFont(surface, fontInterface);
	}
}

void CSettingPage::ShowSubPageByID(SubPage_E subPageId)
{
	if ((subPageId < SubPage_Begin) || (subPageId > SubPage_End))
	{
		return;
	}

	if (m_subPagePtr != NULL)
	{
		return;
	}

	if (m_listBox)
	{
		switch (subPageId)
		{
			case SubPage_MicroPhone:
			{
				m_subPagePtr = new CMicroPhonePage(this);
				break;
			}

			case SubPage_BrightNess:
			{
				m_subPagePtr = new CBrightnessPage(this);
				break;
			}

			case SubPage_Date:
			{
				m_subPagePtr = new CDatePage(this);
				break;
			}

			case SubPage_Time:
			{
				m_subPagePtr = new CTimePage(this);
				break;
			}

			case SubPage_Lang:
			{
				m_subPagePtr = new CLangPage(this);
				break;
			}

			case SubPage_DeleteAll:
			{
				CDesktop *pDesktop = CDesktop::GetDesktop();
				if( pDesktop->isSDCardExsit() )
					m_subPagePtr = new CDeletePage(this);
				else
					m_subPagePtr = NULL;
				break;
			}

			case SubPage_Version:
			{
				break;
			}

		#if 0
			case SubPage_Manufacture: // xjf
			{
				m_subPagePtr = new CManufacturePage(this);
				break;
			}
		#endif

			default:
			{
				m_subPagePtr = NULL;
				DBG("DirectFB_UI-settingPage: do not support this sub page\n");
				break;
			}
		}

		if (m_subPagePtr != NULL)
		{
			m_subPageId = subPageId;

			if( !g_isFirstTimeSetting )
				DestoryListBox();

			m_subPagePtr->OnPageInit();
		}
	}
}

void CSettingPage::DeleteSubPage()
{
	if (m_subPagePtr != NULL)
	{
		DBG("DeleteSubPage, m_subPagePtr: %p, m_subPageId: %d\n", \
			m_subPagePtr, m_subPageId);

		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;

		m_subPageId = SubPage_Begin;
	}
}






