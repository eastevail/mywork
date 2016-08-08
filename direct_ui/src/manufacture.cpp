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

#include "manufacture.h"
#include "setting.h"
#include "desktop.h"

//------------------------------------------------------------------
typedef enum
{
    MFR_BEGIN = -1,
    MFR_TEST_COLOR,
    MFR_RESTORE_PARAM,
    MFR_END,
} teMfrItem;

//------------------------------------------------------------------
typedef enum
{
	COLOR_BEGIN = -1,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_END
} teColor;

typedef enum
{
	DIR_UP,
	DIR_DOWN
} teDirChange;

static const DFBColor color_tb[] = {
	{ 0x00, 0xff, 0x00, 0x00 }, // red
	{ 0x00, 0x00, 0xff, 0x00 }, // green
	{ 0x00, 0x00, 0x00, 0xff }, // blue
	{ 0x00, 0x00, 0x00, 0x00 }, // black
	{ 0x00, 0xff, 0xff, 0xff }, // white
};

static int curColor = COLOR_RED;

//------------------------------------------------------------------
CManufacturePage::CManufacturePage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_listBox = NULL;
	m_bgLabel = NULL;

	m_plblColorBox = NULL;
}

CManufacturePage::~CManufacturePage()
{
}

void CManufacturePage::OnPageInit()
{
	CSettingPage *pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		pSetting->ShowTitleText(g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title.c_str());
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

void CManufacturePage::OnPageExit()
{
	DestoryListBox();
	DestoryBackground();

	SAFE_DEL_LITE(m_plblColorBox);
}

void CManufacturePage::OnKeyEvent(DFBWindowEvent *dfbEvent)
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
				if( m_plblColorBox != NULL )
					TestColorChange(DIR_UP);
				else
					OnUpKeyEvent();
				break;
			}

			case DOWN_BTN:
			{
				if( m_plblColorBox != NULL )
					TestColorChange(DIR_DOWN);
				else
					OnDownKeyEvent();
				break;
			}

			case OK_BTN:
			{
				if( m_plblColorBox != NULL )
				{
					SAFE_DEL_LITE(m_plblColorBox);
				}
				else
				{
					OnOKKeyEvent();
				}
				break;
			}

			case SETTING_BTN:
			{
				if( m_plblColorBox != NULL )
				{
					SAFE_DEL_LITE(m_plblColorBox);
				}
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

void CManufacturePage::OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx)
{
	CManufacturePage *pPage = (CManufacturePage *)ctx;

	if (pPage)
	{
		pPage->DrawListItem(l, draw_item);
	}
}

void CManufacturePage::DrawListItem(LiteList *l, ListDrawItem *draw_item)
{
	IDirectFBSurface *surface = draw_item->surface;
	teMfrItem MfrItem = MFR_END;
	char *pItem = NULL;

	if (surface == NULL)
	{
		return;
	}

	MfrItem = (teMfrItem)draw_item->item_data;

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

	switch( MfrItem )
	{
		case MFR_TEST_COLOR:
		{
			pItem = g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor.c_str();
			break;
		}

		case MFR_RESTORE_PARAM:
		{
			pItem = g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam.c_str();
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

BOOL CManufacturePage::InitListBox()
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

	for (brightness = (int)MFR_TEST_COLOR; brightness < (int)MFR_END; brightness++)
	{
		lite_list_get_item_count(m_listBox, &count);
		lite_list_insert_item(m_listBox, count, (teMfrItem)brightness);
	}

	lite_list_set_selected_item_index(m_listBox, 0);

	return TRUE;
}

void CManufacturePage::DestoryListBox()
{
	SAFE_DEL_LITE(m_listBox);
}

BOOL CManufacturePage::InitBackground()
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

void CManufacturePage::DestoryBackground()
{
	SAFE_DEL_LITE(m_bgLabel);
}

void CManufacturePage::OnUpKeyEvent(void)
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

void CManufacturePage::OnDownKeyEvent(void)
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

void CManufacturePage::OnOKKeyEvent(void)
{
	int curIndex = 0;
	teMfrItem MfrItem = 0;
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

		MfrItem = (teMfrItem)temp;

		switch( MfrItem )
		{
		case MFR_TEST_COLOR:
			InitTestColor();
			break;
		case MFR_RESTORE_PARAM:
			RestoreDefParam();
			JumpToParentPage();
			break;
		default:
			break;
		}
	}
}

void CManufacturePage::JumpToParentPage()
{
	CSettingPage *pSetting = NULL;

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		if (pSetting)
		{
			pSetting->Recover();
		}
	}
}

void CManufacturePage::InitTestColor()
{
	if( m_plblColorBox == NULL )
	{
		DFBColor bgColor;
		DFBRectangle rect;

		curColor = COLOR_RED;

		rect.x = 0;
		rect.y = 0;
		rect.w = g_RunParam.screenWidth;
		rect.h = g_RunParam.screenHeight;

		bgColor.a = color_tb[curColor].a;
		bgColor.r = color_tb[curColor].r;
		bgColor.g = color_tb[curColor].g;
		bgColor.b = color_tb[curColor].b;

		CreateLiTELabel(&rect, &bgColor, 12, NULL, &m_plblColorBox);
		//lite_set_label_draw_after(m_plblColorBox, AfterDrawColorBoxCB, this);

		AfterDrawColorBox();
	}
}

void CManufacturePage::TestColorChange(int dir)
{
	if( dir == DIR_UP )
	{
		if( (curColor+1) < COLOR_END )
			curColor = curColor + 1;
		else
			curColor = COLOR_BEGIN + 1;
	}
	else if( dir == DIR_DOWN )
	{
		if( (curColor-1) > COLOR_BEGIN )
			curColor = curColor - 1;
		else
			curColor = COLOR_END - 1;
	}

	//lite_update_box(LITE_BOX(m_plblColorBox), NULL);
	AfterDrawColorBox();
}

void CManufacturePage::AfterDrawColorBox(void)
{
	if( m_plblColorBox == NULL )
		return ;

	if( curColor > COLOR_BEGIN && curColor < COLOR_END )
	{
		DFBColor bgColor;

		bgColor.a = color_tb[curColor].a;
		bgColor.r = color_tb[curColor].r;
		bgColor.g = color_tb[curColor].g;
		bgColor.b = color_tb[curColor].b;

		lite_set_label_bgcolor(m_plblColorBox, &bgColor);
	}
}

void CManufacturePage::AfterDrawColorBoxCB(void *data)
{
	CManufacturePage *pPage = (CManufacturePage *)data;

	if (pPage)
	{
		pPage->AfterDrawColorBox();
	}
}

void CManufacturePage::RestoreDefParam()
{
	// Default param
	g_RunParam.tuobangPara.bMicroPhone = TRUE;
	g_RunParam.tuobangPara.brightNess = TP_BRIGHTNESS_HIGH;
	g_RunParam.tuobangPara.lang = LANG_ENGLISH;
	g_RunParam.tuobangPara.TimeFmt = TIME_FMT_12HR;

	// Set LCD brightness
	CDesktop* pDesktop = NULL;
	pDesktop = CDesktop::GetDesktop();
	pDesktop->CtrlLCDBright(g_RunParam.tuobangPara.brightNess);

	// Load language
	InitLangDesc(g_RunParam.tuobangPara.lang);

	// Save param
	sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

	DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);
}



