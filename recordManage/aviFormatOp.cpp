/*
 * aviFormatOp.cpp
 *
 *  Created on: 2016年8月8日
 *      Author: lixiang 21588
 */

#include "aviFormatOp.h"
#include <strings.h>
aviFormatOp::aviFormatOp()
{
	// TODO Auto-generated constructor stub

}

aviFormatOp::~aviFormatOp()
{
	// TODO Auto-generated destructor stub
}

int aviFormatOp::init()
{
	bzero(lastCallFilename,sizeof(lastCallFilename));
	return 0;
}

unsigned int aviFormatOp::getCurWriAvFileLen()
{
	return m_curWriLen;
}
int aviFormatOp::createOneAvFile(Avdata* pdata,unsigned char* filename)
{
	int ret;
	if (strncasecmp((const char*) lastCallFilename, (const char*) filename,
			strlen((const char*) filename))) {
		if (m_pFile) {
			fflush(m_pFile);
			fclose(m_pFile);
		}
		m_pFile = fopen((const char*) filename, "wb");
		if (!m_pFile) {
			DBGERROR("fopen :%s", strerror(errno));
			return -1;
		}
		m_curWriLen=0;
		strncpy(lastCallFilename, (const char*) filename, strlen((const char*) filename));
	}
	if (m_pFile) {
		ret = fwrite(pdata->buffV, 1, pdata->lenV, m_pFile);
		if (ret != pdata->lenV) {
			DBGERROR("fwrite error");
			return -1;
		}
		m_curWriLen+=ret;
		fwrite(pdata->buffV, 1, pdata->lenV, m_pFile);
		if (ret != pdata->lenA) {
			DBGERROR("fwrite error");
			return -1;
		}
		m_curWriLen+=ret;
	}
	return 0;
}
