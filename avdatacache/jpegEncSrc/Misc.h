/* Misc.h
 *
 *
 * Copyright (c)2008 Nuvoton technology corporation
 * http://www.nuvoton.com
 *
 * Misc function
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __MISC_H__
#define __MISC_H__
#include "../../common/debug.h"
#include "../../common/tb_tpye.h"
//#define DEBUG_PRINT(x, ...) fprintf(stdout, x, ##__VA_ARGS__)
//#define DEBUG_PRINT(x, ...)

/*
#define DEBUG_PRINT(fmt, args...) \
    do{  \
        fprintf(stdout, "[-DBG-] [%s:%5d] --- " fmt"\n", __FILE__, __LINE__, ## args);  \
    }while(0)
*/
#define ERR_PRINT(x, ...) fprintf(stderr, x, ##__VA_ARGS__)

#ifndef BOOL
#define BOOL int32_t
#define FALSE 0
#define TRUE 1
#endif

#define RETRY_NUM 4

#if 0
#define HOME_KEY		19
#define ENTER_KEY		13
#define CAMERA			212
#define VOL_PLUS_KEY	175
#define VOL_MINUS_KEY	174
#define UP_KEY			14
#define DOWN_KEY		15
#define LEFT_KEY		1
#define RIGHT_KEY		2
#else
/* W55FA93 IN key pad */
#define HOME_KEY		19
#define ENTER_KEY		13
#define CAMERA			13
#define VOL_PLUS_KEY	175
#define VOL_MINUS_KEY	174
#define UP_KEY			14
#define DOWN_KEY		15
#define LEFT_KEY		1
#define RIGHT_KEY		2
#endif



#endif

