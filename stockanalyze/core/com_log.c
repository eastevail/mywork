/*
 * com_log.c
 *
 *  Created on: 2015年9月22日
 *      Author: lixiang
 */


#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/TimeStamp.hh>
#include "com_log.h"
static int inited=0;
static int closed=0;
/*void sky_test(std::string pattern, log4cpp::PatternLayout* layout, log4cpp::Category& cat)
{
    layout->setConversionPattern(pattern);
    cat.error("message");
    return;
}*/
int init_com_log()
{
    if(0==inited){
 //   log4cpp::Category& root=0;
    log4cpp::Category& root = log4cpp::Category::getRoot();
    log4cpp::FileAppender* fileAppender=new log4cpp::FileAppender("stderr",LOG_FILE);
    log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
    fileAppender->setLayout(layout);
//    log4cpp::Category& cat = log4cpp::Category::getInstance("cat1");
    layout->setConversionPattern("%d{%d %b %Y %H:%M:%S.%l} %m %n");
    root.removeAllAppenders();
    root.addAppender(fileAppender);
    inited=1;
/*    sky_test("%% %r %c:%d (%R / %r) [%p] %x %m %% (%u) %n", layout, cat);

    // test format specifiers
    sky_test(">%10m<%n", layout, cat);
    sky_test(">%-10m<%n", layout, cat);
    sky_test(">%3.5m<%n", layout, cat);
    sky_test(">%.5m<%n", layout, cat);

    // category test
    sky_test(">%c{2}<%n", layout, log4cpp::Category::getInstance("c1.c2.c3.c4"));

    // test date format
    sky_test("%d{%d %b %Y %H:%M:%S.%l} %m %n", layout, cat);
    sky_test("%d{%d %b %Y %H:%M:%S.%l", layout, cat);
    sky_test("%d%n", layout, cat);

    sky_test("%m %d%n", layout, cat);*/
/*    log4cpp::OstreamAppender* osAppender =newlog4cpp::OstreamAppender("osAppender",&cout);
    osAppender->setLayout(newlog4cpp::BasicLayout());

    log4cpp::Category& root =log4cpp::Category::getRoot();
    root.addAppender(osAppender);
    root.setPriority(log4cpp::Priority::DEBUG);
    root.error("Hello log4cpp in aError Message!");
    root.warn("Hello log4cpp in aWarning Message!");
    log4cpp::Category::shutdown();*/
    }
    inited++;
    return 0;
}
int close_com_log()
{
    inited--;
    if (0==inited)
    {
        log4cpp::Category::shutdown();
    }
    return 0;
}
/*int com_log(int isDebug,const char* stringFormat, ...){
    log4cpp::Category& root = log4cpp::Category::getRoot();
    va_list va;
    va_start(va,stringFormat);
    root.error(stringFormat, va);
    root.setPriority(log4cpp::Priority::DEBUG);
    root.debug(stringFormat, va);
    va_end(va);
    root.error("%s",__FILE__);
    return 0;
}*/
