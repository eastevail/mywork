/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: label_bar.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.04.07
	Description	:
	History		:
					Create by hul .2015.06.02
******************************************************************************/

#include "saveAction.h"

#define ACTION_MARGIN  4

SaveAction::SaveAction()
{
	m_labelBar = NULL;
	m_screen_w = 320;
	m_screen_h = 210;
	m_x = 0;
	m_y = 30;
	m_w = m_screen_w;
	m_h = m_screen_h;
	overCallBack = NULL;
	m_dataCB = NULL;

	m_actionTimer = -1;
}

SaveAction::~SaveAction()
{
	if(m_labelBar)
	{
		delete m_labelBar;
		m_labelBar = NULL;
	}

	if(overCallBack)
		overCallBack = NULL;

	m_dataCB = NULL;
}

static DFBResult SaveAction::OnTimeOutEvent(void *data)
{
	SaveAction *p = (SaveAction *)data;

	if (p)
	{
		p->TimeOutEvent();
	}

	return DFB_OK;
}

void SaveAction::TimeOutEvent()
{
	DBG("TimeOutEvent\n");

	if(m_x < m_screen_w)
	{
		if(m_labelBar == NULL)
		{
			createLabel(&m_labelBar,m_x,m_y,m_w,m_h);
		}
		else
		{
			delete m_labelBar;
			m_labelBar = NULL;

			m_x += m_screen_w/4;
			m_y += m_screen_h/4;
			m_w -= m_screen_w/4;
			m_h -= m_screen_h/4;
		}
	}
	else
	{
		DestoryLabel();
	}
}

void SaveAction::createLabel(CLabelBar  **labelBar,int x,int y ,int w ,int h)
{
	DFBRectangle rectout;
	DFBRectangle rectin;

	rectout.x			= x;
	rectout.y			= y;
	rectout.w			= w;
	rectout.h			= h;

	rectin.x			= x + ACTION_MARGIN;
	rectin.y			= y + ACTION_MARGIN;
	rectin.w			= w - ACTION_MARGIN*2;
	rectin.h			= h - ACTION_MARGIN*2;

	*labelBar = new CLabelBar(&rectout,&rectin);
}

void SaveAction::actionPlay(ActionOverfun overFunCB , void* data)
{
	if (DFB_OK != lite_enqueue_window_timeout(50, TimeOut_EveryTime_Priority, OnTimeOutEvent, this, &m_actionTimer))//TimeOut_EveryTime
	{
		DBG("[actionPlay] create timer error\n");
	}
	else
	{
		DBG("[actionPlay] create timer succes, id = %d\n", m_actionTimer);
	}

	overCallBack = overFunCB;
	m_dataCB = data;
}

void SaveAction::DestoryLabel()
{
	lite_remove_window_timeout(m_actionTimer);

	m_actionTimer = -1;

	if(overCallBack)
		overCallBack(m_dataCB);

	delete this;
}


