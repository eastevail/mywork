/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: lang_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.25
******************************************************************************/

#include "lang_page.h"
#include "setting.h"
#include "desktop.h"

CLangPage::CLangPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_listBox = NULL;
	m_bgLabel = NULL;
}

CLangPage::~CLangPage()
{
}

void CLangPage::OnPageInit()
{
	CSettingPage *pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());

		tsSubLang *pSubLang = &g_CurLangDesc.Setting.MainLang.SubLang;
		if( g_isFirstTimeSetting )
		//if( 1 )
			pSetting->ShowTitleText(pSubLang->TitleFirst.c_str());
		else
			pSetting->ShowTitleText(pSubLang->Title.c_str());
	}

	if (FALSE == InitBackground())
	{
		DBG("[DirectFBUI-LanguagePage] init background failure\n");
	}

	if (FALSE == InitListBox())
	{
		DBG("[DirectFBUI-LanguagePage] init list box failure\n");
	}
}

void CLangPage::OnPageExit()
{
	DestoryListBox();
	DestoryBackground();
}

void CLangPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (dfbEvent->type == DWET_KEYDOWN)
	{
		switch (dfbEvent->key_code)
		{
			case LEFT_BTN:
			case RIGHT_BTN:
			{
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
				//JumpToParentPage();
				break;
			}

			case REPLAY_BTN:
			{
				break;
			}

			case VIDEO_CAP_BTN:
			{
				break;
			}

			default:
			{
				DBG("[DirectFBUI-LanguagePage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CLangPage::JumpToParentPage()
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

void CLangPage::OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx)
{
	CLangPage *pPage = (CLangPage *)ctx;

	if (pPage)
	{
		pPage->DrawListItem(l, draw_item);
	}
}

void CLangPage::DrawListItem(LiteList *l, ListDrawItem *draw_item)
{
	IDirectFBSurface *surface = draw_item->surface;
	teLang language;

	if( surface == NULL )
	{
		return;
	}

	language = (teLang)(draw_item->item_data);

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

	char *pItem = NULL;
	tsLang *pLang = &g_CurLangDesc.Setting.MainLang.SubLang.Lang;

	switch( language )
	{
	case LANG_ENGLISH:
		pItem = pLang->English.c_str();
		break;
	case LANG_FRENCH:
		pItem = pLang->French.c_str();
		break;
	case LANG_SPANISH:
		pItem = pLang->Spanish.c_str();
		break;
	case LANG_GERMAN:
		pItem = pLang->Germain.c_str();
		break;
	case LANG_DANISH:
		pItem = pLang->Danish.c_str();
		break;
	case LANG_DUTCH:
		pItem = pLang->Dutch.c_str();
		break;
	default:
		break;
	}

	if( pItem )
	{
		surface->SetColor(surface, LIST_STR_COLOR_R, LIST_STR_COLOR_G, LIST_STR_COLOR_B, LIST_STR_COLOR_A);
		surface->DrawString(surface, pItem, strlen(pItem), draw_item->rc_item.x + LIST_LEFT_MARGIN, draw_item->rc_item.y + 2, DSTF_LEFT|DSTF_TOP);
	}
}

BOOL CLangPage::InitListBox()
{
	CDesktop*				pDesktop 		= NULL;
	LiteWindow*				liteWin 		= NULL;
	int						screenWidth 	= 0;
	int						screenHeight	= 0;
	DFBRectangle 			dfbRect;
	int						language		= -1;
	int						count			= 0;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if( liteDefaultListTheme == NULL )
	{
		if (DFB_OK != lite_new_list_theme(PIC_PATH"/"SCROLL_BAR_FILE, 3, &liteDefaultListTheme))
		{
			DBG("DirectFB_UI-LanguagePage: Load list theme fail\n");
			return FALSE;
		}
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= screenWidth;
	dfbRect.h			= screenHeight - TOP_LABEL_H;

	if (DFB_OK != lite_new_list(LITE_BOX(liteWin), &dfbRect, liteDefaultListTheme, LIST_MAIN_FONT_SIZE, &m_listBox))
	{
		DBG("DirectFB_UI-LanguagePage: Create list fail\n");
		return FALSE;
	}

	if (DFB_OK != lite_list_set_row_height(m_listBox, (screenHeight - SETTING_LIST_TOP) / SHOW_LIST_COUNT))
	{
		DBG("DirectFB_UI-LanguagePage: set list row height failure\n");
		return FALSE;
	}

	lite_list_on_draw_item(m_listBox, OnListDrawItem, this);

	for( language = (int)LANG_ENGLISH; language < (int)LANG_END; language++ )
	{
		lite_list_get_item_count(m_listBox, &count);
		lite_list_insert_item(m_listBox, count, language);
	}

	lite_list_set_selected_item_index(m_listBox, g_RunParam.tuobangPara.lang);

	return TRUE;
}

void CLangPage::DestoryListBox()
{
	SAFE_DEL_LITE(m_listBox);
}

BOOL CLangPage::InitBackground()
{
	CDesktop*				pDesktop 		= NULL;
	LiteWindow*				liteWin 		= NULL;
	int						screenWidth 	= 0;
	int						screenHeight	= 0;
	DFBRectangle 			dfbRect;
	DFBColor				bgColor;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-LanguagePage] lite window must init first\n");
		return FALSE;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= screenWidth;
	dfbRect.h			= screenHeight - TOP_LABEL_H;

	bgColor.a			= LIST_BG_COLOR_A;
	bgColor.r			= LIST_BG_COLOR_R;
	bgColor.g			= LIST_BG_COLOR_G;
	bgColor.b			= LIST_BG_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 12, NULL, &m_bgLabel))
	{
		DBG("[DirectFBUI-LanguagePage] create label failure\n");
		return FALSE;
	}

	return TRUE;
}

void CLangPage::DestoryBackground()
{
	SAFE_DEL_LITE(m_bgLabel);
}

void CLangPage::OnUpKeyEvent(void)
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

void CLangPage::OnDownKeyEvent(void)
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

void CLangPage::OnOKKeyEvent(void)
{
	int curIndex = 0;
	unsigned long temp;

	if (m_listBox)
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curIndex))
		{
			return;
		}

		if (DFB_OK != lite_list_get_item(m_listBox, curIndex, (LiteListItemData *)&temp))
		{
			return;
		}

		// ±£¥Ê”Ô—‘
		g_RunParam.tuobangPara.lang = temp;
		sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

		DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);

		// Load language
		InitLangDesc(g_RunParam.tuobangPara.lang); // xjf

		JumpToParentPage();
	}
}




