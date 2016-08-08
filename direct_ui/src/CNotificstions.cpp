/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: CNotifictions.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.05.13
	Description	:
	History		:
					Create by hul.2015.05.13
******************************************************************************/
#include "common.h"
#include "desktop.h"
#include "CNotifictions.h"
#include "setting.h"
#include <sys/time.h>

//-----------------------------------------------------
#if 0 // xjf, for test
#define NOTI_SHOW_DURATION		6
#else
#define NOTI_SHOW_DURATION		2
#endif
#define NOTI_SHOW_INTERVAL		10

#define DBG_NOTI	0

typedef struct
{
	int show_id;
	int show_duration;
	int show_interval;
	BOOL show_status[Notifiction_End];

	int idx_list;
	int cnt_list;
	int show_list[Notifiction_End];
} tsNotiCtrl;

//-----------------------------------------------------
tsNotiCtrl NotiCtrl;

//-----------------------------------------------------
CNotifictions::CNotifictions(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	int i;

	NotiCtrl.show_id = -1;
	NotiCtrl.show_duration = NOTI_SHOW_DURATION;
	NotiCtrl.show_interval = 0; // check right now
	for( i = 0; i < Notifiction_End; i++ )
	{
		NotiCtrl.show_status[i] = FALSE;
	}

	NotiCtrl.idx_list = 0;
	NotiCtrl.cnt_list = 0;

	m_NotifictionImage = NULL;

	m_lblMainText[0] = NULL;
	m_lblMainText[1] = NULL;

	m_lblSubText[0] = NULL;
	m_lblSubText[1] = NULL;
	m_lblSubText[2] = NULL;
}

CNotifictions::~CNotifictions()
{
	SAFE_DEL_LITE(m_NotifictionImage);

	SAFE_DEL_LITE(m_lblMainText[0]);
	SAFE_DEL_LITE(m_lblMainText[1]);

	SAFE_DEL_LITE(m_lblSubText[0]);
	SAFE_DEL_LITE(m_lblSubText[1]);
	SAFE_DEL_LITE(m_lblSubText[2]);
}

void CNotifictions::OnPageInit()
{

}

void CNotifictions::OnPageExit()
{

}

void CNotifictions::OnKeyEvent(DFBWindowEvent *dfbEvent)
{


}

void CNotifictions::JumpToParentPage()
{


}

//----------------------------------------------------------
void CNotifictions::InitDispWnd()
{
	DFBRectangle dfbRect;
	DFBColor fontColor;
	CDesktop *pDesktop = NULL;
	LiteWindow *liteWin = NULL;

	pDesktop = CDesktop::GetDesktop();
	liteWin = pDesktop->GetLiTEWindow();

	// bk image
	dfbRect.x = 0;
	dfbRect.y = 0;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight;
	if (DFB_OK != lite_new_image(LITE_BOX(liteWin), &dfbRect, liteNoImageTheme, &m_NotifictionImage))
	{
		DBG("m_NotifictionImage: create image fail\n");
		return ;
	}
	lite_set_image_visible(m_NotifictionImage, false);

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
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, MAIN_FONT_SIZE, &fontColor, &m_lblMainText[0]))
	{
		lite_set_label_alignment(m_lblMainText[0], LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblMainText[0], false);
	}

	// main text 2
	dfbRect.x 			= 0;
	dfbRect.y 			= MAIN_TEXT2_POS;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= MAIN_TEXT_HEIGHT;
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, MAIN_FONT_SIZE, &fontColor, &m_lblMainText[1]))
	{
		lite_set_label_alignment(m_lblMainText[1], LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblMainText[1], false);
	}

	// sub text 1
	dfbRect.x 			= SUB_TEXT_LEFT;
	dfbRect.y 			= SUB_TEXT1_POS;
	dfbRect.w 			= g_RunParam.screenWidth - SUB_TEXT_LEFT;
	dfbRect.h 			= SUB_TEXT_HEIGHT;
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, SUB_FONT_SIZE, &fontColor, &m_lblSubText[0]))
	{
		lite_set_label_alignment(m_lblSubText[0], LITE_LABEL_LEFT);
		lite_set_label_set_visible(m_lblSubText[0], false);
	}

	// sub text 2
	dfbRect.x 			= SUB_TEXT_LEFT;
	dfbRect.y 			= SUB_TEXT2_POS;
	dfbRect.w 			= g_RunParam.screenWidth - SUB_TEXT_LEFT;
	dfbRect.h 			= SUB_TEXT_HEIGHT;
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, SUB_FONT_SIZE, &fontColor, &m_lblSubText[1]))
	{
		lite_set_label_alignment(m_lblSubText[1], LITE_LABEL_LEFT);
		lite_set_label_set_visible(m_lblSubText[1], false);
	}

	// sub text 3
	dfbRect.x 			= SUB_TEXT_LEFT;
	dfbRect.y 			= SUB_TEXT3_POS;
	dfbRect.w 			= g_RunParam.screenWidth - SUB_TEXT_LEFT;
	dfbRect.h 			= SUB_TEXT_HEIGHT;
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, SUB_FONT_SIZE, &fontColor, &m_lblSubText[2]))
	{
		lite_set_label_alignment(m_lblSubText[2], LITE_LABEL_LEFT);
		lite_set_label_set_visible(m_lblSubText[2], false);
	}
}

//----------------------------------------------------------
void CNotifictions::SetNotification(NotifictionId_E id, BOOL show)
{
	if( id <= Notifiction_Begin || id >= Notifiction_End )
		return ;

	NotiCtrl.show_status[id] = show;

	if( m_NotifictionImage == NULL )
	{
		InitDispWnd();
	}
}

void CNotifictions::DrawNotification()
{
	int id, i;

#if DBG_NOTI
	DBG("NotiCtrl.show_interval: %d\n", NotiCtrl.show_interval);
#endif

	if( NotiCtrl.show_interval <= 0 )
	{
		NotiCtrl.cnt_list = 0;

		for( id = Notifiction_MemeryFull; id < Notifiction_End; id++ )
		{
			if( NotiCtrl.show_status[id] == TRUE )
			{
				NotiCtrl.show_list[NotiCtrl.cnt_list] = id;
				NotiCtrl.cnt_list++;

			#if DBG_NOTI
				DBG("NotiCtrl, idx: %d\n", id);
			#endif
			}
		}

	#if DBG_NOTI
		DBG("NotiCtrl.cnt_list: %d\n", NotiCtrl.cnt_list);
	#endif

		if( NotiCtrl.cnt_list > 0 )
		{
			if( (NotiCtrl.idx_list+1) >= NotiCtrl.cnt_list )
				NotiCtrl.idx_list = 0;
			else
				NotiCtrl.idx_list += 1;

			NotiCtrl.idx_list = NotiCtrl.idx_list % NotiCtrl.cnt_list;
			NotiCtrl.show_id = NotiCtrl.show_list[NotiCtrl.idx_list];

			NotiCtrl.show_interval = NOTI_SHOW_INTERVAL;
			NotiCtrl.show_duration = NOTI_SHOW_DURATION;
		}
		else
		{
			NotiCtrl.idx_list = 0;
			NotiCtrl.show_id = -1;
		}
	}
	else
	{
		NotiCtrl.show_interval -= 1;
	}

#if DBG_NOTI
	DBG("NotiCtrl.show_id: %d, NotiCtrl.show_duration: %d\n", \
		NotiCtrl.show_id, NotiCtrl.show_duration);
#endif

	if( NotiCtrl.show_id >= Notifiction_MemeryFull && \
		NotiCtrl.show_id <= Notifiction_ScreenBatteryLowOff )
	{
		if( NotiCtrl.show_duration >= NOTI_SHOW_DURATION )
		{
			tsLangNotiMsg *pNotiInfo = &g_CurLangDesc.NotiMsg[NotiCtrl.show_id];

			NotiCtrl.show_duration -= 1;

			ShowNotification(FALSE); // All show hide

			// bk image
			lite_load_image(m_NotifictionImage, pNotiInfo->imgPath.c_str());
			lite_set_image_visible(m_NotifictionImage, true);
			lite_update_box(LITE_BOX(m_NotifictionImage), NULL);

			// main text
			lite_set_label_set_visible(m_lblMainText[0], false);
			lite_set_label_set_visible(m_lblMainText[1], false);
			for( i = 0; i < pNotiInfo->vecMainText.size(); i++ )
			{
				lite_set_label_text(m_lblMainText[i], pNotiInfo->vecMainText[i].c_str());
				lite_set_label_set_visible(m_lblMainText[i], true);
			}

			// sub text
			lite_set_label_set_visible(m_lblSubText[0], false);
			lite_set_label_set_visible(m_lblSubText[1], false);
			lite_set_label_set_visible(m_lblSubText[2], false);
			for( i = 0; i < pNotiInfo->vecSubText.size(); i++ )
			{
			#if DBG_NOTI
				DBG("id: %d, vecSubText: %s\n", i, pNotiInfo->vecSubText[i].c_str());
			#endif
				lite_set_label_text(m_lblSubText[i], pNotiInfo->vecSubText[i].c_str());
				lite_set_label_set_visible(m_lblSubText[i], true);
			}
		}
		else
		{
			if( NotiCtrl.show_duration <= 0 )
			{
				ShowNotification(FALSE); // xjf
				NotiCtrl.show_id = -1;
			}
			else
			{
				NotiCtrl.show_duration -= 1;
			}
		}
	}
}

void CNotifictions::ShowNotification(BOOL bShow)
{
	if( m_NotifictionImage )
	{
		lite_set_image_visible(m_NotifictionImage, bShow);

		lite_set_label_set_visible(m_lblMainText[0], bShow);
		lite_set_label_set_visible(m_lblMainText[1], bShow);

		lite_set_label_set_visible(m_lblSubText[0], bShow);
		lite_set_label_set_visible(m_lblSubText[1], bShow);
		lite_set_label_set_visible(m_lblSubText[2], bShow);
	}
}

void CNotifictions::SetAllNotification(BOOL bShow)
{
	int id = 0;

	for( id = Notifiction_MemeryFull; id <= Notifiction_ScreenBatteryLowOff; id++ )
		NotiCtrl.show_status[id] = bShow;
}



