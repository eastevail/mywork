/*
 * avFromatOp.cpp
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#include "avFromatOp.h"

avFromatOp::avFromatOp()
{
	// TODO Auto-generated constructor stub
	m_pFile=NULL;
	m_curWriLen =0;
}

avFromatOp::~avFromatOp()
{
	// TODO Auto-generated destructor stub
	if (m_pFile){
		fflush(m_pFile);
		fclose(m_pFile);
	}
}

