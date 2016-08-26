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
		sync();
	}
	if (read_fd) {
		AVI_close(read_fd);
	}
	pthread_mutex_destroy(&mutex);
}

int aviFormatOp::init()
{
	bzero(lastCallFilename,sizeof(lastCallFilename));
	pthread_mutex_init(&mutex,NULL);
	out_fd=NULL;
	read_fd=NULL;
	return 0;
}

unsigned int aviFormatOp::getCurWriAvFileLen()
{
	return m_curWriLen;
}
int aviFormatOp::openAvFile(char* filename)
{
	read_fd = AVI_open_input_file(filename,1); //把文件描述符绑定到此文件上
	if (read_fd == NULL) {
		DBGERROR("fopen :%s", strerror(errno));
		return -1;
	}
	return 0;
}
int aviFormatOp::getOneFrame(char* videobuf)
{
	pthread_mutex_lock(&mutex);
	int ret = AVI_read_frame(read_fd, videobuf);
	pthread_mutex_unlock(&mutex);
	return ret;
}
int aviFormatOp::getOneAudio(char* audiobuf,int len)
{
	pthread_mutex_lock(&mutex);
	int ret = AVI_read_audio(read_fd, audiobuf, len);
	pthread_mutex_unlock(&mutex);
	return ret;
}
int aviFormatOp::closeAvFile(char* filename)
{
	return AVI_close(read_fd);
}
int aviFormatOp::decodeOneAvFile(Avdata* pdata,char* filename)
{
	int len;
	if(!read_fd){
		read_fd = AVI_open_input_file(filename,1); //把文件描述符绑定到此文件上
		if (read_fd == NULL) {
			DBGERROR("fopen :%s", strerror(errno));
			return -3;
		}
	}
	int ret = AVI_read_data(read_fd, pdata->buffV, pdata->lenV, pdata->buffA, pdata->lenA, &len);
	/*
	 * Return codes:
	 *
	 *    1 = video data read
	 *    2 = audio data read
	 *    0 = reached EOF
	 *   -1 = video buffer too small
	 *   -2 = audio buffer too small
	 */
	if (0 == ret) {
		if (read_fd) {
			AVI_close(read_fd);
		}
		pdata->lenV=0;
		pdata->lenA=0;
		read_fd=NULL;
	} else if (-1 == ret) {
		DBGERROR("video buffer too small");
	} else if (-2 == ret) {
		DBGERROR("audio buffer too small");
	} else if (1 == ret) {
		pdata->lenV=len;
		pdata->lenA=0;
	} else if (2 == ret) {
		pdata->lenA=len;
		pdata->lenV=0;
	}
	return ret;
}

int aviFormatOp::createOneAvFile(Avdata* pdata,char* filename)
{
	if (strncasecmp((const char*) lastCallFilename, (const char*) filename,
			strlen((const char*) filename))) {
		if (out_fd) {
			AVI_close(out_fd);
			sync();
		}
		out_fd = AVI_open_output_file(filename); //把文件描述符绑定到此文件上
		if (out_fd == NULL) {
			DBGERROR("fopen :%s", strerror(errno));
			return -1;
		}
		AVI_set_video(out_fd, 320, 240, 25, "MJPG"); //设置视频文件的格式
		AVI_set_audio(out_fd, 1, 16000, 16, WAVE_FORMAT_PCM,"mp4a");
		m_curWriLen = 0;
		strncpy(lastCallFilename, (const char*) filename, strlen((const char*) filename));
	}
	if (out_fd) {
		if (AVI_write_frame(out_fd, pdata->buffV, pdata->lenV) < 0) {
			DBGERROR("write erro");
			return -1;
		}
		if (AVI_write_audio(out_fd, pdata->buffA, pdata->lenA) < 0) {
			DBGERROR("write audio erro");
			return -1;
		}
		m_curWriLen += pdata->lenV;
	}
	return 0;
}
