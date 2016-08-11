/******************************************************************************
    Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd
 
    File Name    : debug.h
    Version      : 1.0
    Author       : CW Video Server Software Group
    Created      : 2009-03-10
    Description  : 
    History      : 
                        created by hl. 2009-03-10
******************************************************************************/

#ifndef _DEBUG_H
#define _DEBUG_H




#ifndef APPNAME
#define APPNAME "[-DBG-]"
#endif

#define DEBUG_PRINT DBG
#define DBG(fmt, args...) \
    do{  \
        fprintf(stdout, "[-DBG-] [%s:%5d] --- " fmt"\n", __FILE__, __LINE__, ## args);  \
    }while(0)

#define DBGERROR(fmt, args...) \
    do{  \
        fprintf(stdout, "[-ERROR-] [%s:%5d] --- " fmt"\n", __FILE__, __LINE__, ## args);  \
    }while(0)

#define CHECKRET(x) \
	if(x<0){ DBGERROR();return -1;}



#endif

