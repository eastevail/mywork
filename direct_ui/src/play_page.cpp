/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: play_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.09
	Description	:
	History		:
					Create by zyfan.2015.04.09
******************************************************************************/
#include <string.h>

#include "play_page.h"
#include "desktop.h"
#include "review.h"

CPlayPage::CPlayPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	memset(m_fileToPlay, 0, MAX_FILE_LEN);
	m_playState = Play_State_Begin;
	m_playDir = Play_Dir_Begin;
	m_topLabel = NULL;
	m_progress = NULL;
	m_timeLabel = NULL;
	m_playState	= Play_State_Begin;
	m_playDir = Play_Dir_Begin;
	m_listSubFont = NULL;
	m_listSubFontInterface = NULL;
	m_fileTimeTotal = 0.0;
	m_curTime = 0;
	m_ReviewImage = NULL;
	m_PauseOKImage = NULL;
	m_ExitImage = NULL;

	m_iRWArrow = ARROW_NORMAL;
	m_iFFArrow = ARROW_NORMAL;

	m_afterFileDuration = 0;
}

CPlayPage::~CPlayPage()
{
}

void CPlayPage::OnPageInit()
{
	CReviewPage*				pReviewPage 	= NULL;
	DFBRectangle				dfbRect;
	DFBColor					bgColor;
	DFBColor					fontColor;
	CDesktop*					pDesktop 		= NULL;
	LiteWindow*					liteWin 		= NULL;

	if (HasParentPage())
	{
		pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
		pReviewPage->GetCurPlayFile(m_fileToPlay, MAX_FILE_LEN);
	}

	DBG("m_fileToPlay = %s\n", m_fileToPlay);

	m_fileTimeTotal = pReviewPage->GetCurFileduration();
	m_afterFileDuration = pReviewPage->GetAfterFileduration();

	InitSubStrFont();

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	// 上Label
	dfbRect.x				= 0;
	dfbRect.y				= 0;
	dfbRect.w				= g_RunParam.screenWidth - SETTING_TXT_W/2+30;
	dfbRect.h				= REVIEW_TR_H; // 30

	bgColor.a				= TOP_LABEL_COLOR_A;
	bgColor.r				= TOP_LABEL_COLOR_R;
	bgColor.g				= TOP_LABEL_COLOR_G;
	bgColor.b				= TOP_LABEL_COLOR_B;

	fontColor.a				= 0xFF;
	fontColor.r				= 0xCC;
	fontColor.g				= 0xCC;
	fontColor.b				= 0xCC;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 14, NULL, &m_topLabel))
	{
		DBG("[DirectFBUI-PlayPage] create top label failure\n");
		return;
	}
	lite_set_label_draw_after(m_topLabel, AfterDrawTopLabelCB, this);

	// 时间label
	dfbRect.x				= 5;
	dfbRect.y				= 226 - 24 - 14;
	dfbRect.w				= 100;
	dfbRect.h				= 34;

	bgColor.a				= 0xFF;
	bgColor.r				= 0x00;
	bgColor.g				= 0x00;
	bgColor.b				= 0x00;

	fontColor.a				= 0xFF;
	fontColor.r				= 0xFF;
	fontColor.g				= 0xFF;
	fontColor.b				= 0xFF;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 30, &fontColor, &m_timeLabel))
	{
		DBG("[DirectFBUI-PlayPage] create top label failure\n");
		return;
	}
	lite_set_label_alignment(m_timeLabel, LITE_LABEL_LEFT);
	lite_set_label_text(m_timeLabel, "00:00");

	// 进度条
	InitProgreeBar();

	pDesktop->setMic(TRUE);

#if 1
	TOP_BAND_ZOOM_S topbandZoom;
	topbandZoom.zoom_step = Zoom_Normal;
	sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);

	TOP_BAND_RATION_S topbandRation;
	topbandRation.ration_step = Rotate_Normal;
	sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
#endif

	// 发送消息到encode开始解码文件
	m_playState = Play_State_Play;
	m_playDir = Play_Dir_Normal;

	DECODE_MP4_S mp4Info;
	memset(&mp4Info, 0, sizeof(DECODE_MP4_S));
	strcpy(mp4Info.Mp4FileName, m_fileToPlay);
	mp4Info.playState = DECODE_STATE_START;
	mp4Info.Speed = DECODE_PLAYSPEED_NORMAL;
	mp4Info.TimeInterval = 1000;
	sys_msg_cmd_playmp4(&mp4Info, MSG_TYPE_ENCODE_SVR);
}

void CPlayPage::OnPageExit()
{
	DBG("OnPageExit\n");

	m_playState = Play_State_Begin;
	m_playDir = Play_Dir_Begin;

	CDesktop* pDesktop = CDesktop::GetDesktop();
	pDesktop->setMic(FALSE);

	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_progress);
	SAFE_DEL_LITE(m_timeLabel);
	SAFE_DEL_LITE(m_ReviewImage);
	SAFE_DEL_LITE(m_PauseOKImage);
	SAFE_DEL_LITE(m_ExitImage);

	DestorySubStrFont();
}

void CPlayPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (dfbEvent->type == DWET_KEYDOWN)
	{
		DBG("DirectFB_UI-PlayPage: OnKeyEvent %d\n", dfbEvent->key_code);
		switch (dfbEvent->key_code)
		{
			case LEFT_BTN:
			{
				OnLeftKeyEvent();
				g_RunParam.dlyKey = 2;
				break;
			}

			case RIGHT_BTN:
			{
				OnRightKeyEvent();
				g_RunParam.dlyKey = 2;
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
				g_RunParam.dlyKey = 2;
				break;
			}

			case SETTING_BTN:
			{
				OnSettingKeyEvent();
				break;
			}

			case REPLAY_BTN: // Exit play page
			{
				OnReplayKeyEvent();
				g_RunParam.dlyKey = 2;
				break;
			}

			case VIDEO_CAP_BTN:
			{
				OnVideoCaptureKeyEvent();
				break;
			}

			default:
			{
				DBG("[DirectFBUI-PlayPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CPlayPage::OnWaitingEventCb(void *data)
{
	CPlayPage *pPage = (CPlayPage *)data;

	if( pPage )
	{
		pPage->OnWaitingEvent();
	}
}

void CPlayPage::OnWaitingEvent()
{
	CReviewPage *pReviewPage = NULL;

	DBG("OnWaitingEvent, Recover, play end.\n");

	pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());

	pReviewPage->Recover();

}

void CPlayPage::OnWindowEvent(Window_Msg_S *winMsg)
{
	DBG("[DirectFBUI-PlayPage] msgType = %d\n", winMsg->msgType);

	switch (winMsg->msgType)
	{
		case MSG_DECODE_PLAY_CHANGE: //SYS_MSG_CMD_DECODE_PLAY_STATE_CHG
		{
			DECODE_MP4_S *pMp4Info = (DECODE_MP4_S *)winMsg->buffer;

			if( pMp4Info->playState == DECODE_STATE_STOP )
			{
				// Finish play
				DBG("MSG_DECODE_PLAY_CHANGE, Finish play\n");

				if( m_progress )
				{
					if( m_playDir == Play_Dir_Back )
						lite_set_progressbar_value(m_progress, 0.0f);
					else
						lite_set_progressbar_value(m_progress, 1.0f);
				}
			}

			break;
		}

		case MSG_DECODE_GET_CURTIME: // SYS_MSG_CMD_DECODE_GET_CURTIME
		{
			char szTime[64];
			float value = 0;
			DECODE_MP4_S *pMp4Info = (DECODE_MP4_S *)winMsg->buffer;
			memset(szTime, 0, sizeof(szTime));

			// Play time stamp
			if( m_playState == Play_State_Play )
			{
				ComTimToStr(pMp4Info->curTime, szTime);

				if( m_timeLabel )
				{
					lite_set_label_text(m_timeLabel, szTime);
				}

				if( m_progress && pMp4Info->curTime >= 1000 )
				{
					//printf("2.curTime:%f	; m_fileTimeTotal:%f\n",pMp4Info->curTime,m_fileTimeTotal);

				#if 1 // xjf
					value = pMp4Info->curTime / (m_afterFileDuration*1000);
					lite_set_progressbar_value(m_progress, value);
					DBG("value: %f, curTime: %f, m_afterFileDuration: %ld\n", value, pMp4Info->curTime, m_afterFileDuration);
				#else
					value = pMp4Info->curTime / (m_fileTimeTotal*1000);
					lite_set_progressbar_value(m_progress, value);
					DBG("value: %f, curTime: %f, m_fileTimeTotal: %f\n", value, pMp4Info->curTime, m_fileTimeTotal);
				#endif
				}
			}

			break;
		}

		case MSG_DECODE_PARAM: // SYS_MSG_CMD_DECODE_MP4_PARAM
		{
			// Start play
			char szTime[64];
			DECODE_MP4_S *pMp4Info = (DECODE_MP4_S *)winMsg->buffer;
			memset(szTime, 0, sizeof(szTime));

			ComTimToStr(pMp4Info->curTime, szTime);
			if (m_timeLabel)
			{
				lite_set_label_text(m_timeLabel, szTime);
			}

			break;
		}
		case MSG_RECORD_REPLAY: // SYS_MSG_EVENT_DISCONNECT
		{
			DECODE_MP4_S *pMp4Info = (DECODE_MP4_S *)winMsg->buffer;

			if( pMp4Info->playState == DECODE_STATE_STOP )
			{
				char szTime[32];

				sprintf(szTime, "%02d:%02d", 0, 0);

				if( m_timeLabel )
				{
					lite_set_label_text(m_timeLabel, szTime);
				}

				sys_msg_cmd_top_band_pic(&g_RunParam.pictProp, MSG_TYPE_ENCODE_SVR);

				if( m_progress )
				{
					lite_set_progressbar_value(m_progress, 0.0f);
				}

				m_playState = Play_State_Replay;

				if( m_topLabel )
				{
					lite_update_box(LITE_BOX(m_topLabel), NULL);
				}
			}
			else if( pMp4Info->playState == DECODE_STATE_END )
			{
				CReviewPage *pReviewPage = NULL;
				pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
				pReviewPage->Recover();

				DBG("MSG_RECORD_REPLAY, DECODE_STATE_END, Recover\n");
			}

			break;
		}
		case MSG_RECORD_STOP: // shut down
		{
			DECODE_MP4_S mp4Info;
			memset(&mp4Info, 0, sizeof(DECODE_MP4_S));
			mp4Info.playState = DECODE_STATE_IDLE;
			sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

			SAFE_DEL_LITE(m_topLabel);
			SAFE_DEL_LITE(m_progress);
			SAFE_DEL_LITE(m_timeLabel);
			SAFE_DEL_LITE(m_ReviewImage);
			SAFE_DEL_LITE(m_PauseOKImage);
			SAFE_DEL_LITE(m_ExitImage);

			break;
		}
	}
}

void CPlayPage::AfterDrawTopLabelCB(void *data)
{
	CPlayPage *pPage = (CPlayPage *)data;

	if (pPage)
	{
		pPage->AfterDrawTopLabel();
	}
}

void CPlayPage::AfterDrawTopLabel(void)
{
	if (m_topLabel == NULL)
	{
		return;
	}

	LiteBox*			liteBox			= LITE_BOX(m_topLabel);
	IDirectFBSurface*	surface			= liteBox->surface;
	int					height			= 16; 	// pixel
	int					left			= 2;	// pixel
	int					width			= 10;	// pixel
	DFBPoint			p1, p2, p3;
	int					space			= 100;	// pixel
	char 				szBuf[MAX_LANG_LEN];
	IDirectFBFont*		fontInterface 	= NULL;
	int					fontWidth 		= 0;
	int					fontHeight		= 0;
	DFBSurfaceTextFlags flags 			= DSTF_TOP;
	int					marginLeft		= 0;
	DFBRectangle		dfbRect;
	DFBTriangle			dfbTriangle;
	int					triangleBorder	= 6;

	// draw triangle
	if(m_ReviewImage == NULL)
	{
		DFBRectangle dfbRect;

		dfbRect.x			= PLAY_REVIEW_IMG_X;
		dfbRect.y			= PLAY_REVIEW_IMG_Y;
		dfbRect.w			= PLAY_REVIEW_IMG_W;
		dfbRect.h			= PLAY_REVIEW_IMG_H;

		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"PLAY_REVIEW_FILE, &m_ReviewImage))
		{
			return FALSE;
		}
	}

	marginLeft = PLAY_REVIEW_IMG_X + PLAY_REVIEW_IMG_W*2;

	// Draw OK icon
	if(m_PauseOKImage == NULL)
	{
		DFBRectangle dfbRect;

		dfbRect.x			= marginLeft + 50;
		dfbRect.y			= PLAY_PAUSEOK_IMG_Y;
		dfbRect.w			= PLAY_PAUSEOK_IMG_W;
		dfbRect.h			= PLAY_PAUSEOK_IMG_H;

		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"PLAY_PAUSEOK_ON_FILE, &m_PauseOKImage))
		{
			return FALSE;
		}
	}

	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

	// draw PAUSE and OK
	if( m_playState == Play_State_Play )
	{
		surface->DrawString(surface, g_CurLangDesc.Review.PAUSE.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);
	}
	else if( m_playState == Play_State_Stop )
	{
		surface->DrawString(surface, g_CurLangDesc.Review.PLAY.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);
	}
	else if( m_playState == Play_State_Replay )
	{
		surface->DrawString(surface, g_CurLangDesc.Review.PLAY.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);
	}

	marginLeft = marginLeft + 50 + 6 + PLAY_PAUSEOK_IMG_W + 5;

	// draw RW text
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	surface->DrawString(surface, g_CurLangDesc.Review.RW.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);

	// draw RW triggle
	if( m_iRWArrow == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	marginLeft = marginLeft + 25;
	p1.x	= marginLeft;
	p1.y	= p3.y	= liteBox->rect.h / 2 - 2;
	p2.x	= marginLeft + width;
	p2.y	= (liteBox->rect.h - height) / 2 - 2;
	p3.x	= marginLeft + width;
	p3.y	= (liteBox->rect.h + height) / 2 - 2;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	// draw FF text
	marginLeft = marginLeft + width + 6;
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	surface->DrawString(surface, g_CurLangDesc.Review.FF.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);

	// draw FF triggle
	if( m_iFFArrow == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	marginLeft = marginLeft + 20;
	p1.x	= marginLeft;
	p1.y	= (liteBox->rect.h - height) / 2 - 2;
	p2.x	= marginLeft;
	p2.y	= (liteBox->rect.h + height) / 2 - 2;
	p3.x	= marginLeft + width;
	p3.y	= liteBox->rect.h / 2 - 2;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	// draw exit
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	marginLeft = marginLeft +  16;
	surface->DrawString(surface, g_CurLangDesc.Review.EXIT.c_str(), -1, marginLeft, PLAY_FONT_Y, DSTF_TOP);

	if(m_ExitImage == NULL)
	{
		DFBRectangle dfbRect;

		dfbRect.x			= marginLeft + 54;
		dfbRect.y			= PLAY_EXIT_IMG_Y;
		dfbRect.w			= PLAY_EXIT_IMG_W;
		dfbRect.h			= PLAY_EXIT_IMG_H;
		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"PLAY_EXIT_FILE, &m_ExitImage))
		{
			return FALSE;
		}
	}
}
void CPlayPage::OnLeftKeyEvent(void)
{
	if (m_playState != Play_State_Play)
	{
		return;
	}

	DECODE_MP4_S mp4Info;

	memset(&mp4Info, 0, sizeof(DECODE_MP4_S));

	if (m_playDir == Play_Dir_Normal)
	{
		mp4Info.Speed = DECODE_PLAYSPEED_1_2;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_SPEED_CHG, MSG_TYPE_ENCODE_SVR);
		m_playDir = Play_Dir_Back; // 快速倒退播放
	}
	else if (m_playDir == Play_Dir_Back)
	{
		mp4Info.Speed = DECODE_PLAYSPEED_NORMAL;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_SPEED_CHG, MSG_TYPE_ENCODE_SVR);
		m_playDir = Play_Dir_Normal; // 正常播放
	}

	m_iRWArrow = ARROW_RED;
	lite_update_box(LITE_BOX(m_topLabel), NULL);
}

void CPlayPage::OnRightKeyEvent(void)
{
	if (m_playState != Play_State_Play)
	{
		return;
	}

	DECODE_MP4_S mp4Info;

	memset(&mp4Info, 0, sizeof(DECODE_MP4_S));

	if (m_playDir == Play_Dir_Normal)
	{
		mp4Info.Speed = DECODE_PLAYSPEED_2;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_SPEED_CHG, MSG_TYPE_ENCODE_SVR);
		m_playDir = Play_Dir_Forward; // 快速播放
	}
	else if (m_playDir == Play_Dir_Forward)
	{
		mp4Info.Speed = DECODE_PLAYSPEED_NORMAL;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_SPEED_CHG, MSG_TYPE_ENCODE_SVR);
		m_playDir = Play_Dir_Normal; // 正常播放
	}

	m_iFFArrow = ARROW_RED;
	lite_update_box(LITE_BOX(m_topLabel), NULL);
}

void CPlayPage::OnUpKeyEvent(void)
{
}

void CPlayPage::OnDownKeyEvent(void)
{
}

void CPlayPage::OnOKKeyEvent(void)
{
	DECODE_MP4_S mp4Info;
	memset(&mp4Info, 0, sizeof(DECODE_MP4_S));

	if (m_playState == Play_State_Play)
	{
		// 发送信息到encode暂停播放
		mp4Info.playState = DECODE_STATE_PAUSE;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
		m_playState = Play_State_Stop;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
	else if (m_playState == Play_State_Stop)
	{
	#if 1 // xjf, play normal after FF and RW
		mp4Info.Speed = DECODE_PLAYSPEED_NORMAL;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_SPEED_CHG, MSG_TYPE_ENCODE_SVR);
		m_playDir = Play_Dir_Normal; // 正常播放
	#endif

		// 发送信息到encode开始播放
		mp4Info.playState = DECODE_STATE_START;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
		m_playState = Play_State_Play;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
	else if( m_playState == Play_State_Replay )
	{
		m_playState = Play_State_Play;
		m_playDir = Play_Dir_Normal;

		strcpy(mp4Info.Mp4FileName, m_fileToPlay);
		mp4Info.playState = DECODE_STATE_START;
		mp4Info.Speed = DECODE_PLAYSPEED_NORMAL;
		mp4Info.TimeInterval = 1000;
		sys_msg_cmd_playmp4(&mp4Info, MSG_TYPE_ENCODE_SVR);

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CPlayPage::OnSettingKeyEvent(void)
{
#if 0 // xjf, for test
	Window_Msg_S winMsg;
	CDesktop *pDesktop = CDesktop::GetDesktop();

	winMsg.msgType = MSG_RECORD_STOP;
	pDesktop->PostWindowMessage(&winMsg);
#endif
}

void CPlayPage::OnReplayKeyEvent(void)
{
	if( m_curTime == 0 ) // xjf
	{
		DECODE_MP4_S mp4Info;

	#if 1
		memset(&mp4Info, 0, sizeof(DECODE_MP4_S));
		mp4Info.playState = DECODE_STATE_END;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

		CReviewPage *pReviewPage = NULL;
		pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
		pReviewPage->Recover();
	#else
		memset(&mp4Info, 0, sizeof(DECODE_MP4_S));
		mp4Info.playState = DECODE_STATE_IDLE;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

		CReviewPage *pReviewPage = NULL;
		pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
		pReviewPage->Recover();
	#endif
	}
	else
	{
		m_curTime += 1;

		if( m_curTime == 3 )
		{
			CReviewPage *pReviewPage = NULL;
			pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
			pReviewPage->Recover();

			DBG("OnReplayKeyEvent, m_curTime: %d\n", m_curTime);
		}
	}
}

void CPlayPage::OnVideoCaptureKeyEvent(void)
{
}

void CPlayPage::InitProgreeBar()
{
	DFBRectangle 				dfbRect;
	CDesktop*					pDesktop 		= NULL;
	LiteWindow*					liteWin 		= NULL;
	int							screenWidth		= 0;
	int							screenHeight	= 0;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x = 5;
	dfbRect.y = 224;
	dfbRect.w = screenWidth - 2 * 5;
	dfbRect.h = 10;

	if (DFB_OK != lite_new_progressbar( LITE_BOX(liteWin), &dfbRect, liteNoProgressBarTheme, &m_progress))
	{
		DBG("DirectFB_UI-PlayPage: init progressbar failure\n");
		return;
	}

	lite_set_progressbar_value(m_progress, 0);

	DBG("DirectFB_UI-PlayPage: init progressbar success\n");
}


int CPlayPage::ComTimToStr(double curtime, char *szValue)
{
	int second = 0, minute = 0;
	int len = 0;
	second = (int)(curtime / 1000);
	minute = second / 60;
	second -= minute * 60;

	if ((minute >= 0) && (minute <= 9))
	{
		len += sprintf(szValue + len, "0%d:", minute);
	}
	else
	{
		len += sprintf(szValue + len, "%d:", minute);
	}

	if ((second >= 0) && (second <= 9))
	{
		len += sprintf(szValue + len, "0%d", second);
	}
	else
	{
		len += sprintf(szValue + len, "%d", second);
	}

	return len;
}

BOOL CPlayPage::InitSubStrFont()
{
	if (DFB_OK != lite_get_font("default", LITE_FONT_BOLD, 9, DEFAULT_FONT_ATTRIBUTE, &m_listSubFont))
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

void CPlayPage::DestorySubStrFont()
{
	if (DFB_OK != lite_release_font(m_listSubFont))
	{
		DBG("[DirectFBUI-SettingPage] release lite font failure\n");
	}

	m_listSubFont = NULL;
	m_listSubFontInterface = NULL;
}

void CPlayPage::OnUpdateArrow()
{
	if( m_iRWArrow == ARROW_RED || \
		m_iFFArrow == ARROW_RED )
	{
		m_iRWArrow = ARROW_NORMAL;
		m_iFFArrow = ARROW_NORMAL;

		if( m_topLabel )
			lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}



