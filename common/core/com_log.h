/*
 * com_log.h
 *
 *  Created on: 2015年9月22日
 *      Author: lixiang
 */

#ifndef INSTALL_APP_COM_LOG_H_
#define INSTALL_APP_COM_LOG_H_
#include"../debug.h"
//#include <log4cpp/Category.hh>
#define TS_ERR        1
#define TS_DBG        2
#define INIT_TS_LOG init_com_log();
#define CLOSE_TS_LOG close_com_log();
//#define TSever_LOG_Error(format, argv ...) log4cpp::Category::getRoot().error("%s() [%04d]| " format,__FILE__, __LINE__, ##argv)
//#define LOG_FILE "/home/share/libevent-2.0.22-stable/install/app/error.log"
#define LOG_FILE fileno(stderr)
#define TSever_LOG_Error DBG
int init_com_log();
int close_com_log();
#endif /* INSTALL_APP_COM_LOG_H_ */
