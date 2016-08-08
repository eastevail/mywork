/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: abspage.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	: 
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include "abspage.h"
#include "desktop.h"
#include "common.h"

#include "sys_msg_drv.h"

CAbsPage::CAbsPage(CAbsPage *parentPage)
{
	m_parentPage = parentPage;
}

CAbsPage::~CAbsPage()
{
}

BOOL CAbsPage::HasParentPage()
{
	if (m_parentPage == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

CAbsPage* CAbsPage::GetParentPage()
{
	return m_parentPage;
}

BOOL CAbsPage::SendJCPCmd(const char *szJcpCmd)
{
	if (szJcpCmd == NULL)
	{
		return FAILURE;
	}

	if (FAILURE == sys_msg_cmd_get_jcp(szJcpCmd, szJcpCmd, strlen(szJcpCmd), UI_SSID, 0))
	{
		return FAILURE;
	}

	return SUCCESS;
}

DFBResult CAbsPage::CreateLiTEImage(DFBRectangle *dfbRect, const char *szImageFile, LiteImage **liteImage)
{
	CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
	LiteWindow 	*liteWin 	= pDesktop->GetLiTEWindow();

	if ((szImageFile == NULL) || (dfbRect == NULL))
	{
		DBG("  paramter must not null\n");
		return DFB_INVARG;
	}
	
	if (liteWin == NULL)
	{
		DBG("  lite window must init first\n");
		return DFB_INVARG;
	}

	if (DFB_OK != lite_new_image(LITE_BOX(liteWin), dfbRect, liteNoImageTheme, liteImage))
	{
		DBG("  create image fail\n");
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(*liteImage, szImageFile))
	{
		DBG("  load image file %s failed\n", szImageFile);
		return DFB_FAILURE;
	}

	DBG("  load image file %s at[%d %d %d %d] success\n", 
			szImageFile, dfbRect->x, dfbRect->y, dfbRect->w, dfbRect->h);
	
	return DFB_OK;
}

DFBResult CAbsPage::CreateLiTELabel(DFBRectangle *dfbRect, DFBColor *dfbBgColor, int fontSize, DFBColor *dfbFontColor, LiteLabel **liteLabel)
{
	if (dfbRect == NULL)
	{
		DBG("  paramter must not null\n");
		return DFB_INVARG;
	}

	CDesktop*		pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*		liteWin 	= pDesktop->GetLiTEWindow();
	LiteLabelTheme	theme;

	if (liteWin == NULL)
	{
		DBG("  lite window must init first\n");
		return DFB_INVARG;
	}

	memset(&theme, 0, sizeof(LiteLabelTheme));


	if (dfbBgColor == NULL)
	{
		lite_new_label(LITE_BOX(liteWin), dfbRect, NULL, fontSize, dfbFontColor, liteLabel);
	}
	else
	{
		theme.theme.bg_color = *dfbBgColor;
		lite_new_label(LITE_BOX(liteWin), dfbRect, &theme, fontSize, dfbFontColor, liteLabel);
	}

	DBG("  this[%x] crate label at [%d %d %d %d]\n", this ,dfbRect->x, dfbRect->y, dfbRect->w, dfbRect->h);
	
	return DFB_OK;
}
