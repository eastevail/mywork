/*
 * tb_tpye.h
 *
 *  Created on: 2016年8月10日
 *      Author: lixiang 21588
 */

#ifndef TBAPP_COMMON_TB_TPYE_H_
#define TBAPP_COMMON_TB_TPYE_H_

typedef enum{
	AVDATA_CACHE=0,
	TB_UI,
	DISPALY_MANAGE,
	MSGSERVER,
	RECORD_PRO,
	FUNC_PRO_TOTAL,
}FUNC_PRO_TYPE;

typedef enum{
	REGISTER_TO_MSGSERVER=0,

}TB_MSG_TYPE;

typedef struct{
	int destPro;
	int srcPro;
	int msgType;
	int msglen;
	int magic;
	char* msg;
}MsgCommuni;
typedef enum
{
	eIMAGE_QVGA,
	eIMAGE_WQVGA,
	eIMAGE_VGA,
	eIMAGE_SVGA,
	eIMAGE_HD720
}E_IMAGE_RESOL;

typedef enum{
	NOZOOM=0,
	ZOOMINONE=1,
	ZOOMOUTONE,
	ZOOMINTWO=3,
	ZOOMOUTTWO,
	ZOOMINTHI=5,
	ZOOMOUTTHI,
	ZOOMINFOU=7,
	ZOOMOUTFOU,
}ZOOMLEVEL;

typedef enum{
	UPDIRECTION=0,
	RIGHTDIRECTION,
	DOWNDIRECTION,
	LEFTDIRECTION,
}ROTATEDIREC;

typedef unsigned int  __u32;
typedef struct tb_vpe_transform{
	__u32	src_addr;
	__u32	src_format;
	__u32	src_width;
	__u32	src_height;
	__u32	dest_addr;
	__u32	dest_format;
	__u32	dest_width;
	__u32	dest_height;
	ROTATEDIREC	rotation;
	ZOOMLEVEL zoomlev;
} tb_vpe_transform_t;
#endif /* TBAPP_COMMON_TB_TPYE_H_ */
