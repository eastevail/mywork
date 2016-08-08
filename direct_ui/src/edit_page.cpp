/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: play_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.09
	Description	:
	History		:
					Create by hul.2015.06.01
******************************************************************************/
#include <string.h>

#include "edit_page.h"
#include "desktop.h"
#include "review.h"

CEditPage::CEditPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	memset(m_fileToEdit, 0, MAX_FILE_LEN);

	m_topLabel = NULL;
	m_FileImage = NULL;

	m_ZoomDegree = Zoom_Normal;
	m_RotateDegree = Rotate_Normal;

	m_ReviewImage = NULL;
	m_pFontBoldInterface = NULL;

	m_iRotateLeft = ARROW_NORMAL;
	m_iRotateRight = ARROW_NORMAL;
	m_iZoomUp = ARROW_NORMAL;
	m_iZoomDown = ARROW_NORMAL;
}

CEditPage::~CEditPage()
{
}

void CEditPage::OnPageInit()
{
	CReviewPage*				pReviewPage 	= NULL;
	DFBRectangle				dfbRect;
	DFBColor					bgColor;
	CDesktop*					pDesktop 		= NULL;
	LiteWindow*					liteWin 		= NULL;
	int							picWidth		= 0;
	int							picHeight	= 0;
	TOP_BAND_PIC_S pic_info;

	if (HasParentPage())
	{
		pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
		pReviewPage->GetCurPlayFile(m_fileToEdit, MAX_FILE_LEN);
	}

	DBG("CEditPage::OnPageInit +++++++++  %s\n", m_fileToEdit);

#if 1
	TOP_BAND_ZOOM_S topbandZoom;
	topbandZoom.zoom_step = Zoom_Normal;
	sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);

	TOP_BAND_RATION_S topbandRation;
	topbandRation.ration_step = Rotate_Normal;
	sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);
#endif

	InitSubStrFont();

	if(pReviewPage->GetJPEGFileSize(m_fileToEdit,&picWidth,&picHeight) == TRUE)
	{
		pic_info.pic_width = picWidth;
		pic_info.pic_height = picHeight;
		strcpy(pic_info.pic_name, m_fileToEdit);
		sys_msg_cmd_top_band_pic(&pic_info,MSG_TYPE_ENCODE_SVR);
	}

	AfterDrawFileImage();
}

void CEditPage::OnPageExit()
{
	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_ReviewImage);
	SAFE_DEL_LITE(m_FileImage);

	DestorySubStrFont();
}

BOOL CEditPage::InitSubStrFont()
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

void CEditPage::DestorySubStrFont()
{
	if (DFB_OK != lite_release_font(m_pLiteFont))
	{
		DBG("[DirectFBUI-SettingPage] release lite font failure\n");
	}

	m_pLiteFont = NULL;
	m_pFontBoldInterface = NULL;
}

void CEditPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (dfbEvent->type == DWET_KEYDOWN)
	{
		DBG("DirectFB_UI-PlayPage: OnKeyEvent %d\n", dfbEvent->key_code);

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

			case SETTING_BTN:
			{
				OnSettingKeyEvent();
				break;
			}

			case OK_BTN:
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
				DBG("[DirectFBUI-PlayPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CEditPage::OnWindowEvent(Window_Msg_S *winMsg)
{
	DBG("[DirectFBUI-PlayPage] msgType = %d\n", winMsg->msgType);

	switch( winMsg->msgType )
	{
		case MSG_RECORD_STOP: // shut down
		{
			SAFE_DEL_LITE(m_topLabel);
			SAFE_DEL_LITE(m_ReviewImage);
			SAFE_DEL_LITE(m_FileImage);

			break;
		}
		default:
		{
			break;
		}
	}
}

void CEditPage::AfterDrawFileImageCB(void *data)
{
	CEditPage *pPage = (CEditPage *)data;

	if (pPage)
	{
		pPage->AfterDrawFileImage();
	}
}

void CEditPage::AfterDrawFileImage(void)
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
}

void CEditPage::AfterDrawTopLabelCB(void *data)
{
	CEditPage *pPage = (CEditPage *)data;

	if (pPage)
	{
		pPage->AfterDrawTopLabel();
	}
}

void CEditPage::AfterDrawTopLabel(void)
{
	if (m_topLabel == NULL)
	{
		return;
	}

	LiteBox *liteBox = LITE_BOX(m_topLabel);
	IDirectFBSurface *surface = liteBox->surface;
	//IDirectFBFont *pFontOrg = NULL;
	DFBRectangle dfbRect;
	DFBPoint p1, p2, p3;
	int xx, yy, fontWidth = 0;
	int left, width, height;
	char *pItem = NULL;
	char szZoomText[8];

	if(m_ReviewImage == NULL)
	{
		dfbRect.x			= PLAY_REVIEW_IMG_X;
		dfbRect.y			= PLAY_REVIEW_IMG_Y;
		dfbRect.w			= PLAY_REVIEW_IMG_W;
		dfbRect.h			= PLAY_REVIEW_IMG_H;

		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"PLAY_REVIEW_FILE, &m_ReviewImage))
		{
			return FALSE;
		}
	}

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
	xx = ZOOM_TXT_IMG_X;
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
	if (m_ZoomDegree != 0)
	{
		sprintf(szZoomText, "%dX", m_ZoomDegree+1);

		xx = p2.x + 4;
		yy = ROTATE_TXT_IMG_Y;
		surface->DrawString(surface, szZoomText, -1, xx, yy, DSTF_TOP);
	}

	// Restore font
	//surface->SetFont(surface, pFontOrg);
}

void CEditPage::OnLeftKeyEvent(void)
{
	DBG("L:  m_RotateDegree = %d\n",m_RotateDegree);

	DBG("send msg to\n");
	Rotate_E oldRotate = m_RotateDegree;
	TOP_BAND_RATION_S topbandRation;
	int rotateNum = 0;

	rotateNum = (int)m_RotateDegree;
	rotateNum--;

	rotateNum = (rotateNum > (int)Rotate_270) ? (int)Rotate_Normal : rotateNum;
	rotateNum = (rotateNum < (int)Rotate_Normal) ? (int)Rotate_270 : rotateNum;
	m_RotateDegree = (Rotate_E)rotateNum;

	if (oldRotate != m_RotateDegree)
	{
		topbandRation.ration_step = -1;
		sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);

		m_iRotateLeft = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CEditPage::OnRightKeyEvent(void)
{
	DBG("R:  m_RotateDegree = %d\n",m_RotateDegree);

	Rotate_E oldRotate = m_RotateDegree;
	TOP_BAND_RATION_S topbandRation;
	int rotateNum = 0;

	rotateNum = (int)m_RotateDegree;
	rotateNum++;

	rotateNum = (rotateNum > (int)Rotate_270) ? (int)Rotate_Normal : rotateNum;
	rotateNum = (rotateNum < (int)Rotate_Normal) ? (int)Rotate_270 : rotateNum;
	m_RotateDegree = (Rotate_E)rotateNum;

	if (oldRotate != m_RotateDegree)
	{
		topbandRation.ration_step = 1;
		sys_msg_cmd_top_band_ration(&topbandRation, MSG_TYPE_ENCODE_SVR);

		m_iRotateRight = ARROW_RED;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CEditPage::OnUpKeyEvent(void)
{
	int multiNum = 0;
	int oldZoom = m_ZoomDegree;
	TOP_BAND_ZOOM_S topbandZoom;

	multiNum = m_ZoomDegree;
	multiNum ++;
	multiNum = (multiNum > (int)Zoom_X4) ? (int)Zoom_X4 : multiNum;
	multiNum = (multiNum < (int)Zoom_Normal) ? (int)Zoom_Normal : multiNum;
	m_ZoomDegree = multiNum;

	if (oldZoom != m_ZoomDegree)
	{
		// 发消息给encode模块执行放大
		DBG("[DirectFBUI-CEditPage] zoomNum = %d\n", m_ZoomDegree);
		//topbandZoom.zoom_step = 1;
		topbandZoom.zoom_step = m_ZoomDegree;
		sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);

		m_iZoomUp = ARROW_RED;

		DBG("U:  m_ZoomDegree = %d\n", m_ZoomDegree);
		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CEditPage::OnDownKeyEvent(void)
{
	int multiNum = 0;
	int oldZoom = m_ZoomDegree;
	TOP_BAND_ZOOM_S topbandZoom;

	multiNum = m_ZoomDegree;
	multiNum --;
	multiNum = (multiNum > (int)Zoom_X4) ? (int)Zoom_X4 : multiNum;
	multiNum = (multiNum < (int)Zoom_Normal) ? (int)Zoom_Normal : multiNum;
	m_ZoomDegree = (Zoom_E)multiNum;

	if (oldZoom != m_ZoomDegree)
	{
		DBG("[DirectFBUI-VideoPage] zoomNum = %d\n", m_ZoomDegree);
		//topbandZoom.zoom_step = -1;
		topbandZoom.zoom_step = m_ZoomDegree;
		sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);

		m_iZoomDown = ARROW_RED;

		DBG("D:  m_ZoomDegree = %d\n", m_ZoomDegree);
		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}

void CEditPage::OnOKKeyEvent(void)
{

}

void CEditPage::OnSettingKeyEvent(void)
{
#if 0 // xjf, for test
	Window_Msg_S winMsg;
	CDesktop *pDesktop = CDesktop::GetDesktop();

	winMsg.msgType = MSG_RECORD_STOP;
	pDesktop->PostWindowMessage(&winMsg);
#endif

}

void CEditPage::OnReplayKeyEvent(void)
{
	CReviewPage *pReviewPage = NULL;

	pReviewPage = dynamic_cast<CReviewPage *>(GetParentPage());
	pReviewPage->Recover();
}

void CEditPage::OnVideoCaptureKeyEvent(void)
{

}

//------------------------------------------------------
void CEditPage::OnUpdateArrow()
{
	if( m_iRotateLeft == ARROW_RED || \
		m_iRotateRight == ARROW_RED || \
		m_iZoomUp == ARROW_RED || \
		m_iZoomDown == ARROW_RED )
	{
		DBG("OnUpdateArrow\n");

		m_iRotateLeft = ARROW_NORMAL;
		m_iRotateRight = ARROW_NORMAL;

		m_iZoomUp = ARROW_NORMAL;
		m_iZoomDown = ARROW_NORMAL;

		lite_update_box(LITE_BOX(m_topLabel), NULL);
	}
}


