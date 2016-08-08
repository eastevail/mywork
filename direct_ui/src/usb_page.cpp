/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: usb_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2015.04.18
******************************************************************************/

#include "desktop.h"
#include "usb_page.h"

CUSBPage::CUSBPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_msgLabel = NULL;
	m_topLabel = NULL;

	m_imgBk = NULL;
}

CUSBPage::~CUSBPage()
{
}

#if 0
void CUSBPage::OnPageInit()
{
	DFBRectangle		dfbRect;
	DFBColor			bgColor;
	DFBColor			fontColor;

	// top label
	dfbRect.x				= 0;
	dfbRect.y				= 0;
	dfbRect.w				= g_RunParam.screenWidth - SETTING_TXT_W/2 + 30;
	dfbRect.h				= REVIEW_TR_H;

	bgColor.a				= REVIEW_TOP_BG_COLOR_A;
	bgColor.r				= REVIEW_TOP_BG_COLOR_R;
	bgColor.g				= REVIEW_TOP_BG_COLOR_G;
	bgColor.b				= REVIEW_TOP_BG_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 16, NULL, &m_topLabel))
	{
		DBG("[DirectFBUI-ReviewPage] create top label failure\n");
		return FALSE;
	}

	dfbRect.x				= 0;
	dfbRect.y				= TOP_LABEL_H;
	dfbRect.w				= g_RunParam.screenWidth;
	dfbRect.h				= g_RunParam.screenHeight-TOP_LABEL_H;

	bgColor.a				= TOP_LABEL_COLOR_A;
	bgColor.r				= TOP_LABEL_COLOR_R;
	bgColor.g				= TOP_LABEL_COLOR_G;
	bgColor.b				= TOP_LABEL_COLOR_B;
	fontColor.a				= 0xFF;
	fontColor.r				= 0xFF;
	fontColor.g				= 0xFF;
	fontColor.b				= 0xFF;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, DEVICE_CONNECTED_FONT_SIZE, &fontColor, &m_msgLabel))
	{
		DBG("[DirectFBUI-USBPage] create connect to pc message label failure\n");
		return;
	}
	DBG("[DirectFBUI-USBPage] show message\n");

	lite_set_label_text(m_msgLabel, "Device Connected");
	lite_set_label_alignment(m_msgLabel, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_msgLabel, TRUE);

#if 0
	tsLangNotiMsg *pNotiInfo = &g_CurLangDesc.NotiMsg[Notifiction_DeviceConnected];
	// bk image
	dfbRect.x = 0;
	dfbRect.y = REVIEW_TR_H;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight - REVIEW_TR_H;
	CreateLiTEImage(&dfbRect, pNotiInfo->imgPath.c_str(), &m_imgBk);
#endif
}
#else
void CUSBPage::OnPageInit()
{
	DFBRectangle dfbRect;
	DFBColor fontColor;
	DFBColor bgColor;
	CDesktop *pDesktop = NULL;
	LiteWindow *liteWin = NULL;
	tsLangNotiMsg *pNotiInfo;

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_IDLE;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	pNotiInfo = &g_CurLangDesc.NotiMsg[Notifiction_DeviceConnected];

	pDesktop = CDesktop::GetDesktop();
	liteWin = pDesktop->GetLiTEWindow();

	// top label
	dfbRect.x				= 0;
	dfbRect.y				= 0;
	dfbRect.w				= g_RunParam.screenWidth - SETTING_TXT_W/2 + 30;
	dfbRect.h				= REVIEW_TR_H;

	bgColor.a				= TOP_LABEL_COLOR_A;
	bgColor.r				= TOP_LABEL_COLOR_R;
	bgColor.g				= TOP_LABEL_COLOR_G;
	bgColor.b				= TOP_LABEL_COLOR_B;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 16, NULL, &m_topLabel))
	{
		DBG("[DirectFBUI-ReviewPage] create top label failure\n");
		return FALSE;
	}

	// bk image
	dfbRect.x = 0;
	dfbRect.y = REVIEW_TR_H;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight - REVIEW_TR_H;
	if( DFB_OK == lite_new_image(LITE_BOX(liteWin), &dfbRect, liteNoImageTheme, &m_imgBk) )
	{
		lite_load_image(m_imgBk, pNotiInfo->imgPath.c_str());
		lite_set_image_visible(m_imgBk, true);
		lite_update_box(LITE_BOX(m_imgBk), NULL);
	}

	// text
	fontColor.a			= RECORD_TIME_COLOR_A;
	fontColor.r			= RECORD_TIME_COLOR_R;
	fontColor.g			= RECORD_TIME_COLOR_G;
	fontColor.b			= RECORD_TIME_COLOR_B;

	// main text 1
	dfbRect.x 			= 0;
	dfbRect.y 			= MAIN_TEXT1_POS;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= MAIN_TEXT_HEIGHT;
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, MAIN_FONT_SIZE, &fontColor, &m_msgLabel))
	{
		lite_set_label_alignment(m_msgLabel, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_msgLabel, true);
		lite_set_label_text(m_msgLabel, pNotiInfo->vecMainText[0].c_str());
	}
}
#endif

void CUSBPage::OnPageExit()
{
	SAFE_DEL_LITE(m_msgLabel);
	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_imgBk);
}

void CUSBPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	// do nothing
}




