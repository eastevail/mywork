/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: del_page.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include "del_page.h"
#include "desktop.h"
#include "setting.h"
#include <sys/stat.h>
#include <sys/vfs.h>

CDeletePage::CDeletePage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_modalDialog = NULL;
}

CDeletePage::~CDeletePage()
{

}

void CDeletePage::OnPageInit()
{
	DFBRectangle 			dfbRect;
	char					szTextBuf[MAX_LANG_LEN];
	char					szTextBuf2[MAX_LANG_LEN];
	CSettingPage*			pSetting = NULL;

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H;
	dfbRect.w			= g_RunParam.screenWidth;
	dfbRect.h			= g_RunParam.screenHeight - TOP_LABEL_H;

	m_modalDialog 		= new CModalDialog(dfbRect);

	m_modalDialog->SetOKCB(g_CurLangDesc.Public.Cancel.c_str(), OnCancelCB, this);
	m_modalDialog->SetCancelCB(g_CurLangDesc.Public.DeleteAll.c_str(), OnDeleteAllCB, this);

	m_modalDialog->setDailogText(g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1.c_str(), \
								 g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2.c_str());

	m_modalDialog->ShowDialog();

	if (HasParentPage())
	{
		pSetting = dynamic_cast<CSettingPage *>(GetParentPage());
		pSetting->ShowTitleText(g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title.c_str());
	}
}

void CDeletePage::OnPageExit()
{
	if (m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}
}

void CDeletePage::OnKeyEvent(DFBWindowEvent *dfbEvent)
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
				JumpToParentPage();
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
				DBG("[DirectFBUI-DelPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
				break;
			}
		}
	}
}

void CDeletePage::JumpToParentPage()
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

void CDeletePage::OnDeleteAllCB(void *ctx)
{
	CDeletePage *pPage = (CDeletePage *)ctx;

	if (pPage)
	{
		pPage->OnDeleteAll();
	}
}

void CDeletePage::DeleteAllWaitingCB(void *ctx)
{
	CDeletePage *pPage = (CDeletePage *)ctx;

	if (pPage)
	{
		system("sync");
		DBG("Delete over\n");
		pPage->JumpToParentPage();
	}
}

void CDeletePage::OnDeleteAll()
{
	// 删除SD卡上的所有内容
	char szCmd[128];
	CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
	const char	*pDevPath	= NULL;
	unsigned int time = 1;

	pDevPath = pDesktop->GetDevPath();

	struct statfs stfs;
	int ret;
	int free,total;
	int res;

	ret = statfs(pDevPath, &stfs);

	if(ret < 0){

	  DBG("[DirectFBUI-DelPage] NO CARD!!\n");
	  return ;
	}

	free = ((stfs.f_bavail >> 10) * stfs.f_bsize) >> 10; // m
	total = ((stfs.f_blocks >> 10) * stfs.f_bsize) >> 10; // m
	res = total - free;


	DBG("seartch files [%d/%d]... \n", res,total);

	if(res <= 10)
	{
		time =  2;
	}
	else if(res > 10 && res <= 100)
	{
		time = ((float)res/total)*10 + 3;
	}
	else if(res > 100 && res <= 1000)
	{
		time = ((float)res/total)*10 + 5;
	}
	else
	{
		time = ((float)res/total)*10 + 10;
	}

	time += 4;
	DBG("wait time %d s... \n", time);

	memset(szCmd, 0, sizeof(szCmd));
#if 0 // xjf, for debug
	sleep(2);
#else
	sprintf(szCmd, "rm -fr %s/*dcim*", pDevPath);
#endif

	system(szCmd);

	memset(szCmd, 0, sizeof(szCmd));
#if 0 // xjf, for debug
	sleep(2);
#else
	sprintf(szCmd, "rm -fr %s/.thumbs", pDevPath);
#endif
	system(szCmd);

	DBG("[DirectFBUI-DelPage] %s; errno = %d\n", szCmd,errno);
	//ret = remove(pDevPath);

	//DBG("[DirectFBUI-DelPage] return remove = %d ; errno = %d\n", ret,errno);

	DEVICE_NAME_MSG data;
	strcpy(data.devname, pDevPath);

	rcrd_msg_event_req_format(&data);

	CDesktop::GetDesktop()->waitingOperate(time*1000,DeleteAllWaitingCB,this);
}

void CDeletePage::OnCancelCB(void *ctx)
{
	CDeletePage *pPage = (CDeletePage *)ctx;

	if (pPage)
	{
		pPage->OnCancel();
	}
}

void CDeletePage::OnCancel()
{
	// do nothing

	JumpToParentPage();
}



