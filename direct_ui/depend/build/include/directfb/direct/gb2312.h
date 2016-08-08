/*
   (c) Copyright 2001-2009  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrj盲l盲 <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

   UTF8 routines ported from glib-2.0 and optimized

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __DIRECT__GB2312_H__
#define __DIRECT__GB2312_H__

#include <direct/types.h>
#include <direct/gbk_unicode.h>

#define DIRECT_GB2312_GET_CHAR(p) __direct_gb2312_get_char(p)

static __inline__ unichar __direct_gb2312_get_char( const u8 *p )
{
    int              len;
    char             text[2];
    unsigned int     result;

    // GB2313 的编码长度固定为2
    text[0] = p[0];
    text[1] = p[1];

    // freetype2需要的是unicode的字符编码, 根据这个参数转换的是保存的文件的字符的GB2312编码, 不是减去0xA0后的区位编码
    GBKToUnicode((wchar_t *)&result, text, 2);
    return result;
}

#endif
