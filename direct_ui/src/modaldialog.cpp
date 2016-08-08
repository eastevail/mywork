/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: modaldialog.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include "desktop.h"
#include "modaldialog.h"

#define DLG_MAIN_FONT	22

CModalDialog::CModalDialog(DFBRectangle &dfbRect, CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_rect = dfbRect;

	m_okCB = NULL;
	m_okCBData = NULL;
	m_okButton = NULL;
	m_cancelCB = NULL;
	m_cancelCBData = NULL;
	m_cancelButton = NULL;
	m_dialogBg = NULL;
	m_dialogStatus = Dialog_OK;
	m_txtLine1 = NULL ;
	m_txtLine2 = NULL;

	m_pLineText1 = NULL ;
	m_pLineText2 = NULL ;

	m_bgColor.a = DIALOG_BG_COLOR_A;
	m_bgColor.r = DIALOG_BG_COLOR_R;
	m_bgColor.g = DIALOG_BG_COLOR_G;
	m_bgColor.b = DIALOG_BG_COLOR_B;


	m_txtTime = NULL;
	m_pTimeText = NULL;
	//m_micImage = NULL;

	m_bgImage = NULL;

	m_isPressOK = 0;

	memset(m_okText, 0, DIALOG_TEXT_LEN);
	memset(m_cancelText, 0, DIALOG_TEXT_LEN);
	memset(m_bgimagePath, 0, DIALOG_TEXT_LEN);
}

CModalDialog::~CModalDialog()
{
	SAFE_DEL_LITE(m_dialogBg);
	SAFE_DEL_LITE(m_okButton);
	SAFE_DEL_LITE(m_cancelButton);
	SAFE_DEL_LITE(m_txtLine1);
	SAFE_DEL_LITE(m_txtLine2);
	SAFE_DEL_LITE(m_txtTime);
	//SAFE_DEL_LITE(m_micImage);
	SAFE_DEL_LITE(m_bgImage);

	m_isPressOK = 0;

	if(m_pLineText1 != NULL)
		free(m_pLineText1);

	if(m_pLineText2 != NULL)
		free(m_pLineText2);

	if(m_pTimeText != NULL)
		free(m_pTimeText);
}

void CModalDialog::setDailogBackground(int r,int g,int b)
{
	m_bgColor.r = r;
	m_bgColor.g = g;
	m_bgColor.b = b;
}

void CModalDialog::setDailogBackground(char *filePath)
{
	if( filePath )
	{
		strcpy(m_bgimagePath, filePath);
	}
}

void CModalDialog::setRecordTime(char *sztimetxt)
{
	if(sztimetxt != NULL){
		m_pTimeText = (char*)malloc(sizeof(char)*DIALOG_TEXT_LEN);
		strcpy(m_pTimeText,sztimetxt);

		DBG("%s\n",m_pTimeText);
	}
}

void CModalDialog::setDailogText(char *szLine1,char *szLine2)
{
	if(szLine1 != NULL){
		m_pLineText1 = (char*)malloc(sizeof(char)*DIALOG_TEXT_LEN);
		strcpy(m_pLineText1,szLine1);
	}

	if(szLine2 != NULL){
		m_pLineText2 = (char*)malloc(sizeof(char)*DIALOG_TEXT_LEN);
		strcpy(m_pLineText2,szLine2);
	}
}

void CModalDialog::SetOKCB(char *szText, DialogBtnClick okCB, void *data)
{
	strncpy(m_okText, szText, DIALOG_TEXT_LEN - 1);
	m_okCB 			= okCB;
	m_okCBData 		= data;
}

void CModalDialog::SetCancelCB(char *szText, DialogBtnClick cancelCB, void *data)
{
	strncpy(m_cancelText, szText, DIALOG_TEXT_LEN - 1);
	m_cancelCB 		= cancelCB;
	m_cancelCBData	= data;
}

void CModalDialog::ShowDialog()
{
	CDesktop*		pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*		liteWin 	= pDesktop->GetLiTEWindow();
	LiteLabelTheme	theme;
	DFBColor		bgColor;

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-ModalDialog] lite window must init first\n");
		return;
	}

	memset(&theme, 0, sizeof(LiteLabelTheme));

	if (m_dialogBg == NULL && strlen(m_bgimagePath)== 0)
	{
		theme.theme.bg_color 	= m_bgColor;
		DBG("RGB corlor[%x %x %x]\n",	m_bgColor.r	,m_bgColor.g ,m_bgColor.b );

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &m_rect, &theme, 12, NULL, &m_dialogBg))
		{
			DBG("[DirectFBUI-ModalDialog] create dialog background label failure\n");
			return;
		}

		DBG("[DirectFBUI-ModalDialog] create dialog background label at (%d %d %d %d) success\n", m_rect.x, m_rect.y, m_rect.w, m_rect.h);
	}

	DBG("[DirectFBUI-ModalDialog] create background image  %d:%p\n",strlen(m_bgimagePath),m_bgImage);

	//if(strlen(m_bgimagePath) > 0 && m_bgImage == NULL)
	if(strlen(m_bgimagePath) > 0 )
	{

		int picWidth;
		int picHeight;

		if(CDesktop::GetDesktop()->GetJPEGFileSize(m_bgimagePath,&picWidth,&picHeight) == TRUE)
		{
			TOP_BAND_PIC_S pic_info;

		#if 1
			TOP_BAND_ZOOM_S topbandZoom;
			topbandZoom.zoom_step = Zoom_Normal;
			sys_msg_cmd_top_band_zoom(&topbandZoom, MSG_TYPE_ENCODE_SVR);
		#endif

			pic_info.pic_width = picWidth;
			pic_info.pic_height = picHeight;
			strcpy(pic_info.pic_name,m_bgimagePath);

			DBG("pic_width = %d,pic_height=%d\n",pic_info.pic_width,pic_info.pic_height);
			sys_msg_cmd_top_band_pic(&pic_info, MSG_TYPE_ENCODE_SVR); // display snap image & thumb image
		}
	}

	//show line
	if(m_pLineText1 != NULL)
	{
		DFBColor		fontColor;
		DFBRectangle	rect;

		rect.x 					= 0;
		rect.y 					= m_rect.h/2 - DIALOG_TEXT_LINE_H ;
		rect.w 					= m_rect.w;
		rect.h 					= DIALOG_TEXT_LINE_H;

		fontColor.a				= DIALOG_BTN_TEXT_COLOR_A;
		fontColor.r				= DIALOG_BTN_TEXT_COLOR_R;
		fontColor.g				= DIALOG_BTN_TEXT_COLOR_G;
		fontColor.b				= DIALOG_BTN_TEXT_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &rect, &theme, DLG_MAIN_FONT, &fontColor, &m_txtLine1))
		{
			DBG("[DirectFBUI-ModalDialog] create ok button label failure\n");
			return;
		}

		lite_set_label_text(m_txtLine1, m_pLineText1);
		lite_set_label_alignment(m_txtLine1, LITE_LABEL_CENTER);
	}

	if(m_pLineText2 != NULL)
	{
		DFBColor		fontColor;
		DFBRectangle	rect;

		rect.x 					= 0;
		rect.y 					= m_rect.h/2 ;
		rect.w 					= m_rect.w;
		rect.h 					= DIALOG_TEXT_LINE_H;

		fontColor.a				= DIALOG_BTN_TEXT_COLOR_A;
		fontColor.r				= DIALOG_BTN_TEXT_COLOR_R;
		fontColor.g				= DIALOG_BTN_TEXT_COLOR_G;
		fontColor.b				= DIALOG_BTN_TEXT_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &rect, &theme, DLG_MAIN_FONT, &fontColor, &m_txtLine2))
		{
			DBG("[DirectFBUI-ModalDialog] create ok button label failure\n");
			return;
		}

		lite_set_label_text(m_txtLine2, m_pLineText2);
		lite_set_label_alignment(m_txtLine2, LITE_LABEL_CENTER);
	}

	//show record Time
	if(m_pTimeText != NULL)
	{
		DFBColor		fontColor;
		DFBRectangle	rect;

		rect.x					= 29;
		rect.y					= 38 ;
		rect.w					= RECORD_TIME_TEXT_W;
		rect.h					= RECORD_TIME_TEXT_H;

		fontColor.a			= RECORD_TIME_COLOR_A;
		fontColor.r			= RECORD_TIME_COLOR_R;
		fontColor.g			= RECORD_TIME_COLOR_G;
		fontColor.b			= RECORD_TIME_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &rect, &theme, RECORD_TIME_FONT_SIZE, &fontColor, &m_txtTime))
		{
			DBG("[DirectFBUI-ModalDialog] create ok button label failure\n");
			return;
		}

		lite_set_label_text(m_txtTime, m_pTimeText);
		lite_set_label_alignment(m_txtTime, LITE_LABEL_LEFT);
	}

	if (m_dialogStatus == Dialog_OK)
	{
		ShowOKBtn(TRUE, TRUE);
		ShowCancelBtn(TRUE, FALSE);
	}
	else if (m_dialogStatus == Dialog_Cancel)
	{
		ShowOKBtn(TRUE, FALSE);
		ShowCancelBtn(TRUE, TRUE);
	}

	lite_update_box(LITE_BOX(m_dialogBg), NULL);
}

void CModalDialog::setKeyHide()
{
	if(m_okButton)
	{
		lite_set_label_set_visible(m_okButton, FALSE);
	}

	if(m_cancelButton)
	{
		lite_set_label_set_visible(m_cancelButton, FALSE);
	}

	if( m_txtTime )
	{
		lite_set_label_set_visible(m_txtTime, FALSE);
		//lite_update_box(LITE_BOX(m_txtTime), NULL);
	}
}

void CModalDialog::OnPageInit()
{
	// ºöÂÔ
}

void CModalDialog::OnPageExit()
{
	// ºöÂÔ
}

void CModalDialog::OnKeyEvent(DFBWindowEvent *dfbEvent)
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
				m_isPressOK++;
				OnOKKeyEvent();
				break;
			}

			case SETTING_BTN:
			{
				OnSettingKeyEvent();
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
		}
	}
}

void CModalDialog::OnLeftKeyEvent(void)
{
	ToggleBtn();
}

void CModalDialog::OnRightKeyEvent(void)
{
	ToggleBtn();
}

void CModalDialog::OnUpKeyEvent(void)
{
}

void CModalDialog::OnDownKeyEvent(void)
{
}

void CModalDialog::OnOKKeyEvent(void)
{
	if( m_isPressOK > 1 )
		return ;

	if (m_dialogStatus == Dialog_OK)
	{
		if (m_okCB)
		{
			m_okCB(m_okCBData);
		}
	}
	else if (m_dialogStatus == Dialog_Cancel)
	{
		if (m_cancelCB)
		{
			m_cancelCB(m_cancelCBData);
		}
	}
}

void CModalDialog::OnSettingKeyEvent(void)
{
}

void CModalDialog::OnReplayKeyEvent(void)
{
}

void CModalDialog::OnVideoCaptureKeyEvent(void)
{
}

void CModalDialog::ShowOKBtn(BOOL bVisible, BOOL bActive)
{
	CDesktop*		pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*		liteWin 	= pDesktop->GetLiTEWindow();
	LiteLabelTheme	theme;
	DFBColor		bgColor;
	DFBColor		fontColor;
	DFBRectangle	rect;

	if (bActive)
	{
		bgColor.a 			= DIALOG_BTN_ACTIIVE_COLOR_A;
		bgColor.r 			= DIALOG_BTN_ACTIIVE_COLOR_R;
		bgColor.g 			= DIALOG_BTN_ACTIIVE_COLOR_G;
		bgColor.b 			= DIALOG_BTN_ACTIIVE_COLOR_B;
		m_dialogStatus		= Dialog_OK;
	}
	else
	{
		bgColor.a 			= DIALOG_BTN_BG_COLOR_A;
		bgColor.r 			= DIALOG_BTN_BG_COLOR_R;
		bgColor.g 			= DIALOG_BTN_BG_COLOR_G;
		bgColor.b 			= DIALOG_BTN_BG_COLOR_B;
	}

	if (m_okButton == NULL)
	{
		memset(&theme, 0, sizeof(LiteLabelTheme));
		rect.x 					= BTN_SPACE;
		rect.y 					= m_rect.h - DIALOG_BUTTON_H - BTN_BOTTOM_SPACE;
		rect.w 					= DIALOG_BUTTON_W;
		rect.h 					= DIALOG_BUTTON_H;

		theme.theme.bg_color	= bgColor;

		fontColor.a				= DIALOG_BTN_TEXT_COLOR_A;
		fontColor.r				= DIALOG_BTN_TEXT_COLOR_R;
		fontColor.g				= DIALOG_BTN_TEXT_COLOR_G;
		fontColor.b				= DIALOG_BTN_TEXT_COLOR_B;
		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &rect, &theme, DIALOG_BTN_TEXT_SIZE, &fontColor, &m_okButton))
		{
			DBG("[DirectFBUI-ModalDialog] create ok button label failure\n");
			return;
		}

		lite_set_label_text(m_okButton, m_okText);
		lite_set_label_alignment(m_okButton, LITE_LABEL_CENTER);

		DBG("[DirectFBUI-ModalDialog] create ok button label at (%d %d %d %d) success\n", rect.x, rect.y, rect.w, rect.h);
	}
	else
	{
		lite_set_label_set_visible(m_okButton, bVisible);

		lite_set_label_bgcolor(m_okButton, &bgColor);
	}
}

void CModalDialog::ShowCancelBtn(BOOL bVisible, BOOL bActive)
{
	CDesktop*		pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*		liteWin 	= pDesktop->GetLiTEWindow();
	LiteLabelTheme	theme;
	DFBColor		bgColor;
	DFBColor		fontColor;
	DFBRectangle	rect;
	int				width = 0;

	if (bActive)
	{
		bgColor.a 			= DIALOG_BTN_ACTIIVE_COLOR_A;
		bgColor.r 			= DIALOG_BTN_ACTIIVE_COLOR_R;
		bgColor.g 			= DIALOG_BTN_ACTIIVE_COLOR_G;
		bgColor.b 			= DIALOG_BTN_ACTIIVE_COLOR_B;
		m_dialogStatus		= Dialog_Cancel;
	}
	else
	{
		bgColor.a 			= DIALOG_BTN_BG_COLOR_A;
		bgColor.r 			= DIALOG_BTN_BG_COLOR_R;
		bgColor.g 			= DIALOG_BTN_BG_COLOR_G;
		bgColor.b 			= DIALOG_BTN_BG_COLOR_B;
	}

	if (m_cancelButton == NULL)
	{
		memset(&theme, 0, sizeof(LiteLabelTheme));
		rect.x 		= DIALOG_BUTTON_W + BTN_SPACE +9;
		rect.y 		= m_rect.h - DIALOG_BUTTON_H - BTN_BOTTOM_SPACE;
		rect.w 		= DIALOG_BUTTON_W;
		rect.h 		= DIALOG_BUTTON_H;

		theme.theme.bg_color	= bgColor;

		fontColor.a				= DIALOG_BTN_TEXT_COLOR_A;
		fontColor.r				= DIALOG_BTN_TEXT_COLOR_R;
		fontColor.g				= DIALOG_BTN_TEXT_COLOR_G;
		fontColor.b				= DIALOG_BTN_TEXT_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(liteWin), &rect, &theme, DIALOG_BTN_TEXT_SIZE, &fontColor, &m_cancelButton))
		{
			DBG("[DirectFBUI-ModalDialog] create cancel button label failure\n");
			return;
		}

		lite_set_label_text(m_cancelButton, m_cancelText);
		lite_set_label_alignment(m_cancelButton, LITE_LABEL_CENTER);

		DBG("[DirectFBUI-ModalDialog] create cancel button label at (%d %d %d %d) success\n", rect.x, rect.y, rect.w, rect.h);
	}
	else
	{
		lite_set_label_set_visible(m_cancelButton, bVisible);

		lite_set_label_bgcolor(m_cancelButton, &bgColor);
	}
}

void CModalDialog::ToggleBtn(void)
{
	if (m_dialogStatus == Dialog_OK)
	{
		ShowOKBtn(TRUE, FALSE);
		ShowCancelBtn(TRUE, TRUE);
		m_dialogStatus = Dialog_Cancel;
	}
	else if (m_dialogStatus == Dialog_Cancel)
	{
		ShowOKBtn(TRUE, TRUE);
		ShowCancelBtn(TRUE, FALSE);
		m_dialogStatus = Dialog_OK;
	}
}

void CModalDialog::SetCurSel(int sel)
{
	m_dialogStatus = sel;
}



