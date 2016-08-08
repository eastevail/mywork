/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: microphone.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.16
******************************************************************************/

#include "desktop.h"
#include "microphone.h"
#include "setting.h"

#include "sys_msg_drv.h"

CMicroPhonePage::CMicroPhonePage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_modalDialog = NULL;
}

CMicroPhonePage::~CMicroPhonePage()
{
}

void CMicroPhonePage::OnPageInit()
{
	DFBRectangle 			dfbRect;
	CSettingPage*			pSetting = NULL;

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= g_RunParam.screenWidth;
	dfbRect.h			= g_RunParam.screenHeight - TOP_LABEL_H;

	m_modalDialog 		= new CModalDialog(dfbRect);

	m_modalDialog->SetOKCB(g_CurLangDesc.Public.Off.c_str(), OnCancelCB, this); // OK->Off
	m_modalDialog->SetCancelCB(g_CurLangDesc.Public.On.c_str(), OnSaveCB, this); // Cancel->On

	if( g_RunParam.tuobangPara.bMicroPhone == 0 )
		m_modalDialog->SetCurSel(Dialog_OK);
	else
		m_modalDialog->SetCurSel(Dialog_Cancel);

	m_modalDialog->setDailogText(g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message.c_str(), NULL);

	m_modalDialog->ShowDialog();

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		pSetting->ShowTitleText(g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title.c_str());
	}
}

void CMicroPhonePage::OnPageExit()
{
	if (m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}
}

void CMicroPhonePage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	if (dfbEvent->type == DWET_KEYDOWN)
	{
		switch (dfbEvent->key_code)
		{
			case LEFT_BTN:
			case RIGHT_BTN:
			case OK_BTN:
			{
				if (m_modalDialog)
				{
					m_modalDialog->OnKeyEvent(dfbEvent);
				}
				break;
			}

			case UP_BTN:
			case DOWN_BTN:
			{
				// do nothing
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
				DBG("[DirectFBUI-MicroPhonePage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CMicroPhonePage::JumpToParentPage()
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

void CMicroPhonePage::OnSaveCB(void *ctx)
{
	CMicroPhonePage *pPage = (CMicroPhonePage *)ctx;

	if (pPage)
	{
		pPage->OnSave();
	}
}

void CMicroPhonePage::OnSave()
{
	g_RunParam.tuobangPara.bMicroPhone	= 1;
	sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

	DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);

	sys_msg_event_chng_cfg(MSG_TYPE_RECORD_SVR, SYS_MSG_CMD_GET_AUDIO, 0, 0);

	JumpToParentPage();
}

void CMicroPhonePage::OnCancelCB(void *ctx)
{
	CMicroPhonePage *pPage = (CMicroPhonePage *)ctx;

	if (pPage)
	{
		pPage->OnCancel();
	}
}

void CMicroPhonePage::OnCancel()
{
	g_RunParam.tuobangPara.bMicroPhone = 0;
	sys_msg_cmd_set_tuobang(&g_RunParam.tuobangPara);

	DBG("tuobangPara, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
			g_RunParam.tuobangPara.bMicroPhone, \
			(int)g_RunParam.tuobangPara.brightNess, \
			g_RunParam.tuobangPara.lang, \
			g_RunParam.tuobangPara.TimeFmt);

	sys_msg_event_chng_cfg(MSG_TYPE_RECORD_SVR, SYS_MSG_CMD_GET_AUDIO, 0, 0);


	JumpToParentPage();
}



