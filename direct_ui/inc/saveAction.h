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

#ifndef __SAVEACTION_H__
#define __SAVEACTION_H__

#include "common.h"
#include "label_bar.h"

typedef void (*ActionOverfun) (void *ctx);

class SaveAction
{
public:
	SaveAction();
	~SaveAction();

    void actionPlay(ActionOverfun overFunCB , void* data);

private:
	static DFBResult OnTimeOutEvent(void *data);

	void TimeOutEvent();
	void DestoryLabel();
	void createLabel(CLabelBar  **labelBar,int x,int y ,int w ,int h);

	int m_actionTimer;

	ActionOverfun overCallBack;
	void * m_dataCB;

	CLabelBar  *m_labelBar ;
	int m_screen_w ;
	int m_screen_h ;
	int m_x ;
	int m_y ;
	int m_w;
	int m_h;
};

#endif


