/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: brightness.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.22
******************************************************************************/

#include "brightness.h"
#include "setting.h"
#include "desktop.h"

CBrightnessPage::CBrightnessPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_listBox = NULL;
	m_bgLabel = NULL;
}

CBrightnessPage::~CBrightnessPage()
{
}

void CBrightnessPage::OnPageInit()
{
	CSettingPage *pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		pSetting->ShowTitleText(g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title.c_str());

	}

	if (FALSE == InitBackground())
	{
		DBG("[DirectFBUI-BrightnessPage] init background failure\n");
	}

	if (FALSE == InitListBox())
	{
		DBG("[DirectFBUI-BrightnessPage] init list box failure\n");
	}
}

void CBrightnessPage::OnPageExit()
{
	DestoryListBox();
	DestoryBackground();
}

void CBrightnessPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
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
				DBG("[DirectFBUI-BrightnessPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CBrightnessPage::OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx)
{
	CBrightnessPage *pPage = (CBrightnessPage *)ctx;

	if (pPage)
	{
		pPage->DrawListItem(l, draw_item);
	}
}

void CBrightnessPage::DrawListItem(LiteList *l, ListDrawItem *draw_item)
{
	IDirectFBSurface*		surface 					= draw_item->surface;
	TP_BRIGHTNESS_E			brightness 					= TP_BRIGHTNESS_END;
	char *pItem = NULL;

	if (surface == NULL)
	{
		return;
	}

	brightness = (TP_BRIGHTNESS_E)draw_item->item_data;

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

	switch (brightness)
	{
		case TP_BRIGHTNESS_LOW:
		{
			pItem = g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low.c_str();
			break;
		}

		case TP_BRIGHTNESS_MEDIUM:
		{
			pItem = g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium.c_str();
			break;
		}

		case TP_BRIGHTNESS_HIGH:
		{
			pItem = g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High.c_str();
			break;
		}

		default:
		{
			break;
		}
	}

	if( pItem )
	{
		surface->SetColor(surface, LIST_STR_COLOR_R, LIST_STR_COLOR_G, LIST_STR_COLOR_B, LIST_STR_COLOR_A);
		surface->DrawString(surface, pItem, strlen(pItem), draw_item->rc_item.x + LIST_LEFT_MARGIN, draw_item->rc_item.y + 2, DSTF_LEFT|DSTF_TOP);
	}
}

BOOL CBrightnessPage::InitListBox()
{
	CDesktop*				pDesktop 		= NULL;
	LiteWindow*				liteWin 		= NULL;
	DFBRectangle 			dfbRect;
	int						brightness		= -1;
	int						count			= 0;
	CSettingPage *pSetting = NULL;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-BrightnessPage] lite window must init first\n");
		return FALSE;
	}

	if (liteDefaultListTheme == NULL)
	{
		if (DFB_OK != lite_new_list_theme(PIC_PATH"/"SCROLL_BAR_FILE, 3, &liteDefaultListTheme))
		{
			D_INFO("DirectFB_UI-BrightnessPage: Load list theme fail\n");
			return FALSE;
		}
	}

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= g_RunParam.screenWidth;
	dfbRect.h			= g_RunParam.screenHeight - TOP_LABEL_H;

	if (DFB_OK != lite_new_list(LITE_BOX(liteWin), &dfbRect, liteDefaultListTheme, LIST_MAIN_FONT_SIZE, &m_listBox))
	{
		DBG("DirectFB_UI-BrightnessPage: Create list fail\n");
		return FALSE;
	}

	if (DFB_OK != lite_list_set_row_height(m_listBox, (g_RunParam.screenHeight - SETTING_LIST_TOP) / SHOW_LIST_COUNT))
	{
		DBG("DirectFB_UI-BrightnessPage: set list row height failure\n");
		return FALSE;
	}

	lite_list_on_draw_item(m_listBox, OnListDrawItem, this);

	for (brightness = (int)TP_BRIGHTNESS_LOW; brightness < (int)TP_BRIGHTNESS_END; brightness ++)
	{
		lite_list_get_item_count(m_listBox, &count);
		lite_list_insert_item(m_listBox, count, (TP_BRIGHTNESS_E)brightness);
	}

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());

		if( g_RunParam.tuobangPara.brightNess >= TP_BRIGHTNESS_LOW && \
			g_RunParam.tuobangPara.brightNess <= TP_BRIGHTNESS_HIGH )
		{
			lite_list_set_selected_item_index(m_listBox, (int)g_RunParam.tuobangPara.brightNess);
		}
		else
		{
			lite_list_set_selected_item_index(m_listBox, 0);
		}
	}
	else
	{
		lite_list_set_selected_item_index(m_listBox, 0);
	}

	return TRUE;
}

void CBrightnessPage::DestoryListBox()
{
	SAFE_DEL_LITE(m_listBox);
}

BOOL CBrightnessPage::InitBackground()
{
	CDesktop*				pDesktop 		= NULL;
	LiteWindow*				liteWin 		= NULL;
	DFBRectangle 			dfbRect;
	DFBColor				bgColor;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-BrightnessPage] lite window must init first\n");
		return FALSE;
	}

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= g_RunParam.screenWidth;
	dfbRect.h			= g_RunParam.screenHeight - TOP_LABEL_H;

	bgColor.a			= LIST_BG_COLOR_A;
	bgColor.r			= LIST_BG_COLOR_R;
	bgColor.g			= LIST_BG_COLOR_G;
	bgColor.b			= LIST_BG_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 12, NULL, &m_bgLabel))
	{
		DBG("[DirectFBUI-BrightnessPage] create label failure\n");
		return FALSE;
	}

	return TRUE;
}

void CBrightnessPage::DestoryBackground()
{
	SAFE_DEL_LITE(m_bgLabel);
}

void CBrightnessPage::OnUpKeyEvent(void)
{
	int curSelIndex = 0;
	int totalCount = 0;

	if (m_listBox)
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
		{
			DBG("[DirectFBUI-BrightnessPage] get list index failure\n");
			return;
		}

		if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
		{
			DBG("[DirectFBUI-BrightnessPage] get list count failure\n");
			return;
		}

		curSelIndex -= 1;
		curSelIndex = (curSelIndex < 0) ? (totalCount - 1) : curSelIndex;

		lite_list_set_selected_item_index(m_listBox, curSelIndex);
	}
}

void CBrightnessPage::OnDownKeyEvent(void)
{
	int curSelIndex = 0;
	int totalCount = 0;

	if (m_listBox)
	{
		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
		{
			DBG("[DirectFBUI-BrightnessPage] get list index failure\n");
			return;
		}

		if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
		{
			DBG("[DirectFBUI-BrightnessPage] get list count failure\n");
			return;
		}

		curSelIndex += 1;
		curSelIndex = (curSelIndex >= totalCount) ? 0 : curSelIndex;

		lite_list_set_selected_item_index(m_listBox, curSelIndex);
	}
}

void CBrightnessPage::OnOKKeyEvent(void)
{
	int 				curIndex = 0;
	TP_BRIGHTNESS_E		brightness;
	unsigned long 		temp;

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

		brightness = (TP_BRIGHTNESS_E)temp;

		// 保存亮度信息, 同时设置屏幕亮度
		g_RunParam.tuobangPara.brightNess = brightness;
		sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

		CDesktop* pDesktop = NULL;
		pDesktop = CDesktop::GetDesktop();
		pDesktop->CtrlLCDBright(brightness);

		DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);

		JumpToParentPage();
	}
}

void CBrightnessPage::JumpToParentPage()
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




