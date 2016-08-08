/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: arrow.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include "arrow.h"
#include "desktop.h"

CArrow::CArrow(DFBRectangle &dfbRect, ArrowDir_E dir)
{
	m_rectPos = dfbRect;
	m_arrowDir = dir;
	//m_arrowLabel = NULL;
	m_ArrowImage1 = NULL;
	m_ArrowImage2 = NULL;
	m_bNOActiveFlag  = 1;
	m_triangleIndex = 0;
	m_blinkTimeID = -1;
	m_bBlinkFlag = 0;

	InitArrow();
}

CArrow::~CArrow()
{
	UnInitArrow();
}

void CArrow::BlinkLeftArrow()
{
	if (m_arrowDir != Arrow_Horizontal)
	{
		return;
	}

	m_triangleIndex = 0;
	m_bBlinkFlag	= 1;
	m_bNOActiveFlag = 0;

	ShowArrowActive(TRUE);
}

void CArrow::BlinkRightArrow()
{
	if (m_arrowDir != Arrow_Horizontal)
	{
		return;
	}

	m_bNOActiveFlag = 0;
	m_triangleIndex = 1;
	m_bBlinkFlag	= 1;

	ShowArrowActive(TRUE);
}

void CArrow::BlinkUpArrow()
{
	if (m_arrowDir != Arrow_Vertical)
	{
		return;
	}
	m_bNOActiveFlag = 0;

	m_triangleIndex = 0;
	m_bBlinkFlag	= 1;

	ShowArrowActive(TRUE);
}

void CArrow::BlinkDownArrow()
{
	if (m_arrowDir != Arrow_Vertical)
	{
		return;
	}
	m_bNOActiveFlag = 0;

	m_triangleIndex = 1;
	m_bBlinkFlag	= 1;

	ShowArrowActive(TRUE);
}

DFBResult CArrow::BlinkArrowCB(void *data)
{
	CArrow *pArrow = (CArrow *)data;

	if (pArrow)
	{
		pArrow->BlinkArrow();
	}

	return DFB_OK;
}

void CArrow::BlinkArrow()
{
	DBG("BlinkArrow\n");
	if (m_bBlinkFlag == 1)
	{
		ShowArrowActive(FALSE);
		m_bBlinkFlag = 0;
	}
}


void CArrow::ShowArrowActive(BOOL active)
{
#if 0 // xjf
	char szImageFile1[64];
	char szImageFile2[64];


	memset(szImageFile1,0,sizeof(szImageFile1));
	memset(szImageFile2,0,sizeof(szImageFile2));

	if(	m_bNOActiveFlag)
		return ;


	if (active == TRUE)
	{

		if(m_arrowDir == Arrow_Horizontal)
		{

			if(m_triangleIndex)
			{
				sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT2);
				sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT3);
			}
			else
			{
				sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT3);
				sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT2);
			}
		}
		else
		{
			if(m_triangleIndex)
			{
				sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP2);
				sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN3);
			}
			else
			{
				sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP3);
				sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN2);
			}

		}


	}
	else if (active == FALSE)
	{

		if(m_arrowDir == Arrow_Horizontal)
		{

			sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT2);
			sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT2);

		}
		else
		{

			sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP2);
			sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN2);

		}


	}

	if (DFB_OK != lite_load_image(m_ArrowImage1, szImageFile1))
	{
		DBG("  load image file %s failed\n", szImageFile2);
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(m_ArrowImage2, szImageFile2))
	{
		DBG("  load image file %s failed\n", szImageFile2);
		return DFB_FAILURE;
	}

	DBG("  load image file %s \n", szImageFile1);
	DBG("  load image file %s \n", szImageFile2);


	//lite_update_box(LITE_BOX(m_arrowLabel), NULL);
#endif
}


int CArrow::setArrowNOActive()
{

	char szImageFile1[64];
	char szImageFile2[64];


	memset(szImageFile1,0,sizeof(szImageFile1));
	memset(szImageFile2,0,sizeof(szImageFile2));

	if(m_arrowDir == Arrow_Horizontal)
	{

		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT1);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT1);

	}
	else
	{

		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP1);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN1);

	}


	if (DFB_OK != lite_load_image(m_ArrowImage1, szImageFile1))
	{
		DBG("  load image file %s failed\n", szImageFile1);
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(m_ArrowImage2, szImageFile2))
	{
		DBG("  load image file %s failed\n", szImageFile2);
		return DFB_FAILURE;
	}

	m_bNOActiveFlag = 1;

	return TRUE;
}

int CArrow::setArrowActive()
{
	char szImageFile1[64];
	char szImageFile2[64];

	memset(szImageFile1,0,sizeof(szImageFile1));
	memset(szImageFile2,0,sizeof(szImageFile2));

	if(m_arrowDir == Arrow_Horizontal)
	{
		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT2);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT2);
	}
	else
	{
		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP2);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN2);

	}

	if (DFB_OK != lite_load_image(m_ArrowImage1, szImageFile1))
	{
		DBG("  load image file %s failed\n", szImageFile1);
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(m_ArrowImage2, szImageFile2))
	{
		DBG("  load image file %s failed\n", szImageFile2);
		return DFB_FAILURE;
	}

	return TRUE;
}

void CArrow::InitArrow()
{
	CDesktop*			pDesktop 	= CDesktop::GetDesktop();
	LiteWindow*			liteWin		= pDesktop->GetLiTEWindow();
	DFBRectangle dfbRect1;
	DFBRectangle dfbRect2;
	char szImageFile1[64];
	char szImageFile2[64];

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-arrow] lite window must init first\n");
		return;
	}

	memset(szImageFile1,0,sizeof(szImageFile1));
	memset(szImageFile2,0,sizeof(szImageFile2));

	if(m_arrowDir == Arrow_Horizontal)
	{

		dfbRect1.x 			= m_rectPos.x;
		dfbRect1.y 			= m_rectPos.y;
		dfbRect1.w 			= ARROW_IMG_W;
		dfbRect1.h 			= ARROW_IMG_H;

		dfbRect2.x			= m_rectPos.x + ARROW_IMG_W + 1;
		dfbRect2.y			= m_rectPos.y;
		dfbRect2.w			= ARROW_IMG_W;
		dfbRect2.h			= ARROW_IMG_H;

		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_LEFT1);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_RIGHT1);


	}else{

		dfbRect1.x			= m_rectPos.x;
		dfbRect1.y			= m_rectPos.y;
		dfbRect1.w			= ARROW_IMG_W;
		dfbRect1.h			= ARROW_IMG_H;

		dfbRect2.x			= m_rectPos.x ;
		dfbRect2.y			= m_rectPos.y + ARROW_IMG_H + 1;
		dfbRect2.w			= ARROW_IMG_W;
		dfbRect2.h			= ARROW_IMG_H;


		sprintf(szImageFile1,"%s/%s",PIC_PATH,ARROW_IMG_UP1);
		sprintf(szImageFile2,"%s/%s",PIC_PATH,ARROW_IMG_DOWN1);

	}

	if (DFB_OK != lite_new_image(LITE_BOX(liteWin), &dfbRect1, liteNoImageTheme, &m_ArrowImage1))
	{
		DBG("  create image fail\n");
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(m_ArrowImage1, szImageFile1))
	{
		DBG("  load image file %s failed\n", szImageFile1);
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_new_image(LITE_BOX(liteWin), &dfbRect2, liteNoImageTheme, &m_ArrowImage2))
	{
		DBG("  create image fail\n");
		return DFB_FAILURE;
	}

	if (DFB_OK != lite_load_image(m_ArrowImage2, szImageFile2))
	{
		DBG("  load image file %s failed\n", szImageFile2);
		return DFB_FAILURE;
	}


#if 0 // xjf
	lite_enqueue_window_timeout(BLINK_TIME, TimeOut_EveryTime, BlinkArrowCB, this, &m_blinkTimeID);
	DBG("[DirectFBUI-arrow] create timer, id = %d\n", m_blinkTimeID);
#endif
}

void CArrow::UnInitArrow()
{
	SAFE_DEL_LITE(m_ArrowImage1);
	SAFE_DEL_LITE(m_ArrowImage2);

	if (m_blinkTimeID != -1)
	{
		DBG("UnInitArrow, m_blinkTimeID: %d\n", m_blinkTimeID);

		lite_remove_window_timeout(m_blinkTimeID);
		m_blinkTimeID = -1;
	}
}

