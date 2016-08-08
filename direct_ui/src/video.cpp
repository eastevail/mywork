/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: abspage.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "desktop.h"
#include "video.h"

#include "type_common.h"
#include "msg_common.h"
#include "alarm_msg_drv.h"
#include "sys_msg_drv.h"
#include "msg_util.h"
#include "cw_pthread_mng.h"
#include "cw_util.h"
#include "saveAction.h"

//--------------------------------------------------------
int CVideoPage::s_zoomNum = Zoom_Normal;
Record_Mode_E CVideoPage::m_mode = Record_Mode_Snap;
Rotate_E CVideoPage::m_rotateState = Rotate_Normal;

//--------------------------------------------------------
CVideoPage::CVideoPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_modalDialog = NULL;
	m_recordStatus = Record_Init;

	m_recordBeginTime = 0;
	m_updatIng = 0;

	memset(m_szUpdateFile, 0, sizeof(m_szUpdateFile));
	memset(m_szUpdateFileOk, 0, sizeof(m_szUpdateFileOk));
	memset(m_TimeText,0,sizeof(m_TimeText));
	m_updateThread = 0;
	m_updateBar = NULL;
	m_updateText = NULL;
	m_updateTimerID  = -1;
	m_updateBarValue = 0;

	m_pipefd = -1;
	memset(m_photoName, 0, sizeof(m_photoName));

    m_videoImage = NULL;
    m_recordImage = NULL;
    m_recordTimeLabel = NULL;
    m_micImage = NULL;
    m_noStorageMsg = NULL;
	m_lblStorageMsg = NULL;
    m_rockBrightImage = NULL;
	m_topLabel = NULL;
	m_pLiteFont = NULL;
	m_pFontBoldInterface = NULL;

	m_iRotateLeft = ARROW_NORMAL;
	m_iRotateRight = ARROW_NORMAL;
	m_iZoomUp = ARROW_NORMAL;
	m_iZoomDown = ARROW_NORMAL;

	m_bLedShow = FALSE;
	m_lCurVideoRec = 0;
}

CVideoPage::~CVideoPage()
{
}

void CVideoPage::OnPageInit()
{
	DBG("[DirectFBUI-VideoPage] page init\n");

	InitSubStrFont();
	InitTopLabel();

	InitRecTimeMic();
	UpdateRecTimeMic();

	// 检查是否要升级
	CheckUpdate(); // xjf

	// 启动USB是否插上检测
	CDesktop *pDesktop = NULL;
	pDesktop = CDesktop::GetDesktop();
	pDesktop->StartCheckUSB();

#if 0 // xjf
	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_STOP;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
	usleep(10000);
#endif

	TOP_BAND_RATION_S topbandRation;
	topbandRation.ration_step = m_rotateState;
	sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);

	TOP_BAND_ZOOM_S topbandZoom;
	topbandZoom.zoom_step = CVideoPage::s_zoomNum;
	sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);

	DBG("OnPageInit, s_zoomNum: %d, m_rotateState: %d\n", s_zoomNum, m_rotateState);

	m_pipefd = open(fifo_name, O_RDONLY);
	if( m_pipefd < 0 )
	{
		DBG("open read pipe FAIL\n");
	}

	usleep(50000);
	system("sync");
}

void CVideoPage::OnPageExit()
{
	DBG("[DirectFBUI-VideoPage] page exit\n");

	SAFE_DEL_LITE(m_updateBar);
	SAFE_DEL_LITE(m_updateText);
	if (m_updateTimerID != -1)
	{
		lite_remove_window_timeout(m_updateTimerID);
		m_updateTimerID = -1;
	}

	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_videoImage);
    SAFE_DEL_LITE(m_recordImage);
    SAFE_DEL_LITE(m_recordTimeLabel);
    SAFE_DEL_LITE(m_micImage);
    SAFE_DEL_LITE(m_noStorageMsg);
	SAFE_DEL_LITE(m_lblStorageMsg);
    SAFE_DEL_LITE(m_rockBrightImage);

	DestorySubStrFont();

	if( m_pipefd > 0 )
	{
		close(m_pipefd);
		m_pipefd = -1;
	}
}

BOOL CVideoPage::InitSubStrFont()
{
	if (DFB_OK != lite_get_font("default", LITE_FONT_BOLD, TOPLABEL_FONT_SIZE, DEFAULT_FONT_ATTRIBUTE, &m_pLiteFont))
	{
		DBG("InitSubStrFont, failed m_FontBoldInterface\n");
		return FALSE;
	}

	if (DFB_OK != lite_font(m_pLiteFont, &m_pFontBoldInterface))
	{
		DBG("[DirectFBUI-SettingPage] get lit font object\n");
		return FALSE;
	}

	DBG("InitSubStrFont, over\n");

	return TRUE;
}

void CVideoPage::DestorySubStrFont()
{
	if (DFB_OK != lite_release_font(m_pLiteFont))
	{
		DBG("[DirectFBUI-SettingPage] release lite font failure\n");
	}

	m_pLiteFont = NULL;
	m_pFontBoldInterface = NULL;
}

void CVideoPage::InitTopLabel()
{
	DFBRectangle				dfbRect;
	DFBColor					bgColor;

	if( m_topLabel == NULL )
	{
		dfbRect.x				= 0;
		dfbRect.y				= 0;
		dfbRect.w				= g_RunParam.screenWidth - SETTING_TXT_W/2+30;
		dfbRect.h				= REVIEW_TR_H;

		bgColor.a				= TOP_LABEL_COLOR_A;
		bgColor.r				= TOP_LABEL_COLOR_R;
		bgColor.g				= TOP_LABEL_COLOR_G;
		bgColor.b				= TOP_LABEL_COLOR_B;

		if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, TOPLABEL_FONT_SIZE, NULL, &m_topLabel))
		{
			DBG("[DirectFBUI-PlayPage] create top label failure\n");
			return;
		}
		lite_set_label_draw_after(m_topLabel, AfterDrawTopLabelCB, this);
	}

	// 创建并显示实时视频图标
	dfbRect.x 			= VIDEO_IMG_X;
	dfbRect.y 			= VIDEO_IMG_Y;
	dfbRect.w 			= VIDEO_IMG_W;
	dfbRect.h 			= VIDEO_IMG_H;
	if( m_videoImage == NULL )
	{
		if(CVideoPage::m_mode == Record_Mode_Snap)
			CreateLiTEImage(&dfbRect, PIC_PATH"/"CAPTURE_FILE_1, &m_videoImage);
		else if(CVideoPage::m_mode == Record_Mode_Record)
			CreateLiTEImage(&dfbRect, PIC_PATH"/"VIDEO_FILE_1, &m_videoImage);
	}

	// 创建摇杆LED灯亮度图标
	dfbRect.x			= ROCK_BRIGHT_IMG_X;
	dfbRect.y			= ROCK_BRIGHT_IMG_Y;
	dfbRect.w			= ROCK_BRIGHT_IMG_W;
	dfbRect.h			= ROCK_BRIGHT_IMG_H;
	if( m_rockBrightImage == NULL )
	{
		CreateLiTEImage(&dfbRect, PIC_PATH"/"ROCK_BRIGHT_FILE, &m_rockBrightImage);
	}
	lite_set_image_visible(m_rockBrightImage, false);
}

void CVideoPage::AfterDrawTopLabelCB(void *data)
{
	CVideoPage *pPage = (CVideoPage *)data;

	if (pPage)
	{
		pPage->AfterDrawTopLabel();
	}
}

void CVideoPage::AfterDrawTopLabel(void)
{
	if (m_topLabel == NULL)
	{
		return;
	}

#if 0
	DBG("AfterDrawTopLabel, m_iRotateLeft: %d, m_iRotateRight: %d, m_iZoomUp: %d, m_iZoomDown: %d\n", \
		m_iRotateLeft, m_iRotateRight, m_iZoomUp, m_iZoomDown);
#endif

	LiteBox *liteBox = LITE_BOX(m_topLabel);
	IDirectFBSurface *surface = liteBox->surface;
	//IDirectFBFont *pFontOrg = NULL;
	DFBRectangle dfbRect;
	DFBPoint p1, p2, p3;
	int xx, yy, fontWidth = 0;
	int left, width, height;
	char *pItem = NULL;
	char szZoomText[8];

	// Load bold font
	//surface->GetFont(surface, &pFontOrg);
	//surface->SetFont(surface, m_pFontBoldInterface);

	// Rotate
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	xx = ROTATE_TXT_IMG_X;
	yy = ROTATE_TXT_IMG_Y;
	pItem = g_CurLangDesc.Public.ROTATE.c_str();
	m_pFontBoldInterface->GetStringWidth(m_pFontBoldInterface, pItem, strlen(pItem), &fontWidth);
	surface->DrawString(surface, pItem, -1, xx, yy, DSTF_TOP);

	// Draw rotate left arrow
	if( m_iRotateLeft == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	left = xx + fontWidth + 4;
	height = 8;
	width = 6;
	p1.x = left;
	p1.y = liteBox->rect.h / 2;
	p2.x = left + width;
	p2.y = (liteBox->rect.h - height) / 2;
	p3.x = left + width;
	p3.y = p2.y + height;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	// Draw rotate right arrow
	if( m_iRotateRight == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	p1.x = p2.x + 2;
	p1.y = p2.y;
	p2.x = p1.x + width;
	p2.y = liteBox->rect.h / 2;
	p3.x = p1.x;
	p3.y = p1.y + height;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	// Zoom
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	xx = ZOOM_TXT_IMG_X - 8;
	yy = ZOOM_TXT_IMG_Y;
	pItem = g_CurLangDesc.Public.ZOOM.c_str();
	m_pFontBoldInterface->GetStringWidth(m_pFontBoldInterface, pItem, strlen(pItem), &fontWidth);
	surface->DrawString(surface, pItem, -1, xx, yy, DSTF_TOP);

	// Draw zoom up arrow
	if( m_iZoomUp == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	left = xx + fontWidth + 4;
	height = 6;
	width = 8;
	yy = (liteBox->rect.h - height*2 - 2) / 2;
	p1.x = left;
	p1.y = yy + height;
	p2.x = p1.x + width;
	p2.y = p1.y;
	p3.x = left + width / 2;
	p3.y = yy;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	// Draw zomm down arrow
	if( m_iZoomDown == ARROW_RED )
	{
		surface->SetColor(surface, 0xFF, 0x00, 0x00, 0xFF);
	}
	else
	{
		surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	}
	p1.x = left;
	p1.y = p2.y + 2;
	p2.x = p1.x + width;
	p2.y = p1.y;
	p3.x = p1.x + width / 2;
	p3.y = p1.y + height;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	if (CVideoPage::s_zoomNum != Zoom_Normal)
	{
		sprintf(szZoomText, "%dX", CVideoPage::s_zoomNum+1);

		xx = p2.x + 4;
		yy = ROTATE_TXT_IMG_Y;
		surface->DrawString(surface, szZoomText, -1, xx, yy, DSTF_TOP);
	}

	// Restore font
	//surface->SetFont(surface, pFontOrg);

	//lite_update_box(LITE_BOX(m_videoImage), NULL);

	//lite_update_box(LITE_BOX(m_rockBrightImage), NULL);
}

void CVideoPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	DBG("[DirectFBUI-VideoPage] key_code = %X\n", dfbEvent->key_code);

	if (m_updatIng == 1)
	{
		// 正在升级, 不响应按键消息
		DBG("updating, do not response key event\n");
		return;
	}

	if (m_recordStatus == Record_Stop || m_recordStatus == Record_cap)
	{
		if (m_modalDialog)
		{
			m_modalDialog->OnKeyEvent(dfbEvent);
		}
	}
	else
	{
		if( dfbEvent->type == DWET_KEYDOWN )
		{
			if( m_noStorageMsg || m_lblStorageMsg )
			{
				ReleaseStorageMsg();
			}
			else
			{
				switch (dfbEvent->key_code)
				{
					case LEFT_BTN:
					{
						OnLeftKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case RIGHT_BTN:
					{
						OnRightKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case UP_BTN:
					{
						OnUpKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case DOWN_BTN:
					{
						OnDownKeyEvent();
						g_RunParam.dlyKey = 1;
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
						g_RunParam.dlyKey = 1;
						break;
					}

					case REPLAY_BTN:
					{
						OnReplayKeyEvent();
						g_RunParam.dlyKey = 1;
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
	}
}

void CVideoPage::OnLeftKeyEvent(void)
{
	if (m_recordStatus == Record_Init ||m_recordStatus == Record_Ing)
	{
		// 发送旋转命令到encode
		DBG("send msg to\n");
		Rotate_E oldRotate = m_rotateState;
		TOP_BAND_RATION_S topbandRation;
		int rotateNum = 0;

		rotateNum = (int)m_rotateState;
		rotateNum --;

		rotateNum = (rotateNum > (int)Rotate_270) ? (int)Rotate_Normal : rotateNum;
		rotateNum = (rotateNum < (int)Rotate_Normal) ? (int)Rotate_270 : rotateNum;
		m_rotateState = (Rotate_E)rotateNum;

		if (oldRotate != m_rotateState)
		{
			topbandRation.ration_step = rotateNum;
			sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
		}

		m_iRotateLeft = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

//ration_step = 1 -- 0 ; 2 -- 90 , 3 -- 180 ,4 -- 270
void CVideoPage::OnRightKeyEvent(void)
{
	if (m_recordStatus == Record_Init ||m_recordStatus == Record_Ing)
	{
		// 发送旋转命令到encode
		DBG("send msg to\n");
		Rotate_E oldRotate = m_rotateState;
		TOP_BAND_RATION_S topbandRation;
		int rotateNum = 0;

		rotateNum = (int)m_rotateState;
		rotateNum ++;

		rotateNum = (rotateNum > (int)Rotate_270) ? (int)Rotate_Normal : rotateNum;
		rotateNum = (rotateNum < (int)Rotate_Normal) ? (int)Rotate_270 : rotateNum;
		m_rotateState = (Rotate_E)rotateNum;

		if (oldRotate != m_rotateState)
		{
			topbandRation.ration_step = rotateNum;
			sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
		}

		m_iRotateRight = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CVideoPage::OnUpKeyEvent(void)
{
	int multiNum = 0;
	int oldZoom = CVideoPage::s_zoomNum;
	TOP_BAND_ZOOM_S topbandZoom;

	if( m_recordStatus == Record_Init || m_recordStatus == Record_Ing )
	{
		multiNum = CVideoPage::s_zoomNum;
		multiNum++;
		multiNum = (multiNum > (int)Zoom_X4) ? (int)Zoom_X4 : multiNum;
		multiNum = (multiNum < (int)Zoom_Normal) ? (int)Zoom_Normal : multiNum;
		CVideoPage::s_zoomNum = multiNum;

		if (oldZoom != CVideoPage::s_zoomNum)
		{
			// 发消息给encode模块执行放大
			DBG("[DirectFBUI-VideoPage] zoomNum = %d\n", CVideoPage::s_zoomNum);
			//topbandZoom.zoom_step = 1;
			topbandZoom.zoom_step = CVideoPage::s_zoomNum;
			sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
		}

		m_iZoomUp = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CVideoPage::OnDownKeyEvent(void)
{
	if(m_recordStatus == Record_Init ||m_recordStatus == Record_Ing)
	{
		int multiNum = 0;
		int oldZoom = CVideoPage::s_zoomNum;
		TOP_BAND_ZOOM_S topbandZoom;

		multiNum = CVideoPage::s_zoomNum;
		multiNum --;
		multiNum = (multiNum > (int)Zoom_X4) ? (int)Zoom_X4 : multiNum;
		multiNum = (multiNum < (int)Zoom_Normal) ? (int)Zoom_Normal : multiNum;
		CVideoPage::s_zoomNum = (Zoom_E)multiNum;

		if (oldZoom != CVideoPage::s_zoomNum)
		{
			DBG("[DirectFBUI-VideoPage] zoomNum = %d\n", CVideoPage::s_zoomNum);
			//topbandZoom.zoom_step = -1;
			topbandZoom.zoom_step = CVideoPage::s_zoomNum;
			sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
		}

		m_iZoomDown = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

BOOL CVideoPage::abnormalStopRecord()
{
	BOOL bRet = FALSE;

	if(CVideoPage::m_mode == Record_Mode_Record && m_recordStatus == Record_Ing)
	{
		DBG("!!! STOP RECORD AND SAVE !!!\n");

		rcrd_msg_event_req_rcrd(RECORD_STATUS_STOP, 0, -1, -1);

		//CancelShowRecordTimeText();

		ShowVideoFlag(TRUE);
		ShowRecordFlag(FALSE);

		if(m_modalDialog)
		{
			delete m_modalDialog;
			m_modalDialog = NULL;
		}

		m_recordStatus = Record_Init;

		UpdateRecTimeMic();

		bRet = TRUE;

		if( m_pipefd > 0 )
		{
			char tmpBuf[128];
			int res;

			res = read(m_pipefd, tmpBuf, sizeof(tmpBuf)-1);
			if( res > 0 )
			{
				tmpBuf[res] = 0;
				DBG("abnormalStopRecord: %s\n", tmpBuf);
			}
		}
	}

	return bRet;
}

void CVideoPage::OnOKKeyEvent(void)
{
	DFBRectangle 		dfbRect;
	CDesktop*			pDesktop 		= NULL;

	pDesktop = CDesktop::GetDesktop();

	if( pDesktop->isSDCardExsit() == FALSE )
	{
		if( m_noStorageMsg != NULL )
		{
			ReleaseStorageMsg();
		}
		else
		{
			ShowNoStorageMsg();
		}

		return;
	}
	else
	{
		ReleaseStorageMsg();
	}

	if( pDesktop->CheckCanRecordCapture() > 0 )
	{
		DBG("CheckCanRecordCapture, FALSE\n");
		return;
	}

	pDesktop->m_pNotifictionlMng->ShowNotification(FALSE);

	//pDesktop->delayKeyOperate(1*1000); // xjf

	if (CVideoPage::m_mode == Record_Mode_Record)
	{
		if (m_recordStatus == Record_Init)
		{
			// 请求开始录像
			rcrd_msg_event_req_rcrd(RECORD_STATUS_MANUAL, 3600, -1, -1);

			ShowVideoFlag(FALSE);
			ShowRecordFlag(TRUE);

			m_recordStatus = Record_Ing;
			memset(m_photoName,0,sizeof(m_photoName));

			// 显示定时录像
			BeginShowRecordTimeText("00:00");
			m_lCurVideoRec = 0;
		}
		else if (m_recordStatus == Record_Ing)
		{
			if( m_lCurVideoRec < 2 )
			{
				// Can't stop when record less than 2s
				DBG("Can't stop, m_lCurVideoRec: %d\n", m_lCurVideoRec);
				return ;
			}

		#if 1 // xjf
			if(m_pipefd > 0)
			{
				read(m_pipefd, m_photoName, sizeof(m_photoName));

				DBG("OnOKKeyEvent, Thumb name: %s\n", m_photoName);
			}
		#endif

			rcrd_msg_event_req_rcrd(RECORD_STATUS_STOP, 0, -1, -1);

			// 停止定时录像
			//CancelShowRecordTimeText();

			ShowVideoFlag(TRUE);
			ShowRecordFlag(FALSE);

			// Display save comfirm dialog
			dfbRect.x			= 0;
			dfbRect.y			= 0;
			dfbRect.w			= g_RunParam.screenWidth;
			dfbRect.h			= g_RunParam.screenHeight;

			DBG("[DirectFBUI-VideoPage] create dialog\n");

		#if 1 // xjf
			if( m_recordTimeLabel )
				lite_set_label_set_visible(m_recordTimeLabel, false);

			if( m_micImage )
				lite_set_image_visible(m_micImage, false);
		#endif

			m_modalDialog = new CModalDialog(dfbRect);

			m_modalDialog->SetOKCB(g_CurLangDesc.Public.Save.c_str(), OnSaveCB, this);
			m_modalDialog->SetCancelCB(g_CurLangDesc.Public.Cancel.c_str(), OnCancelCB, this);

			m_modalDialog->setDailogBackground(0, 0, 0);
			m_modalDialog->setRecordTime(m_TimeText);
			m_modalDialog->setDailogBackground(m_photoName);

			m_modalDialog->ShowDialog();

			m_recordStatus = Record_Stop;
		}
		else if (m_recordStatus == Record_Stop)
		{
			if (m_modalDialog != NULL)
			{
				// m_modalDialog->OnKeyEvent()
			}
		}
	}
	else if (CVideoPage::m_mode == Record_Mode_Snap)
	{
		DBG("[DirectFBUI-VideoPage] Record_Mode_Snap\n");

		if( m_recordStatus != Record_cap )
		{
			int res = -1;

			memset(m_photoName, 0, sizeof(m_photoName));

			rcrd_msg_event_req_capture(-1, ALARM_MSG_CMD_BEGIN);

			if( m_pipefd > 0 )
			{
				res = read(m_pipefd, m_photoName, sizeof(m_photoName));
				if( res > 0 )
				{
					system("sync");
					usleep(300*1000);

					DBG("OnOKKeyEvent, Photo: %s\n", m_photoName);
				}
			}

			// Display save comfirm dialog
			dfbRect.x			= 0;
			dfbRect.y			= 0;
			dfbRect.w			= g_RunParam.screenWidth;
			dfbRect.h			= g_RunParam.screenHeight;

			DBG("[DirectFBUI-VideoPage] create dialog\n");

			m_modalDialog = new CModalDialog(dfbRect);

			m_modalDialog->SetOKCB(g_CurLangDesc.Public.Save.c_str(), OnPhotoSaveCB, this);
			m_modalDialog->SetCancelCB(g_CurLangDesc.Public.Cancel.c_str(), OnPhotoCancelCB, this);
			m_modalDialog->setDailogBackground(m_photoName);

		#if 1
			TOP_BAND_ZOOM_S topbandZoom;
			topbandZoom.zoom_step = Zoom_Normal;
			sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
		#endif

			m_modalDialog->ShowDialog();

			m_recordStatus = Record_cap;
		}
	}
}

void CVideoPage::OnSettingKeyEvent(void)
{
	if(m_recordStatus != Record_Ing && m_recordStatus != Record_cap)
	{
		CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
		pDesktop->ShowPageByID(Page_Setting);
	}
}

void CVideoPage::OnReplayKeyEvent(void)
{
	if(m_recordStatus != Record_Ing && m_recordStatus != Record_cap)
	{
		CDesktop *pDesktop = CDesktop::GetDesktop();

		DECODE_MP4_S mp4Info;
		mp4Info.playState = DECODE_STATE_IDLE;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

		pDesktop->ShowPageByID(Page_Review);
	}
}

void CVideoPage::OnVideoCaptureKeyEvent(void)
{
	if( m_recordStatus != Record_Ing && m_recordStatus != Record_cap )
	{
		if( CVideoPage::m_mode == Record_Mode_Record )
		{
			lite_load_image(m_videoImage, PIC_PATH"/"CAPTURE_FILE_1);
			CVideoPage::m_mode = Record_Mode_Snap;

			DBG("m_mode = %d \n",m_mode);
		}
		else if( CVideoPage::m_mode == Record_Mode_Snap )
		{
			lite_load_image(m_videoImage, PIC_PATH"/"VIDEO_FILE_1);
			CVideoPage::m_mode = Record_Mode_Record;

			DBG("m_mode = %d \n",m_mode);
		}

		lite_update_box(LITE_BOX(m_videoImage), NULL);

		UpdateRecTimeMic();
	}
}

//--------------------------------------------------------
void CVideoPage::UpdateRockLed()
{
	if( m_rockBrightImage == NULL )
		return ;

#if 0 // xjf, for test
	/*if( g_SysBatCtrl.iLedBright > 0 )
		g_SysBatCtrl.iLedBright = 0;
	else*/
		g_SysBatCtrl.iLedBright = 1;

	DBG("g_SysBatCtrl.iLedBright: %d\n", g_SysBatCtrl.iLedBright);
#endif

	if( g_SysBatCtrl.iLedBright <= 0 )
	{
		if( m_bLedShow == TRUE )
		{
			lite_set_image_visible(m_rockBrightImage, false);
			m_bLedShow = FALSE;
		}
	}
	else
	{
		if( m_bLedShow == FALSE )
		{
			lite_set_image_visible(m_rockBrightImage, true);
			m_bLedShow = TRUE;
		}
	}
}

//-------------------------------------------------------------
void CVideoPage::UpdateRecTimeMic()
{
	DBG("UpdateRecTimeMic, m_recordStatus: %d\n", m_recordStatus);

	if( m_recordTimeLabel )
	{
		if( CVideoPage::m_mode == Record_Mode_Snap )
		{
			lite_set_label_set_visible(m_recordTimeLabel, false);
		}
		else
		{
			lite_set_label_set_visible(m_recordTimeLabel, true);
			ShowRecordTimeText("00:00");
		}
	}

	if( m_micImage )
	{
		if( CVideoPage::m_mode == Record_Mode_Snap )
		{
			lite_set_image_visible(m_micImage, false);
		}
		else
		{
			lite_set_image_visible(m_micImage, true);
		}
	}
}

//-------------------------------------------------------------
void CVideoPage::InitRecTimeMic()
{
	DFBRectangle 		dfbRect;
	DFBColor			fontColor;
	CDesktop*			pDesktop 	= NULL;
	LiteWindow*			liteWin 	= NULL;

	// Record time stamp
	if( m_recordTimeLabel == NULL )
	{
		pDesktop 		= CDesktop::GetDesktop();
		liteWin			= pDesktop->GetLiTEWindow();

		dfbRect.x 			= RECORD_TIME_TEXT_X;
		dfbRect.y 			= g_RunParam.screenHeight - RECORD_TIME_TEXT_Y_SPACE + 2;
		dfbRect.w 			= RECORD_TIME_TEXT_W;
		dfbRect.h 			= RECORD_TIME_TEXT_H;

		fontColor.a			= RECORD_TIME_COLOR_A;
		fontColor.r			= RECORD_TIME_COLOR_R;
		fontColor.g			= RECORD_TIME_COLOR_G;
		fontColor.b			= RECORD_TIME_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, RECORD_TIME_FONT_SIZE, &fontColor, &m_recordTimeLabel))
		{
			DBG("[DirectFBUI-VideoPage] create record time label failure\n");
			return ;
		}
	}

	// Microphone icon
	if( g_RunParam.tuobangPara.bMicroPhone == 1 )
	{
		if( m_micImage == NULL )
		{
			dfbRect.x	= 8;
			dfbRect.y	= g_RunParam.screenHeight - RECORD_TIME_TEXT_Y_SPACE + 2 + 2;
			dfbRect.w	= 16;
			dfbRect.h	= 24;

			CreateLiTEImage(&dfbRect, PIC_PATH"/"MIC_FILE, &m_micImage);
		}
	}
}

BOOL CVideoPage::BeginShowRecordTimeText(const char *szTimeText)
{
	m_recordBeginTime = time(NULL);

	return TRUE;
}

void CVideoPage::ShowRecordTimeText(const char *szTimeText)
{
	if( m_recordTimeLabel == NULL )
	{
		DBG("[DirectFBUI-VideoPage] must invoke BeginShowRecordTimeText first\n");
		return;
	}

	lite_set_label_text(m_recordTimeLabel, szTimeText);
}

BOOL CVideoPage::CancelShowRecordTimeText()
{

	return TRUE;
}

void CVideoPage::UpdateRecTimeStamp()
{
	if( m_recordStatus == Record_Ing )
		RecordTimerEvent();
}

#if 1
void CVideoPage::RecordTimerEvent()
{
	time_t curTime = time(NULL);
	time_t seconds = curTime - m_recordBeginTime;
	int min	= 0;
	int sec = 0;

	min = seconds / 60;
	sec = seconds % 60;

	sprintf(m_TimeText, "%02d:%02d", min, sec);

	m_lCurVideoRec = seconds;

	//DBG("m_lCurVideoRec: %ld, time: %s\n", m_lCurVideoRec, m_TimeText);

	ShowRecordTimeText(m_TimeText);
}
#else
void CVideoPage::RecordTimerEvent()
{
	time_t curTime 		= time(NULL);
	time_t seconds 		= curTime - m_recordBeginTime;
	int minutes 		= 0;
	int len 			= 0;

	memset(m_TimeText,0,sizeof(m_TimeText));

	minutes = seconds / 60;
	seconds -= minutes * 60;

	if (minutes < 10)
	{
		len += sprintf(m_TimeText + len, "0%d", minutes);
	}
	else
	{
		len += sprintf(m_TimeText + len, "%d", minutes);
	}
	len += sprintf(m_TimeText + len, ":");
	if (seconds < 10)
	{
		len += sprintf(m_TimeText + len, "0%ld", seconds);
	}
	else
	{
		len += sprintf(m_TimeText + len, "%ld", seconds);
	}

	ShowRecordTimeText(m_TimeText);
}
#endif

BOOL CVideoPage::ShowVideoFlag(BOOL bVisible)
{
	if (m_videoImage != NULL)
	{
		lite_set_image_visible(m_videoImage, bVisible);
	}

	return TRUE;
}

BOOL CVideoPage::ShowRecordFlag(BOOL bVisible)
{
	DFBRectangle 		dfbRect;

	if (m_recordImage == NULL)
	{
		// 创建并显示录像图标
		dfbRect.x 			= RECORD_IMG_X;
		dfbRect.y 			= RECORD_IMG_Y;
		dfbRect.w 			= RECORD_IMG_W;
		dfbRect.h 			= RECORD_IMG_H;
		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"RECORD_FILE, &m_recordImage))
		{
			return FALSE;
		}
	}

	if (m_recordImage != NULL)
	{
		lite_set_image_visible(m_recordImage, bVisible);
	}

	return TRUE;
}

void CVideoPage::OnSaveCB(void *ctx)
{
	CVideoPage *pPage = (CVideoPage *)ctx;

	if (pPage)
	{
		pPage->OnSave();
	}
}

void CVideoPage::OnPhotoSaveCB(void *ctx)
{
	CVideoPage *pPage = (CVideoPage *)ctx;

	if (pPage)
	{
		pPage->OnPhotoSave();
	}
}

void CVideoPage::saveAction()
{
	if(m_modalDialog)
		m_modalDialog->setKeyHide();

	 SaveAction *pAction = new SaveAction;
	 pAction->actionPlay(OnActionOverCB, (void*)this);
}

void CVideoPage::OnWindowEvent(Window_Msg_S *winMsg)
{
	if( winMsg )
	{
		if( winMsg->msgType == MSG_RECORD_STOP )
		{
			// Shut down save record
			if( CVideoPage::m_mode == Record_Mode_Record && m_recordStatus == Record_Ing )
			{
				//DBG("Shut down save record\n");
				rcrd_msg_event_req_rcrd(RECORD_STATUS_STOP, 0, -1, -1);
			}
		}
	}
}

void CVideoPage::OnActionOverCB(void *ctx)
{
	CVideoPage *pPage = (CVideoPage *)ctx;

	if (pPage)
	{
		pPage->OnActionOver();
	}
}

void CVideoPage::OnActionOver()
{
	if(m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_STOP;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	if(CVideoPage::m_rotateState !=  Rotate_Normal)
	{
		usleep(5000);
		TOP_BAND_RATION_S topbandRation;
		topbandRation.ration_step = m_rotateState;
		sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
	}

	if(CVideoPage::s_zoomNum != Zoom_Normal)
	{
		usleep(5000);
		TOP_BAND_ZOOM_S topbandZoom;
		topbandZoom.zoom_step = CVideoPage::s_zoomNum;
		sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
	}

	system("sync");

	m_recordStatus = Record_Init;
}

// OK, Save photo file
void CVideoPage::OnPhotoSave()
{
	DBG("[DirectFBUI-VideoPage] OnPhotoSave\n");

	saveAction();
}

// OK, Save video file
void CVideoPage::OnSave()
{
	DBG("[DirectFBUI-VideoPage] OnSave\n");

	// 发送保存这个文件的消息
	rcrd_msg_event_req_rcrd(RECORD_STATUS_STOP, 0, -1, -1);

	saveAction();
	UpdateRecTimeMic();
}

void CVideoPage::OnPhotoCancelCB(void *ctx)
{
	CVideoPage *pPage = (CVideoPage *)ctx;

	if (pPage)
	{
		pPage->OnPhotoCancel();
	}
}

// Cancel, cancel photo save
void CVideoPage::OnPhotoCancel()
{
	DBG("[DirectFBUI-VideoPage] OnPhotoCancel\n");

	if (m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_STOP;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	if(CVideoPage::m_rotateState !=  Rotate_Normal)
	{
		usleep(5000);
		TOP_BAND_RATION_S topbandRation;
		topbandRation.ration_step = m_rotateState;
		sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);

	}

	if(CVideoPage::s_zoomNum != Zoom_Normal)
	{
		usleep(5000);
		TOP_BAND_ZOOM_S topbandZoom;
		topbandZoom.zoom_step = CVideoPage::s_zoomNum;
		sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
	}

	if(strlen(m_photoName) )
	{
		remove(m_photoName);
		memset(m_photoName, 0, sizeof(m_photoName));
	}

	system("sync");

	rcrd_msg_event_req_strginfo();
	usleep(100000);

	m_recordStatus = Record_Init;
}

void CVideoPage::OnCancelCB(void *ctx)
{
	CVideoPage *pPage = (CVideoPage *)ctx;

	if (pPage)
	{
		pPage->OnCancel();
	}
}

// Cancel, cancel video save
void CVideoPage::OnCancel()
{
	DBG("[DirectFBUI-VideoPage] OnCancel\n");

	if (m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_STOP;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	usleep(5000);

	if(CVideoPage::m_rotateState !=  Rotate_Normal)
	{
		usleep(5000);
		TOP_BAND_RATION_S topbandRation;
		topbandRation.ration_step = m_rotateState;
		sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
	}

	if(CVideoPage::s_zoomNum != Zoom_Normal)
	{
		usleep(5000);
		TOP_BAND_ZOOM_S topbandZoom;
		topbandZoom.zoom_step = CVideoPage::s_zoomNum;
		sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
	}

	char *ptr = NULL;
	int index = -1;
	char szFilePath[64] = {0};
	char *pDevPath = NULL;
	CDesktop* pDesktop = CDesktop::GetDesktop();
	if( strstr(m_photoName, ".db") != NULL )
	{
		remove(m_photoName);

		ptr = strrchr(m_photoName, '/');
		if (ptr != NULL)
		{
			ptr ++;
			sscanf(ptr, "%d.db", &index);
			pDevPath = pDesktop->GetDevPath();
			sprintf(szFilePath, "%s/DCIM/REC%03d.AVI", pDevPath, index);
		}

		remove(szFilePath);
	}

	system("sync");

	rcrd_msg_event_req_strginfo();
	usleep(100000);

	m_recordStatus = Record_Init;
	UpdateRecTimeMic();
}

void CVideoPage::ShowNoStorageMsg()
{
	DFBRectangle			dfbRect;
	DFBColor fontColor;
	CDesktop*				pDesktop		= NULL;
	LiteWindow* 			liteWin 		= NULL;

	DBG("No SD Card. \n");

	pDesktop = CDesktop::GetDesktop();
	liteWin = pDesktop->GetLiTEWindow();

	tsLangNotiMsg *pNotiInfo = &g_CurLangDesc.NotiMsg[Notifiction_NoSDCard];

	// bk image
	dfbRect.x = 0;
	dfbRect.y = 0;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight;
	if( DFB_OK == lite_new_image(LITE_BOX(liteWin), &dfbRect, liteNoImageTheme, &m_noStorageMsg) )
	{
		lite_load_image(m_noStorageMsg, pNotiInfo->imgPath.c_str());
		lite_set_image_visible(m_noStorageMsg, true);
		lite_update_box(LITE_BOX(m_noStorageMsg), NULL);
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
	if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, MAIN_FONT_SIZE, &fontColor, &m_lblStorageMsg))
	{
		lite_set_label_alignment(m_lblStorageMsg, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblStorageMsg, true);
		lite_set_label_text(m_lblStorageMsg, pNotiInfo->vecMainText[0].c_str());
	}
}

void CVideoPage::ReleaseStorageMsg()
{
	SAFE_DEL_LITE(m_noStorageMsg);
	SAFE_DEL_LITE(m_lblStorageMsg);
}

void CVideoPage::UpdateFont(const char * pFontFile)
{
	char cmd[256];
	memset(cmd,0,sizeof(cmd));

	sprintf(cmd,"tar xvzf %s -C/usr/share/LiTE/share/fonts/truetype",pFontFile);

	DBG("%s\n",cmd);

	system(cmd);

	sleep(1);

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "mv %s %s.ok", pFontFile,pFontFile);

	system(cmd);
}

void CVideoPage::CheckUpdate()
{
	CDesktop*			pDesktop 		= NULL;
	LiteWindow*			liteWin 		= NULL;
	char				*pDevPath		= NULL;
	char				szUpdateFile[256];
	char				szFontFile[256];
	DFBRectangle		dfbRect;
	int					screenWidth = 0, screenHeight = 0;
	DFBColor			fontColor;

	pDesktop = CDesktop::GetDesktop();

	if (NULL == (pDevPath = pDesktop->GetDevPath()))
	{
		return;
	}

	memset(szFontFile, 0, sizeof(szFontFile));
	sprintf(szFontFile, "%s/%s", pDevPath, UPDATE_FONT_FILE);
	if (access(szFontFile, F_OK) == 0)
	{
		UpdateFont(szFontFile);
	}

	memset(szUpdateFile, 0, sizeof(szUpdateFile));
	sprintf(szUpdateFile, "%s/%s", pDevPath, UPDATE_FILE);
	if (access(szUpdateFile, F_OK) != 0)
	{
		return;
	}

	// 升级
	m_updatIng = 1;
	strcpy(m_szUpdateFile, szUpdateFile);
	sprintf(m_szUpdateFileOk, "%s/%s.ok", pDevPath, UPDATE_FILE);

	DBG("[DirectFBUI-ReviewPage] *** update begin ***\n");

	if (SUCCESS != CW_PthreadMNG_Create("UpdateThread", &m_updateThread, NULL, UpdateThreadCB, this))
	{
		DBG("[DirectFBUI-Desktop] init desktop success\n");
	}

	DBG("[DirectFBUI-Desktop] *** create message thread success ***\n");

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x		= 10;
	dfbRect.y		= (screenHeight - 10) / 2 + 24;
	dfbRect.w		= screenWidth - 2 * 10;
	dfbRect.h		= 10;

	// 升级进度条
	if (DFB_OK != lite_new_progressbar(LITE_BOX(liteWin), &dfbRect, liteNoProgressBarTheme, &m_updateBar))
	{
		DBG("[DirectFBUI-Desktop] create update progressbar error\n");
	}
	m_updateBarValue = 0;

	// 更新升级进度条
	if (DFB_OK != lite_enqueue_window_timeout(UPDATE_TOTAL_TIME * 1000 / 100, TimeOut_EveryTime, OnUpdateTimerEvent, this, &m_updateTimerID))
	{
		DBG("[DirectFBUI-Desktop] create update timer error\n");
	}

	fontColor.a		= 0xFF;
	fontColor.r		= 0xFF;
	fontColor.g		= 0xFF;
	fontColor.b		= 0xFF;

	dfbRect.h		= 24;
	dfbRect.w		= 150;
	dfbRect.x		= (screenWidth - 150) / 2;
	dfbRect.y		= (screenHeight - 10) / 2;
	if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, 24, &fontColor, &m_updateText))
	{
		DBG("[DirectFBUI-Desktop] create update text error\n");
	}

	lite_set_label_text(m_updateText, "Updating");
	lite_set_label_alignment(m_updateText, LITE_LABEL_CENTER);
}

int CVideoPage::ReadFully(int fd, void* buf, size_t nbytes)
{
	int nread;

    nread = 0;
    while ( (size_t)nread < nbytes )
    {
        int r;

        r = read( fd, (char*) buf + nread, nbytes - nread );
        if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) )
        {
        	perror("[DirectFBUI-ReviewPage] ReadFully error");
            usleep(100000);
            continue;
        }

        if ( r < 0 )
            return r;

        if ( r == 0 )
            break;

        nread += r;
    }

    return nread;
}

int CVideoPage::WriteFully(int fd, const void* buf, size_t nbytes)
{
	int nwritten;

    nwritten = 0;

    while ( (size_t)nwritten < nbytes )
    {
        int r;

        r = write( fd, (char*) buf + nwritten, nbytes - nwritten );
        if ( r < 0 && ( errno == EINTR || errno == EAGAIN ) )
        {
            usleep(100000);
            continue;
        }

        if ( r < 0 )
            return r;

        if ( r == 0 )
            break;

        nwritten += r;
    }

    return nwritten;
}

void* CVideoPage::UpdateThreadCB(void *arg)
{
	CVideoPage *pVideo = (CVideoPage *)arg;

	if (pVideo)
	{
		pVideo->DoUpdateThread();
	}

	return NULL;
}

void CVideoPage::DoUpdateThread(void)
{
	int sock = -1;
	struct sockaddr_in addr;
	int iUpdate = 8006;
	int readLen = 0, writeLen = 0;
	int updateFileFd = -1;
	char buffer[UPDATE_BLOCK_LEN];
	char sz_cmd[1024] = {0};

	printf("%s : %d : enter DoUpdateThread...\n", __FUNCTION__, __LINE__);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sock)
	{
		return;
	}

	printf("%s : %d : create scoket %d for update...\n", __FUNCTION__, __LINE__, sock);

	int retry = 15;
	for (; retry > 0; retry--)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(iUpdate);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		printf("%s : %d : start connect, retry %d\n", __FUNCTION__, __LINE__, retry);

		if(-1 == connect(sock, (struct sockaddr *)&addr, (int)sizeof(addr)))
		{
			printf("%s : %d : connect fail, retry %d\n", __FUNCTION__, __LINE__, retry);

			usleep(1000*1000);
			continue;
		}
		else
		{
			break;
		}
	}

	if (0 >= retry)
	{// 连接服务器
	    DBG("[DirectFBUI-ReviewPage] connect update inter error!\n");
		goto END;
	}

	DBG("[DirectFBUI-ReviewPage] open %s success, updateFileFd = %d\n", m_szUpdateFile, updateFileFd);

	if (-1 == (updateFileFd = open(m_szUpdateFile, O_RDONLY)))
	{
		DBG("[DirectFBUI-ReviewPage] open %s failure\n", m_szUpdateFile);
		goto END;

	}

	DBG("[DirectFBUI-ReviewPage] open %s success, updateFileFd = %d\n", m_szUpdateFile, updateFileFd);

	do
	{
		readLen = ReadFully(updateFileFd, buffer, UPDATE_BLOCK_LEN);

		if (readLen <= 0)
		{
			DBG("[DirectFBUI-ReviewPage], read %s error\n", m_szUpdateFile);
			break;
		}

		writeLen = WriteFully(sock, buffer, readLen);
		if (writeLen != readLen)
		{
			DBG("[DirectFBUI-ReviewPage], write error\n");
			break;
		}
	} while (readLen == UPDATE_BLOCK_LEN);

	sprintf(sz_cmd, "rm -rf %s;mv %s %s", m_szUpdateFileOk, m_szUpdateFile, m_szUpdateFileOk);
	system(sz_cmd);

END:
	if (sock > 0)
	{
		close(sock);
	}

	if (updateFileFd > 0)
	{
		close(updateFileFd);
	}

	DBG("[DirectFBUI-ReviewPage] update end\n");
}

DFBResult CVideoPage::OnUpdateTimerEvent(void *data)
{
	CVideoPage *pPage = (CVideoPage *)data;

	if (pPage)
	{
		pPage->UpdateTimerEvent();
	}

	return DFB_OK;
}

void CVideoPage::UpdateTimerEvent(void)
{
	if (m_updateBar == NULL)
	{
		return;
	}

	m_updateBarValue += 0.01;
	m_updateBarValue = (m_updateBarValue > 1) ? 1 : m_updateBarValue;

	lite_set_progressbar_value(m_updateBar, m_updateBarValue);
}

//------------------------------------------------------
void CVideoPage::UpdateMsg()
{
	DBG("UpdateMsg\n");

	if( m_noStorageMsg != NULL )
	{
		ReleaseStorageMsg();
	}
}

void CVideoPage::HideModalDlg()
{
	if( m_modalDialog )
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}
}

void CVideoPage::OnUpdateArrow()
{
	if( m_iRotateLeft == ARROW_RED || \
		m_iRotateRight == ARROW_RED || \
		m_iZoomUp == ARROW_RED || \
		m_iZoomDown == ARROW_RED )
	{
		m_iRotateLeft = ARROW_NORMAL;
		m_iRotateRight = ARROW_NORMAL;

		m_iZoomUp = ARROW_NORMAL;
		m_iZoomDown = ARROW_NORMAL;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}





