/* JpegEnc.c
 *
 *
 * Copyright (c)2008 Nuvoton technology corporation
 * http://www.nuvoton.com
 *
 * JPEG encode function
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <sys/statfs.h>

#include <linux/videodev.h>
#include "dsc.h"
#include "JpegEnc.h"

uint8_t *s_pu8JpegEncBuf = MAP_FAILED;
static int32_t s_i32JpegFd = -1;
uint32_t s_u32JpegBufSize;
static jpeg_param_t s_sJpegParam;
static jpeg_info_t *s_pJpegInfo = NULL;	

#define DBG_POINT()		printf("Function:%s. Line: %d\n", __FUNCTION__, __LINE__);


extern void create_simple_EXIF(char *Buffer, int thumbnail_offset,int thumbnail_size);



//////////////////////////////jpeg_demo

__u8 g_qtflag = 0;

__u8 g_au8QT0[64] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
      g_au8QT1[64] = { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02};


static       int       s_ai32RToY[256];
static       int       s_ai32GToY[256];
static       int       s_ai32BToY[256];
static       int       s_ai32RToCb[256];
static       int       s_ai32GToCb[256];
static       int       s_ai32BToCb[256];
static       int       s_ai32RToCr[256];
static       int       s_ai32GToCr[256];
static       int       s_ai32BToCr[256];

unsigned char *pJpegBuffer = NULL, *pSRCbuffer = NULL, *pJpegSWaddr = NULL; 
//////////////////////////////jpeg_demo

int 
InitJpegDevice(
	E_IMAGE_RESOL eImageResol
)
{
	int32_t i;
	char achDevice[] = "/dev/video1";

	for(i=0; i < RETRY_NUM; i++){
		s_i32JpegFd = open(achDevice, O_RDWR);         //maybe /dev/video1, video2, ...
		if (s_i32JpegFd  < 0){
			ERR_PRINT("open %s error\n", achDevice);
			achDevice[10]++;
			continue;       
		}
		/* allocate memory for JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_GET_JPEG_BUFFER, &s_u32JpegBufSize)) < 0){
			close(s_i32JpegFd);
			achDevice[10]++;
			DEBUG_PRINT("Try to open %s\n",achDevice);
		}
		else{       
			DEBUG_PRINT("jpegcodec is %s\n",achDevice);
			break;                  
		}
	}
	if(s_i32JpegFd < 0)
		return s_i32JpegFd; 

	s_pJpegInfo = malloc(sizeof(jpeg_info_t) + sizeof(__u32));
	if(s_pJpegInfo == NULL){
		ERR_PRINT("Allocate JPEG info failed\n");
		close(s_i32JpegFd);
		return -1;		
	}

	s_pu8JpegEncBuf = mmap(NULL, s_u32JpegBufSize, PROT_READ|PROT_WRITE, MAP_SHARED, s_i32JpegFd, 0);
	//	pJpegBuffer = mmap(NULL, jpeg_buffer_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(s_pu8JpegEncBuf == MAP_FAILED){
		ERR_PRINT("JPEG Map Failed!\n");
		close(s_i32JpegFd);
		return -1;
	}

	memset((void*)&s_sJpegParam, 0, sizeof(s_sJpegParam));


	s_sJpegParam.encode = 1;						/* Encode */
	if(eImageResol == eIMAGE_VGA){
		s_sJpegParam.encode_width = 640;			/* JPEG width */
		s_sJpegParam.encode_height = 480;			/* JPGE Height */
	}
	else if(eImageResol == eIMAGE_WQVGA){
		s_sJpegParam.encode_width = 480;			/* JPEG width */
		s_sJpegParam.encode_height = 272;			/* JPGE Height */
	}
	else{
		s_sJpegParam.encode_width = 320;			/* JPEG width */
		s_sJpegParam.encode_height = 240;			/* JPGE Height */
	}


	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;
	//s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;

	s_sJpegParam.buffersize = 0;		/* only for continuous shot */
	s_sJpegParam.buffercount = 1;		

	s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */	
	s_sJpegParam.qscaling = 7;			
	s_sJpegParam.qadjust = 0xf;			
	printf("JPEG file descriptor = %d\n", s_i32JpegFd);
	return s_i32JpegFd;
}

int 
JpegEncFromVIN(E_IMAGE_RESOL eEncodeImgResol,
	uint32_t u32PlanarYBufPhyAddr,	/* src pattern */
	uint8_t **ppu8JpegEncBuf,
	uint32_t *pu32JpegEncLen,
	S_JPEG_ENC_FEAT *psJpegEncFeat,
	struct v4l2_crop *psVideoCrop
)
{
	uint32_t u32Width, u32Height;	
	uint8_t *pu8EncBitStmAddr;
	uint8_t *pu8ReservedAddr = NULL;
	int32_t i32Len;
	uint32_t u32PlanarUBufPhyAddr, u32PlanarVBufPhyAddr;

	if(eEncodeImgResol == eIMAGE_WQVGA){
		u32Width = 480;				
		u32Height = 272;					
	}else if(eEncodeImgResol == eIMAGE_VGA){
		u32Width = 640;
		u32Height = 480;		
	}else if(eEncodeImgResol == eIMAGE_SVGA){
		u32Width = 800;
		u32Height = 600;
	}else if(eEncodeImgResol == eIMAGE_HD720){
		u32Width = 1280;
		u32Height = 720;
	}else{
		u32Width = 320;				
		u32Height = 240;				
	} 


	DBG_POINT();
#if 1
		if(psVideoCrop->c.width>u32Width){//Cropping window bigger than encode dimension, 
			u32PlanarUBufPhyAddr = 	u32PlanarYBufPhyAddr + u32Width*u32Height;
	#ifdef _PLANAR_YUV420_
			//YUV420
			u32PlanarVBufPhyAddr = 	u32PlanarUBufPhyAddr + u32Width*u32Height/4;		
	#else
			//YUV422
			u32PlanarVBufPhyAddr = 	u32PlanarUBufPhyAddr + u32Width*u32Height/2;	
	#endif	
		}
		else{
			u32PlanarUBufPhyAddr = 	u32PlanarYBufPhyAddr + psVideoCrop->c.width*psVideoCrop->c.height;
	#ifdef _PLANAR_YUV420_
			//YUV420
			u32PlanarVBufPhyAddr = 	u32PlanarUBufPhyAddr + psVideoCrop->c.width*psVideoCrop->c.height/4;		
	#else
			//YUV422
			u32PlanarVBufPhyAddr = 	u32PlanarUBufPhyAddr + psVideoCrop->c.width*psVideoCrop->c.height/2;	
	#endif				
		}
		if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_YADDRESS, u32PlanarYBufPhyAddr)) < 0){
			ERR_PRINT("Set jpeg Y address failed:%d\n",errno);
			return -1;
		}
		if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_UADDRESS, u32PlanarUBufPhyAddr)) < 0){
			ERR_PRINT("Set jpeg U address failed:%d\n",errno);
			return -1;
		}
		if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_VADDRESS, u32PlanarVBufPhyAddr)) < 0){
			ERR_PRINT("Set jpeg V address failed:%d\n",errno);
			return -1;
		}	
#endif 
	printf("Y Addr = 0x%x\n", u32PlanarYBufPhyAddr);	
	printf("U Addr = 0x%x\n", u32PlanarUBufPhyAddr);	
	printf("V Addr = 0x%x\n", u32PlanarVBufPhyAddr);	
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422; 
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* Source is Planar Format */	

	pu8EncBitStmAddr = s_pu8JpegEncBuf;

	if(psJpegEncFeat->eJpegEncThbSupp){
	/* Set reserved size for exif */
		int32_t i32EncReservedSize = 0x84;
		pu8ReservedAddr = pu8EncBitStmAddr + 6;
		memset((char *)pu8ReservedAddr, 0xFF, i32EncReservedSize);
		ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, i32EncReservedSize); 
	}


	if((psVideoCrop->c.width <  u32Width) ||
		(psVideoCrop->c.height < u32Height) )
	{//Encode upscale 
		s_sJpegParam.scaled_width = u32Width;	 
		s_sJpegParam.scaled_height = u32Height;
		s_sJpegParam.encode_width = psVideoCrop->c.width;
		s_sJpegParam.encode_height = psVideoCrop->c.height;					
		s_sJpegParam.scale = 1;				
		printf("JPEG encode source WxH= %d x %d\n", s_sJpegParam.encode_width, s_sJpegParam.encode_height);
		printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);	
		/* Set encode source stride (Must calling after IOCTL - JPEG_S_PARAM) */
		ioctl(s_i32JpegFd, JPEG_SET_ENC_STRIDE, psVideoCrop->c.width>u32Width);
	}	
	else
	{//
		s_sJpegParam.scale = 0; 
		s_sJpegParam.encode_width = u32Width;
		s_sJpegParam.encode_height = u32Height;
		/* Set encode source stride (Must calling after IOCTL - JPEG_S_PARAM) */
		ioctl(s_i32JpegFd, JPEG_SET_ENC_STRIDE, u32Width);

		printf("JPEG encode target WxH= %d x %d, stride = %d\n", u32Width, u32Height, u32Width);
	}
		
	DBG_POINT();
	/* Set operation property to JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0){
		ERR_PRINT("Set jpeg parame failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();
	
	/* Set Encode Source from VideoIn */
	//ioctl(s_i32JpegFd,JPEG_SET_ENC_SRC_FROM_VIN,NULL);	

	if(psJpegEncFeat->eJpegEncThbSupp){

		if(psJpegEncFeat->eJpegEncThbSupp == eJPEG_ENC_THB_QQVGA){
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 1);	
		}
		else{
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 0);	
		}

	}
	DBG_POINT();

	
	
	/* Trigger JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0){
		ERR_PRINT("Trigger jpeg failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();
	i32Len = read(s_i32JpegFd, s_pJpegInfo, sizeof(jpeg_info_t) + sizeof (__u32));
	
	if(i32Len < 0){
		ERR_PRINT("No data can get\n");
		if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			ERR_PRINT("Memory Stortage\n");	
		return -1;
	}

	if(psJpegEncFeat->eJpegEncThbSupp){
		int32_t i32ThumbnailSize;
		int32_t i32ThumbnailOffset;
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_SIZE, &i32ThumbnailSize);
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_OFFSET, &i32ThumbnailOffset);
		create_simple_EXIF((char *)pu8ReservedAddr, i32ThumbnailOffset,i32ThumbnailSize);
	}

	*ppu8JpegEncBuf = s_pu8JpegEncBuf;
	*pu32JpegEncLen = s_pJpegInfo->image_size[0];
	return 0;
}



int 
JpegEncFromVIN_Packet(E_IMAGE_RESOL eEncodeImgResol,
	uint32_t u32PacketBufPhyAddr,	/* src pattern */
	uint8_t **ppu8JpegEncBuf,
	uint32_t *pu32JpegEncLen,
	S_JPEG_ENC_FEAT *psJpegEncFeat,
	struct v4l2_crop *psVideoCrop
)
{
	uint32_t u32Width, u32Height;	
	uint8_t *pu8EncBitStmAddr;
	uint8_t *pu8ReservedAddr = NULL;
	int32_t i32Len;


	if(eEncodeImgResol == eIMAGE_WQVGA){
		u32Width = 480;				
		u32Height = 272;					
	}else if(eEncodeImgResol == eIMAGE_VGA){
		u32Width = 640;
		u32Height = 480;		
	}else if(eEncodeImgResol == eIMAGE_SVGA){
		u32Width = 800;
		u32Height = 600;
	}else if(eEncodeImgResol == eIMAGE_HD720){
		u32Width = 1280;
		u32Height = 720;
	}else{
		u32Width = 320;				
		u32Height = 240;				
	} 
	
	
	DBG_POINT();
#if 1					
	if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_YADDRESS, u32PacketBufPhyAddr)) < 0){
		ERR_PRINT("Set jpeg Y address failed:%d\n",errno);
		return -1;
	}
	#if 0
		if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_UADDRESS, u32PlanarUBufPhyAddr)) < 0){
			ERR_PRINT("Set jpeg U address failed:%d\n",errno);
			return -1;
		}
		if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_VADDRESS, u32PlanarVBufPhyAddr)) < 0){
			ERR_PRINT("Set jpeg V address failed:%d\n",errno);
			return -1;
		}
	#endif	
#endif 
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422; /* Useless */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* Source is Packet Format */	

	/* Set encode source stride (Must calling after IOCTL - JPEG_S_PARAM) */
	ioctl(s_i32JpegFd, JPEG_SET_ENC_STRIDE, u32Width);

	pu8EncBitStmAddr = s_pu8JpegEncBuf;

	if(psJpegEncFeat->eJpegEncThbSupp){
	/* Set reserved size for exif */
		int32_t i32EncReservedSize = 0x84;
		pu8ReservedAddr = pu8EncBitStmAddr + 6;
		memset((char *)pu8ReservedAddr, 0xFF, i32EncReservedSize);
		ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, i32EncReservedSize); 
	}


#if 0
	if((psVideoCrop->c.width <  u32Width) ||
		(psVideoCrop->c.height < u32Height) )
	{
		s_sJpegParam.scaled_width = u32Width;	 
		s_sJpegParam.scaled_height = u32Height;
		s_sJpegParam.encode_width = psVideoCrop->c.width;
		s_sJpegParam.encode_height = psVideoCrop->c.height;					
		s_sJpegParam.scale = 1;				
		printf("JPEG encode source WxH= %d x %d\n", s_sJpegParam.encode_width, s_sJpegParam.encode_height);
		printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);	
	}	
	else
	{
		s_sJpegParam.scale = 0; 
		s_sJpegParam.encode_width = u32Width;
		s_sJpegParam.encode_height = u32Height;
		printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);		
	}
#else
	s_sJpegParam.scale = 0; 
	s_sJpegParam.encode_width = u32Width;
	s_sJpegParam.encode_height = u32Height;
	printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);	
#endif		
	DBG_POINT();
	/* Set operation property to JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0){
		ERR_PRINT("Set jpeg parame failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();

	/* Set Encode Source from VideoIn */
	//ioctl(s_i32JpegFd,JPEG_SET_ENC_SRC_FROM_VIN,NULL);	

	if(psJpegEncFeat->eJpegEncThbSupp){

		if(psJpegEncFeat->eJpegEncThbSupp == eJPEG_ENC_THB_QQVGA){
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 1);	
		}
		else{
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 0);	
		}

	}
	DBG_POINT();
	/* Trigger JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0){
		ERR_PRINT("Trigger jpeg failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();
	i32Len = read(s_i32JpegFd, s_pJpegInfo, sizeof(jpeg_info_t) + sizeof (__u32));
	
	if(i32Len < 0){
		ERR_PRINT("No data can get\n");
		if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			ERR_PRINT("Memory Stortage\n");	
		return -1;
	}

	if(psJpegEncFeat->eJpegEncThbSupp){
		int32_t i32ThumbnailSize;
		int32_t i32ThumbnailOffset;
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_SIZE, &i32ThumbnailSize);
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_OFFSET, &i32ThumbnailOffset);
		create_simple_EXIF((char *)pu8ReservedAddr, i32ThumbnailOffset,i32ThumbnailSize);
	}

	*ppu8JpegEncBuf = s_pu8JpegEncBuf;
	*pu32JpegEncLen = s_pJpegInfo->image_size[0];
	return 0;
}


void _BGR565ToYCbCr(__u16 *pu16SrcBuf,__u16 *pu16DestBuf,__u16 u16Width, __u16 u16Height)
{
        __u16    u16X, u16Y, u16SrcBuf;
        __u8      u8R, u8G, u8B;
        __u8      u8Y0, u8U0, u8V0, u8Y1;
	__u8 *pu8DestBufY;
	__u8 *pu8DestBufU;
	__u8 *pu8DestBufV;

	pu8DestBufY = (__u8 *)	pu16DestBuf;
	pu8DestBufU = pu8DestBufY + u16Width * u16Height;
	pu8DestBufV = pu8DestBufU + u16Width * u16Height / 2;

        for ( u16Y = 0; u16Y < u16Height; u16Y += 1 )
        {
                for ( u16X = 0; u16X < u16Width; u16X += 2 )
                {
                        u16SrcBuf = pu16SrcBuf[u16Y * u16Width + u16X];
                        u8B = (__u8)(u16SrcBuf & 0x001F);
                        u8B = (u8B << 3) | (u8B >> 2);
                        u16SrcBuf >>= 5;
                        u8G = (__u8)(u16SrcBuf & 0x003F);
                        u8G = (u8G << 2) | (u8G >> 4);
                        u16SrcBuf >>= 6;
                        u8R = (__u8)(u16SrcBuf & 0x001F);
                        u8R = (u8R << 3) | (u8R >> 2);

                        u8Y0 = (s_ai32RToY[u8R] + s_ai32GToY[u8G] + s_ai32BToY[u8B]) >> 16;    // -128
                        u8U0 = (s_ai32RToCb[u8R] + s_ai32GToCb[u8G] + s_ai32BToCb[u8B]) >> 16;     // -128
                        u8V0 = (s_ai32RToCr[u8R] + s_ai32GToCr[u8G] + s_ai32BToCr[u8B]) >> 16;       // -128

                        u16SrcBuf = pu16SrcBuf[u16Y * u16Width + u16X + 1];
                        u8B = (__u8)(u16SrcBuf & 0x001F);
                        u8B = (u8B << 3) | (u8B >> 2);
                        u16SrcBuf >>= 5;
                        u8G = (__u8)(u16SrcBuf & 0x003F);
                        u8G = (u8G << 2) | (u8G >> 4);
                        u16SrcBuf >>= 6;
                        u8R = (__u8)(u16SrcBuf & 0x001F);
                        u8R = (u8R << 3) | (u8R >> 2);

                        u8Y1 = (s_ai32RToY[u8R] + s_ai32GToY[u8G] + s_ai32BToY[u8B]) >> 16;    // -128

                        pu8DestBufY[u16Y * u16Width + u16X] = u8Y0;
                        pu8DestBufY[u16Y * u16Width + u16X + 1] = u8Y1;

                	pu8DestBufU[u16Y * u16Width/2 + u16X/2] = u8U0;
                        pu8DestBufV[u16Y * u16Width/2 + u16X/2] = u8V0;

                }
        }
	return;
}

int Write2File_j(char *filename, char *buffer, int len)
{
	int fd;
	unsigned long totalcnt, wlen;
	int ret = 0;
	
	fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC);
	if(fd < 0)
	{
		ret = -1;
		printf("open file fail\n");
		goto out;
	}

	totalcnt = 0;
	while(totalcnt < len)
	{
		wlen = write(fd, buffer + totalcnt, len - totalcnt);
		if(wlen < 0)
		{
			printf("write file error, errno=%d, len=%x\n", errno, wlen);
			ret = -1;
			break;
		}
		totalcnt += wlen;
	}
	close(fd);
out:	
	return ret;
}

int jpegCodec(int mode)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;

	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
		s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
		s_sJpegParam.encode_width = 320;		/* JPEG width */
		s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
printf("jpegCodec 320x240\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
			printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
			printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
			printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 			printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
		fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}

		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}

		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
		printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
			printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
			printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
			printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
			printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

			ret = Write2File_j("./NormalEncodeReserved.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);				

		}
		else
			ret = Write2File_j("./NormalEncode.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}


int jpegCodec_reserved_vpe_QVGA(uint8_t *pu8PicPtr)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	
	mode=TEST_ENCODE_RESERVED;
	
	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
	//	s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
		s_sJpegParam.encode_width = 320;		/* JPEG width */
		s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

			ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}

int jpegCodec_reserved_vpe_QVGA_to_buff(uint8_t *pu8PicPtr, char **dstbuf, int* pdstbuflen,
		int width_af_vpe, int height_af_vpe)
{
	unsigned long BufferSize, bufferCount, readSize;
	int enc_reserved_size;
	int ret = 0;
	int len, jpeginfo_size;
	int width, height, parser;
	FILE *fp;


	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;

	memset((void*) &s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32 );
	s_pJpegInfo = malloc(jpeginfo_size);

	char *filename = "jpegEnc.dat";

	s_sJpegParam.encode = 1; /* Encode */
	s_sJpegParam.src_bufsize = width_af_vpe * height_af_vpe * 2; /* Src buffer (Raw Data) */
	s_sJpegParam.dst_bufsize = 100 * 1024; /* Dst buffer (Bitstream) */
	s_sJpegParam.encode_width = width_af_vpe; /* JPEG width */
	s_sJpegParam.encode_height = height_af_vpe; /* JPGE Height */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET; /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422; /* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

	s_sJpegParam.buffersize = 0; /* only for continuous shot */
	s_sJpegParam.buffercount = 1;

	__u32 u32BitstreamAddr, i;
	__u8 *u8Ptr;
	/* Reserve memory space for user application
	 # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically.
	 # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).
	 # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530).
	 # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream.
	 Byte 0 & 1 :  FF D8
	 Byte 2 & 3 :  FF E0
	 Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
	 Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application

	 Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0);
	 FF D8 FF E0 04 02 [1024 bytes]
	 */
	enc_reserved_size = 1024; /* JPGE Reserved size for user application */
	u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize; /* Bistream Address */
	u8Ptr = (__u8 *) (u32BitstreamAddr + 6); /* Pointer for Reserved memory */

	for (i = 0; i < enc_reserved_size; i++) /* Set data to Reserved Buffer */
		*(u8Ptr + i) = i & 0xFF;

	ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);

	s_sJpegParam.scale = 0; /* Scale function is disabled when scale 0 */

	if (g_qtflag) {
		ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
		ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
		ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
	} else
		ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

	/* Set operation property to JPEG engine */
	if ((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0) {
		fprintf(stderr, "set jpeg param failed:%d\n", errno);
		ret = -1;
		goto out;
	}

	/* Total buffer size for JPEG engine */
	BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);
	if (BufferSize > s_u32JpegBufSize) {
		printf("JPEG Engine Buffer isn't enough\n");
		goto out;
	}

/*	pSRCbuffer = s_pu8JpegEncBuf;
	bufferCount = 0;

	memcpy(pSRCbuffer, pu8PicPtr, s_sJpegParam.src_bufsize);*/
	if ((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0) {
		fprintf(stderr, "trigger jpeg failed:%d\n", errno);
		ret = -1;

		goto out;
	}

	/* Get JPEG Encode information */

	len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	if (len < 0) {
		fprintf(stderr, "No data can get\n");
		if (s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			printf("Memory Stortage\n");
		ret = -1;
		goto out;
	}

	__u32 appMarkerSize;
	__u8 *u8BistreamPtr;
	u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize; /* Bistream Address */
	appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);

	if (*pdstbuflen >= s_pJpegInfo->image_size[0]) {
		*pdstbuflen = s_pJpegInfo->image_size[0];
		*dstbuf = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;
		//memcpy(dstbuf, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
	}else{
		printf("buffer is small,*pdstbuflen=%d  s_pJpegInfo->image_size[0]=%d\n",*pdstbuflen,s_pJpegInfo->image_size[0]);
		ret =-1;
		goto out;
	}


	if (ret < 0) {
		printf("write to file, errno=%d\n", errno);
	}

out:
	free(s_pJpegInfo);

	return ret;

}

int jpegCodec_reserved_vpe_QVGA_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount, readSize;
	int enc_reserved_size;
	int ret = 0;
	int len, jpeginfo_size;
	int width, height, parser;
	FILE *fp;

	//////////////////////////
	if ((pchSaveFolder == NULL))
		return -1;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;

	if (statfs(pchSaveFolder, &sFSStat) < 0) {
		return -1;
	}

	memset((void*) &s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32 );
	s_pJpegInfo = malloc(jpeginfo_size);

	char *filename = "jpegEnc.dat";

	s_sJpegParam.encode = 1; /* Encode */
	s_sJpegParam.src_bufsize = 320 * 240 * 2; /* Src buffer (Raw Data) */
	s_sJpegParam.dst_bufsize = 100 * 1024; /* Dst buffer (Bitstream) */
	s_sJpegParam.encode_width = 320; /* JPEG width */
	s_sJpegParam.encode_height = 240; /* JPGE Height */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET; /* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422; /* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

	s_sJpegParam.buffersize = 0; /* only for continuous shot */
	s_sJpegParam.buffercount = 1;

	__u32 u32BitstreamAddr, i;
	__u8 *u8Ptr;
	/* Reserve memory space for user application
	 # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically.
	 # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).
	 # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530).
	 # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream.
	 Byte 0 & 1 :  FF D8
	 Byte 2 & 3 :  FF E0
	 Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
	 Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application

	 Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0);
	 FF D8 FF E0 04 02 [1024 bytes]
	 */
	enc_reserved_size = 1024; /* JPGE Reserved size for user application */
	u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize; /* Bistream Address */
	u8Ptr = (__u8 *) (u32BitstreamAddr + 6); /* Pointer for Reserved memory */

	for (i = 0; i < enc_reserved_size; i++) /* Set data to Reserved Buffer */
		*(u8Ptr + i) = i & 0xFF;

	ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);

	s_sJpegParam.scale = 0; /* Scale function is disabled when scale 0 */

	if (g_qtflag) {
		ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
		ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
		ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
	} else
		ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

	/* Set operation property to JPEG engine */
	if ((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0) {
		fprintf(stderr, "set jpeg param failed:%d\n", errno);
		ret = -1;
		goto out;
	}

	/* Total buffer size for JPEG engine */
	BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);
	if (BufferSize > s_u32JpegBufSize) {
		printf("JPEG Engine Buffer isn't enough\n");
		goto out;
	}

	pSRCbuffer = s_pu8JpegEncBuf;
	bufferCount = 0;

	memcpy(pSRCbuffer, pu8PicPtr, s_sJpegParam.src_bufsize);
	if ((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0) {
		fprintf(stderr, "trigger jpeg failed:%d\n", errno);
		ret = -1;

		goto out;
	}

	/* Get JPEG Encode information */

	len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	if (len < 0) {
		fprintf(stderr, "No data can get\n");
		if (s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			printf("Memory Stortage\n");
		ret = -1;
		goto out;
	}

	__u32 appMarkerSize;
	__u8 *u8BistreamPtr;
	u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize; /* Bistream Address */
	appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if (err != ERR_DSC_SUCCESS)
		return -1;

	write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();

	if (ret < 0) {
		printf("write to file, errno=%d\n", errno);
	}

out:
	free(s_pJpegInfo);

	return ret;

}

int jpegCodec_reserved_planar_QVGA_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	//////////////////////////
	if((pchSaveFolder == NULL))
	return;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;
		
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	
	mode=TEST_ENCODE_RESERVED;
	


	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
	//	s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
		s_sJpegParam.encode_width = 320;		/* JPEG width */
		s_sJpegParam.encode_height = 240;		/* JPGE Height */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

		///////////////////////
		err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
		if(err != ERR_DSC_SUCCESS)
		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
		//	ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
		//		s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
				
					write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
						///////////////////////
			//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	///////////////////////
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}


int jpegCodec_reserved_planar_VGA_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	//////////////////////////
	if((pchSaveFolder == NULL))
	return;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;
		
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	//u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
	
	mode=TEST_ENCODE_RESERVED;
	


	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
		s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
	//	s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
		s_sJpegParam.encode_width = 640;		/* JPEG width */
		s_sJpegParam.encode_height = 480;		/* JPGE Height */
	//	s_sJpegParam.encode_width = 320;		/* JPEG width */
	//	s_sJpegParam.encode_height = 240;		/* JPGE Height */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

		///////////////////////
		err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
		if(err != ERR_DSC_SUCCESS)
		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
		//	ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
		//		s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
				
					write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
						///////////////////////
			//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	///////////////////////
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}


int jpegCodec_reserved_vpe_VGA_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	//////////////////////////
	if((pchSaveFolder == NULL))
	return;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;
		
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	//u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
	
	mode=TEST_ENCODE_RESERVED;
	


	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
		s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
	//	s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
		s_sJpegParam.encode_width = 640;		/* JPEG width */
		s_sJpegParam.encode_height = 480;		/* JPGE Height */
	//	s_sJpegParam.encode_width = 320;		/* JPEG width */
	//	s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

		///////////////////////
		err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
		if(err != ERR_DSC_SUCCESS)
		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
		//	ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
		//		s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
				
					write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
						///////////////////////
			//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	///////////////////////
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}


int jpegCodec_reserved_vpe_scaled_VGA_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	//////////////////////////
	if((pchSaveFolder == NULL))
	return;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;
		
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	//u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
	
	mode=TEST_ENCODE_RESERVED;
	


	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
	//	s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
		s_sJpegParam.encode_width = 320;		/* JPEG width */
		s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

	//	if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		if (1)	/* Upscale function */
		{
			//s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			//s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scaled_width = 640;	/* Width after scaling */
			s_sJpegParam.scaled_height = 480;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		g_qtflag=1;
		
		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

		///////////////////////
		err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
		if(err != ERR_DSC_SUCCESS)
		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
		//	ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
		//		s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
				
					write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
						///////////////////////
			//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	///////////////////////
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}




int jpegCodec_reserved_vpe_QVGA_cut_file(uint8_t *pu8PicPtr,char *pchSaveFolder)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	//////////////////////////
	if((pchSaveFolder == NULL))
	return;
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	int32_t i32CapFileFD;
	ERRCODE err;
		
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	//u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
	
	mode=TEST_ENCODE_RESERVED;
	


	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
	//	s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
	//	s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.src_bufsize = 180*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
	//	s_sJpegParam.encode_width = 320;		/* JPEG width */
	//	s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_width = 240;		/* JPEG width */
		s_sJpegParam.encode_height = 180;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_THUMBNAIL_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

		///////////////////////
		err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
		if(err != ERR_DSC_SUCCESS)
		return;
	
	
//	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	///////////////////////
		//	ret = Write2File_j("./NormalEncodeReserved_vpe.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
		//		s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
				
					write(i32CapFileFD, s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, s_pJpegInfo->image_size[0]);
						///////////////////////
			//	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
//	if(err != ERR_DSC_SUCCESS)
//		return;
	
	
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	///////////////////////
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}



int jpegCodec_reserved_QVGA_420(uint8_t *pu8PicPtr)
{
	unsigned long BufferSize, bufferCount,readSize;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;	
	int width,height, parser;
	FILE *fp;
	int mode;
	
	mode=TEST_ENCODE_RESERVED;
	
	
	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	//jpeginfo = malloc(jpeginfo_size);
	s_pJpegInfo = malloc(jpeginfo_size);
	//TEST_DECODE_DOWNSCALE

	/* Encode operation Test */
	//TEST_ENCODE_RESERVED
	{
		//char *filename = "./jpegEnc.dat";
		char *filename = "jpegEnc.dat";	

		s_sJpegParam.encode = 1;			/* Encode */
	//	s_sJpegParam.src_bufsize = 640*480*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.src_bufsize = 320*240*2;	/* Src buffer (Raw Data) */
		s_sJpegParam.dst_bufsize = 100*1024;	/* Dst buffer (Bitstream) */
	//	s_sJpegParam.encode_width = 640;		/* JPEG width */
	//	s_sJpegParam.encode_height = 480;		/* JPGE Height */
		s_sJpegParam.encode_width = 320;		/* JPEG width */
		s_sJpegParam.encode_height = 240;		/* JPGE Height */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PACKET;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	//	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	//	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */

		
	
	
		s_sJpegParam.buffersize = 0;		/* only for continuous shot */
    		s_sJpegParam.buffercount = 1;		
	//	printf("jpegCodec 320x240\n");
	//	printf("jpegCodec 640x480\n");
		if(mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr,i;
			__u8 *u8Ptr;
			/* Reserve memory space for user application 
			   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user automatically. 
			   # User can set the data before or after trigger JPEG (Engine will not access the reserved memory space).	
			   # The size parameter is the actual size that can used by user and it must be multiple of 2 but not be multiple of 4 (Max is 65530). 	   
			   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		   		Byte 0 & 1 :  FF D8
				Byte 2 & 3 :  FF E0
				Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		   		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	   	   
		   		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 1024,0); 
			        	FF D8 FF E0 04 02 [1024 bytes]	   	   
			*/
		//	printf("Enable Reserved space function\n");
			enc_reserved_size = 1024;		/* JPGE Reserved size for user application */
		//	printf("Reserved space size %d\n", enc_reserved_size);
		//	u32BitstreamAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u32BitstreamAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;		/* Bistream Address */			
			u8Ptr =(__u8 *) (u32BitstreamAddr + 6);		/* Pointer for Reserved memory */	 

			for(i=0;i<enc_reserved_size;i++)	/* Set data to Reserved Buffer */
				*(u8Ptr + i) = i & 0xFF;
			
			ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);
		//	printf("Bitstream address : 0x%X\n", u32BitstreamAddr);
 		//	printf("Reserved memory : 0x%X~ 0x%X\n", u32BitstreamAddr+ 6, (u32BitstreamAddr + 6 + enc_reserved_size));			
		}

		if (mode == TEST_ENCODE_UPSCALE)	/* Upscale function */
		{
			s_sJpegParam.scaled_width = 800;	/* Width after scaling */
			s_sJpegParam.scaled_height = 600;	/* Height after scaling */
			s_sJpegParam.scale = 1;		/* Enable scale function */	
		}
		else
			s_sJpegParam.scale = 0;		/* Scale function is disabled when scale 0 */		

		if(g_qtflag)
		{
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE0, g_au8QT0);
			ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_QTABLE1, g_au8QT1);
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_USER_QTABLE, 0);
		}
		else
			ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

		/* Set operation property to JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
		{
			fprintf(stderr,"set jpeg param failed:%d\n",errno);
			ret = -1;
			goto out;
		}
		
		/* Total buffer size for JPEG engine */
		BufferSize = (s_sJpegParam.src_bufsize + s_sJpegParam.dst_bufsize);

		//if(BufferSize > jpeg_buffer_size)
		if(BufferSize > s_u32JpegBufSize)
		{
			printf("JPEG Engine Buffer isn't enough\n");
			goto out;
		}
				
		/* Open JPEG file */
	/*	fp = fopen(filename, "r+");
    	
		if(fp == NULL)
    		{
    			printf("open %s error!\n", fp);
    			return 0;
  		}
*/
		pSRCbuffer = s_pu8JpegEncBuf;
		bufferCount = 0;
	
		/* Read Bitstream to JPEG engine src buffer */
		
		/*
		while(!feof(fp))	
		{	
			fd_set writefds;
			struct timeval tv;
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
			FD_ZERO( &writefds );
			FD_SET( s_i32JpegFd , &writefds );
			tv.tv_sec       = 0;
			tv.tv_usec      = 0;
		
			select( s_i32JpegFd + 1 , NULL , &writefds , NULL, &tv );
			if( FD_ISSET( s_i32JpegFd, &writefds ))
			{					
				readSize = fread(pSRCbuffer, 1, 4096 , fp);
		
				pSRCbuffer += readSize;	
				bufferCount += readSize;
			}

			if( bufferCount > s_sJpegParam.src_bufsize)
			{
				printf("Raw Data size is larger than src buffer, %d!!\n",bufferCount);			
				return 0;
			}
		}
*/
		//pu8PicPtr  src
		//pSRCbuffer dst

		memcpy(pSRCbuffer,pu8PicPtr,s_sJpegParam.src_bufsize);
		/* Trigger JPEG engine */
		if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
		{
			fprintf(stderr,"trigger jpeg failed:%d\n",errno);
			ret = -1;

			goto out;
		}

		/* Get JPEG Encode information */

		len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	//	printf("Encode Complete\n");
		
		if(len<0) {
			fprintf(stderr, "No data can get\n");
			if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
				printf("Memory Stortage\n");	
			ret = -1;
			goto out;
		}
		if (mode == TEST_ENCODE_UPSCALE)
			ret = Write2File_j("./EncodeUpscale.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);		
		else if (mode == TEST_ENCODE_RESERVED)
		{
			__u32 u32BitstreamAddr, appMarkerSize;
			__u8 *u8BistreamPtr;
	//		printf("Reserved space information from bitstream\n");
			u8BistreamPtr = s_pu8JpegEncBuf + s_sJpegParam.src_bufsize;	/* Bistream Address */		
			appMarkerSize = (*(u8BistreamPtr + 4) << 8) + *(u8BistreamPtr + 5);
	//		printf("App Marker size (Byte 4 & Byte 5) : %d\n",appMarkerSize);
	//		printf("Reserved size (App Marker size -2): %d\n",appMarkerSize - 2); 
	//		printf("Reserved buffer : Byte 6 ~ %d\n", (6 + appMarkerSize - 2));

			ret = Write2File_j("./NormalEncodeReserved420.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
				s_pJpegInfo->image_size[0]);				
				printf("jpeg frame size :  %d\n", s_pJpegInfo->image_size[0]);
		}
		
		if(ret < 0)
		{
			printf("write to file, errno=%d\n", errno);
		}
	}
out:
	free(s_pJpegInfo);
//	if(mode == TEST_DECODE_OUTPUTWAIT)
//		free(pJpegSWaddr);
	return 0;

}



int jpegEncFB(
char * pVideoBuffer,
int s_i32FBWidth,
int s_i32FBHeight
)
{
	//jpeg_buffer_size
	__u32 u32BitstreamAddr,u32ExifAddr;
	__u8 *u8Ptr;
	__u8 *u8BistreamPtr;
	int enc_reserved_size;
	int ret = 0;
	int i,len, jpeginfo_size;
	int i32ThumbnailOffset, i32ThumbnailSize, i32BitstreamSize, i32PrimaryOffset;	
	FILE *fp;


	
	__u32 xres, yres;
	
	xres = s_i32FBWidth;
	yres = s_i32FBHeight;
	
	/* Convert RGB565 (Frame buffer) to Planar YUV422 (JPEG engine buffer) */
//	_BGR565ToYCbCr((__u16 *)pVideoBuffer, (__u16 *)pJpegBuffer,(__u16)xres, (__u16)yres);
	_BGR565ToYCbCr((__u16 *)pVideoBuffer, (__u16 *)s_pu8JpegEncBuf,(__u16)xres, (__u16)yres);

	memset((void*)&s_sJpegParam, 0, sizeof(jpeg_param_t));
	jpeginfo_size = sizeof(jpeg_info_t) + sizeof(__u32);
	s_pJpegInfo = malloc(jpeginfo_size);

/* Encode Thumbnail First (Downscale 1/2) */
	s_sJpegParam.encode = 1;			/* Encode */
	s_sJpegParam.src_bufsize = xres*yres*2;	/* Src buffer (Raw Data) */
	s_sJpegParam.dst_bufsize = s_u32JpegBufSize - s_sJpegParam.src_bufsize;	/* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
	s_sJpegParam.encode_width = xres;		/* JPEG width */
	s_sJpegParam.encode_height = yres;	/* JPGE Height */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	s_sJpegParam.buffersize = 0;		/* only for continuous shot */
	s_sJpegParam.buffercount = 1;	
	s_sJpegParam.scale = 1;			/* Scale function is disabled when scale 0 */	
	s_sJpegParam.scaled_width = xres / 2;	/* width after scaling */
	s_sJpegParam.scaled_height = yres / 2;	/* height after scaling */

	
	/* Reserve memory space for user application 
	   # Reserve memory space Start address is Bit stream Address + 6 and driver will add the app marker (FF E0 xx xx)for user   
	   # User can get the marker length (reserved length + 2) from byte 4 and byte 5 in the bitstream. 
		Byte 0 & 1 :  FF D8
		Byte 2 & 3 :  FF E0
		Byte 4 & 5 :  [High-byte of Marker Length][Low-byte of Marker Length]
		Byte 6 ~ (Length + 4)  :  [(Marker Length - 2)-byte Data] for user application
	 	   
		Ex : jpegIoctl(JPEG_IOCTL_ENC_RESERVED_FOR_SOFTWARE, 134,0); 
		       	FF D8 FF E0 00 88 [134 bytes for EXIF][Bit stream without FF D8]
	*/
	
	/* Reserved space for EXIF size */
	enc_reserved_size = EXIF_SIZE + 2;		/* JPGE Reserved size for user application (2 is FF D8 for Thumbnail) */

	if(enc_reserved_size % 4)
		enc_reserved_size = (enc_reserved_size + 0x03) & ~0x03;

	ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);	

	ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

	/* Set operation property to JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
	{
		fprintf(stderr,"set jpeg param failed:%d\n",errno);
		ret = -1;
		goto out;
	}
				
	/* Trigger JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
	{
		fprintf(stderr,"trigger jpeg failed:%d\n",errno);
		ret = -1;
		goto out;
	}

	/* Get JPEG Encode information */

	len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	printf("Encode Thumbnail Complete\n");

	if(len<0) {
		fprintf(stderr, "No data can get\n");
		if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			printf("Memory Stortage\n");	
		ret = -1;
		goto out;
	}

	/*
	       	FF D8 FF E0 00 88 [134 bytes for EXIF + 2 Bytes for Thumbnail FF D8][Bit stream without FF D8]
		|<-------6------->|<---------------enc_reserved_size ------------->|<----i32ThumbnailSize--->|
		|<---------------------------------jpeginfo->image_size[0]---------------------------------->|
	*/

	i32ThumbnailSize = s_pJpegInfo->image_size[0] - EXIF_SIZE - 6;
	i32ThumbnailOffset = enc_reserved_size + 6 - 2; 			/* 2 Bytes for Thumbnail FF D8 */

	/* Set New FF D8 to the Thumbnail jpeg */
	//u8Ptr =(__u8 *) (pJpegBuffer + s_sJpegParam.src_bufsize + i32ThumbnailOffset);	 
	u8Ptr =(__u8 *) (s_pu8JpegEncBuf + s_sJpegParam.src_bufsize + i32ThumbnailOffset);	 
	*(u8Ptr) = 0xFF;
	*(u8Ptr + 1) = 0xD8;

//	u32ExifAddr = (__u32) pJpegBuffer + s_sJpegParam.src_bufsize + 6;		/* Bistream Address */		
	u32ExifAddr = (__u32) s_pu8JpegEncBuf + s_sJpegParam.src_bufsize + 6;		/* Bistream Address */			

	/* Set EXIF Header */
	create_simple_EXIF((char *)u32ExifAddr, i32ThumbnailOffset, i32ThumbnailSize);

	printf("Thumbnail Offst 0x%X Size 0x%X\n", i32ThumbnailOffset,i32ThumbnailSize);

	i32PrimaryOffset = s_pJpegInfo->image_size[0];
	
/* Encode Primary */

	/*
	       	FF D8 FF E0 00 88 [134 bytes for EXIF + 2 Bytes for Thumbnail FF D8][Bit stream without FF D8][Primary without FF D8]
		Thumbnail setting  
		|<-------6------->|<------enc_reserved_size for Thumbnail--------->|<----i32ThumbnailSize--->|
		|<---------------------------------jpeginfo->image_size[0]---------------------------------->|
		Primary setting
		|<-------6------->|<-------------------enc_reserved_size for Primary------------------------>|
		|<------------------------------jpeginfo->image_size[0](Thumbnail)-------------------------->|	
	*/
	
	enc_reserved_size = i32PrimaryOffset - 6;

	if(enc_reserved_size % 4)
		enc_reserved_size = (enc_reserved_size + 0x03) & ~0x03;

	s_sJpegParam.encode = 1;			/* Encode */
	s_sJpegParam.src_bufsize = xres*yres*2;	/* Src buffer (Raw Data) */
//	s_sJpegParam.dst_bufsize = jpeg_buffer_size - s_sJpegParam.src_bufsize;	/* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
	s_sJpegParam.dst_bufsize = s_u32JpegBufSize - s_sJpegParam.src_bufsize;	/* Dst buffer size (Decoded Raw data buffer size for JPEG engine) */
	s_sJpegParam.encode_width = xres;		/* JPEG width */
	s_sJpegParam.encode_width = xres;		/* JPEG width */
	s_sJpegParam.encode_height = yres;	/* JPGE Height */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;	/* DRVJPEG_ENC_SRC_PACKET/DRVJPEG_ENC_SRC_PLANAR */
	s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422;	/* DRVJPEG_ENC_PRIMARY_YUV420/DRVJPEG_ENC_PRIMARY_YUV422 */
	s_sJpegParam.buffersize = 0;		/* only for continuous shot */
	s_sJpegParam.buffercount = 1;	
	s_sJpegParam.scale = 0;			/* Scale function is disabled when scale 0 */	

	ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, enc_reserved_size);	

	ioctl(s_i32JpegFd, JPEG_ACTIVE_ENC_DEFAULTQTABLE, 0);

	/* Set operation property to JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0)
	{
		fprintf(stderr,"set jpeg param failed:%d\n",errno);
		ret = -1;
		goto out;
	}
	
	/* Trigger JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0)
	{
		fprintf(stderr,"trigger jpeg failed:%d\n",errno);
		ret = -1;
		goto out;
	}

	/* Get JPEG Encode information */

	len = read(s_i32JpegFd, s_pJpegInfo, jpeginfo_size);
	printf("Primary Encode Complete\n");

	i32BitstreamSize = s_pJpegInfo->image_size[0];

	/* Set New FF E1 for EXIF app marker */
	//u8Ptr =(__u8 *) (pJpegBuffer + s_sJpegParam.src_bufsize + 3);	 
	u8Ptr =(__u8 *) (s_pu8JpegEncBuf + s_sJpegParam.src_bufsize + 3);	 
	*(u8Ptr) = 0xE1;

	if(len<0) {
		fprintf(stderr, "No data can get\n");
		if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			printf("Memory Stortage\n");	
		ret = -1;
		goto out;
	}

//	ret = Write2File("./Snapshotjpeg.jpg", pJpegBuffer + s_sJpegParam.src_bufsize, 
//	i32BitstreamSize);		

	ret = Write2File_j("./Snapshotjpeg.jpg", s_pu8JpegEncBuf + s_sJpegParam.src_bufsize, 
	i32BitstreamSize);		

	if(ret < 0)
	{
		printf("write to file, errno=%d\n", errno);
	}
	printf("Please check ./Snapshot.jpg\n");


out:
	free(s_pJpegInfo);

}




int 
JpegEncFromVIN_Planar(E_IMAGE_RESOL eEncodeImgResol,
	uint32_t u32PlanarFmt,			/* Planar YUV422 or YUV420 */	
	uint32_t u32PlanarBufPhyAddr,	/* src pattern address*/
	uint8_t **ppu8JpegEncBuf,
	uint32_t *pu32JpegEncLen,
	S_JPEG_ENC_FEAT *psJpegEncFeat,
	struct v4l2_crop *psVideoCrop
)
{
	uint32_t u32Width, u32Height;	
	uint8_t *pu8EncBitStmAddr;
	uint8_t *pu8ReservedAddr = NULL;
	int32_t i32Len;
	uint32_t u32UAddr, u32VAddr;

	if(eEncodeImgResol == eIMAGE_WQVGA){
		u32Width = 480;				
		u32Height = 272;					
	}else if(eEncodeImgResol == eIMAGE_VGA){
		u32Width = 640;
		u32Height = 480;		
	}else if(eEncodeImgResol == eIMAGE_SVGA){
		u32Width = 800;
		u32Height = 600;
	}else if(eEncodeImgResol == eIMAGE_HD720){
		u32Width = 1280;
		u32Height = 720;
	}else{
		u32Width = 320;				
		u32Height = 240;				
	} 
	
	
	DBG_POINT();
	if(u32PlanarFmt == VIDEO_PALETTE_YUV422P)
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV422; 	/* JPEG Output format */
	else
		s_sJpegParam.encode_image_format = DRVJPEG_ENC_PRIMARY_YUV420; 	/* JPEG Output format */
	s_sJpegParam.encode_source_format = DRVJPEG_ENC_SRC_PLANAR;		/* Source is Packet Format */	

	/* Set encode source stride (Must calling after IOCTL - JPEG_S_PARAM) */
	ioctl(s_i32JpegFd, JPEG_SET_ENC_STRIDE, u32Width);

	pu8EncBitStmAddr = s_pu8JpegEncBuf;

	if(psJpegEncFeat->eJpegEncThbSupp){
	/* Set reserved size for exif */
		int32_t i32EncReservedSize = 0x84;
		pu8ReservedAddr = pu8EncBitStmAddr + 6;
		memset((char *)pu8ReservedAddr, 0xFF, i32EncReservedSize);
		ioctl(s_i32JpegFd, JPEG_SET_ENCOCDE_RESERVED, i32EncReservedSize); 
	}


#if 1
	if((psVideoCrop->c.width <  u32Width) ||
		(psVideoCrop->c.height < u32Height) )
	{
		s_sJpegParam.scaled_width = u32Width;	 
		s_sJpegParam.scaled_height = u32Height;
		s_sJpegParam.encode_width = psVideoCrop->c.width;
		s_sJpegParam.encode_height = psVideoCrop->c.height;					
		s_sJpegParam.scale = 1;				
		printf("JPEG encode source WxH= %d x %d\n", s_sJpegParam.encode_width, s_sJpegParam.encode_height);
		printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);	
	}	
	else
	{
		s_sJpegParam.scale = 0; 
		s_sJpegParam.encode_width = u32Width;
		s_sJpegParam.encode_height = u32Height;
		printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);		
	}
#else
	s_sJpegParam.scale = 0; 
	s_sJpegParam.encode_width = u32Width;
	s_sJpegParam.encode_height = u32Height;
	printf("JPEG encode target WxH= %d x %d\n", u32Width, u32Height);	
#endif		

#if 1						
	if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_YADDRESS, u32PlanarBufPhyAddr)) < 0){
		ERR_PRINT("Set jpeg Y address failed:%d\n",errno);
		return -1;
	}
	u32UAddr = u32PlanarBufPhyAddr+s_sJpegParam.encode_width*s_sJpegParam.encode_height; 
	if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_UADDRESS, u32UAddr )) < 0){
		ERR_PRINT("Set jpeg U address failed:%d\n",errno);
		return -1;
	}
	if(u32PlanarFmt == VIDEO_PALETTE_YUV422P)
		u32VAddr = u32UAddr + s_sJpegParam.encode_width * s_sJpegParam.encode_height/2;
	else
		u32VAddr = u32UAddr + s_sJpegParam.encode_width * s_sJpegParam.encode_height/4;
	if((ioctl(s_i32JpegFd, JPEG_SET_ENC_USER_VADDRESS, u32VAddr)) < 0){
		ERR_PRINT("Set jpeg V address failed:%d\n",errno);
		return -1;
	}	
	printf("New Encode Planar Y/U/V = 0x%x, 0x%x, 0x%x\n", u32PlanarBufPhyAddr, u32UAddr, u32VAddr);
#endif 
	DBG_POINT();
	/* Set operation property to JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_S_PARAM, &s_sJpegParam)) < 0){
		ERR_PRINT("Set jpeg parame failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();

	/* Set Encode Source from VideoIn */
	//ioctl(s_i32JpegFd,JPEG_SET_ENC_SRC_FROM_VIN,NULL);	

	if(psJpegEncFeat->eJpegEncThbSupp){

		if(psJpegEncFeat->eJpegEncThbSupp == eJPEG_ENC_THB_QQVGA){
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 1);	
		}
		else{
			ioctl(s_i32JpegFd,JPEG_SET_ENC_THUMBNAIL, 0);	
		}

	}
	DBG_POINT();
	/* Trigger JPEG engine */
	if((ioctl(s_i32JpegFd, JPEG_TRIGGER, NULL)) < 0){
		ERR_PRINT("Trigger jpeg failed:%d\n",errno);
		return -1;
	}
	DBG_POINT();
	i32Len = read(s_i32JpegFd, s_pJpegInfo, sizeof(jpeg_info_t) + sizeof (__u32));
	
	if(i32Len < 0){
		ERR_PRINT("No data can get\n");
		if(s_pJpegInfo->state == JPEG_MEM_SHORTAGE)
			ERR_PRINT("Memory Stortage\n");	
		return -1;
	}

	if(psJpegEncFeat->eJpegEncThbSupp){
		int32_t i32ThumbnailSize;
		int32_t i32ThumbnailOffset;
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_SIZE, &i32ThumbnailSize);
		ioctl(s_i32JpegFd, JPEG_GET_ENC_THUMBNAIL_OFFSET, &i32ThumbnailOffset);
		create_simple_EXIF((char *)pu8ReservedAddr, i32ThumbnailOffset,i32ThumbnailSize);
	}

	*ppu8JpegEncBuf = s_pu8JpegEncBuf;
	*pu32JpegEncLen = s_pJpegInfo->image_size[0];
	return 0;
}


int 
FinializeJpegDevice(void)
{
	if(s_pJpegInfo){
		free(s_pJpegInfo);
		s_pJpegInfo = NULL;
	}

	if(s_pu8JpegEncBuf !=  MAP_FAILED){
		munmap(s_pu8JpegEncBuf, s_u32JpegBufSize);
		s_pu8JpegEncBuf = MAP_FAILED;	
	}

	if(s_i32JpegFd > 0){
		close(s_i32JpegFd);
		s_i32JpegFd = -1;
	}
	return 0;
}

