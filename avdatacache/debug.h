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


#ifdef __cplusplus
extern "C"{
#endif 

#ifndef APPNAME
#define APPNAME "[-DBG-]"
#endif

#define DBG(fmt, args...) \
    do{  \
        fprintf(stdout, "[-DBG-] [%s:%5d] --- " fmt"\n", __FILE__, __LINE__, ## args);  \
    }while(0)


#ifdef __cplusplus
}
#endif 

#endif

