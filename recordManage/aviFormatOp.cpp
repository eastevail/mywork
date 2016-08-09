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
	if (out_fd) {
		AVI_close(out_fd);
	}
}

int aviFormatOp::init()
{
	bzero(lastCallFilename,sizeof(lastCallFilename));
	out_fd=NULL;
	return 0;
}

unsigned int aviFormatOp::getCurWriAvFileLen()
{
	return m_curWriLen;
}
int aviFormatOp::createOneAvFile(Avdata* pdata,char* filename)
{
	if (strncasecmp((const char*) lastCallFilename, (const char*) filename,
			strlen((const char*) filename))) {
		if (out_fd) {
			AVI_close(out_fd);
		}
		out_fd = AVI_open_output_file(filename); //把文件描述符绑定到此文件上
		if (out_fd == NULL) {
			DBGERROR("fopen :%s", strerror(errno));
			return -1;
		}
		AVI_set_video(out_fd, 320, 240, 15, "MJPG"); //设置视频文件的格式

		m_curWriLen = 0;
		strncpy(lastCallFilename, (const char*) filename, strlen((const char*) filename));
	}
	if (out_fd) {
		if (AVI_write_frame(out_fd, pdata->buffV, pdata->lenV) < 0) {
			DBGERROR("write erro");
			return -1;
		}
		m_curWriLen += pdata->lenV;
	}
	return 0;
}
