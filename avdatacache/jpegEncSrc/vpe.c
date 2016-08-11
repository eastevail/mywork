#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <linux/fb.h>
#include <unistd.h>
#include "port.h"
#include "port.h"
#include "w55fa92_vpe.h"
//#include "h264.h"

#define DBGPRINTF(...)
/* VPE device */
char vpe_device[] = "/dev/vpe";
int vpe_fd;

#define LCD_ENABLE_INT		_IO('v', 28)
#define LCD_DISABLE_INT		_IO('v', 29)

//extern __u32 g_u32VpostWidth, g_u32VpostHeight;

/*
int FormatConversion(void* data, char* pDstBuf, int Srcwidth, int Srcheight, int Tarwidth, int Tarheight)
{
	//unsigned int volatile vworkbuf, tmpsize;
    AVFrame             *pict=(AVFrame *)data;	
	unsigned int value;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	int total_x, total_y;
	int PAT_WIDTH, PAT_HEIGHT;
	unsigned int ret =0;	
	
	PAT_WIDTH = (Srcwidth+3) & ~0x03;
	PAT_HEIGHT = Srcheight;
	
	//printf("SrcWidth = %d, SrcHeight = %d, Tarwidth=%d, Tarheight = %d\n",Srcwidth, Srcheight, Tarwidth, Tarheight);
	
	total_x = (Srcwidth + 15) & ~0x0F;
	total_y = (Srcheight + 15) & ~0x0F;	
	ptr_y = (unsigned int)pict->data[0];
	ptr_u = (unsigned int)pict->data[1];		// Planar YUV420 
	ptr_v = (unsigned int)pict->data[2];
	
	while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
		if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
	}
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0)		
	{
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
	}	
	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0)		
	{
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
	}			
	
	value = 0;		
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);	
	
	vpe_setting.src_width = PAT_WIDTH;
	
		
	vpe_setting.src_addrPacY = ptr_y;				// Planar YUV420 
	vpe_setting.src_addrU = ptr_u;	
	vpe_setting.src_addrV = ptr_v;				
	vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
	vpe_setting.src_width = PAT_WIDTH ;
	vpe_setting.src_height = PAT_HEIGHT;		
		

	vpe_setting.src_leftoffset = 0;
	vpe_setting.src_rightoffset = total_x - PAT_WIDTH;
		
	vpe_setting.dest_addrPac = (unsigned int)pDstBuf;	
	vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
	vpe_setting.dest_width = Tarwidth;
	vpe_setting.dest_height = Tarheight;
	
	if (g_u32VpostHeight > Tarheight)
	{  // Put image at the center of panel
	    int offset_Y;
	    offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
	    vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;		    
	}
	
	
	if (g_u32VpostWidth > Tarwidth)
	{
		vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
	}
	else
	{
	    vpe_setting.dest_width = g_u32VpostWidth;
	    vpe_setting.dest_height = g_u32VpostHeight;
		    
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;		
	}
	


	vpe_setting.algorithm = VPE_SCALE_DDA;
	vpe_setting.rotation = VPE_OP_NORMAL;

		
	if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0)		
	{
		close(vpe_fd);
		printf("VPE_IO_GET fail\n");
	}		
	

	if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0)	
	{
		close(vpe_fd);
		printf("VPE_TRIGGER info fail\n");
	}		
	
	return ret;	
	
	
}
*/

#define PAT_WIDTH_VGA		640
#define PAT_HEIGHT_VGA		480
#define PAT_WIDTH		320
#define PAT_HEIGHT		240
//#define STEP_WIDTH		32
//#define STEP_HEIGHT		18
#define STEP_WIDTH		140
#define STEP_HEIGHT		0
int FormatConversion_up(char* pSrcBuf, char* pDstBuf, int Srcwidth, int Srcheight, int Tarwidth, int Tarheight)
{
	//unsigned int volatile vworkbuf, tmpsize;
  //  AVFrame             *pict=(AVFrame *)data;	
	unsigned int value;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	int total_x, total_y;
	//int PAT_WIDTH, PAT_HEIGHT;
	unsigned int ret =0;	
	
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
//	PAT_WIDTH = (Srcwidth+3) & ~0x03;
	//PAT_HEIGHT = Srcheight;
	
	//printf("SrcWidth = %d, SrcHeight = %d, Tarwidth=%d, Tarheight = %d\n",Srcwidth, Srcheight, Tarwidth, Tarheight);
	
	total_x = (Srcwidth + 15) & ~0x0F;
	total_y = (Srcheight + 15) & ~0x0F;	
	//ptr_y = (unsigned int)pict->data[0];
	//ptr_u = (unsigned int)pict->data[1];		/* Planar YUV420 */ 
	//ptr_v = (unsigned int)pict->data[2];
	
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	//g_u32VpostWidth = 320;
	//g_u32VpostHeight = 240;
	g_u32VpostWidth = Srcwidth;
	g_u32VpostHeight = Srcheight;
	
	while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
		if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
	}
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0)		
	{
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
	}	
	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0)		
	{
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
	}			
	
	value = 0;		
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);	
	
	vpe_setting.src_width = PAT_WIDTH;
	
		
	vpe_setting.src_addrPacY = ptr_y;				/* Planar YUV420 */
	vpe_setting.src_addrU = ptr_u;	
	vpe_setting.src_addrV = ptr_v;				
//	vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
	vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
	vpe_setting.src_width = PAT_WIDTH ;
	vpe_setting.src_height = PAT_HEIGHT;		
		

	vpe_setting.src_leftoffset = 0;
	//vpe_setting.src_rightoffset = total_x - PAT_WIDTH;
	vpe_setting.src_rightoffset = 0;
		
	vpe_setting.dest_addrPac = (unsigned int)pDstBuf;	
	//vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
	vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
	vpe_setting.dest_width = Tarwidth;
	vpe_setting.dest_height = Tarheight;
	
	if (g_u32VpostHeight > Tarheight)
	{  // Put image at the center of panel
	    int offset_Y;
	    offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
	    vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;		    
	}
	
	
	if (g_u32VpostWidth > Tarwidth)
	{
		vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
	}
	else
	{
	    vpe_setting.dest_width = g_u32VpostWidth;
	    vpe_setting.dest_height = g_u32VpostHeight;
		    
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;		
	}
	


	vpe_setting.algorithm = VPE_SCALE_DDA;
	vpe_setting.rotation = VPE_OP_NORMAL;

		
	if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0)		
	{
		close(vpe_fd);
		printf("VPE_IO_GET fail\n");
	}		
	

	if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0)	
	{
		close(vpe_fd);
		printf("VPE_TRIGGER info fail\n");
	}		
	
	return ret;	
	
	
}


int FormatConversion_down(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
	g_u32VpostWidth = 640;
	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
	/*if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){// Show buffer 1. Because VPE always filled from buffer 0. 
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
			printf("ioctl FBIOPAN_DISPLAY");
		}
#endif
	}
	*/
	while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
		vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				/* Planar YUV422 */
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
	
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
	//  vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		DBGPRINTF("Src dimension = %d x %d\n", vpe_setting.src_width, vpe_setting.src_height);
		DBGPRINTF("Src offset(L,R) = %d , %d\n", vpe_setting.src_leftoffset, vpe_setting.src_rightoffset);
		
		if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
		}
		else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
		}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		//vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		vpe_setting.dest_width = Tarwidth;
		vpe_setting.dest_height = Tarheight;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_UPSIDEDOWN;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
	//	ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	/*	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){//Show buffer 0
			var.yoffset = 0;
#ifdef _USE_FBIOPAN_
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
				printf("ioctl FBIOPAN_DISPLAY");
			}
#else
#ifdef _NON_TEARING_
			ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif

		}	
		else{
			var.yoffset = var.yres;
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
			}		
		}
		step = step +1;
#ifdef _USE_FBIOPAN_
		if(buf == 0)
			buf =1;
		else
			buf =0;
#endif
*/
		break;
	}
	/*
	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf)
	{//VPOST should show buffer 1 for tearing issue if restart. 
		vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
		}
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
		}			
#else
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif
	}
	*/
	return ret;
	
}	


int FormatConversion_up_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
//	g_u32VpostWidth = 640;
//	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
	/*if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){// Show buffer 1. Because VPE always filled from buffer 0. 
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
			printf("ioctl FBIOPAN_DISPLAY");
		}
#endif
	}
	*/
	//while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
	
	//printf ("========vpe_setting========\n");
		/*vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				//Planar YUV422 
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
	
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
	//  vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		*/
		vpe_setting.src_addrPacY = ptr_y ;				// Planar YUV422 
		vpe_setting.src_addrU = ptr_u;	
		vpe_setting.src_addrV = ptr_v;				
	
	//	vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
	  vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH ;
		vpe_setting.src_height = PAT_HEIGHT;

		vpe_setting.src_leftoffset = 0;
		vpe_setting.src_rightoffset = 0;
	
	//	if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
	//	}
	//	else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
	//	}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
//		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		vpe_setting.dest_width = Tarwidth;
		vpe_setting.dest_height = Tarheight;
	//	vpe_setting.dest_width = PAT_WIDTH;
	//	vpe_setting.dest_height = PAT_HEIGHT;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_NORMAL;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
	//	ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	/*	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){//Show buffer 0
			var.yoffset = 0;
#ifdef _USE_FBIOPAN_
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
				printf("ioctl FBIOPAN_DISPLAY");
			}
#else
#ifdef _NON_TEARING_
			ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif

		}	
		else{
			var.yoffset = var.yres;
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
			}		
		}
		step = step +1;
#ifdef _USE_FBIOPAN_
		if(buf == 0)
			buf =1;
		else
			buf =0;
#endif
*/

//		break;
//	}

	/*
	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf)
	{//VPOST should show buffer 1 for tearing issue if restart. 
		vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
		}
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
		}			
#else
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif
	}
	*/
	return ret;
	
}	


int FormatConversion_up_VGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH_VGA*PAT_HEIGHT_VGA;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH_VGA*PAT_HEIGHT_VGA/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
//	g_u32VpostWidth = 640;
//	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH_VGA;
	unsigned int test=0;
	
		vpe_setting.src_addrPacY = ptr_y ;				// Planar YUV422 
		vpe_setting.src_addrU = ptr_u;	
		vpe_setting.src_addrV = ptr_v;				
	
	//	vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
	  vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH_VGA ;
		vpe_setting.src_height = PAT_HEIGHT_VGA;

		vpe_setting.src_leftoffset = 0;
		vpe_setting.src_rightoffset = 0;
	
	//	if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
	//	}
	//	else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
	//	}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
//		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
	//	vpe_setting.dest_width = Tarwidth;
	//	vpe_setting.dest_height = Tarheight;
		vpe_setting.dest_width = PAT_WIDTH_VGA;
		vpe_setting.dest_height = PAT_HEIGHT_VGA;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_NORMAL;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
	//	ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	return ret;
	
}	



int FormatConversion_up_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
//	g_u32VpostWidth = 640;
//	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;

		vpe_setting.src_addrPacY = ptr_y ;				// Planar YUV422 
		vpe_setting.src_addrU = ptr_u;	
		vpe_setting.src_addrV = ptr_v;				
	
	//	vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
	  vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH ;
		vpe_setting.src_height = PAT_HEIGHT;

		vpe_setting.src_leftoffset = 0;
		vpe_setting.src_rightoffset = 0;
	
	//	if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
	//	}
	//	else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
	//	}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
//		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		vpe_setting.dest_width = Tarwidth;
		vpe_setting.dest_height = Tarheight;
	//	vpe_setting.dest_width = PAT_WIDTH;
	//	vpe_setting.dest_height = PAT_HEIGHT;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_NORMAL;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				

		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	return ret;
	
}	



int FormatConversion_right_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
	
	step=0;
	while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
		vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				/* Planar YUV422 */
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
		
	//  vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		DBGPRINTF("Src dimension = %d x %d\n", vpe_setting.src_width, vpe_setting.src_height);
		DBGPRINTF("Src offset(L,R) = %d , %d\n", vpe_setting.src_leftoffset, vpe_setting.src_rightoffset);
		
		if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
		}
		else{
		//	vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
		}
		/* if (g_u32VpostHeight > Tarheight)
         {  // Put image at the center of panel
             int offset_Y;
             offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
             vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;             
         }
		*/
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		//vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		//vpe_setting.dest_width = Tarwidth;
		//vpe_setting.dest_height = Tarheight;
		//vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		//vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_width = Tarheight;
		vpe_setting.dest_height = Tarwidth;
		if (g_u32VpostWidth >= Tarwidth)
        {
			vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
            vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		
        }
        else
        {
            vpe_setting.dest_width = g_u32VpostWidth;
            vpe_setting.dest_height = g_u32VpostHeight;
                      
            vpe_setting.dest_leftoffset = 0;
            vpe_setting.dest_rightoffset = 0;           
        }
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_RIGHT;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

		break;
	}
	

	return ret;
	
}	


int FormatConversion_right_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;

	while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
		vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				/* Planar YUV422 */
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
		
	//  vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		DBGPRINTF("Src dimension = %d x %d\n", vpe_setting.src_width, vpe_setting.src_height);
		DBGPRINTF("Src offset(L,R) = %d , %d\n", vpe_setting.src_leftoffset, vpe_setting.src_rightoffset);
		
		if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
		}
		else{
		//	vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
		}
		/* if (g_u32VpostHeight > Tarheight)
         {  // Put image at the center of panel
             int offset_Y;
             offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
             vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;             
         }
		*/
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		//vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		//vpe_setting.dest_width = Tarwidth;
		//vpe_setting.dest_height = Tarheight;
		//vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		//vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_width = Tarheight;
		vpe_setting.dest_height = Tarwidth;
		if (g_u32VpostWidth >= Tarwidth)
        {
			vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
            vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
        }
        else
        {
            vpe_setting.dest_width = g_u32VpostWidth;
            vpe_setting.dest_height = g_u32VpostHeight;
                      
            vpe_setting.dest_leftoffset = 0;
            vpe_setting.dest_rightoffset = 0;           
        }
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_RIGHT;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

		break;
	}
	

	return ret;
	
}	


int FormatConversion_down_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
//	g_u32VpostWidth = 640;
//	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
	/*if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){// Show buffer 1. Because VPE always filled from buffer 0. 
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
			printf("ioctl FBIOPAN_DISPLAY");
		}
#endif
	}
	*/
	//while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
	
	//printf ("========vpe_setting========\n");
		/*vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				//Planar YUV422 
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
	
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
	//  vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		*/
		vpe_setting.src_addrPacY = ptr_y ;				// Planar YUV422 
		vpe_setting.src_addrU = ptr_u;	
		vpe_setting.src_addrV = ptr_v;				
	
	//	vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
	  vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH ;
		vpe_setting.src_height = PAT_HEIGHT;

		vpe_setting.src_leftoffset = 0;
		vpe_setting.src_rightoffset = 0;
	
	//	if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
	//	}
	//	else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
	//	}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
//		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
	//	vpe_setting.dest_width = Tarwidth;
	//	vpe_setting.dest_height = Tarheight;
		vpe_setting.dest_width = PAT_WIDTH;
		vpe_setting.dest_height = PAT_HEIGHT;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_UPSIDEDOWN;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
	//	ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	/*	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){//Show buffer 0
			var.yoffset = 0;
#ifdef _USE_FBIOPAN_
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
				printf("ioctl FBIOPAN_DISPLAY");
			}
#else
#ifdef _NON_TEARING_
			ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif

		}	
		else{
			var.yoffset = var.yres;
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
			}		
		}
		step = step +1;
#ifdef _USE_FBIOPAN_
		if(buf == 0)
			buf =1;
		else
			buf =0;
#endif
*/

//		break;
//	}

	/*
	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf)
	{//VPOST should show buffer 1 for tearing issue if restart. 
		vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
		}
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
		}			
#else
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif
	}
	*/
	return ret;
	
}	


int FormatConversion_down_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
//	g_u32VpostWidth = 640;
//	g_u32VpostHeight = 480;
	
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
	/*if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){// Show buffer 1. Because VPE always filled from buffer 0. 
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
			printf("ioctl FBIOPAN_DISPLAY");
		}
#endif
	}
	*/
	//while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
	
	//printf ("========vpe_setting========\n");
		/*vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				//Planar YUV422 
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
	
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
	//  vpe_setting.src_format = VPE_SRC_PLANAR_YUV420;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		*/
		vpe_setting.src_addrPacY = ptr_y ;				// Planar YUV422 
		vpe_setting.src_addrU = ptr_u;	
		vpe_setting.src_addrV = ptr_v;				
	
	//	vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
	  vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH ;
		vpe_setting.src_height = PAT_HEIGHT;

		vpe_setting.src_leftoffset = 0;
		vpe_setting.src_rightoffset = 0;
	
	//	if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
	//	}
	//	else{
			//vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
	//	}
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
	//	vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
	//	vpe_setting.dest_width = Tarwidth;
	//	vpe_setting.dest_height = Tarheight;
		vpe_setting.dest_width = PAT_WIDTH;
		vpe_setting.dest_height = PAT_HEIGHT;
	//	vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
	//	vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_leftoffset = 0;
		vpe_setting.dest_rightoffset = 0;
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_UPSIDEDOWN;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
	//	ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

	/*	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf){//Show buffer 0
			var.yoffset = 0;
#ifdef _USE_FBIOPAN_
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {
				printf("ioctl FBIOPAN_DISPLAY");
			}
#else
#ifdef _NON_TEARING_
			ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif

		}	
		else{
			var.yoffset = var.yres;
			if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
			}		
		}
		step = step +1;
#ifdef _USE_FBIOPAN_
		if(buf == 0)
			buf =1;
		else
			buf =0;
#endif
*/

//		break;
//	}

	/*
	if(vpe_setting.dest_addrPac==(unsigned int)pDstBuf)
	{//VPOST should show buffer 1 for tearing issue if restart. 
		vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
		}
		var.yoffset = var.yres;
#ifdef _USE_FBIOPAN_
		if (ioctl(vpost_fd, FBIOPAN_DISPLAY, &var) < 0) {//Show buffer 1
				printf("ioctl FBIOPAN_DISPLAY");
		}			
#else
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_ENABLE_INT);
#endif
#endif
	}
	*/
	return ret;
	
}	



int FormatConversion_left_QVGA(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;
 step=0;
	while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
		vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				/* Planar YUV422 */
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
		
	//  vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		DBGPRINTF("Src dimension = %d x %d\n", vpe_setting.src_width, vpe_setting.src_height);
		DBGPRINTF("Src offset(L,R) = %d , %d\n", vpe_setting.src_leftoffset, vpe_setting.src_rightoffset);
		
		if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
		}
		else{
		//	vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
		}
		/* if (g_u32VpostHeight > Tarheight)
         {  // Put image at the center of panel
             int offset_Y;
             offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
             vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;             
         }
		*/
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		//vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
		vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		//vpe_setting.dest_width = Tarwidth;
		//vpe_setting.dest_height = Tarheight;
		//vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		//vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_width = Tarheight;
		vpe_setting.dest_height = Tarwidth;
		if (g_u32VpostWidth >= Tarwidth)
        {
			vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
            vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
			
		
        }
        else
        {
            vpe_setting.dest_width = g_u32VpostWidth;
            vpe_setting.dest_height = g_u32VpostHeight;
                      
            vpe_setting.dest_leftoffset = 0;
            vpe_setting.dest_rightoffset = 0;           
        }
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_LEFT;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

		break;
	}
	

	return ret;
	
}	



int FormatConversion_left_QVGA_rgb(char* pSrcBuf, char* pDstBuf, int Tarwidth, int Tarheight)
{
	unsigned int volatile vworkbuf, tmpsize;
	unsigned int value, buf=0, ret =0;
	vpe_transform_t vpe_setting;
	unsigned int ptr_y, ptr_u, ptr_v, step=0;
	unsigned int g_u32VpostWidth, g_u32VpostHeight;
	g_u32VpostWidth = 320;
	g_u32VpostHeight = 240;
	
	ptr_y = (unsigned int)pSrcBuf;
	ptr_u = ptr_y+PAT_WIDTH*PAT_HEIGHT;		/* Planar YUV422 */ 
	ptr_v = ptr_u+PAT_WIDTH*PAT_HEIGHT/2;
	// Get Video Decoder IP Register size
	if( ioctl(vpe_fd, VPE_INIT, NULL)  < 0){
		close(vpe_fd);
		printf("VPE_INIT fail\n");		
		ret = -1;
	}	
	value = 0x5a;
	if((ioctl(vpe_fd, VPE_IO_SET, &value)) < 0){
		close(vpe_fd);
		printf("VPE_IO_SET fail\n");
		ret = -1;
	}			

	value = 1;	
	ioctl(vpe_fd, VPE_SET_MMU_MODE, &value);

	vpe_setting.src_width = PAT_WIDTH;
	unsigned int test=0;

	while( ((vpe_setting.src_width>=g_u32VpostWidth))&&(vpe_setting.src_height>=g_u32VpostHeight ) ){	
		vpe_setting.src_addrPacY = ptr_y + PAT_WIDTH*STEP_HEIGHT*step/2;				/* Planar YUV422 */
		vpe_setting.src_addrU = ptr_u+PAT_WIDTH/2*STEP_HEIGHT*step/2;	
		vpe_setting.src_addrV = ptr_v+PAT_WIDTH/2*STEP_HEIGHT*step/2;				
		
	//  vpe_setting.src_format = VPE_SRC_PACKET_YUV422;
		vpe_setting.src_format = VPE_SRC_PLANAR_YUV422;
		vpe_setting.src_width = PAT_WIDTH - step*STEP_WIDTH;
		vpe_setting.src_height = PAT_HEIGHT - step*STEP_HEIGHT;

		vpe_setting.src_leftoffset = step*STEP_WIDTH/2;
		vpe_setting.src_rightoffset = step*STEP_WIDTH/2;
		DBGPRINTF("Src dimension = %d x %d\n", vpe_setting.src_width, vpe_setting.src_height);
		DBGPRINTF("Src offset(L,R) = %d , %d\n", vpe_setting.src_leftoffset, vpe_setting.src_rightoffset);
		
		if(buf==0){/* Render to the first buffer, VPOST show the 2nd buffer */
			vpe_setting.dest_addrPac = (unsigned int)pDstBuf;
		}
		else{
		//	vpe_setting.dest_addrPac = (unsigned int)pDstBuf+ var.xres*var.yres*var.bits_per_pixel/8;	
		}
		/* if (g_u32VpostHeight > Tarheight)
         {  // Put image at the center of panel
             int offset_Y;
             offset_Y = (g_u32VpostHeight - Tarheight)/2 * g_u32VpostWidth * 2;      // For RGB565
             vpe_setting.dest_addrPac = (unsigned int)pDstBuf + offset_Y;             
         }
		*/
		DBGPRINTF("Buf %x, Start Addr = 0x%x", buf, vpe_setting.dest_addrPac);
		vpe_setting.dest_format = VPE_DST_PACKET_RGB565;
	//	vpe_setting.dest_format = VPE_DST_PACKET_YUV422;
		//vpe_setting.dest_width = Tarwidth;
		//vpe_setting.dest_height = Tarheight;
		//vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
		//vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
		vpe_setting.dest_width = Tarheight;
		vpe_setting.dest_height = Tarwidth;
		if (g_u32VpostWidth >= Tarwidth)
        {
			vpe_setting.dest_leftoffset = (g_u32VpostWidth-Tarwidth)/2;
            vpe_setting.dest_rightoffset = (g_u32VpostWidth-Tarwidth)/2;
        }
        else
        {
            vpe_setting.dest_width = g_u32VpostWidth;
            vpe_setting.dest_height = g_u32VpostHeight;
                      
            vpe_setting.dest_leftoffset = 0;
            vpe_setting.dest_rightoffset = 0;           
        }
		DBGPRINTF("Dst dimension = %d x %d\n", vpe_setting.dest_width, vpe_setting.dest_height);
		DBGPRINTF("Dst offset(L,R) = %d , %d\n", vpe_setting.dest_leftoffset, vpe_setting.dest_rightoffset);
		vpe_setting.algorithm = VPE_SCALE_DDA;
		vpe_setting.rotation = VPE_OP_LEFT;
		
		if((ioctl(vpe_fd, VPE_SET_FORMAT_TRANSFORM, &vpe_setting)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_IO_GET fail\n");
			ret = -1;
		}				
#ifndef _USE_FBIOPAN_
#ifdef _NON_TEARING_
		ioctl(vpost_fd,LCD_DISABLE_INT);
#endif
#endif
		if((ioctl(vpe_fd, VPE_TRIGGER, NULL)) < 0){
			close(vpe_fd);
			DBGPRINTF("VPE_TRIGGER info fail\n");
			ret = -1;
		}		
		//ioctl(vpe_fd, VPE_WAIT_INTERRUPT, &value);
		while (ioctl (vpe_fd, VPE_WAIT_INTERRUPT, &value)) {
			if (errno == EINTR) {
				continue;
			}
			else {
				printf ("%s: VPE_WAIT_INTERRUPT failed: %s\n", __FUNCTION__, strerror (errno));
				ret = -1;
				break;
			}
		}

		break;
	}
	

	return ret;
	
}	

int InitVPE(void)
{
	unsigned int version;

	vpe_fd = open(vpe_device, O_RDWR);		
	if (vpe_fd < 0){
		printf("open %s error\n", vpe_device);
		return -1;	
	}	
	else
		printf("open %s successfully\n", vpe_device);
	
	return 0;
}




