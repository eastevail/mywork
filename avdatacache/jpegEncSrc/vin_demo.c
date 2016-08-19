/* vin_demo.c
 *
 *
 * Copyright (c)2008 Nuvoton technology corporation
 * http://www.nuvoton.com
 *
 * vin_demo application
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
#include <linux/input.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/statfs.h>
#include <linux/fb.h>
#include <wchar.h>
#include <linux/videodev.h>
#include <signal.h>

#include "dsc.h"
#include "FB_IOCTL.h"
#include "JpegEnc.h"
#include "Misc.h"
#include "V4L.h"
#include "vin_demo.h"
#include "w55fa92_vpe.h"
#define DEFAULT_SAVE_FOLDER "/mnt/sdcard/dsc"

/*static int savefd;*/
extern uint8_t *s_pu8JpegEncBuf;
extern uint32_t s_u32JpegBufSize;
int s_i32FBWidth;
int s_i32FBHeight;
static S_JPEG_ENC_FEAT s_sJpegEncFeat;

static struct v4l2_cropcap 	s_sVideoCropCap;
static struct v4l2_crop		s_sVideoCrop;


#define push_out()		printf("                                                                        \n");

// VPE
extern int vpe_fd;
extern int InitVPE(void);
extern int FormatConversion_up(char* pSrcBuf, char* pDstBuf, int SrcWidth, int SrcHeight, int Tarwidth, int Tarheight);
extern int FormatConversion_down(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);

extern int FormatConversion_up_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_right_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_down_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_left_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);

extern int FormatConversion_up_VGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);

extern int FormatConversion_up_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_right_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_down_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);
extern int FormatConversion_left_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight);

extern int scalePro(tb_vpe_transform_t* pvpe);
uint32_t
GetTime(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


static int 
InitKpdDevice(void)
{
	char achDevName[256];
	char achKeypad[] = "Keypad";
	char achKpdDevice[] = "/dev/input/event0";
	int32_t i32KpdFd= -1;
	uint32_t u32Result;
	int32_t i;	

	u32Result = 0;
	for (i = 0; i < RETRY_NUM; i++) {
		DEBUG_PRINT("trying to open %s ...\n", achKpdDevice);
		if ((i32KpdFd=open(achKpdDevice, O_RDONLY | O_NONBLOCK)) < 0) {
			break;
		}
		memset(achDevName, 0x0, sizeof(achDevName));
		if (ioctl(i32KpdFd, EVIOCGNAME(sizeof(achDevName)), achDevName) < 0) {
			DEBUG_PRINT("%s evdev ioctl error!\n", achKpdDevice);
		}
		else{
			if (strstr(achDevName, achKeypad) != NULL) {
				DEBUG_PRINT("Keypad device is %s\n", achKpdDevice);
				u32Result = 1;
				break;
			}
		}
		close(i32KpdFd);
		achKpdDevice[16]++;
	}

	if (u32Result == 0) {
		DEBUG_PRINT("can not find any Keypad device!\n");
		return -1;
	}

	return i32KpdFd;
}

static int
GetKey(
	int32_t i32KpdFd,
	int32_t *pi32KeyCode
)
{
	struct input_event sKpdData;

	read(i32KpdFd,&sKpdData,sizeof(sKpdData));
	if(sKpdData.type == 0){
		return -1;
	}
	if ((sKpdData.value != 0)){
		return -1;
	}
	*pi32KeyCode = sKpdData.code;

	return 0;
}

static int 
OSDCheckExist(
	int i32FBFd 
)
{
#if 0
	osd_cmd_t sOSDCmd;
	int i32Err;
	sOSDCmd.cmd = OSD_Open;			
	i32Err = ioctl(i32FBFd,OSD_SEND_CMD, (unsigned long) &sOSDCmd);	
	if(i32Err >= 0){
		DEBUG_PRINT("OSD enable\n");
		return 1;	
	}
	else{
		DEBUG_PRINT("OSD disable\n");
		return 0;	
	}
#else
	return 0;
#endif
}

void 
OSDInit(
	int i32FBFd, 
	int i32Width, 
	int i32Height,
	uint8_t *pu8OSDBuff
)
{
	// STEP: set color key to white(0xFFFF)
		
	osd_cmd_t sOSDCmd;
	FILE *fpOSDImg = NULL;	

	sOSDCmd.cmd = OSD_SetTrans;
	sOSDCmd.color = 0xffff;
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);
	
	// STEP: clear all of OSD buffer
	sOSDCmd.cmd = OSD_Open;			
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);
	sOSDCmd.cmd = OSD_Clear;    			
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);

	sOSDCmd.cmd = OSD_Fill;
	sOSDCmd.x0 = 0;		// OSD_Fill command test
	sOSDCmd.y0 = 0;
	sOSDCmd.x0_size = i32Width;
	sOSDCmd.y0_size = i32Height;
	sOSDCmd.color = 0x0;
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);

	// STEP: enable OSD
	sOSDCmd.cmd = OSD_Show;
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);

	// OSD is replaced by another image 
	sOSDCmd.cmd = OSD_SetTrans;		
	sOSDCmd.color = 0x0000;		
	ioctl(i32FBFd, OSD_SEND_CMD, (unsigned long) &sOSDCmd);
	

	if( (i32Width==480) && (i32Height==272))
		fpOSDImg = fopen("OSD_480_272_RGB565.bin", "r");	
	if( (i32Width==320) && (i32Height==240))
		fpOSDImg = fopen("OSD_320_240_RGB565.dat", "r");	
	if( (i32Width==800) && (i32Height==480))
		fpOSDImg = fopen("OSD_800_480_RGB565.dat", "r");

	if(fpOSDImg == NULL){
		DEBUG_PRINT("open OSD FILE fail !! \n");
		return;
	}  
    
	if( fread(pu8OSDBuff, (i32Width*i32Height*2), 1, fpOSDImg) <= 0){
		DEBUG_PRINT("Cannot Read the OSD File!\n");
		return;
	}
	fclose(fpOSDImg); 
}
static unsigned char *backupVideoBuffer;
static int 
InitFBDevice(
	uint8_t **ppu8FBBuf,
	uint32_t *pu32FBBufSize
)
{
	int32_t i32FBFd;
	uint8_t *pu8FBBuf;
	char achFBDevice[] = "/dev/fb0";
	static struct fb_var_screeninfo sVar;

	*ppu8FBBuf = MAP_FAILED;
	*pu32FBBufSize = 0;

	i32FBFd = open(achFBDevice, O_RDWR);
	if(i32FBFd < 0){
		DEBUG_PRINT("Open FB fail\n");
		push_out();		
		return -1;
	}

	printf("fb open successful\n");
	push_out();
	if (ioctl(i32FBFd, FBIOGET_VSCREENINFO, &sVar) < 0) {
		DEBUG_PRINT("ioctl FBIOGET_VSCREENINFO \n");
		push_out();
		close(i32FBFd);
		return -1;
	}

	s_i32FBWidth = sVar.xres;
	s_i32FBHeight = sVar.yres;
	if(OSDCheckExist(i32FBFd)){
		pu8FBBuf = mmap( NULL, s_i32FBWidth*s_i32FBHeight*2*2 , PROT_READ|PROT_WRITE, MAP_SHARED, i32FBFd, 0 );
		if (MAP_FAILED == pu8FBBuf) {
			close(i32FBFd);		
			return -1;
		}
		*pu32FBBufSize = s_i32FBWidth*s_i32FBHeight*2*2 ;
		OSDInit(i32FBFd, s_i32FBWidth, s_i32FBHeight, pu8FBBuf + (s_i32FBWidth*s_i32FBHeight*2));	
	}
	else {
		pu8FBBuf = mmap( NULL, s_i32FBWidth*s_i32FBHeight*2 , PROT_READ|PROT_WRITE, MAP_SHARED, i32FBFd, 0 );
		if ((unsigned char*)-1 == pu8FBBuf) {
			close(i32FBFd);
			return -1;
		}
		*pu32FBBufSize = s_i32FBWidth*s_i32FBHeight*2;
	}
	//////////////////////////////////////
	//ioctl(i32FBFd, IOCTL_LCD_DISABLE_INT, 0);
	//usleep(50000);		/* Delay 50ms to not update the off-line buffer to on line buffer */


	//ioctl(i32FBFd, VIDEO_FORMAT_CHANGE, DISPLAY_MODE_RGB565);
	//2012-0808 wmemset((wchar_t *)pu8FBBuf, 0x80108010, (s_i32FBWidth*s_i32FBHeight*2)/4);		

	//ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	//////////////////////////////////////
	*ppu8FBBuf = pu8FBBuf; 
	return i32FBFd;
}

static void
ClearFB(
	int32_t i32FBFd,
	uint8_t *pu8FBBufAddr
)
{
	ioctl(i32FBFd, IOCTL_LCD_DISABLE_INT, 0);	
	wmemset((wchar_t *)pu8FBBufAddr, 0x80008000, (s_i32FBWidth*s_i32FBHeight*2)/4);
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
}

static void
CaptureImage(E_IMAGE_RESOL eEncodeImgResol,
	char *pchSaveFolder,
	int32_t i32FBFd,
	uint8_t *pu8FBBufAddr
)
{
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	uint8_t *pu8JpegBuff;
	uint32_t u32JpegEncLen;
	ERRCODE err;
	int32_t i32CapFileFD;
	uint8_t *pu8PicPtr;
	uint64_t u64TS;
	uint32_t u32PicPhyAdr;


	if((pchSaveFolder == NULL))
		return;
//	disable preview
	StopPreview();
	ClearFB(i32FBFd, pu8FBBufAddr);
	usleep(250000); //250ms
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	enable preview
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartPreview();
	if(ReadV4LPicture(&pu8PicPtr, &u64TS, &u32PicPhyAdr) == ERR_V4L_SUCCESS){ 
		// got the planar buffer address ==> u32PicPhyAdr
	}

	if(QueryV4LZoomInfo(&s_sVideoCropCap, &s_sVideoCrop) != ERR_V4L_SUCCESS)
		goto exit_capture_image;
	// Encode jpeg
	if(JpegEncFromVIN(eEncodeImgResol, u32PicPhyAdr, &pu8JpegBuff, &u32JpegEncLen, &s_sJpegEncFeat, &s_sVideoCrop) < 0)
		goto exit_capture_image;

	TriggerV4LNextFrame();
	StopV4LCapture();
	if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
		goto exit_capture_image;

	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if(err != ERR_DSC_SUCCESS)
		goto exit_capture_image;

	write(i32CapFileFD, pu8JpegBuff, u32JpegEncLen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
exit_capture_image:
	
	usleep(1000000); //1 sec
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartV4LCapture();
}

int Write2File(int fd, char *buffer, int len)
{
	//int fd;
	unsigned long totalcnt, wlen;
	int ret = 0;
	
	//fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC);
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
	//close(fd);
out:	
	return ret;
}

static void
CaptureImageToAVI(E_IMAGE_RESOL eEncodeImgResol,
	uint32_t u32PicPhyAdr,
	int savefd
)
{
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	uint8_t *pu8JpegBuff;
	uint32_t u32JpegEncLen;
	ERRCODE err;
	int32_t i32CapFileFD;
	uint8_t *pu8PicPtr;
	uint64_t u64TS;
//	uint32_t u32PicPhyAdr;

/*
	if((pchSaveFolder == NULL))
		return;
//	disable preview
	StopPreview();
	ClearFB(i32FBFd, pu8FBBufAddr);
	usleep(250000); //250ms
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;
//	enable preview
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartPreview();
	if(ReadV4LPicture(&pu8PicPtr, &u64TS, &u32PicPhyAdr) == ERR_V4L_SUCCESS){ 
		// got the planar buffer address ==> u32PicPhyAdr
	}
*/
	if(QueryV4LZoomInfo(&s_sVideoCropCap, &s_sVideoCrop) != ERR_V4L_SUCCESS)
		goto exit_capture_image;
	// Encode jpeg
	if(JpegEncFromVIN(eEncodeImgResol, u32PicPhyAdr, &pu8JpegBuff, &u32JpegEncLen, &s_sJpegEncFeat, &s_sVideoCrop) < 0)
		goto exit_capture_image;
/*
	TriggerV4LNextFrame();
	StopV4LCapture();
	if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
		goto exit_capture_image;

	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if(err != ERR_DSC_SUCCESS)
		goto exit_capture_image;

	write(i32CapFileFD, pu8JpegBuff, u32JpegEncLen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	*/
	//write file
	////////////////////////////////
	
	//if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
	//	goto exit_capture_image;
	
	if(Write2File(savefd, pu8JpegBuff, u32JpegEncLen)<0)
	{
		goto exit_capture_image;	
	}	
	////////////////////////////////
	//sync();
exit_capture_image:
	
//	usleep(1000000); //1 sec
	//ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	//StartV4LCapture();
	return;
}

static void
CaptureImage_Packet(E_IMAGE_RESOL eEncodeImgResol,
	char *pchSaveFolder,
	int32_t i32FBFd,
	uint8_t *pu8FBBufAddr
)
{
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	uint8_t *pu8JpegBuff;
	uint32_t u32JpegEncLen;
	ERRCODE err;
	int32_t i32CapFileFD;
	
	S_PIPE_INFO s_packetInfo;

	if((pchSaveFolder == NULL))
		return;

	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;

	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartPreview();

	if(QueryV4LPacketInformation(&s_packetInfo)!=ERR_V4L_SUCCESS)
	{
		printf("Query packet information fail\n");
		goto exit_capture_image;
	}

	if(JpegEncFromVIN_Packet(eEncodeImgResol, s_packetInfo.i32CurrPipePhyAddr,\
								&pu8JpegBuff, &u32JpegEncLen, &s_sJpegEncFeat, &s_sVideoCrop) < 0)
	{
		printf("Encode from packet pipe fail\n");	
		goto exit_capture_image;
	}
	TriggerV4LNextFrame();
	StopV4LCapture();
	if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
		goto exit_capture_image;

	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if(err != ERR_DSC_SUCCESS)
		goto exit_capture_image;

	write(i32CapFileFD, pu8JpegBuff, u32JpegEncLen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
exit_capture_image:
	
	usleep(1000000); //1 sec
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartV4LCapture();
}


static void
CaptureImage_Packet_VGA(E_IMAGE_RESOL eEncodeImgResol,
	char *pchSaveFolder,
	int32_t i32FBFd,
	uint8_t *pu8FBBufAddr
)
{
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	uint8_t *pu8JpegBuff;
	uint32_t u32JpegEncLen;
	ERRCODE err;
	int32_t i32CapFileFD;
	
	S_PIPE_INFO s_packetInfo;

	if((pchSaveFolder == NULL))
		return;

	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;

	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartPreview();

	if(QueryV4LPacketInformation(&s_packetInfo)!=ERR_V4L_SUCCESS)
	{
		printf("Query packet information fail\n");
		goto exit_capture_image;
	}

	if(JpegEncFromVIN_Packet(eEncodeImgResol, s_packetInfo.i32CurrPipePhyAddr,\
								&pu8JpegBuff, &u32JpegEncLen, &s_sJpegEncFeat, &s_sVideoCrop) < 0)
	{
		printf("Encode from packet pipe fail\n");	
		goto exit_capture_image;
	}
	TriggerV4LNextFrame();
	StopV4LCapture();
	if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
		goto exit_capture_image;

	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if(err != ERR_DSC_SUCCESS)
		goto exit_capture_image;

	write(i32CapFileFD, pu8JpegBuff, u32JpegEncLen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
exit_capture_image:
	
	usleep(1000000); //1 sec
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	StartV4LCapture();
}

static void
CaptureImage_Planar(E_IMAGE_RESOL eEncodeImgResol,
					char *pchSaveFolder,
					int32_t i32FBFd,
					uint8_t *pu8FBBufAddr)
{
	struct statfs sFSStat;
	uint64_t u64DiskAvailSize;
	uint8_t *pu8JpegBuff;
	uint32_t u32JpegEncLen;
	ERRCODE err;
	int32_t i32CapFileFD;
	static uint32_t u32Fmt=VIDEO_PALETTE_YUV422P;	
	S_PIPE_INFO s_planarInfo;
	uint32_t u32EncodeImgWidth, u32EncodeImgHeight;
	
	int8_t  i8count;		

	if((pchSaveFolder == NULL))
		return;

	if(eEncodeImgResol == eIMAGE_QVGA){
		u32EncodeImgWidth = 320;
		u32EncodeImgHeight = 240;
	}else if(eEncodeImgResol == eIMAGE_WQVGA){
		u32EncodeImgWidth = 480;
		u32EncodeImgHeight = 272;
	}else if(eEncodeImgResol == eIMAGE_VGA){
		u32EncodeImgWidth = 640;
		u32EncodeImgHeight = 480;
	}else if(eEncodeImgResol == eIMAGE_SVGA){
		u32EncodeImgWidth = 800;
		u32EncodeImgHeight = 600;
	}else if(eEncodeImgResol == eIMAGE_HD720){
		u32EncodeImgWidth = 1280;
		u32EncodeImgHeight = 720;
	}

#if 0	
	StopV4LCapture();	
	if(u32Fmt==VIDEO_PALETTE_YUV422P){
		printf("Set planar 420 format\n");
		SetV4LEncode(0, u32EncodeImgWidth, u32EncodeImgHeight, VIDEO_PALETTE_YUV420P);
		u32Fmt = VIDEO_PALETTE_YUV420P;
	}	
	else if(u32Fmt==VIDEO_PALETTE_YUV420P){
		printf("Set planar 422 format\n");
		SetV4LEncode(0, u32EncodeImgWidth, u32EncodeImgHeight, VIDEO_PALETTE_YUV422P);
		u32Fmt = VIDEO_PALETTE_YUV422P;
	}
	StartV4LCapture();	/* Start capture to set planar buffer base on specified */
	/* if change format, need wait for one~two frame */
	for(i8count=2 ;i8count>0; i8count--)
	{
		printf("W %d\n", i8count);
		TriggerV4LNextFrame();
	}
#endif 
	if(statfs(pchSaveFolder, &sFSStat) < 0){
		return;
	}
	u64DiskAvailSize = (uint64_t)sFSStat.f_bavail * (uint64_t)sFSStat.f_bsize;

	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
//	StartPreview();

	if(QueryV4LPlanarInformation(&s_planarInfo)!=ERR_V4L_SUCCESS){
		printf("Query planar information fail\n");
		goto exit_capture_image;
	}
	if(QueryV4LZoomInfo(&s_sVideoCropCap, &s_sVideoCrop) != ERR_V4L_SUCCESS){
		printf("Query zoom information fail\n");
		goto exit_capture_image;
	}
	printf("s_planarInfo.i32CurrPipePhyAddr = 0x%x\n", s_planarInfo.i32CurrPipePhyAddr);
	if(JpegEncFromVIN_Planar(eEncodeImgResol, u32Fmt,
								s_planarInfo.i32CurrPipePhyAddr,\
								&pu8JpegBuff, &u32JpegEncLen, &s_sJpegEncFeat, &s_sVideoCrop) < 0)
	{
		printf("Encode from packet pipe fail\n");	
		goto exit_capture_image;
	}
	TriggerV4LNextFrame();
	//sw StopV4LCapture();
	if(u64DiskAvailSize <= (uint64_t)u32JpegEncLen)
		goto exit_capture_image;

	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if(err != ERR_DSC_SUCCESS)
		goto exit_capture_image;

	write(i32CapFileFD, pu8JpegBuff, u32JpegEncLen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
exit_capture_image:
	
	usleep(1000000); //1 sec
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	//sw StartV4LCapture();
}

static void ShowUsage()
{
	printf("vin_demo [options]\n");
	printf("-s qvga: Preview image resolution\n");
	printf("-e qvga: Encode image resolution\n");
	printf("-p /mnt/sdcard/vin_demo: Output folder\n");
	printf("-t qqvga/qvga :jpeg thumbnail encode support\n");
	printf("-u :jpeg upscale function. Encode image width*2 and height*2 \n");
	printf("-h : Help\n");
}
int32_t i32FBFd;
//int32_t i32KpdFd;
uint8_t *pu8FBBufAddr;
uint32_t u32FBBufSize;
/*char *pchSaveFolder = NULL;*/
static void myExitHandler (int sig)
{
	
	/////////save avi
/*	close(savefd);*/
/*	sync();*/
	/////////save avi
	ioctl(i32FBFd, IOCTL_LCD_DISABLE_INT, 0);
	StopV4LCapture();	
	printf("Clear Frame Buffer Addr = 0x%x\n",  (uint32_t)pu8FBBufAddr);
	wmemset((wchar_t *)pu8FBBufAddr, 0x00000000, (s_i32FBWidth*s_i32FBHeight*2)/4);
	FinializeJpegDevice();
	FinializeV4LDevice();
	ioctl(i32FBFd, VIDEO_FORMAT_CHANGE, DISPLAY_MODE_RGB565);
	munmap(pu8FBBufAddr, u32FBBufSize);
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	close(i32FBFd);
	DEBUG_PRINT();
	////////////////
	//memcpy((void*)pu8FBBufAddr, (char*)backupVideoBuffer, u32FBBufSize);
	//free(backupVideoBuffer);
	DEBUG_PRINT();
	////////////////////////////
//	close(i32KpdFd);

	return ;
}


static void getfps()
{
	static int fr_calc = 0;
    static double pre_time = 0;

    double fps = 0;
    double cur_time = 0;
    struct timeval tm_now;

    fr_calc += 1;

    gettimeofday(&tm_now, NULL);
    cur_time = (double)(tm_now.tv_sec + (tm_now.tv_usec / 1000000.0));

    if (0 == pre_time)
    {
        pre_time = cur_time;
    }
    else
    {
        if ((cur_time - pre_time) >= 10)
        {
            fps = (fr_calc / (cur_time - pre_time));
     
            fr_calc = 0;
            pre_time = cur_time;

           
       
            printf("stream info : fps= %7f \n", fps);
        }
    }
}

#define SRCBUFSIZE	(640*480*2)

void loadSrcPattern(char *p8SrcBuf)
{
	FILE *fpr;
	int ret;

	printf("The pattern \"src.pb\" and application \"vpe_demo\" have to store in the same directory.\n");
	fpr = fopen("src.pb", "rb");
	if(!fpr){
		printf("Open read file  error \n"); 
		exit(-1);
	}
	fread(p8SrcBuf, 1, (320*240*2), fpr);
	fclose(fpr);
	sync();
}

static uint8_t *pu8PicPtr;
static uint64_t u64TS;
static uint32_t u32PicPhyAdr;
static char *p8SrcBuf_black;
static uint32_t u32EncodeImgWidth=0, u32EncodeImgHeight=0;
int initJpegPro(E_IMAGE_RESOL resol)
{
	int32_t i32Opt;
	E_IMAGE_RESOL ePreviewImgResol = resol;
//	E_IMAGE_RESOL eEncodeImgResol = eIMAGE_VGA;
	E_IMAGE_RESOL eEncodeImgResol = resol;
	
	int32_t i32SensorId;
	//S_PIPE_INFO s_packetInfo;
	int32_t i32Ret = 0;
	volatile uint32_t u32MDJudgeTime;
	volatile uint32_t u32CurrTime;
	
	int32_t i32KeyCode;


	uint32_t u32PreviewImgWidth=0, u32PreviewImgHeight=0;


/*	signal (SIGINT, myExitHandler);
	//signal (SIGQUIT, myExitHandler);
 	//signal (SIGILL, myExitHandler);
  	//signal (SIGABRT, myExitHandler);
  	//signal (SIGFPE, myExitHandler);
  	signal (SIGKILL, myExitHandler);
	//signal (SIGPIPE, myExitHandler);
	signal (SIGTERM, myExitHandler);*/

	DEBUG_PRINT("\n");
	s_sJpegEncFeat.eJpegEncThbSupp = eJPEG_ENC_THB_NONE;
	s_sJpegEncFeat.bUpScale = FALSE;

	// Parse options

	if(ePreviewImgResol == eIMAGE_WQVGA){
		u32PreviewImgWidth = 480;
		u32PreviewImgHeight = 272;
	}
	else if(ePreviewImgResol == eIMAGE_VGA){
		u32PreviewImgWidth = 640;
		u32PreviewImgHeight = 480;
	}
	else if(ePreviewImgResol == eIMAGE_QVGA){
		u32PreviewImgWidth = 320;
		u32PreviewImgHeight = 240;
	}
	else if(ePreviewImgResol == eIMAGE_SVGA){
		u32PreviewImgWidth = 800;
		u32PreviewImgHeight = 600;
	}
	else if(ePreviewImgResol == eIMAGE_HD720){
		u32PreviewImgWidth = 1280;
		u32PreviewImgHeight = 720;
	}

	if(eEncodeImgResol == eIMAGE_QVGA){
		u32EncodeImgWidth = 320;
		u32EncodeImgHeight = 240;
	}else if(eEncodeImgResol == eIMAGE_WQVGA){
		u32EncodeImgWidth = 480;
		u32EncodeImgHeight = 272;
	}else if(eEncodeImgResol == eIMAGE_VGA){
		u32EncodeImgWidth = 640;
		u32EncodeImgHeight = 480;
	}else if(eEncodeImgResol == eIMAGE_SVGA){
		u32EncodeImgWidth = 800;
		u32EncodeImgHeight = 600;
	}else if(eEncodeImgResol == eIMAGE_HD720){
		u32EncodeImgWidth = 1280;
		u32EncodeImgHeight = 720;
	}

	if(s_sJpegEncFeat.eJpegEncThbSupp){
		if(eEncodeImgResol == eIMAGE_WQVGA){
			ERR_PRINT("Error on encode image resolution\n");
			return -1;
		}
	}

	DEBUG_PRINT("Set preview image resolution %d \n", ePreviewImgResol);
	DEBUG_PRINT("Set encode image resolution %d \n", eEncodeImgResol);

	if(s_sJpegEncFeat.eJpegEncThbSupp == eJPEG_ENC_THB_QQVGA){
		DEBUG_PRINT("Enable thumbnail QQVGA support in JPEG file \n");	
	}
	else if(s_sJpegEncFeat.eJpegEncThbSupp == eJPEG_ENC_THB_QVGA){
		DEBUG_PRINT("Enable thumbnail QVGA support in JPEG file \n");	
	}
	if(s_sJpegEncFeat.bUpScale){
		DEBUG_PRINT("Enable JPEG upscale support\n");	
	}
/*
	i32KpdFd = InitKpdDevice();
	if(i32KpdFd < 0){
		i32Ret = -1;
		goto exit_prog;
	}*/
	i32FBFd = InitFBDevice(&pu8FBBufAddr, &u32FBBufSize);
	printf("FB = %d\n",  i32FBFd);
	if(i32FBFd < 0){
/*		close(i32KpdFd);*/
		i32Ret = -1;
		printf("Init fb device fail\n");
		goto exit_prog;
	}
	////////////////////////////

	backupVideoBuffer = malloc(u32FBBufSize);

	if(backupVideoBuffer < 0)
	{
		printf("Malloc fail\n");
		return -1;
	}
  memcpy((void*)backupVideoBuffer, (char*)pu8FBBufAddr, u32FBBufSize);
	////////////////////////////

	if(InitV4LDevice(eEncodeImgResol) != ERR_V4L_SUCCESS){
		ioctl(i32FBFd, IOCTL_LCD_DISABLE_INT, 0);
		ioctl(i32FBFd, VIDEO_FORMAT_CHANGE, DISPLAY_MODE_RGB565); //2012-0808 
		ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
		munmap(pu8FBBufAddr, u32FBBufSize);
		close(i32FBFd);
		//close(i32KpdFd);
		i32Ret = -1;
		printf("Init V4L device fail\n");

		return (-1);
	}
//	StartPreview();
/*	// Test the kernel configuration whether match with application, TBD 
	if(QueryV4LPacketInformation(&s_packetInfo)!=ERR_V4L_SUCCESS){		
		ioctl(i32FBFd, IOCTL_LCD_DISABLE_INT, 0);
		ioctl(i32FBFd, VIDEO_FORMAT_CHANGE, DISPLAY_MODE_RGB565); //2012-0808 
		ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);				
		printf("Application is hardware preview mode\n");	
		printf("Please choice hardware preview mode in kernel configuration and rebuild\n");	
		close(i32FBFd);
		printf("close FB\n");
		close(i32KpdFd);
		printf("close KPD\n");
		exit(-1);		
		FinializeV4LDevice();
		printf("Close VideoIn\n");
		munmap(pu8FBBufAddr, u32FBBufSize);
		printf("munmap V4L\n");
		goto exit_prog;
	}	
*/
//	printf("Init V4L device pass\n");
//
	if(InitJpegDevice(eEncodeImgResol) < 0){		
		FinializeV4LDevice();
		munmap(pu8FBBufAddr, u32FBBufSize);
		close(i32FBFd);
/*		close(i32KpdFd);*/
		i32Ret = -1;
		printf("Init JPEG device fail\n");
		goto exit_prog;
	}
	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);

	
		
#ifdef _PLANAR_YUV420_
	//Planar YUV420
	SetV4LEncode(0, u32EncodeImgWidth, u32EncodeImgHeight, VIDEO_PALETTE_YUV420P);
#else
	//Planar YUV422
	SetV4LEncode(0, u32EncodeImgWidth, u32EncodeImgHeight, VIDEO_PALETTE_YUV422P);
	//SetV4LEncode(0, 320, 240, VIDEO_PALETTE_YUV422P);
#endif

	SetV4LViewWindow(s_i32FBWidth, s_i32FBHeight, u32PreviewImgWidth, u32PreviewImgHeight);
	printf("Ready to start capture\n");	
	StartV4LCapture();
	printf("Start capturing\n");	

	ioctl(i32FBFd, IOCTL_LCD_ENABLE_INT, 0);
	

	MotionDetV4LSetThreshold(20);
	printf("Ready to query user control\n");	
	
	QueryV4LUserControl();

	QueryV4LSensorID(&i32SensorId);
	printf("Sensor ID = %d\n", i32SensorId);
	printf("Ready to start preview\n");	
	u32MDJudgeTime = GetTime() + 2000;
	////////////////////////////

	
/*	char *p8SrcBuf=NULL;
	p8SrcBuf = malloc(SRCBUFSIZE);	
	if(p8SrcBuf == NULL){
		printf("Allocate src buffer faile \n");
		return (-1);
	}
	

	printf("Allocate Buf addr = 0x%x\n", (unsigned int)p8SrcBuf);
	memset (p8SrcBuf, 0x00, SRCBUFSIZE);*/

	/* Allocate buffer for 3M pixels srouce pattern */
	if ( InitVPE() ){				
		return (-1);
	}	
	printf("InitVPE ok \n");
	/////////////////////////////

/*	p8SrcBuf_black = malloc(SRCBUFSIZE);
	if(p8SrcBuf_black == NULL){
		printf("Allocate src buffer faile \n");
		exit(-1);
	}

	printf("Allocate Buf addr = 0x%x\n", (unsigned int)p8SrcBuf_black);
	memset (p8SrcBuf_black, 0x00, SRCBUFSIZE);
	loadSrcPattern(p8SrcBuf_black);*/
	return 0;
exit_prog:
	return -1;
}

int unInitJpecPro()
{
	myExitHandler(100);
/*	FinializeJpegDevice();
	FinializeV4LDevice();*/

	return 0;
}

static int setBlackYuv422(uint8_t* start,uint32_t len)
{
	int i=0;
	for(i=0;i<len;i=i+2){
		start[i]=0x00;//y
		start[i+1]=0x80;//uv
	}
	return 0;
}

static int getOneJpegNormal(ROTATEDIREC rot_num,uint8_t** buff,uint32_t* buflen,ZOOMLEVEL zoom,int needswitch)
{
	// refresh image
	if (needswitch) {
		StopV4LCapture();
		SetV4LEncode(0, 320, 240, VIDEO_PALETTE_YUV422P);
		StartV4LCapture();
		printf("Start capturing\n");
	}


    struct timeval start, end;

	int width_af_vpe=320,height_af_vpe=240,dispW=320,dispH=240;
	tb_vpe_transform_t tbpara={0};
	tbpara.rotation=rot_num;
	tbpara.dest_width=320;
	tbpara.dest_height=240;
	tbpara.zoomlev=NOZOOM;
/*	tbpara.src_width=u32EncodeImgWidth;
	tbpara.src_height=u32EncodeImgHeight;*/
	tbpara.src_width=320;
	tbpara.src_height=240;
	static char firstSwitchTo180=1;

	if (ReadV4LPicture(&pu8PicPtr, &u64TS, &u32PicPhyAdr) == ERR_V4L_SUCCESS) {
		TriggerV4LNextFrame();
		//printf("@@@@@@@@@@@@%d  %d\n",u32EncodeImgWidth,u32EncodeImgHeight);
		getfps();
		/**************for buffer***********************************/
		switch (rot_num) {
		case RIGHTDIRECTION:
		case LEFTDIRECTION:
			width_af_vpe = 240;
			height_af_vpe = 320;
			dispW=240*width_af_vpe/height_af_vpe;
			dispH=240;
			break;
		default:
			//FormatConversion_up_QVGA(pu8PicPtr, s_pu8JpegEncBuf, 320, 240); //ok
			break;
		}
		tbpara.dest_addr=s_pu8JpegEncBuf;
		tbpara.dest_format=VPE_DST_PACKET_YUV422;
		tbpara.src_addr=pu8PicPtr;
		tbpara.src_format=VPE_SRC_PLANAR_YUV422;
	    scalePro(&tbpara);
		jpegCodec_reserved_vpe_QVGA_to_buff(s_pu8JpegEncBuf,buff,buflen,width_af_vpe,height_af_vpe);
	    FormatConversion_Normal_QVGA_rgb(s_pu8JpegEncBuf, pu8FBBufAddr, dispW, dispH,width_af_vpe,height_af_vpe,zoom);

	} else {

		ERR_PRINT("Read V4L Error\n");
		return -1;
	}
	return 0;
}
static int getOneJpegForTake(ROTATEDIREC rot_num,uint8_t** buff,uint32_t* buflen,ZOOMLEVEL zoom,int needswitch)
{
	// refresh image
	if (needswitch) {
		StopV4LCapture();
		SetV4LEncode(0, u32EncodeImgWidth, u32EncodeImgHeight, VIDEO_PALETTE_YUV422P);
		StartV4LCapture();
		printf("Start capturing\n");
	}
    struct timeval start, end;
	int width_af_vpe=u32EncodeImgWidth,height_af_vpe=u32EncodeImgHeight,dispW=320,dispH=240;
	tb_vpe_transform_t tbpara={0};
	tbpara.rotation=rot_num;
	tbpara.dest_width=u32EncodeImgWidth;
	tbpara.dest_height=u32EncodeImgHeight;
	tbpara.zoomlev=NOZOOM;
	tbpara.src_width=u32EncodeImgWidth;
	tbpara.src_height=u32EncodeImgHeight;
	static char firstSwitchTo180=1;

	if (ReadV4LPicture(&pu8PicPtr, &u64TS, &u32PicPhyAdr) == ERR_V4L_SUCCESS) {
		TriggerV4LNextFrame();
		//printf("@@@@@@@@@@@@%d  %d\n",u32EncodeImgWidth,u32EncodeImgHeight);
		getfps();
		/**************for buffer***********************************/
		switch (rot_num) {
		case RIGHTDIRECTION:
		case LEFTDIRECTION:
			width_af_vpe = u32EncodeImgHeight;
			height_af_vpe = u32EncodeImgWidth;
			dispW=240*width_af_vpe/height_af_vpe;
			dispH=240;
			break;
		default:
			//FormatConversion_up_QVGA(pu8PicPtr, s_pu8JpegEncBuf, 320, 240); //ok
			break;
		}
		tbpara.dest_addr=s_pu8JpegEncBuf;
		tbpara.dest_format=VPE_DST_PACKET_YUV422;
		tbpara.src_addr=pu8PicPtr;
		tbpara.src_format=VPE_SRC_PLANAR_YUV422;
	    scalePro(&tbpara);
		jpegCodec_reserved_vpe_QVGA_to_buff(s_pu8JpegEncBuf,buff,buflen,width_af_vpe,height_af_vpe);
	    FormatConversion_Normal_QVGA_rgb(s_pu8JpegEncBuf, pu8FBBufAddr, dispW, dispH,width_af_vpe,height_af_vpe,zoom);

	} else {

		ERR_PRINT("Read V4L Error\n");
		return -1;
	}
	return 0;
}
int getOneJpeg(ROTATEDIREC rot_num, uint8_t** buff, uint32_t* buflen, ZOOMLEVEL zoom, int takephoto,int SwitchRes)
{
	int ret = 0;
	if (takephoto) {
		getOneJpegForTake(rot_num, buff, buflen, zoom,SwitchRes);
	} else {
		getOneJpegNormal(rot_num, buff, buflen, zoom,SwitchRes);
	}
	return ret;
}
int saveJpegToFile(char* pchSaveFolder,unsigned char* buf,int buflen)
{
	ERRCODE err;
	int32_t i32CapFileFD;
	err = DSC_NewCapFile(pchSaveFolder, &i32CapFileFD);
	if (err != ERR_DSC_SUCCESS)
		return -1;

	write(i32CapFileFD, buf, buflen);
	DSC_CloseCapFile(pchSaveFolder, i32CapFileFD);
	sync();
	return 0;
}

/*int main(int argc, char **argv)
{
	char* pchSaveFolder = DEFAULT_SAVE_FOLDER;
	if(mkdir(pchSaveFolder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0){
		if(errno != EEXIST)
			return -1;
	}
	DEBUG_PRINT("Set save folder %s \n", pchSaveFolder);
	unsigned char buff[10*1024]={0};
	int len =10*1024;
	if(initJpegPro()<0)
		return -1;
	while (1) {
		len=10*1024;
		getOneJpeg(0,buff,&len);
		saveJpegToFile(pchSaveFolder,buff,len);
	}

	unInitJpecPro();
	munmap(pu8FBBufAddr, u32FBBufSize);

	ioctl(i32FBFd, VIDEO_FORMAT_CHANGE, DISPLAY_MODE_RGB565);		
	close(i32FBFd);
	close(i32KpdFd);


	return 0;
}*/

