/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: main.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	: 
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include <stdio.h>

#include "desktop.h"

int main(int argc, char *argv[])
{
	CDesktop *pDesktop = CDesktop::GetDesktop();

	pDesktop->DoLoop();
	
	return 0;
}

