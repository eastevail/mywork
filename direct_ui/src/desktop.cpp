/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: desktop.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <sys/statvfs.h>
#include <sys/vfs.h>
#include <linux/videodev2.h>

#include "desktop.h"
#include "video.h"
#include "setting.h"
#include "review.h"
#include "usb_page.h"

//--------------------------------------------
#define DELAY_SDCARD_DETECT 	5
#define DELAY_HEARTBEAT			35

#define VIDEO_LOST_CHKCNT		10
#define COMM_NORMAL_CHKCNT		10

#if 0 // xjf, for test
#define DELAY_PWRON_STAMP		12 // 2s
#else
#define DELAY_PWRON_STAMP		4 // 2s
#endif
#define MAIN_DURATION			1

#define DELAY_USB_KEYDOWN		20

//--------------------------------------------
// MM
// Charge mode
#define CM_CHARGE_NONE		0x00 // None of charge
#define CM_CHARGE_HANDLE	0x02 // Handle
#define CM_CHARGE_USB		0x01 // USB
#define CM_CHARGE_HU		0x03 // Handle & USB

#define CM_CHARGE_ERRCOMM	0x0f // Error in communication

// Error code
#define ERR_CHARGE_NORMAL	0x00 // Charging normal
#define ERR_CHARGE_STOP		0x01 // Charge stop
#define ERR_NOT_REACH_3V	0x02 // Charge 45min, voltage not up to 3.0V

//--------------------------------------------
// LL
#define LL_SHUTDOWN			0x00 // STC shutdown

//--------------------------------------------
// PP
#define PP_OK				0x01 // Trigger OK

//--------------------------------------------
tsSysBatCrtl g_SysBatCtrl = {0};

//--------------------------------------------
#define NEC_BUF_SIZE		32
static char nec_buf[NEC_BUF_SIZE] = {0};
static int nec_len = 0;
static BOOL nec_found = FALSE;

//--------------------------------------------
struct tm g_curLocalTime = {0};
BOOL g_isFirstTimeSetting = FALSE;

//--------------------------------------------
tsRunParam g_RunParam;
static volatile int cntChkDlyKey = 0;

//--------------------------------------------
CDesktop *CDesktop::g_desktop = NULL;
BOOL CDesktop::g_keyLock = TRUE;

//--------------------------------------------
CDesktop::CDesktop()
{
//--------------------------------------------
	m_pagePtr = NULL;

	m_lblBat = NULL;
	m_imgLogo = NULL;
	m_lblShutDown = NULL;

	m_imgStamp = NULL;
	m_lblStampTime = NULL;
	m_lblStampWeek = NULL;
	m_lblStampDate = NULL;

	InitRunParam(); // xjf

//--------------------------------------------
	m_bConnectToPC		= false;
	m_bUSBCharge        = false;

	m_checkUsbTimer		= -1;
	m_curPage           = Page_Invalid;
	m_liteWin = NULL;
	memset(m_devPath, 0, MAX_DEVPATH_LEN);
	m_vedioStatus = 0;
	m_ledBrightValue = 0;
	m_vediofd = -1;
	m_lcdDevFd = -1;


	m_waitingLabel = NULL;
	m_RRCount =0;
	m_QQCount = 0;
	m_MMCount = 0;

	m_waitpassTime = 0;
	m_waitsetTime = 0;
	m_waitingBar   = NULL;
	m_waitfunData = NULL;
	m_waitfunCb   = NULL;
	InitDesktop();
 }

BOOL CDesktop::InitDesktop()
{
	int pipefd = -1;
	int res = 0;
	char enInfo[2] = {0};
	DBG("xjf InitDesktop start\n");
/*	DBG("\n");
	m_liteWin = NULL;
	memset(m_devPath, 0, MAX_DEVPATH_LEN);
	DBG("\n");
	m_vedioStatus = 0;
	DBG("\n");
	m_ledBrightValue = 0;
	DBG("\n");
	m_vediofd = -1;
	DBG("\n");
	m_lcdDevFd = -1;
	DBG();*/
	//sys_msg_drv_init(MSG_TYPE_UI_PROC);
	//InitLCD();//change by lixiang ,this is for adjust lightness with pwm

	g_RunParam.tuobangPara.lang = LANG_ENGLISH;
	InitLangDesc(g_RunParam.tuobangPara.lang);
#if sys_msg_cmd_get_tuobang_DEF // xjf
//#if 0
	do{
		sleep(1);

		pipefd = open("/tmp/tb_pipe", O_RDONLY);

		if(pipefd <0)
		{
			DBG("open read pipe FAIL\n");
			continue;
		}

		DBG("RES open! \n");

		memset(enInfo,0x0, sizeof(enInfo));
		res = read(pipefd, enInfo, sizeof(enInfo));

		if(res > 0){
			close(pipefd);

			DBG("enInfo = %s \n",enInfo);
			if(!strcmp(enInfo,"o")){

				DBG("RES OK! \n");
				break;
			}
		}
		else{
			DBG("read  fail ! \n");
			close(pipefd);
		}
	}while(1);
#endif

/*	m_vediofd = open("/dev/video0",O_RDWR);
	DBG("InitDesktop, Open video0, m_vediofd: %p\n", m_vediofd);*/

	InitLiteWindow();
	DBG();
	InitBatProp(); // xjf
	DBG();
	InitTopLabel();
	InitShutDownUI();
	InitTimeStampUI();
	DBG();
	m_pNotifictionlMng = new CNotifictions;

/*	pChargeManager = new ChargeManager;
	pChargeManager->setAGCfd(m_lcdDevFd);
	pChargeManager->Start();*/
	DBG();
	// ��ʼ����Ϣ
	//alarm_msg_drv_init(ALARM_MSG_TYPE_UI_PROC);

	// Check SD card
	//g_RunParam.sdDetect = pChargeManager->isboardCharge();
	//InitStorageMount();
	//SDCardDetected();
	DBG("lixiang");
	// Check video signal
	g_RunParam.m_cntVideoLostDetect = VIDEO_LOST_CHKCNT;
	//videoLostDetected();
	DBG();
	//DoHeartbeat();
/*	DBG();
	if (SUCCESS != CW_PthreadMNG_Create("UiMsgSvr", &m_msgThread, NULL, MsgThreadCB, this))
	{
		DBG("[DirectFBUI-Desktop] create message thread failure\n");
	}
	else
	{
		DBG("[DirectFBUI-Desktop] create message thread success\n");
	}*/

#if 0 // xjf
	if (SUCCESS != CW_PthreadMNG_Create("Serial", &m_serialThread, NULL, SerialThreadCB, this))
	{
		DBG("[DirectFBUI-Desktop] create serial thread failure\n");
	}
	else
	{
		DBG("[DirectFBUI-Desktop] create serial thread success\n");
	}
#endif

	m_boardCellTimeID = -1;
	InitTimer();
	DBG();
	DBG("xjf InitDesktop over\n");

	return SUCCESS;
}

BOOL CDesktop::UnInitDesktop()
{
	DestoryTimer();

	DestoryTopLabel();

	DestoryLiteWindow();

	m_liteWin = NULL;

	m_topLabel			= NULL;

	UninitLCD();
	m_lcdDevFd			= -1;

	if( m_vediofd > 0 )
	{
		close(m_vediofd);
		m_vediofd           = -1;
	}

	delete m_pNotifictionlMng;
	delete pChargeManager;

	DBG("[DirectFBUI-Desktop] uninit desktop success\n");

	return SUCCESS;
}

CDesktop::~CDesktop()
{
	UnInitDesktop();
}

CDesktop* CDesktop::GetDesktop()
{
	if (g_desktop == NULL)
	{
		g_desktop = new CDesktop();
	}

	return g_desktop;
}

void CDesktop::deskKeyLock(BOOL lock)
{
	CDesktop::g_keyLock = lock;
	DBG("[DirectFBUI-Desktop] Now , key Board [ %d ]\n",CDesktop::g_keyLock);
}

void CDesktop::DoLoop(void)
{
	if( m_liteWin != NULL )
	{
		DBG("[DirectFBUI-Desktop] enter DirectFB event loop...\n");

		char szUpdateFile[128];
		memset(szUpdateFile, 0, sizeof(szUpdateFile));
		sprintf(szUpdateFile, "%s/%s", m_devPath, UPDATE_FILE);

		DBG("[DirectFBUI-Desktop] szUpdateFile  %s\n",szUpdateFile);

		if( access(szUpdateFile, F_OK) != 0 )
		{
			// Show logo and product name
			DBG();
			lite_set_image_visible(m_imgStamp, true);
			lite_set_label_set_visible(m_lblStampTime, true);
			lite_set_label_set_visible(m_lblStampWeek, true);
			lite_set_label_set_visible(m_lblStampDate, true);
		}
		else
		{
			DBG("[DirectFBUI-Desktop]%s is Exsit \n", szUpdateFile);
			ShowPageByID(DEFAULT_PAGE);
		}
		DBG();
		lite_window_event_loop(m_liteWin, 0);
		DBG();
	}

	UnInitDesktop();

	DBG("[DirectFBUI-Desktop] Exit DirectFB event loop\n");
}

void CDesktop::CheckTriggerUpdateCb(void *param)
{
	CDesktop *pDesktop = (CDesktop *)param;

	pDesktop->CheckTriggerUpdate();
}

void CDesktop::CheckTriggerUpdate()
{
	//DBG("CheckTriggerUpdate\n");

	// Handle trigger OK key
	if( g_SysBatCtrl.iTriggerKey == 1 )
	{
		//if( m_curPage == Page_Video )
			PostKeyMessage(OK_BTN);

		g_SysBatCtrl.iTriggerKey = 0;
	}

	// Low voltage to auto save video record
	if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
	{
		if( g_SysBatCtrl.HandleBat.bat_val == 0 )
		{
			// M12 battery flash flat will stop video record
			if( m_pagePtr != NULL && m_curPage == Page_Video )
				((CVideoPage*)m_pagePtr)->abnormalStopRecord();
		}
	}
	else
	{
		if( g_SysBatCtrl.LcdBat.bat_val == 0 )
		{
			// Screen battery flash flat will stop video record
			if( m_pagePtr != NULL && m_curPage == Page_Video )
				((CVideoPage*)m_pagePtr)->abnormalStopRecord();
		}
	}

	// Check USB connect
	if( g_RunParam.keyValid )
		CheckUsbConnect();
#if 0
	// Init timer
	if( g_RunParam.dlyKey != 0 )
	{
		cntChkDlyKey++;
		if( cntChkDlyKey >= 3 )
		{
			DBG("\nStartCheckUSB\n");
			StartCheckUSB();
			cntChkDlyKey = 0;
		}
	}
#endif
}

DFBResult CDesktop::InitLiteWindow()
{
	DFBRectangle 		rect;
	int 				screenWidth 	= 0;
	int 				screenHeight 	= 0;
	DBG();
	DFBCHECK(lite_open(NULL, NULL));
	DBG();
	DFBCHECK(lite_get_layer_size(&screenWidth, &screenHeight));
	DBG();

	DBG("[DirectFBUI-Desktop] screenWidth = %d screenHeight = %d\n", screenWidth, screenHeight);
	rect.x 				= LITE_CENTER_HORIZONTALLY;
	rect.y 				= LITE_CENTER_VERTICALLY;
	rect.w			 	= screenWidth;
	rect.h 				= screenHeight;
	DFBCHECK(lite_new_window(NULL, &rect, DWCAPS_ALPHACHANNEL|DWCAPS_DOUBLEBUFFER, liteNoWindowTheme, NULL, &m_liteWin));
	DBG("[DirectFBUI-Desktop] create new lite window %p\n", m_liteWin);

	// ͸��ɫ 0x000000FF (RGBA)
	DFBCHECK(lite_set_window_background_color(m_liteWin, KEY_COLOR_R, KEY_COLOR_G, KEY_COLOR_B, KEY_COLOR_A)); // white color
	//DFBCHECK(lite_set_window_background_color(m_liteWin, 204, 31, 48, 0xff)); // red color

	/* register keyboard event to window */
	m_liteWin->keyboard_data = this;
	m_liteWin->keyboard_func = OnDesktopKeyboardEvent;

	m_liteWin->check_trigger_updateData = this;
	m_liteWin->check_trigger_updatefunCb = CheckTriggerUpdateCb;

	// show the window, if not we will not receive the event message
	lite_set_window_opacity(m_liteWin, liteFullWindowOpacity);

	g_RunParam.screenWidth = screenWidth;
	g_RunParam.screenHeight = screenHeight;
	DBG("InitLiteWindow, screenWidth: %d, screenHeight: %d\n", g_RunParam.screenWidth, g_RunParam.screenHeight);

	DBG("[DirectFBUI-Desktop] init lite window success\n");

	return DFB_OK;
}

BOOL CDesktop::InitTimer()
{
	int ret = -1;

	if( m_boardCellTimeID != -1 )
	{
		lite_remove_window_timeout(m_boardCellTimeID);
		m_boardCellTimeID = -1;
	}

	if( m_boardCellTimeID == -1 )
	{
		//ret = lite_enqueue_window_timeout(UPDATE_BOARD_CELL_TIME, TimeOut_EveryTime, BoardCellCB, this, &m_boardCellTimeID);
		ret = lite_enqueue_window_timeout(500, TimeOut_EveryTime, BoardCellCB, this, &m_boardCellTimeID);
		if( ret == DFB_OK)
		{
			DBG("[DirectFBUI-Desktop] create update board cell timer succes, id = %d\n", m_boardCellTimeID);
		}
	}

	return SUCCESS;
}

BOOL CDesktop::DestoryTimer()
{
	lite_remove_window_timeout(m_boardCellTimeID);
	//DBG("[DirectFBUI-Desktop] delete update board cell timer succes, id = %d\n", m_boardCellTimeID);
	m_boardCellTimeID = -1;

	lite_remove_window_timeout(m_checkUsbTimer);
	//DBG("[DirectFBUI-Desktop] delete check USB timer succes, id = %d\n", m_checkUsbTimer);
	m_checkUsbTimer = -1;

	return SUCCESS;
}

//-----------------------------------------------------
void CDesktop::InitRunParam()
{
	g_RunParam.cntDispStamp = DELAY_PWRON_STAMP;

	g_RunParam.sd_disk = DISK_INVALID;
	g_RunParam.sdDetect = 0;

	g_RunParam.m_cntCommLostDetect = 0;
	g_RunParam.m_cntVideoLostDetect = 0;

#if 0 // xjf, for test
	g_RunParam.m_bVideoLost = TRUE; // Lost video signal
#else
	g_RunParam.m_bVideoLost = FALSE;
#endif
	g_RunParam.m_bCommLost = FALSE;

	g_RunParam.m_cntSDCardDetect = DELAY_SDCARD_DETECT;
	g_RunParam.m_cntHeartbeat = DELAY_HEARTBEAT;

	g_RunParam.HalfSec = 0;
	g_RunParam.cntHalf1 = MAIN_DURATION;
	g_RunParam.cntHalf2= MAIN_DURATION;

	g_RunParam.dlyKey = 0;
	g_RunParam.keyValid = FALSE; // xjf

	g_RunParam.bShutDown = FALSE;

	g_RunParam.dlyUsbKeyDown = 0;
	g_RunParam.bkStatus = 1;
	g_RunParam.bUsbDrvMount = FALSE;

	g_RunParam.fileRemove.bAdd = FALSE;
	g_RunParam.fileRemove.lstFiles.clear();
}

//-----------------------------------------------------
void CDesktop::InitBatProp()
{
	// Init system battery prop
	g_SysBatCtrl.iChargeMode = 0xff; // Unknown mode
	g_SysBatCtrl.iErrCode = 0x00; // no error
	g_SysBatCtrl.LcdBat.show_flat = FLAT_HIDE;
	g_SysBatCtrl.LcdBat.flash_flat = 0;
	g_SysBatCtrl.LcdBat.bat_val = 0x0f;
	g_SysBatCtrl.LcdBat.flash_bar = 0;
	g_SysBatCtrl.HandleBat.show_flat = FLAT_HIDE;
	g_SysBatCtrl.HandleBat.flash_flat = 0;
	g_SysBatCtrl.HandleBat.bat_val = 0x0f;
	g_SysBatCtrl.HandleBat.flash_bar = 0;
	g_SysBatCtrl.iLedBright = 0;

	// Battery normal is white color
	m_clrBatNormal.a = 0xff;
	m_clrBatNormal.r = 0xff;
	m_clrBatNormal.g = 0xff;
	m_clrBatNormal.b = 0xff;

	// Battery low is red color
	m_clrBatLow.a = 0xff;
	m_clrBatLow.r = 0xff;
	m_clrBatLow.g = 0x00;
	m_clrBatLow.b = 0x00;

	// Battery none is backgroud color
	m_clrBatBk.a = TOP_LABEL_COLOR_A;
	m_clrBatBk.r = TOP_LABEL_COLOR_R;
	m_clrBatBk.g = TOP_LABEL_COLOR_G;
	m_clrBatBk.b = TOP_LABEL_COLOR_B;
	//m_clrBatBk.b = 0xff; // xjf

	// Battery position
	InitLcdBatPos();
	InitHandleBatPos();
}

//-----------------------------------------------------
void CDesktop::InitLcdBatPos()
{
	m_spaceLcdBat = 1;

	m_rcLcdBat.x = 4;
	m_rcLcdBat.y = BATTERY_HAND_Y;
	m_rcLcdBat.w = 19;
	m_rcLcdBat.h = BATTERY_HAND_H - 2 * 4;

	m_rcLcdBat2.x = m_rcLcdBat.x + m_rcLcdBat.w;
	m_rcLcdBat2.w = 2;
	m_rcLcdBat2.h = 8;
	m_rcLcdBat2.y = BATTERY_HAND_Y + (m_rcLcdBat.h - m_rcLcdBat2.h) / 2;

	m_rcLcdBat3.x = m_rcLcdBat.x;
	m_rcLcdBat3.y = 0;
	m_rcLcdBat3.w = 24;
	m_rcLcdBat3.h = TOP_LABEL_H;

	m_blockWidthLcdBat = (m_rcLcdBat.w - 7 * m_spaceLcdBat) / 4;

#if 0
	DBG("m_rcLcdBat, x: %d, y: %d, cx: %d, cy: %d\n", \
		m_rcLcdBat.x, m_rcLcdBat.y, m_rcLcdBat.w, m_rcLcdBat.h);

	DBG("m_rcLcdBat2, x: %d, y: %d, cx: %d, cy: %d\n", \
		m_rcLcdBat2.x, m_rcLcdBat2.y, m_rcLcdBat2.w, m_rcLcdBat2.h);

	DBG("m_blockWidthLcdBat: %d\n", m_blockWidthLcdBat);
#endif
}

void CDesktop::DrawLcdBat()
{
	LiteBox*			liteBox	= LITE_BOX(m_lblBat);
	IDirectFBSurface*	surface	= liteBox->surface;

	switch( g_SysBatCtrl.LcdBat.show_flat )
	{
	case FLAT_HIDE: // Hide
		surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
		surface->FillRectangle(surface, m_rcLcdBat3.x, m_rcLcdBat3.y, m_rcLcdBat3.w, m_rcLcdBat3.h);
		break;
	case FLAT_NORMAL: // Normal
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		surface->DrawRectangle(surface, m_rcLcdBat.x, m_rcLcdBat.y, m_rcLcdBat.w, m_rcLcdBat.h);
		surface->FillRectangle(surface, m_rcLcdBat2.x, m_rcLcdBat2.y, m_rcLcdBat2.w, m_rcLcdBat2.h);
		break;
	case FLAT_LOW: // Low, red flat
		surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
		surface->DrawRectangle(surface, m_rcLcdBat.x, m_rcLcdBat.y, m_rcLcdBat.w, m_rcLcdBat.h);
		surface->FillRectangle(surface, m_rcLcdBat2.x, m_rcLcdBat2.y, m_rcLcdBat2.w, m_rcLcdBat2.h);
		break;
	case FLAT_FLASH: // Flash
		if( g_SysBatCtrl.LcdBat.flash_flat == 0 )
		{
			// none
			surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
			surface->FillRectangle(surface, m_rcLcdBat3.x, m_rcLcdBat3.y, m_rcLcdBat3.w, m_rcLcdBat3.h);

			g_SysBatCtrl.LcdBat.flash_flat = 1;
		}
		else
		{
			// low
			surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
			surface->DrawRectangle(surface, m_rcLcdBat.x, m_rcLcdBat.y, m_rcLcdBat.w, m_rcLcdBat.h);
			surface->FillRectangle(surface, m_rcLcdBat2.x, m_rcLcdBat2.y, m_rcLcdBat2.w, m_rcLcdBat2.h);

			g_SysBatCtrl.LcdBat.flash_flat = 0;
		}
		break;
	default:
		break;
	}
}

void CDesktop::DispLcdBatBar(IDirectFBSurface *surface, int bar)
{
	int x, y, height;

	x = m_rcLcdBat.x + 1 + (bar + 1) * m_spaceLcdBat + bar * m_blockWidthLcdBat;
	y = BATTERY_HAND_Y + 2;
	height = BATTERY_HAND_H - 2 * 4 - 4;
	surface->FillRectangle(surface, x, y, m_blockWidthLcdBat, height);
}

void CDesktop::FillLcdBat()
{
	LiteBox*			liteBox	= LITE_BOX(m_lblBat);
	IDirectFBSurface*	surface	= liteBox->surface;

	int i;
	int x, y;
	int height;
	int bar, val;

	val = g_SysBatCtrl.LcdBat.bat_val;

	switch( val )
	{
	case 0:
		if( (g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_HU) && \
			g_SysBatCtrl.iErrCode == ERR_CHARGE_NORMAL )
		{
			// Charge mode, normal, Flash 1 bar
			if( g_SysBatCtrl.LcdBat.flash_bar == 0 )
			{
				surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 1;
			}
			else
			{
				surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 0;
			}

			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		}
		else
		{
			// Discharge mode, Flash flat, draw to bk
			surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
			for( i = 0; i < 4; i++ )
			{
				DispLcdBatBar(surface, i);
			}

		#if 0 // xjf
			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, TRUE);
		#else
			if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
			{
				// Docked mode, Not display screen battery low
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
			}
			else
			{
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, TRUE);
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
			}
		#endif
		}
		break;
	case 1: // Flash 1 bar
		if( (g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_HU) && \
			g_SysBatCtrl.iErrCode == ERR_CHARGE_NORMAL )
		{
			// Charge mode, normal
			if( g_SysBatCtrl.LcdBat.flash_bar == 0 )
			{
				surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 1;
			}
			else
			{
				surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 0;
			}

			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		}
		else
		{
			// Discharge mode, red
			if( g_SysBatCtrl.LcdBat.flash_bar == 0 )
			{
				surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 1;
			}
			else
			{
				surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 0;
			}

			if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
			{
				// Docked mode, Not display screen battery low
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
			}
			else
			{
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, TRUE);
				m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
			}
		}
		break;
	case 2: // 1 bar, normal
		if( (g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_HU) && \
			g_SysBatCtrl.iErrCode == ERR_CHARGE_NORMAL )
		{
			// Charge mode
			if( g_SysBatCtrl.LcdBat.flash_bar == 0 )
			{
				surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 1;
			}
			else
			{
				surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
				DispLcdBatBar(surface, 0);
				g_SysBatCtrl.LcdBat.flash_bar = 0;
			}
		}
		else
		{
			// Discharge mode
			surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
			DispLcdBatBar(surface, 0);
		}

		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		break;
	case 3: // 2 bar, normal
	case 4: // 3 bar, normal
	case 5: // 4 bar, normal
		bar = val - 1;

		if( (g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
			g_SysBatCtrl.iChargeMode == CM_CHARGE_HU) && \
			g_SysBatCtrl.iErrCode == ERR_CHARGE_NORMAL )
		{
			// Charge mode
			surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
			for( i = 0; i < (bar-1); i++ )
			{
				DispLcdBatBar(surface, i);
			}

			if( g_SysBatCtrl.LcdBat.flash_bar == 0 )
			{
				surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
				DispLcdBatBar(surface, bar-1);
				g_SysBatCtrl.LcdBat.flash_bar = 1;
			}
			else
			{
				surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
				DispLcdBatBar(surface, bar-1);
				g_SysBatCtrl.LcdBat.flash_bar = 0;
			}

			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		}
		else
		{
			// Discharge mode
			surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
			for( i = 0; i < bar; i++ )
			{
				DispLcdBatBar(surface, i);
			}
		}

		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		break;
	case 6: // 4 bar, full
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		bar = val - 2;
		for( i = 0; i < bar; i++ )
		{
			DispLcdBatBar(surface, i);
		}

		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		break;
	default: // Hide
		surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
		for( i = 0; i < 4; i++ )
		{
			DispLcdBatBar(surface, i);
		}
		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_ScreenBatteryLowOff, FALSE);
		break;
	}
}

//-----------------------------------------------------
void CDesktop::InitHandleBatPos()
{
	m_spaceHandleBat = 1;

	m_rcHandleBat.x = m_rcLcdBat3.x + m_rcLcdBat3.w;
	m_rcHandleBat.y = BATTERY_HAND_Y;
	m_rcHandleBat.w = 27;
	m_rcHandleBat.h = BATTERY_HAND_H - 2 * 4;

	m_rcHandleBat2.x = m_rcHandleBat.x + m_rcHandleBat.w;
	m_rcHandleBat2.w = 2;
	m_rcHandleBat2.h = 8;
	m_rcHandleBat2.y = BATTERY_HAND_Y + (m_rcHandleBat.h - m_rcHandleBat2.h) / 2;

	m_rcHandleBat3.x = m_rcHandleBat.x;
	m_rcHandleBat3.y = 0;
	m_rcHandleBat3.w = 30;
	m_rcHandleBat3.h = TOP_LABEL_H;

	m_blockWidthHandleBat = (m_rcHandleBat.w - 7 * m_spaceHandleBat) / 4;

#if 0
	DBG("m_rcHandleBat, x: %d, y: %d, cx: %d, cy: %d\n", \
		m_rcHandleBat.x, m_rcHandleBat.y, m_rcHandleBat.w, m_rcHandleBat.h);

	DBG("m_rcHandleBat2, x: %d, y: %d, cx: %d, cy: %d\n", \
		m_rcHandleBat2.x, m_rcHandleBat2.y, m_rcHandleBat2.w, m_rcHandleBat2.h);

	DBG("m_blockWidthHandleBat: %d\n", m_blockWidthHandleBat);
#endif
}

void CDesktop::DrawHandleBat()
{
	LiteBox*			liteBox	= LITE_BOX(m_lblBat);
	IDirectFBSurface*	surface	= liteBox->surface;

	switch( g_SysBatCtrl.HandleBat.show_flat )
	{
	case FLAT_HIDE: // Hide
		surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
		surface->FillRectangle(surface, m_rcHandleBat3.x, m_rcHandleBat3.y, m_rcHandleBat3.w, m_rcHandleBat3.h);
		break;
	case FLAT_NORMAL: // Normal
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		surface->DrawRectangle(surface, m_rcHandleBat.x, m_rcHandleBat.y, m_rcHandleBat.w, m_rcHandleBat.h);
		surface->FillRectangle(surface, m_rcHandleBat2.x, m_rcHandleBat2.y, m_rcHandleBat2.w, m_rcHandleBat2.h);
		break;
	case FLAT_LOW: // Low, red flat
		surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
		surface->DrawRectangle(surface, m_rcHandleBat.x, m_rcHandleBat.y, m_rcHandleBat.w, m_rcHandleBat.h);
		surface->FillRectangle(surface, m_rcHandleBat2.x, m_rcHandleBat2.y, m_rcHandleBat2.w, m_rcHandleBat2.h);
		break;
	case FLAT_FLASH: // Flash
		if( g_SysBatCtrl.HandleBat.flash_flat == 0 )
		{
			// none
			surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
			surface->FillRectangle(surface, m_rcHandleBat3.x, m_rcHandleBat3.y, m_rcHandleBat3.w, m_rcHandleBat3.h);

			g_SysBatCtrl.HandleBat.flash_flat = 1;
		}
		else
		{
			// low
			surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
			surface->DrawRectangle(surface, m_rcHandleBat.x, m_rcHandleBat.y, m_rcHandleBat.w, m_rcHandleBat.h);
			surface->FillRectangle(surface, m_rcHandleBat2.x, m_rcHandleBat2.y, m_rcHandleBat2.w, m_rcHandleBat2.h);

			g_SysBatCtrl.HandleBat.flash_flat = 0;
		}
		break;
	default:
		break;
	}
}

void CDesktop::DispHandleBatBar(IDirectFBSurface *surface, int bar)
{
	int x, y, height;

	x = m_rcHandleBat.x + 1 + (bar + 1) * m_spaceHandleBat + bar * m_blockWidthHandleBat;
	y = BATTERY_HAND_Y + 2;
	height = BATTERY_HAND_H - 2 * 4 - 4;
	surface->FillRectangle(surface, x, y, m_blockWidthHandleBat, height);
}

void CDesktop::FillHandleBat()
{
	LiteBox*			liteBox	= LITE_BOX(m_lblBat);
	IDirectFBSurface*	surface	= liteBox->surface;

	int i;
	int x, y;
	int height;
	int bar, val;

	val = g_SysBatCtrl.HandleBat.bat_val;

	switch( val )
	{
	case 0: // Flash flat
		surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
		for( i = 0; i < 4; i++ )
		{
			DispHandleBatBar(surface, i);
		}

	#if 0 // xjf
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, TRUE);
	#else
		if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
		{
			// Only display handle battery icon
			m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, TRUE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		}
		else
		{
			m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, TRUE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		}
	#endif
		break;
	case 1: // Flash 1 bar, red
		if( g_SysBatCtrl.HandleBat.flash_bar == 0 )
		{
			surface->SetColor(surface, m_clrBatLow.r, m_clrBatLow.g, m_clrBatLow.b, m_clrBatLow.a);
			DispHandleBatBar(surface, 0);
			g_SysBatCtrl.HandleBat.flash_bar = 1;
		}
		else
		{
			surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
			DispHandleBatBar(surface, 0);
			g_SysBatCtrl.HandleBat.flash_bar = 0;
		}

		if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
		{
			// Only display handle battery icon
			m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, TRUE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		}
		else
		{
			m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, TRUE);
			m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		}
		break;
	case 2: // 1 bar, normal
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		DispHandleBatBar(surface, 0);

		m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
	case 3: // 2 bar, normal
	case 4: // 3 bar, normal
	case 5: // 4 bar, normal
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		bar = val - 1;
		for( i = 0; i < bar; i++ )
		{
			DispHandleBatBar(surface, i);
		}

		m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		break;
	case 6: // 4 bar, full
		surface->SetColor(surface, m_clrBatNormal.r, m_clrBatNormal.g, m_clrBatNormal.b, m_clrBatNormal.a);
		bar = val - 2;
		for( i = 0; i < bar; i++ )
		{
			DispHandleBatBar(surface, i);
		}

		m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		break;
	default: // Hide
		surface->SetColor(surface, m_clrBatBk.r, m_clrBatBk.g, m_clrBatBk.b, m_clrBatBk.a);
		for( i = 0; i < 4; i++ )
		{
			DispHandleBatBar(surface, i);
		}

		m_pNotifictionlMng->SetNotification(Notifiction_BatteryLifeLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLow, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_HandleBatteryLowOff, FALSE);
		break;
	}
}

void CDesktop::CheckBatShow()
{
	if( g_RunParam.m_bCommLost == TRUE )
	{
		// Error communication 10s
		g_SysBatCtrl.iChargeMode = CM_CHARGE_ERRCOMM;
	}

	// Lcd battery show
#if 0 // xjf
	/*static int cnt = 0;
	g_SysBatCtrl.iChargeMode = CM_CHARGE_NONE;
	if( ++cnt >= 5 )
	{
		g_SysBatCtrl.iChargeMode = CM_CHARGE_ERRCOMM;
	}*/
	g_SysBatCtrl.iChargeMode = CM_CHARGE_USB;
	g_SysBatCtrl.LcdBat.bat_val = 5;
#endif
//DBG("SysBatCtrl.iChargeMode: %d\n", g_SysBatCtrl.iChargeMode);
	if( g_SysBatCtrl.iChargeMode == CM_CHARGE_ERRCOMM )
	{
		// Error communication, show hide
		g_SysBatCtrl.LcdBat.show_flat = FLAT_HIDE;
		g_SysBatCtrl.LcdBat.bat_val = 0x0f;
	}
	else
	{
		if( g_SysBatCtrl.iErrCode == ERR_NOT_REACH_3V )
		{
			// charge error, red flat blink
			g_SysBatCtrl.LcdBat.show_flat = FLAT_FLASH;
			g_SysBatCtrl.LcdBat.bat_val = 0;
		}
		else if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE )
		{
			// Charge mode, show one battery icon
			g_SysBatCtrl.LcdBat.show_flat = FLAT_HIDE;
			g_SysBatCtrl.LcdBat.bat_val = 0x0f;
		}
		else if( (g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
			     g_SysBatCtrl.iChargeMode == CM_CHARGE_HU) && \
			     g_SysBatCtrl.iErrCode == ERR_CHARGE_NORMAL )
		{
			// Charge mode, show two battery icon
			g_SysBatCtrl.LcdBat.show_flat = FLAT_NORMAL;
		}
		else
		{
			// Discharge mode
			if( g_SysBatCtrl.LcdBat.bat_val == 0x0f )
				g_SysBatCtrl.LcdBat.show_flat = FLAT_HIDE;
			else if( g_SysBatCtrl.LcdBat.bat_val == 0 )
				g_SysBatCtrl.LcdBat.show_flat = FLAT_FLASH;
			else if( g_SysBatCtrl.LcdBat.bat_val == 1 )
				g_SysBatCtrl.LcdBat.show_flat = FLAT_NORMAL;
			else
				g_SysBatCtrl.LcdBat.show_flat = FLAT_NORMAL;
		}
	}

	// Handle battery show
#if 0 // xjf
	g_SysBatCtrl.HandleBat.bat_val = 1;
#endif
	if( g_SysBatCtrl.iChargeMode == CM_CHARGE_ERRCOMM )
	{
		// Error communication, show hide
		g_SysBatCtrl.HandleBat.show_flat = FLAT_HIDE;
		g_SysBatCtrl.HandleBat.bat_val = 0x0f;
	}
	else
	{
		if( g_SysBatCtrl.HandleBat.bat_val == 0x0f )
			g_SysBatCtrl.HandleBat.show_flat = FLAT_HIDE;
		else if( g_SysBatCtrl.HandleBat.bat_val == 0 )
			g_SysBatCtrl.HandleBat.show_flat = FLAT_FLASH;
		else if( g_SysBatCtrl.HandleBat.bat_val == 1 )
			g_SysBatCtrl.HandleBat.show_flat = FLAT_NORMAL;
		else
			g_SysBatCtrl.HandleBat.show_flat = FLAT_NORMAL;
	}
}

//-----------------------------------------------------
void CDesktop::DrawLabelBat()
{
	//DBG("DrawLabelBat\n");
	DBG();
	CheckBatShow();

#if 1
	// Draw LCD battery
	DrawLcdBat(); // Draw flat
	FillLcdBat(); // Draw bar
#endif

#if 1
	// Draw handle battery
	DrawHandleBat(); // Draw flat
	FillHandleBat(); // Draw bar
#endif
}

//-----------------------------------------------------
void CDesktop::CheckNotification()
{
	//-----------------------------------------------------
	// Handle Not Found
	//DBG("ProcNotification, m_bVideoLost: %d, m_bCommLost: %d\n", \
	//	g_RunParam.m_bVideoLost, g_RunParam.m_bCommLost);

	if( g_RunParam.m_bVideoLost == TRUE || g_RunParam.m_bCommLost == TRUE )
	{
		m_pNotifictionlMng->SetNotification(Notifiction_HandleNotFound, TRUE);

		if( m_pagePtr != NULL && m_curPage == Page_Video )
			((CVideoPage*)m_pagePtr)->abnormalStopRecord();
	}
	else
	{
		m_pNotifictionlMng->SetNotification(Notifiction_HandleNotFound, FALSE);
	}

	//-----------------------------------------------------
	// SDCARD
	if( g_RunParam.sd_disk == DISK_ALMOST_FULL )
	{
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryAlmostFull, TRUE);
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryFull, FALSE);
	}
	else if( g_RunParam.sd_disk == DISK_FULL )
	{
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryAlmostFull, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryFull, TRUE);
	}
	else
	{
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryAlmostFull, FALSE);
		m_pNotifictionlMng->SetNotification(Notifiction_MemeryFull, FALSE);
	}
}

//-----------------------------------------------------
void CDesktop::CheckRunError()
{
#if 0 // xjf, Don't display "battery fault"
	if( g_SysBatCtrl.iErrCode == ERR_NOT_REACH_3V )
	{
		m_pNotifictionlMng->SetAllNotification(FALSE);

		// Only display battery fault
		m_pNotifictionlMng->SetNotification(Notifiction_BatteryFault, TRUE);
	}
	else
	{
		m_pNotifictionlMng->SetNotification(Notifiction_BatteryFault, FALSE);
	}
#else
	m_pNotifictionlMng->SetNotification(Notifiction_BatteryFault, FALSE);
#endif
}

//-----------------------------------------------------
void CDesktop::ProcNotification()
{
	if( m_curPage == Page_Video && !isvideoSavePage() )
	{
		m_pNotifictionlMng->DrawNotification();
	}
}

void CDesktop::AfterDrawLabelBatCB(void *data)
{
	CDesktop *pDesktop = (CDesktop *)data;

	if( pDesktop )
	{
		pDesktop->DrawLabelBat();
	}
}

//-----------------------------------------------------
void CDesktop::AfterDrawTopLabelCB(void *data)
{
	CDesktop *pPage = (CDesktop *)data;

	if (pPage)
	{
		pPage->AfterDrawTopLabel();
	}
}

//-----------------------------------------------------
void CDesktop::AfterDrawTopLabel(void)
{
	LiteBox*			liteBox			= LITE_BOX(m_topLabel);
	IDirectFBSurface*	surface			= liteBox->surface;

	// Draw nothing
}

DFBResult CDesktop::InitTopLabel()
{
	DFBRectangle 		rect;
	LiteLabelTheme 		theme;

	if (m_liteWin == NULL)
	{
		ERR("[DirectFBUI-Desktop] lite window must init first\n");
		return DFB_INIT;
	}

	memset(&theme, 0, sizeof(LiteLabelTheme));

#if 1
	rect.x 			= 0;
	rect.y 			= 0;
	rect.w 			= g_RunParam.screenWidth - SETTING_TXT_W/2+30;
	rect.h 			= TOP_LABEL_H;

	memset(&theme, 0, sizeof(LiteLabelTheme));
	theme.theme.bg_color.a = TOP_LABEL_COLOR_A;
	theme.theme.bg_color.r = TOP_LABEL_COLOR_R;
	theme.theme.bg_color.g = TOP_LABEL_COLOR_G;
	theme.theme.bg_color.b = TOP_LABEL_COLOR_B;

	if(m_topLabel == NULL)
	{
		DFBCHECK(lite_new_label(LITE_BOX(m_liteWin), &rect, &theme, TOP_LABEL_SIZE, NULL, &m_topLabel));
	}

	//lite_set_label_draw_after(m_topLabel, AfterDrawTopLabelCB, this);
#endif

#if 1 // xjf
	// Create battery icon label
	m_rcTwoBat.x 			= rect.w;
	m_rcTwoBat.y 			= 0;
	m_rcTwoBat.w 			= g_RunParam.screenWidth - m_rcTwoBat.x;
	m_rcTwoBat.h 			= TOP_LABEL_H;

	DBG("m_rcTwoBat, x: %d, y: %d, w: %d, h: %d\n", \
		m_rcTwoBat.x, m_rcTwoBat.y, m_rcTwoBat.w, m_rcTwoBat.h);

	memset(&theme, 0, sizeof(LiteLabelTheme));
	theme.theme.bg_color.a = m_clrBatBk.a;
	theme.theme.bg_color.r = m_clrBatBk.r;
	theme.theme.bg_color.g = m_clrBatBk.g;
	theme.theme.bg_color.b = m_clrBatBk.b;

	if( m_lblBat == NULL )
	{
		DFBCHECK(lite_new_label(LITE_BOX(m_liteWin), &m_rcTwoBat, &theme, TOP_LABEL_SIZE, NULL, &m_lblBat));
	}

	lite_set_label_draw_after(m_lblBat, AfterDrawLabelBatCB, this);
#endif

	return DFB_OK;
}

//-----------------------------------------------------
// shut down UI
void CDesktop::InitShutDownUI()
{
	DFBRectangle dfbRect;
	DFBColor fontColor;

	dfbRect.x = 0;
	dfbRect.y = 0;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight;

	if (DFB_OK != lite_new_image(LITE_BOX(m_liteWin), &dfbRect, liteNoImageTheme, &m_imgLogo))
	{
		DBG("m_lblShutDown: create image fail\n");
	}

#if 1
	lite_load_image(m_imgLogo, PIC_PATH"/powerdown-logo.png");
	lite_set_image_visible(m_imgLogo, false);
	lite_update_box(LITE_BOX(m_imgLogo), NULL);
#else
	lite_load_image(m_imgLogo, PIC_PATH"/logo-red.png");
	lite_set_image_visible(m_imgLogo, false);
	lite_update_box(LITE_BOX(m_imgLogo), NULL);

	dfbRect.x 			= 0;
	dfbRect.y 			= g_RunParam.screenHeight - 60;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= RECORD_TIME_TEXT_H;

	fontColor.a			= RECORD_TIME_COLOR_A;
	fontColor.r			= RECORD_TIME_COLOR_R;
	fontColor.g			= RECORD_TIME_COLOR_G;
	fontColor.b			= RECORD_TIME_COLOR_B;

	if (DFB_OK != lite_new_label(LITE_BOX(m_liteWin), &dfbRect, NULL, 18, &fontColor, &m_lblShutDown))
	{
		DBG("m_lblShutDown: create image fail\n");
	}

	// "Shut down" change to "M MSPECTOR FLEX"
	lite_set_label_text(m_lblShutDown, g_CurLangDesc.PowerOnOff.ProductName.c_str());
	lite_set_label_alignment(m_lblShutDown, LITE_LABEL_CENTER);
	lite_set_label_set_visible(m_lblShutDown, false);
#endif
}

//-----------------------------------------------------
// Time stamp UI
void CDesktop::InitTimeStampUI()
{
	DFBRectangle dfbRect;
	DFBColor fontColor;
	char szBuf[32];
	struct tm *ptm;

	time_t now = time(NULL);
	ptm = localtime(&now);

	DBG("Now, %04d-%02d-%02d %02d:%02d:%02d\n", \
		ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, \
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

#if 0 // xjf
	time_t timep;
	time(&timep);

struct tm *tmSrc = gmtime(&timep);
DBG("Src: %u, Year: %d, Month: %d, Day: %d; hour: %d, minute: %d, second: %d\n", \
timep, tmSrc->tm_year, tmSrc->tm_mon, tmSrc->tm_mday, \
tmSrc->tm_hour, tmSrc->tm_min, tmSrc->tm_sec);

struct tm *tmDst = localtime(&now);
DBG("Dst: %u, Year: %d, Month: %d, Day: %d; hour: %d, minute: %d, second: %d\n", \
now, tmDst->tm_year, tmDst->tm_mon, tmDst->tm_mday, \
tmDst->tm_hour, tmDst->tm_min, tmDst->tm_sec);

#endif

	dfbRect.x = 0;
	dfbRect.y = 0;
	dfbRect.w = g_RunParam.screenWidth;
	dfbRect.h = g_RunParam.screenHeight;

	if (DFB_OK != lite_new_image(LITE_BOX(m_liteWin), &dfbRect, liteNoImageTheme, &m_imgStamp))
	{
		DBG("m_imgStamp: create image fail\n");
	}

	lite_load_image(m_imgStamp, PIC_PATH"/bk-red.png");
	lite_set_image_visible(m_imgStamp, false);
	lite_update_box(LITE_BOX(m_imgStamp), NULL);

	//--------------------------------------------------
	int yy = 72;
	int yoff = 0;
	int fontSize = 24;
	int cy = 26;

	fontColor.a			= RECORD_TIME_COLOR_A;
	fontColor.r			= RECORD_TIME_COLOR_R;
	fontColor.g			= RECORD_TIME_COLOR_G;
	fontColor.b			= RECORD_TIME_COLOR_B;

	dfbRect.x 			= 0;
	dfbRect.y 			= yy;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= cy;
	if (DFB_OK == lite_new_label(LITE_BOX(m_liteWin), &dfbRect, NULL, fontSize, &fontColor, &m_lblStampTime))
	{
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			if( ptm->tm_hour >= 12 )
			{
				if( ptm->tm_hour == 12 )
					sprintf(szBuf, "%02d:%02d %s", ptm->tm_hour, ptm->tm_min, g_CurLangDesc.Public.PM.c_str());
				else
					sprintf(szBuf, "%02d:%02d %s", (ptm->tm_hour-12), ptm->tm_min, g_CurLangDesc.Public.PM.c_str());
			}
			else
			{
				if( ptm->tm_hour == 0 )
					sprintf(szBuf, "%02d:%02d %s", (ptm->tm_hour+12), ptm->tm_min, g_CurLangDesc.Public.AM.c_str());
				else
					sprintf(szBuf, "%02d:%02d %s", ptm->tm_hour, ptm->tm_min, g_CurLangDesc.Public.AM.c_str());
			}
		}
		else
		{
			sprintf(szBuf, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}

		lite_set_label_text(m_lblStampTime, szBuf);
		lite_set_label_alignment(m_lblStampTime, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblStampTime, true);
	}

	dfbRect.x 			= 0;
	dfbRect.y 			= dfbRect.y + cy + yoff;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= cy;
	if (DFB_OK == lite_new_label(LITE_BOX(m_liteWin), &dfbRect, NULL, fontSize, &fontColor, &m_lblStampWeek))
	{
		sprintf(szBuf, "%s", g_CurLangDesc.Week[ptm->tm_wday].LongDesc.c_str());
		lite_set_label_text(m_lblStampWeek, szBuf);
		lite_set_label_alignment(m_lblStampWeek, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblStampWeek, true);
	}

	dfbRect.x 			= 0;
	dfbRect.y 			= dfbRect.y + cy + yoff;
	dfbRect.w 			= g_RunParam.screenWidth;
	dfbRect.h 			= cy;
	if (DFB_OK == lite_new_label(LITE_BOX(m_liteWin), &dfbRect, NULL, fontSize, &fontColor, &m_lblStampDate))
	{
	#if 0
		sprintf(szBuf, "%s %02d, %04d", g_CurLangDesc.Month[ptm->tm_mon].ShortDesc.c_str(), \
			ptm->tm_mday, 1900+ptm->tm_year);
	#else
		LoadTimeStamp(ptm, szBuf);
	#endif
		lite_set_label_text(m_lblStampDate, szBuf);
		lite_set_label_alignment(m_lblStampDate, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblStampDate, true);
	}
}

DFBResult CDesktop::DestoryTopLabel()
{
	DFBResult res = DFB_OK;

	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_lblBat);

	SAFE_DEL_LITE(m_lblShutDown);
	SAFE_DEL_LITE(m_imgLogo);

	SAFE_DEL_LITE(m_imgStamp);
	SAFE_DEL_LITE(m_lblStampTime);
	SAFE_DEL_LITE(m_lblStampWeek);
	SAFE_DEL_LITE(m_lblStampDate);

	return res;
}

DFBResult CDesktop::DestoryLiteWindow()
{
	DFBCHECK(lite_destroy_window(m_liteWin));
	DFBCHECK(lite_close());

	DBG("[DirectFBUI-Desktop] uninit lite window success\n");

	return DFB_OK;
}

CAbsPage* CDesktop::GetPagePtr()
{
	return m_pagePtr;
}

void CDesktop::ShowPageByID(Page_E pageID)
{
	DBG("m_curPage: %d, pageID: %d\n", m_curPage, pageID);

	if ((pageID <= Page_Invalid) || (pageID >= Page_End) || pageID == m_curPage )
	{
		return;
	}

#if 1 // xjf
	// Not display all notification
	m_pNotifictionlMng->ShowNotification(FALSE);

	// hide the save modal dialog
	if( m_curPage == Page_Video )
	{
		((CVideoPage*)m_pagePtr)->HideModalDlg();
	}
#endif

	if (m_pagePtr != NULL)
	{
		m_pagePtr->OnPageExit();
		delete m_pagePtr;
		m_pagePtr = NULL;
	}

	switch (pageID)
	{
		case Page_Video:
		{
			m_pagePtr = new CVideoPage();
			break;
		}

		case Page_Setting:
		{
			m_pagePtr = new CSettingPage();
			break;
		}

		case Page_Review:
		{
			m_pagePtr = new CReviewPage();
			break;
		}

		case Page_ConnecttoPC:
		{
			m_pagePtr = new CUSBPage();
			break;
		}

		default:
		{
			break;
		}
	}

	m_curPage = pageID;

	if( m_pagePtr )
	{
		m_pagePtr->OnPageInit();

	#if 1 // xjf
		if( m_curPage == Page_Video )
		{
			DECODE_MP4_S mp4Info;
			mp4Info.playState = DECODE_STATE_STOP; // Enter enc state
			sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
		}
	#endif
	}
}

int CDesktop::videoLostDetected()
{
	v4l2_std_id std = -1;

	if( m_vediofd < 0 )
	{
		m_vediofd = open("/dev/video0",O_RDWR);
		DBG("videoLostDetected, Open video0, m_vediofd: %p\n", m_vediofd);
		if( m_vediofd < 0 )
			return ;
	}
	//change by lixiang
	/*	ioctl(m_vediofd, VIDIOC_QUERYSTD, &std);


	if( (std & V4L2_STD_LOST_LOCK) != V4L2_STD_LOST_LOCK )
	{
		g_RunParam.m_cntVideoLostDetect = 0;
		g_RunParam.m_bVideoLost = FALSE;
	}*/
	if( std  != 0 )
	{
		g_RunParam.m_cntVideoLostDetect = 0;
		g_RunParam.m_bVideoLost = FALSE;
	}
	//done
	else
	{
		// ��Ƶ�źŶ�ʧ
		if( g_RunParam.m_bVideoLost == FALSE )
		{
			if( g_RunParam.m_cntVideoLostDetect < VIDEO_LOST_CHKCNT )
			{
				g_RunParam.m_cntVideoLostDetect++;
			}
			else if( g_RunParam.m_cntVideoLostDetect == VIDEO_LOST_CHKCNT )
			{
				g_RunParam.m_bVideoLost = TRUE;
				g_RunParam.m_cntVideoLostDetect++;
			}
		}
	}

	return 1;
}

void CDesktop::StartDateTimeCheck()
{
	struct tm oTime;
	struct tm *ptm;

	memset(&oTime,0,sizeof oTime);
	oTime.tm_year = 115; //2015
	oTime.tm_mon = 1 - 1;
	oTime.tm_mday = 1;

	time_t timebase = mktime(&oTime);
	time_t t = time(NULL);

	memset(&g_curLocalTime, 0, sizeof(g_curLocalTime));

#if 0 // xjf

struct tm *tmSrc = gmtime(&timebase);
DBG("Src: %u, Year: %d, Month: %d, Day: %d; hour: %d, minute: %d, second: %d\n", \
timebase, tmSrc->tm_year, tmSrc->tm_mon, tmSrc->tm_mday, \
tmSrc->tm_hour, tmSrc->tm_min, tmSrc->tm_sec);

struct tm *tmDst = gmtime(&t);
DBG("Dst: %u, Year: %d, Month: %d, Day: %d; hour: %d, minute: %d, second: %d\n", \
t, tmDst->tm_year, tmDst->tm_mon, tmDst->tm_mday, \
tmDst->tm_hour, tmDst->tm_min, tmDst->tm_sec);

#endif
	//if( t > timebase ) // xjf
	if(1 )
	{
		ptm = localtime(&t);

		g_curLocalTime.tm_year = ptm->tm_year;
		g_curLocalTime.tm_mon = ptm->tm_mon;
		g_curLocalTime.tm_mday = ptm->tm_mday;
		g_curLocalTime.tm_hour = ptm->tm_hour;
		g_curLocalTime.tm_min = ptm->tm_min;
		g_curLocalTime.tm_sec = ptm->tm_sec;
		g_curLocalTime.tm_wday = ptm->tm_wday;

		DBG("aaa, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);

		ShowPageByID(DEFAULT_PAGE);
	}
	else
	{
		// 2015.01.01
		g_curLocalTime.tm_year = 115;
		g_curLocalTime.tm_mon = 0;
		g_curLocalTime.tm_mday = 1;
		// 00:00:00
		g_curLocalTime.tm_hour = 0;
		g_curLocalTime.tm_min = 0;
		g_curLocalTime.tm_sec = 0;

		DBG("bbb, Year: %04d, Month: %02d, Day: %02d; hour: %d, minute: %d, second: %d\n", \
			(1900+g_curLocalTime.tm_year), (g_curLocalTime.tm_mon+1), g_curLocalTime.tm_mday, \
			g_curLocalTime.tm_hour, g_curLocalTime.tm_min, g_curLocalTime.tm_sec);

		g_isFirstTimeSetting = TRUE;
		ShowPageByID(Page_Setting);

		if(m_pagePtr)
		{
			((CSettingPage*)m_pagePtr)->ShowSubPageByID(SubPage_Date);
		}
	}
}

void CDesktop::DoHeartbeat()
{
	//DBG("[DirectFBUI-Desktop] Send heartbeat !\n");

	sys_msg_event_heartbeat(MSG_TYPE_UI_SVR);
}

DFBResult CDesktop::BoardCellCB(void *data)
{
	CDesktop *pDesktop = (CDesktop *)data;

	if (pDesktop)
	{
		pDesktop->ShowBoardCell();
	}

	return DFB_OK;
}

// 1�붨ʱ����
void CDesktop::ShowBoardCell()
{
	int boardBatValue = 0;
	int rockBatValue = 0;

	//DBG("ShowBoardCell\n");

	if( g_RunParam.bShutDown )
		return ;

	if( g_RunParam.cntDispStamp > 0 )
	{
		g_RunParam.cntDispStamp -= 1;
		if( g_RunParam.cntDispStamp == 0 )
		{
		#if 1 // xjf
			SAFE_DEL_LITE(m_imgStamp);
			SAFE_DEL_LITE(m_lblStampTime);
			SAFE_DEL_LITE(m_lblStampWeek);
			SAFE_DEL_LITE(m_lblStampDate);

			StartDateTimeCheck();

			//sys_msg_cmd_encode_enc(); // hu

			g_RunParam.keyValid = TRUE;

		#endif
		}

		DBG("g_RunParam.cntDispStamp: %d\n", g_RunParam.cntDispStamp);
	}
	else
	{
		m_pagePtr->OnUpdateArrow();

		if( g_RunParam.dlyKey > 0 )
		{
			g_RunParam.dlyKey -= 1;
			DBG("g_RunParam.dlyKey: %d\n", g_RunParam.dlyKey);
		}

		g_RunParam.HalfSec = !g_RunParam.HalfSec;
#if 1
		if( g_RunParam.HalfSec == 0 )
		{
			if( g_RunParam.cntHalf1 > 0 )
			{
				g_RunParam.cntHalf1--;
				//DBG("HalfSec: %d, cntHalf1: %d\n", g_RunParam.HalfSec, g_RunParam.cntHalf1);

				if( g_RunParam.cntHalf1 <= 0 )
				{
					lite_update_box(LITE_BOX(m_lblBat), NULL); // DrawLabelBat

					/*videoLostDetected(); // xjf

					// SD Card detect
					if( g_RunParam.m_cntSDCardDetect > 0 )
					{
						g_RunParam.m_cntSDCardDetect--;

					#if sys_msg_cmd_get_tuobang_DEF // xjf
						if( g_RunParam.m_cntSDCardDetect == (DELAY_SDCARD_DETECT-2) )
						{
							// 2S
							SDCardDetected();
							g_RunParam.m_cntSDCardDetect = DELAY_SDCARD_DETECT;
						}
					#else
						if( g_RunParam.m_cntSDCardDetect == (DELAY_SDCARD_DETECT-2) )
						{
							// 2S
							SDCardDetected();
						}
						else if( g_RunParam.m_cntSDCardDetect <= 0 )
						{
							InitStorageMount(); // xjf, for debug
							g_RunParam.m_cntSDCardDetect = DELAY_SDCARD_DETECT;
						}
					#endif
					}
					*/
					// Heartbeat detect
					/*if( g_RunParam.m_cntHeartbeat > 0 )
					{
						g_RunParam.m_cntHeartbeat--;

						if( g_RunParam.m_cntHeartbeat <= 0 )
						{
							DoHeartbeat();
							g_RunParam.m_cntHeartbeat = DELAY_HEARTBEAT;
						}
					}*/

					g_RunParam.cntHalf1 = MAIN_DURATION;
				}
			}
		}
		else
		{
			if( g_RunParam.cntHalf2 > 0 )
			{
				g_RunParam.cntHalf2--;
				//DBG("HalfSec: %d, cntHalf2: %d\n", g_RunParam.HalfSec, g_RunParam.cntHalf2);

				if( g_RunParam.cntHalf2 <= 0 )
				{
					if( !g_isFirstTimeSetting )
					{
						//CheckNotification();

						//CheckRunError();

						//ProcNotification();

						if( m_curPage == Page_Video )
						{
							((CVideoPage*)m_pagePtr)->UpdateRockLed();
						}
					}

					g_RunParam.cntHalf2 = MAIN_DURATION;
				}
			}
		}
	#endif
	}
}

void CDesktop::SDCardDetected()
{
	int ret = 0;

	if( g_RunParam.sdDetect == 0 && !m_bUSBCharge && m_SDCardExsit )
	{
		ret = IsSDCardFree();
		if( ret >= 0 )
		{
			if( ret <= SDCARD_FULL )
			{
				if(m_pagePtr != NULL && m_curPage == Page_Video)
				{
					((CVideoPage*)m_pagePtr)->abnormalStopRecord();
				}

				g_RunParam.sd_disk = DISK_FULL;
			}
			else if( ret <= SDCARD_ALMOST_FULL && ret > SDCARD_FULL )
			{
				g_RunParam.sd_disk = DISK_ALMOST_FULL;
			}
			else
			{
				g_RunParam.sd_disk = DISK_ENOUGH;
			}
		}
		else
		{
			g_RunParam.sd_disk = DISK_INVALID;
		}
	}
	else
	{
		g_RunParam.sd_disk = DISK_INVALID;
	}

#if 0 // xjf
	DBG("g_RunParam.sdDetect: %d, m_SDCardExsit: %d, g_RunParam.sd_disk: %d, free: %d\n", \
		g_RunParam.sdDetect, m_SDCardExsit, g_RunParam.sd_disk, ret);
#endif
}

DFBResult CDesktop::OnDesktopKeyboardEvent(DFBWindowEvent *dfbEvent, void *data)
{
	CDesktop *pDesktop = (CDesktop *)data;
	DFBResult dfbRet = DFB_OK;

	dfbRet = pDesktop->DesktopKeyBoardEvent(dfbEvent);

	return dfbRet;
}

//------------------------------------------------
void CDesktop::HandlePageVideoKeyEvent(DFBWindowEvent *dfbEvent)
{
#if 0 // xjf, test
	static int idx = Notifiction_Begin;
	switch( dfbEvent->key_code)
	{
	case UP_BTN:
	{
		/*if( idx < Notifiction_ScreenBatteryLowOff )
		{
			idx++;
			m_pNotifictionlMng->SetNotification(idx, TRUE);
		}*/
		//g_SysBatCtrl.iErrCode = ERR_NOT_REACH_3V;
		//g_SysBatCtrl.iChargeMode = CM_CHARGE_HU;
		//g_SysBatCtrl.iLedBright = 0;

		/*m_pagePtr->OnPageExit();
		lite_set_image_visible(m_imgLogo, true);
		lite_set_label_set_visible(m_lblShutDown, true);*/

		/*
		DECODE_MP4_S mp4Info;
		mp4Info.playState = DECODE_STATE_IDLE;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
		*/

		CtrlLCDBright(TP_BRIGHTNESS_BEGIN);
	}
		break;
	case DOWN_BTN:
	{
		/*if( idx >= Notifiction_MemeryFull )
		{
			m_pNotifictionlMng->SetNotification(idx, FALSE);
			idx--;
		}*/
		//g_SysBatCtrl.iErrCode = 0;
		//g_SysBatCtrl.iChargeMode = CM_CHARGE_NONE;
		//g_SysBatCtrl.iLedBright = 5;

		/*lite_set_image_visible(m_imgLogo, false);
		lite_set_label_set_visible(m_lblShutDown, false);*/

		/*
		DECODE_MP4_S mp4Info;
		mp4Info.playState = DECODE_STATE_STOP;
		sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);
		*/

		CtrlLCDBright(g_RunParam.tuobangPara.brightNess);
	}
		break;
	default:
		break;
	}
#else
	m_pagePtr->OnKeyEvent(dfbEvent);
#endif

	return DFB_OK;
}

void CDesktop::HandlePageRecordKeyEvent(DFBWindowEvent *dfbEvent)
{
	m_pagePtr->OnKeyEvent(dfbEvent);

	return DFB_OK;
}

void CDesktop::HandlePageSettingKeyEvent(DFBWindowEvent *dfbEvent)
{
#if 0
	if( dfbEvent->key_code == REPLAY_BTN )
	{
		((CSettingPage*)m_pagePtr)->DeleteSubPage();
		ShowPageByID(Page_Review);
	}
	else
	{
		m_pagePtr->OnKeyEvent(dfbEvent);
	}
#else
	m_pagePtr->OnKeyEvent(dfbEvent);
#endif

	return DFB_OK;
}

void CDesktop::HandlePageReviewKeyEvent(DFBWindowEvent *dfbEvent)
{
#if 0
	if( dfbEvent->key_code == SETTING_BTN )
	{
		((CReviewPage*)m_pagePtr)->DeleteSubPage();
		ShowPageByID(Page_Setting);
	}
	else
	{
		m_pagePtr->OnKeyEvent(dfbEvent);
	}
#else
	m_pagePtr->OnKeyEvent(dfbEvent);
#endif

	return DFB_OK;
}

void CDesktop::HandlePageConnectToPCKeyEvent(DFBWindowEvent *dfbEvent)
{
	m_pagePtr->OnKeyEvent(dfbEvent);

	return DFB_OK;
}

void CDesktop::UpdateBacklight()
{
	if( g_RunParam.bkStatus == 0 )
		CtrlLCDBright(TP_BRIGHTNESS_BEGIN);
	else
		CtrlLCDBright(g_RunParam.tuobangPara.brightNess);
}

DFBResult CDesktop::DesktopKeyBoardEvent(DFBWindowEvent *dfbEvent)
{
	DBG("dfbEvent->key_code: %d, g_RunParam.keyValid: %d, g_RunParam.dlyKey: %d\n", \
			dfbEvent->key_code, g_RunParam.keyValid, g_RunParam.dlyKey);

	if( g_RunParam.keyValid && g_RunParam.dlyKey <= 0 )
	{
		if( m_pagePtr )
		{
			switch( m_curPage )
			{
			case Page_Video:
				HandlePageVideoKeyEvent(dfbEvent);
				break;
			case Page_Record:
				HandlePageRecordKeyEvent(dfbEvent);
				break;
			case Page_Setting:
				HandlePageSettingKeyEvent(dfbEvent);
				break;
			case Page_Review:
				HandlePageReviewKeyEvent(dfbEvent);
				break;
			case Page_ConnecttoPC: // Not response any key
				//HandlePageConnectToPCKeyEvent(dfbEvent);
				g_RunParam.dlyUsbKeyDown = DELAY_USB_KEYDOWN;
				g_RunParam.bkStatus = !g_RunParam.bkStatus;
				UpdateBacklight();
				break;
			default:
				m_pagePtr->OnKeyEvent(dfbEvent);
				break;
			}
		}

	#if 0
		if( m_curPage == Page_Video )
		{
			if( dfbEvent->key_code == OK_BTN )
				g_RunParam.dlyKey = 0; // delay for snap or record
			else
				g_RunParam.dlyKey = 0;
		}
		else
		{
			g_RunParam.dlyKey = 0;
		}
	#endif
	}
	else
	{
		cntChkDlyKey++;

		if( cntChkDlyKey >= 3 )
		{
			DBG("StartCheckUSB\n");
			g_RunParam.dlyKey = 0;
			StartCheckUSB();
			cntChkDlyKey = 0;
		}
	}

	if( m_curPage != Page_ConnecttoPC && g_RunParam.bkStatus == 0 )
	{
		g_RunParam.bkStatus = 1;
		UpdateBacklight();
	}

	return DFB_OK;
}

BOOL CDesktop::isvideoSavePage()
{
	BOOL bRet = FALSE;

	if( m_pagePtr != NULL && m_curPage == Page_Video )
	{
		if( ((CVideoPage*)m_pagePtr)->m_modalDialog == NULL )
			bRet = FALSE;
		else
			bRet = TRUE;
	}

	return bRet;
}

void CDesktop::PostKeyMessage(int iMsg)
{
	DFBWindowEvent dfbEvent;

	memset(&dfbEvent, 0, sizeof(DFBWindowEvent));

	dfbEvent.type = DWET_KEYDOWN;
	dfbEvent.key_code = iMsg;

	DesktopKeyBoardEvent(&dfbEvent);
}

void CDesktop::PostWindowMessage(Window_Msg_S *winMsg)
{
	if( g_RunParam.bShutDown == TRUE )
	{
		// Shut down
		//system("sync");

		DBG("PostWindowMessage, shut down, sync, 111\n");
	}
	else
	{
		if( m_pagePtr )
		{
			m_pagePtr->OnWindowEvent(winMsg);
		}

		if( winMsg->msgType == MSG_RECORD_STOP )
		{
			// Shut down
			//DBG("PostWindowMessage, shut down, sync, 000\n");
			//system("sync");

			g_RunParam.keyValid = FALSE; // Not response any key

		#if 1 // xjf
			// Not display all notification
			m_pNotifictionlMng->ShowNotification(FALSE);

			if( m_pagePtr )
				m_pagePtr->OnPageExit();

			lite_set_image_visible(m_imgLogo, true);
			lite_set_label_set_visible(m_lblShutDown, true);
		#endif

			g_RunParam.bShutDown = TRUE;

			DestoryTimer();

			system("sync");
			DBG("PostWindowMessage, shut down, sync, 222\n");
		}
	}
}

BOOL CDesktop::HasStorage(void)
{
	if (strlen(m_devPath) == 0  )
	{
		DBG("DirectFB_UI-Desktop: has not valid devPath!\n");
		return FALSE;
	}

	DBG("DirectFB_UI-Desktop: devPath: %s !\n", m_devPath);

	return TRUE;
}

char *CDesktop::GetDevPath(void)
{
	if (strlen(m_devPath) == 0)
	{
		return NULL;
	}

	return m_devPath;
}

LiteWindow *CDesktop::GetLiTEWindow()
{
	return m_liteWin;
}

void* CDesktop::MsgThreadCB(void *arg)
{
	CDesktop *pDesktop = (CDesktop *)arg;

	if (pDesktop)
	{
		pDesktop->DoMsgThread();
	}

	return NULL;
}

void CDesktop::DoMsgThread(void)
{
	char bufRecv[MSG_BUF_MAX_LEN + sizeof(MSG_BUF_S)];
	char bufSend[MSG_BUF_MAX_LEN + sizeof(MSG_BUF_S)];
	MSG_BUF_S *pMsgbufRecv = (MSG_BUF_S *)bufRecv;
	MSG_BUF_S *pMsgbufSend = (MSG_BUF_S *)bufSend;
	int msg_size;
	int qid;
	int ret;
	BOOL bReply;
	qid = get_sys_msg_id();
	while (GblGetRun())
	{
		bReply = FALSE;
		memset(bufRecv, 0, sizeof(bufRecv));
		msg_size = msg_rcv(qid, (void *)pMsgbufRecv, sizeof(bufRecv), MSG_TYPE_UI_SVR);
		if (msg_size < 0)
		{
			DBG("DirectFB_UI-Desktop: msg server recieve error !\n");
		}
		else if (GET_MSG_DES(*pMsgbufRecv) != MSG_TYPE_UI_SVR)
		{
			DBG("DirectFB_UI-Desktop: msg server recieve error msg ! des:%d src:%d \n",
			GET_MSG_DES(*pMsgbufRecv),GET_MSG_SRC(*pMsgbufRecv));
		}
		else
		{
			DBG("DirectFB_UI-Desktop: src:%d,des:%d,cmd:%d,size:%d!\n",
							GET_MSG_SRC(*pMsgbufRecv),
							GET_MSG_DES(*pMsgbufRecv),
							GET_MSG_CMD(*pMsgbufRecv),
							msg_size);

			ret = ProcMsg(pMsgbufSend, pMsgbufRecv, &bReply);
			if (bReply)
			{
				GET_MSG_DES(*pMsgbufSend) = GET_MSG_SRC(*pMsgbufRecv);
				GET_MSG_SRC(*pMsgbufSend) = MSG_TYPE_UI_SVR;
				GET_MSG_RET(*pMsgbufSend) = ret;
				msg_snd(qid,(void *)pMsgbufSend, sizeof(MSG_BUF_S) + pMsgbufSend->len);
			}
		}
	}
}

int CDesktop::ProcMsg(MSG_BUF_S *pDst, MSG_BUF_S *pSrc, BOOL *bReply)
{
	int ret = MSG_SUCESS;
	Window_Msg_S winMsg;

	switch(GET_MSG_CMD(*pSrc))
	{
		case SYS_MSG_CMD_GET_VERSION:
		{
			MSG_CMD_VERSION_S *pVer = (MSG_CMD_VERSION_S *)pDst->buf;

			pVer->major = MAJOR_VERSION;
			pVer->minor = MINOR_VERSION;
			strncpy(pVer->date ,__DATE__, sizeof(pVer->date) - 1);
			strncpy(pVer->time ,__TIME__, sizeof(pVer->time) - 1);
			pDst->len = sizeof(MSG_CMD_VERSION_S);

			*bReply = TRUE;
			break;
		}

		case SYS_MSG_EVENT_CHNG_TIME:
		{
			DBG("DirectFB_UI-Desktop: time change\n");
			break;
		}

		case SYS_MSG_EVENT_CHNG_DEV:
		{
			MSG_EVENT_CHNG_DEV_S *pReq = (MSG_EVENT_CHNG_DEV_S *)pSrc->buf;
			DealDevChgMsg(pReq);
			break;
		}

		case SYS_MSG_CMD_DECODE_MP4_PARAM:
		{
			DBG("DirectFB_UI-Desktop: SYS_MSG_CMD_DECODE_MP4_PARAM\n");
			DECODE_MP4_S *Mp4Info = (DECODE_MP4_S *)pSrc->buf;

			memset(&winMsg, 0, sizeof(Window_Msg_S));
			winMsg.msgType = MSG_DECODE_PARAM;
			memcpy(winMsg.buffer, Mp4Info, sizeof(DECODE_MP4_S));
			PostWindowMessage(&winMsg);

			break;
		}

		case SYS_MSG_CMD_DECODE_PLAY_STATE_CHG:
		{
			DBG("DirectFB_UI-Desktop: SYS_MSG_CMD_DECODE_PLAY_STATE_CHG\n");
			DECODE_MP4_S *Mp4Info = (DECODE_MP4_S *)pSrc->buf;

			memset(&winMsg, 0, sizeof(Window_Msg_S));
			winMsg.msgType = MSG_DECODE_PLAY_CHANGE;
			memcpy(winMsg.buffer, Mp4Info, sizeof(DECODE_MP4_S));
			PostWindowMessage(&winMsg);

			break;
		}

		case SYS_MSG_CMD_DECODE_GET_CURTIME:
		{
			DBG("DirectFB_UI-Desktop: SYS_MSG_CMD_DECODE_GET_CURTIME\n");
			DECODE_MP4_S *Mp4Info = (DECODE_MP4_S *)pSrc->buf;

			memset(&winMsg, 0, sizeof(Window_Msg_S));
			winMsg.msgType = MSG_DECODE_GET_CURTIME;
			memcpy(winMsg.buffer, Mp4Info, sizeof(DECODE_MP4_S));
			PostWindowMessage(&winMsg);
			break;
		}

		case SYS_MSG_EVENT_DISCONNECT: // To replay
		{
			DBG("DirectFB_UI-Desktop: SYS_MSG_EVENT_DISCONNECT\n");
			DECODE_MP4_S *Mp4Info = (DECODE_MP4_S *)pSrc->buf;

			memset(&winMsg, 0, sizeof(Window_Msg_S));
			winMsg.msgType = MSG_RECORD_REPLAY;
			memcpy(winMsg.buffer, Mp4Info, sizeof(DECODE_MP4_S));
			PostWindowMessage(&winMsg);
		}

		case 4501:
		{

			break;
		}
		default:
		{
			DBG("DirectFB_UI-Desktop: unkown command=%d\n!", GET_MSG_CMD(*pSrc));
			*bReply = FALSE;
			break;
		}
	}

	return ret;
}

void CDesktop::DealDevChgMsg(MSG_EVENT_CHNG_DEV_S *pReq)
{
	DBG("DirectFB_UI-Desktop: devName: %s, action: %s\n", pReq->devName, (pReq->action == 1) ? "remove" : "add");

	if (pReq->action == 1)
	{
		// Remove SD card
		memset(&m_devPath, 0, MAX_DEVPATH_LEN);
    	m_SDCardExsit = FALSE;

		DBG("DealDevChgMsg, m_SDCardExsit = %d\n", m_SDCardExsit);
	}
	else if (pReq->action == 0)
	{
		// Insert SD card
		if( g_RunParam.sdDetect == 0 )
		{
			memset(&m_devPath, 0, MAX_DEVPATH_LEN);
			strncpy(m_devPath, pReq->devName, MAX_DEVPATH_LEN - 1);
			m_SDCardExsit = TRUE;
		}
		else
		{
			memset(&m_devPath, 0, MAX_DEVPATH_LEN);
    		m_SDCardExsit = FALSE;
		}

	#if 0 // xjf, Not use
		if( m_curPage == Page_Video )
			((CVideoPage*)m_pagePtr)->UpdateMsg();
		else if( m_curPage == Page_Review )
			((CReviewPage*)m_pagePtr)->UpdateMsg();
	#endif

		DBG("DealDevChgMsg, m_SDCardExsit = %d, m_curPage: %d\n", m_SDCardExsit, m_curPage);
	}

#if 0 // // xjf, comment 2016.02.24
	if(m_pagePtr != NULL && m_curPage == Page_Setting)
	{
		((CSettingPage*)m_pagePtr)->UpdateCurItem();
		//ShowPageByID(Page_Setting);
	}
#endif
}

void CDesktop::InitStorageMount()
{
	int ret = 0;
    char curPath[256] = {0};
    DIR *p_cur_dir;
    struct stat st;
    struct dirent *p_dirent;

	//DBG("InitStorageMount\n");

	m_SDCardExsit = FALSE;
	memset(&m_devPath, 0, MAX_DEVPATH_LEN);

    memset(&st, 0, sizeof(st));

	if( g_RunParam.sdDetect == 1 )
		goto ERR_EXIT;

    sprintf(curPath, "%s", "/media");
    p_cur_dir = opendir("/media");
    if(p_cur_dir == NULL)
    {
        DBG("opendir error:%s\n", strerror(errno));
        return;
    }

    while( (p_dirent = readdir(p_cur_dir)) != 0 )
    {
        if( '.' == p_dirent->d_name[0] )
            continue;

        sprintf(curPath, "%s/%s", "/media", p_dirent->d_name);
        if( stat(curPath, &st) != 0 )
        {
            DBG("stat :%s error:%s\n", curPath, strerror(errno));
            continue;
        }

        if( S_ISDIR(st.st_mode) )
        {
            ret = StorageMountCheck(p_dirent->d_name);
            //DBG("ret: %d, curPath: %s\n", ret, curPath);
            if( TRUE == ret )
        	{
				strcpy(m_devPath, curPath);
				m_SDCardExsit = TRUE;

			#if 0 // xjf, Not use
				if( m_curPage == Page_Video )
					((CVideoPage*)m_pagePtr)->UpdateMsg();
				else if( m_curPage == Page_Review )
					((CReviewPage*)m_pagePtr)->UpdateMsg();
			#endif

				break;
        	}
        }
    }

	closedir(p_cur_dir);

ERR_EXIT:
	ret = 0;
#if 0
	DBG("InitStorageMount, g_RunParam.sdDetect: %d, m_SDCardExsit = %d, m_devPath: %s\n", \
			g_RunParam.sdDetect, m_SDCardExsit, m_devPath);
#endif
}

int CDesktop::IsSDCardFree()
{
	struct statfs stfs;
	int ret;
	int free,total;
	unsigned long long availableDisk;

	if( strlen(m_devPath) == 0 )
	{
		ret = -1;
		goto ERR_EXIT;
	}

	ret = statfs(m_devPath, &stfs);
	if( ret < 0 )
	{
		ret = -2;
		goto ERR_EXIT;
	}

#if 0
	free = ((stfs.f_bavail >> 10) * stfs.f_bsize) >> 10; // m
	total = ((stfs.f_blocks >> 10) * stfs.f_bsize) >> 10; // m
#else
	availableDisk = stfs.f_bavail * stfs.f_bsize;
	free = availableDisk >> 20; // m
	//total = (stfs.f_blocks * stfs.f_bsize) >> 20; // m
#endif

	ret = free;

	//DBG("Storage path [%s]	Storage total: %d M  free: %d M\n", m_devPath,total,free);

ERR_EXIT:
	return ret;
}

BOOL CDesktop::StorageMountCheck(char *devName)
{
	FILE *fp = NULL;
    char line[512] = {0};
    char dev[64] = {0};
    char point[64] = {0};
    char fsType[64] = {0};
	BOOL bRet = FALSE;

	//DBG("StorageMountCheck, devName: %s\n", devName);

    if (NULL == (fp = fopen("/proc/mounts", "r")))
    {
        ERR("fopen /proc/mounts  errmsg:%s", strerror(errno));
		goto ERR_EXIT;
    }

    while( fgets(line, sizeof(line)-1, fp) )
    {
        sscanf(line, "%s %s %s", dev, point, fsType);
		//DBG("dev: %s, point: %s, fsType: %s\n", dev, point, fsType);

        if (strstr(dev, devName))
        {
        	//DBG("StorageMountCheck, strstr, dev: %s\n", dev);
            bRet =  TRUE;
			break;
        }
        else
        {
            char *ptr = strrchr(point, '/');
            if (ptr && !strcmp(ptr + 1, devName))
            {
            	//DBG("StorageMountCheck, strrchr, ptr: %s\n", ptr);
                bRet = TRUE;
				break;
            }
        }
    }

    fclose(fp);

ERR_EXIT:
	//DBG("StorageMountCheck, bRet = %d\n", bRet);
    return bRet;
}

int CDesktop::ComSetParity(int fd, int databits, int stopbits, int parity)
{
	struct termios options;

	if (tcgetattr(fd, &options) != 0)
	{
		perror("tcgetattr error");
		return -1;
	}

	options.c_cflag &= ~CSIZE;

	// ��������λ��
	switch (databits)
	{
		case 5:
		{
			options.c_cflag |= CS5;
			break;
		}

		case 6:
		{
			options.c_cflag |= CS6;
			break;
		}

		case 7:
		{
			options.c_cflag |= CS7;
			break;
		}

		case 8:
		{
			options.c_cflag |= CS8;
			break;
		}

		default:
		{
			DBG("DirectFB_UI-Desktop: Unsupported data size\n!");
			return -1;
		}
	}

	// �� ��ֹͣλ
	switch (stopbits)
	{
		case 1:
		{
			options.c_cflag &= ~CSTOPB;
			break;
		}
		case 2:
		{
			options.c_cflag |= CSTOPB;
			break;
		}
		default:
		{
			DBG("DirectFB_UI-Desktop: Unsupported stop bits\n!");
			return -1;
		}
	}

	// ������żУ��λ
	switch (parity)
	{
		case 'n':
		case 'N':
		{
			options.c_cflag &= ~PARENB;	// Clear parity enable
			options.c_iflag &= ~INPCK;	// Enable parity checking
			break;
		}

		case 'o':
		case 'O':		// ����Ϊ��Ч��
		{
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;	// Disnable parity checking
			break;
		}

		case 'e':
		case 'E':	// ����ΪżЧ��
		{
			options.c_cflag |= PARENB;	// Enable parity
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;	// Disnable parity checking
			break;
		}

		case 'S':
		case 's':  // as no parity
		{
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		}
		default:
		{
			DBG("DirectFB_UI-Desktop: Unsupported parity\n!");
			return -1;
		}
	}

	// Set input parity option
	if (parity != 'n')
	{
		options.c_iflag |= INPCK;
	}

	options.c_iflag &= ~(IXON | IXOFF | IXANY);	// avoid 0x13 stop the termios
	options.c_iflag &= ~(ICRNL | IGNCR | INLCR);
	options.c_iflag &= ~(ICRNL | IGNCR);
	options.c_oflag &= ~(ONLCR|OCRNL);
	//raw input mode
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//raw output
	options.c_oflag &= ~OPOST;
	tcflush(fd, TCIFLUSH); // Update the options and do it NOW
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("tcsetattr error");
		return -1;
	}

	return 0;
}

void CDesktop::ComSetSpeed(int fd, int speed)
{
	int speed_arr[] = {B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400, B4800, B9600,
						B19200, B38400, B57600, B115200};
	int name_arr[] = {50,  75,	110,  134,	150,  200,	300,  600,	1200,  1800,  2400,  4800,	9600,
						19200,  38400,  57600,  115200};

	int i = 0;
	struct termios Opt;

	tcgetattr(fd, &Opt);

	for (i = 0;  i < ARRAY_SIZE(speed_arr);  i++)
	{
		if (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			if (tcsetattr(fd, TCSANOW, &Opt))
			{
				perror("tcsetattr fd1");
			}

			return;
		}

		tcflush(fd,TCIOFLUSH);
	}
}

void *CDesktop::SerialThreadCB(void *arg)
{
	CDesktop *pDesktop = (CDesktop *)arg;

	if (pDesktop)
	{
		pDesktop->SerialThread();
	}

	return NULL;
}

//------------------------------------------------------------------------------
typedef struct
{
	int fd;

    int baud;
    unsigned char data_bits;
    unsigned char parity;
    unsigned char stop_bits;
} tsUartCtrl, *tspUartCtrl;

//------------------------------------------------------------------------------
int configUart(tspUartCtrl uart)
{
    int speed_arr[] = {B115200, B57600, B9600, B38400, B19200, B4800, B2400, B1200};
    int name_arr[] = {115200, 57600, 9600, 38400, 19200, 4800, 2400, 1200};
    struct termios opt;
    int ret = 0;
    int i = 0;
    int len = 0;

    // get the port attr
    memset(&opt, 0, sizeof(opt));

    opt.c_cflag |= CLOCAL | CREAD;

    // parity bits
    switch( uart->parity )
    {
    case 'N':
    case 'n':
        opt.c_cflag &= ~PARENB;
        opt.c_cflag &= ~INPCK;
        break;
    case 'O':
    case 'o':
        opt.c_cflag |= (INPCK|ISTRIP); // enable parity check, strip parity bits
        opt.c_cflag |= (PARODD | PARENB);
        break;
    case 'E':
    case 'e':
        opt.c_cflag |= (INPCK|ISTRIP); // enable parity check, strip parity bits
        opt.c_cflag |= PARENB;
        opt.c_cflag &= ~PARODD;
        break;
    default:
        ret = -1;
        printf("Unsupported parity bits.\n");
        goto ERR_EXIT;
        break;
    }

    // stop bits
    switch( uart->stop_bits )
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        ret = -1;
        DBG("Unsupported stop bits.\n");
        goto ERR_EXIT;
        break;
    }

    opt.c_cflag &= ~CSIZE; // mask the character size bits

    // data bits
    switch( uart->data_bits )
    {
    case 8:
        opt.c_cflag |= CS8;
        break;
    case 7:
        opt.c_cflag |= CS7;
        break;
    default:
        ret = -1;
        DBG("Unsupported data bits.\n");
        goto ERR_EXIT;
        break;
    }

    // baud rate
    len = sizeof(speed_arr) / sizeof(int);
    for( i = 0; i < len; i++ )
    {
        if( uart->baud == name_arr[i] )
        {
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
            break;
        }
    }
    if( i == len )
    {
        DBG("Unsupported baud rate.\n");
        ret = -1;
        goto ERR_EXIT;
    }

    // stream control
    opt.c_iflag &= ~(IXON | IXOFF | IXANY);

    // raw input
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // raw ouput
    opt.c_oflag &= ~OPOST;

    opt.c_cc[VTIME] = 0; // no time out
    opt.c_cc[VMIN] = 0; // minimum number of characters to read

    tcflush(uart->fd, TCIOFLUSH); // clear buf
    ret = tcsetattr(uart->fd, TCSANOW, &opt); // update it now
    if( ret < 0 )
    {
        DBG("Unable to setup the port.\n");
    }

    //DBG("opt.c_cflag: 0x%x\n", opt.c_cflag); // 0x8bd

ERR_EXIT:
    return ret;
}

void CDesktop::SerialThread(void)
{
	int devFd = -1;
	int databit = 8;
	int stopbit = 1;
	char parity[8] = {"N"};
	fd_set set;
	struct timeval tv;
	int readlen = 0, status = 0;
	//char buffer[NEC_BUF_SIZE] = {0};
	int i;

	volatile int timeoutCount = 0;

	if (0 > (devFd = open(DEV_TTYS1, O_RDWR | O_NOCTTY | O_NDELAY)))
	{
		DBG("DirectFB_UI-Desktop: open %s failure\n", DEV_TTYS1);
		return;
	}

#if 0
	ComSetSpeed (devFd, DEV_BAUD);

	if (-1 == ComSetParity(devFd, databit, stopbit, parity[0]))
	{
		DBG("DirectFB_UI-Desktop: set parity failure\n");
		return;
	}
#else
	tsUartCtrl uart;

	uart.fd = devFd;

	uart.baud = 9600;
    uart.data_bits = 8;
    uart.parity = 'N';
    uart.stop_bits = 1;

	configUart(&uart);
#endif

	while (1)
	{
		FD_ZERO(&set);
		FD_SET(devFd, &set);

		g_RunParam.sdDetect = pChargeManager->isboardCharge();

#define SELECT_WAIT			20
#define SELECT_WAIT_1S		(1000/SELECT_WAIT) // 1��

		tv.tv_sec  = 0;
		tv.tv_usec = SELECT_WAIT*1000;

		status = select(devFd + 1, &set, NULL, NULL, &tv);
		if( status < 0 && errno != EINTR )
		{
			DBG("DirectFB_UI-Desktop: select error\n!");
			break;
		}

		if( status < 0 )
		{
			continue;
		}

		if( status == 0 )
		{
			if( nec_len >= 7 )
			{
				//DBG("ParseNECData\n");
				ParseNECData(nec_buf, nec_len);
			}

			nec_len = 0;
			nec_found = FALSE;

			timeoutCount++;
			//DBG("Select time out, timeoutCount: %d\n", timeoutCount);

			if( timeoutCount >= SELECT_WAIT_1S )
			{
				timeoutCount = 0;

				if( g_RunParam.m_bCommLost == FALSE )
				{
					if( g_RunParam.m_cntCommLostDetect < COMM_NORMAL_CHKCNT )
					{
						g_RunParam.m_bCommLost = FALSE;
						g_RunParam.m_cntCommLostDetect++;
					}
					else if( g_RunParam.m_cntCommLostDetect == COMM_NORMAL_CHKCNT )
					{
						g_RunParam.m_bCommLost = TRUE;
						g_RunParam.m_cntCommLostDetect++;
					}
				}
			}

		#if REMOVE_BAD_FILE
			if( timeoutCount == (SELECT_WAIT_1S-2) && \
				g_RunParam.fileRemove.bAdd == FALSE && \
				g_RunParam.sdDetect == 0 && \
				m_SDCardExsit == TRUE && \
				m_curPage == Page_Review )
			{
				int cntFile = g_RunParam.fileRemove.lstFiles.size();
				if( cntFile > 0 )
				{
					char szFilePath[64];
					string name = g_RunParam.fileRemove.lstFiles.front();

					sprintf(szFilePath, "%s/DCIM/%s", m_devPath, name.c_str());
					DBG("fileRemove, cntFile: %d, szFilePath: %s\n", cntFile, szFilePath);

					//remove(szFilePath);
					g_RunParam.fileRemove.lstFiles.pop_front();
				}
			}
		#endif

			continue;
		}

		//if (FD_ISSET(devFd, &set))
		{
			if (0 > (readlen = read(devFd, nec_buf, sizeof(nec_buf)-nec_len)))
			{
				nec_len = 0;
				nec_found = FALSE;
				continue;
			}

			nec_len += readlen;

			//DBG("readlen: %d, nec_len: %d\n", readlen, nec_len);

			if( nec_len >= sizeof(nec_buf) )
			{
				ParseNECData(nec_buf, nec_len);
				nec_len = 0;
				nec_found = FALSE;
			}

			timeoutCount = 0;
			g_RunParam.m_cntCommLostDetect = 0;
			g_RunParam.m_bCommLost = FALSE;
		}
	}

	if (devFd > 0)
	{
		close(devFd);
	}
}

//�Ƿ�ƽ��, COUNT_MAX����Ⱥ󷵻�true
inline int CDesktop::isSmooth(char *array,int* count,char value)
{
	int k = 0;
	int ret = true;

	array[(*count)++] = value;

	for(k=1;k<COUNT_MAX;k++)
	{
		if(array[k] != array[0])
		{
			ret = false; // ����COUNT_MAX�ζ����
		}
	}

	*count = (*count == COUNT_MAX)?0:*count;

#if 0
	for( int i = 0; i < COUNT_MAX; i++ )
		DBG("isSmooth, array[%d] = %d\n", i, array[i]);
	DBG("isSmooth, ret = %d\n", ret);
#endif

	return ret;
}

/*
��ʼ�� 0xFF
�ܹ�7���ֽ�, ���һ���ֽ�ΪУ����
����
0xFF 0x0x 0x0x PP QQ RR CS
0xFF MM LL PP QQ RR CS
PP: 0x01 ��ӦOK��
QQ: 1 - 100 ����
RR: 1 - 100 LED ����
CS= 0x0x + 0x0x + PP + QQ + RR
*/
int CDesktop::ParseNECData(char *szBuffer, int iLen)
{
	if ((szBuffer == NULL) || (iLen <= 0))
	{
		return -1;
	}

#if 0
	DBG("ParseNECData, iLen: %d\n", iLen);
	for (int j = 0; j < iLen; j ++)
	{
		if (j > 0 && j % 16 == 0)
			printf("\n");
		printf("0x%02x\t", szBuffer[j]);
	}
	printf("\n");
#endif

	int i = 0;
	char beginCode = 0xFF;
	char checkValue = 0;
	char sum = 0;
	char *ptr = NULL;
	U8 MM = 0, PP = 0, QQ = 0, RR = 0, LL = 1;

	for( i = 0; i < iLen; i++ )
	{
		if( szBuffer[i] == beginCode )
			break;
	}

	if( (i == iLen) || (i + 7 > iLen) )
	{
		return -1;
	}

	ptr = &szBuffer[i + 1];
	checkValue = szBuffer[i + 6];
	MM = szBuffer[i + 1];
	LL = szBuffer[i + 2];
	PP = szBuffer[i + 3];
	QQ = szBuffer[i + 4];
	RR = szBuffer[i + 5];

	for (i = 0; i < 5; i++)
	{
		sum += ptr[i];
	}

#if 0 // xjf
	DBG("MM: %02X, LL: %02X, PP: %02X, QQ: %02X, RR: %02X, checkValue: %02X, sum: %02X\n", \
		MM, LL, PP, QQ, RR, checkValue, sum);
#endif

	if (sum != checkValue)
	{
		return -1;
	}

	// STC shut down
	if( LL == LL_SHUTDOWN )
	{
		Window_Msg_S winMsg;
		winMsg.msgType = MSG_RECORD_STOP;

		PostWindowMessage(&winMsg);
	}
	else
	{
		U8 qlo, qhi;

		// STC send OK
		if( PP == PP_OK )
		{
			//PostKeyMessage(OK_BTN);
			g_SysBatCtrl.iTriggerKey = 1;
		}
		else
		{
			g_SysBatCtrl.iTriggerKey = 0;
		}

		// Charge mode
		g_SysBatCtrl.iChargeMode = (MM >> 4) & 0x0f;
		g_SysBatCtrl.iErrCode = MM & 0x0f;

		// Battery Q value
		qhi = (QQ >> 4) & 0x0f;
		// Lcd battery Q value
		if( qhi >= 0 && qhi <= 6 )
		{
			//if( isSmooth(m_BatsValueQQ, &m_QQCount, qhi) )
				g_SysBatCtrl.LcdBat.bat_val = qhi;
		}
		else if( qhi == 0x0f )
		{
			g_SysBatCtrl.LcdBat.bat_val = qhi;
		}

		// Handle battery Q value
		qlo = QQ & 0x0f;
		if( qlo >= 0 && qlo <= 6 )
		{
			//if( isSmooth(m_chargeMM, &m_MMCount, qlo) )
				g_SysBatCtrl.HandleBat.bat_val = qlo;
		}
		else if( qlo == 0x0f )
		{
			g_SysBatCtrl.HandleBat.bat_val = qlo;
		}

		// Led brightness
		RR = RR > 10 ? 10 : RR;
		if( isSmooth(m_ledBrightRR, &m_RRCount, RR) )
		{
			switch( RR )
			{
			case 10:
				m_ledBrightValue = 100;break;
			case 9:
				m_ledBrightValue = 60;break;
			case 8:
				m_ledBrightValue = 30;break;
			case 7:
			case 6:
			case 5:
			case 4:
			case 3:
			case 2:
			case 1:
				m_ledBrightValue = 5;break;
			case 0:
			default:
				m_ledBrightValue = 0;break;
			}
		}

		g_SysBatCtrl.iLedBright = RR;
	}

	return 0;
}

int CDesktop::InitLCD(void)
{
	int retry = 15;
	int value = 0;

	for (; retry > 0; retry--)
	{
		if (0 > (m_lcdDevFd = open(LCD_DEV, O_RDWR | O_NOCTTY)))
		{
			DBG("DirectFB_UI-Desktop: open %s failure, retry = %d\n!", LCD_DEV, retry);
			usleep(10*1000);
			continue;
		}

		break;
	}

	if (0 >= retry)
	{
		DBG("DirectFB_UI-Desktop: open %s failure, m_lcdDevFd = %d\n!", LCD_DEV, m_lcdDevFd);
		return -1;
	}

	DBG("DirectFB_UI-Desktop: open %s success, %d\n!", LCD_DEV, m_lcdDevFd);

	// ������Ļ����
	memset(&g_RunParam.tuobangPara, 0, sizeof(TOP_BAND_PARA_S));

#if sys_msg_cmd_get_tuobang_DEF
	sys_msg_cmd_get_tuobang(&g_RunParam.tuobangPara);
#else
	g_RunParam.tuobangPara.bMicroPhone = 1;
	g_RunParam.tuobangPara.brightNess = 2;
	g_RunParam.tuobangPara.lang = LANG_ENGLISH;
	g_RunParam.tuobangPara.TimeFmt = TIME_FMT_12HR;
#endif

	DBG("InitLCD, bMicroPhone: %d, brightness: %d, lang: %d, TimeFmt: %d\n",
		g_RunParam.tuobangPara.bMicroPhone, \
		(int)g_RunParam.tuobangPara.brightNess, \
		g_RunParam.tuobangPara.lang, \
		g_RunParam.tuobangPara.TimeFmt);

	// Load language
	InitLangDesc(g_RunParam.tuobangPara.lang); // xjf

	CtrlLCDBright(g_RunParam.tuobangPara.brightNess);
	setMic(g_RunParam.tuobangPara.bMicroPhone);

	g_RunParam.bkStatus = 1;

	return 0;
}

void CDesktop::UninitLCD(void)
{
	if (m_lcdDevFd > 0)
	{
		close(m_lcdDevFd);
	}
}

int CDesktop::CtrlLCDBright(TP_BRIGHTNESS_E brightLevel)
{
	DBG("m_lcdDevFd = %d\n", m_lcdDevFd);
	if (m_lcdDevFd < 0)
	{
		return -1;
	}

	DBG("brightLevel = %d\n", (int)brightLevel);
	int bright = BRIGHTNESS_10;

	if (brightLevel == TP_BRIGHTNESS_LOW)
	{
		bright = BRIGHTNESS_8;
	}
	else if (brightLevel == TP_BRIGHTNESS_MEDIUM)
	{
		bright = BRIGHTNESS_5;
	}
	else if (brightLevel == TP_BRIGHTNESS_HIGH)
	{
		bright = BRIGHTNESS_1;
	}
	else
	{
		bright = BRIGHTNESS_10; // all black
	}

	if (0 > ioctl(m_lcdDevFd, LIGHT_CONTROL, &bright))
	{
		return -1;
	}

	return 0;
}

#if 0
void CDesktop::StartCheckUSB()
{
	if (m_checkUsbTimer != -1)
	{
		return;
	}

	// xjf
	lite_enqueue_window_timeout(CHECK_CONNECTTOPC, TimeOut_EveryTime, CheckUSBCB, this, &m_checkUsbTimer);
	DBG("[DirectFBUI-Desktop] create check usb timer succes, id = %d\n", m_checkUsbTimer);
}
#else
void CDesktop::StartCheckUSB()
{
	DFBResult ret = DFB_OK;

	if( g_RunParam.bShutDown )
		return ;

	if( m_boardCellTimeID != -1 ) // xjf
	{
		lite_remove_window_timeout(m_boardCellTimeID);
		m_boardCellTimeID = -1;
	}

	if( m_boardCellTimeID == -1 )
	{
		//ret = lite_enqueue_window_timeout(UPDATE_BOARD_CELL_TIME, TimeOut_EveryTime, BoardCellCB, this, &m_boardCellTimeID);
		ret = lite_enqueue_window_timeout(500, TimeOut_EveryTime, BoardCellCB, this, &m_boardCellTimeID);
		if( ret == DFB_OK)
		{
			DBG("[DirectFBUI-Desktop] create update board cell timer succes, id = %d\n", m_boardCellTimeID);
		}
	}

	if( m_checkUsbTimer != -1 ) // xjf
	{
		lite_remove_window_timeout(m_checkUsbTimer);
		m_checkUsbTimer = -1;
	}

	if( m_checkUsbTimer == -1 )
	{
		ret = lite_enqueue_window_timeout(CHECK_CONNECTTOPC, TimeOut_EveryTime, CheckUSBCB, this, &m_checkUsbTimer);
		if( ret == DFB_OK)
		{
			DBG("[DirectFBUI-Desktop] create check usb timer succes, id = %d\n", m_checkUsbTimer);
		}
	}
}

#endif

int CDesktop::GetRockBrightValue(void)
{
	return m_ledBrightValue;
}

//-----------------------------------------------------
void CDesktop::CheckUsbConnect()
{
	if( g_SysBatCtrl.iChargeMode == CM_CHARGE_USB || \
		g_SysBatCtrl.iChargeMode == CM_CHARGE_HU )
	{
		if( m_bUSBCharge == false )
		{
			if( m_pagePtr != NULL && m_curPage == Page_Video )
			{
				if( ((CVideoPage*)m_pagePtr)->abnormalStopRecord() )
					sleep(1);
			}

			system("umount /media/mmcblk0p1");

			sleep(1);

			if( g_RunParam.sdDetect == 0 )
			{
				system("insmod /app/extdrv/g_file_storage.ko  file=/dev/mmcblk0 removable=1 stall=0");
				DBG("insmod /app/extdrv/g_file_storage.ko file=/dev/mmcblk0 removable=1 stall=0\n");
				g_RunParam.bUsbDrvMount = TRUE;
			}

			// Display "Connect To PC"
			ShowPageByID(Page_ConnecttoPC);

			m_bUSBCharge = true;

			g_RunParam.dlyUsbKeyDown = DELAY_USB_KEYDOWN;
		}
		else
		{
			if( g_RunParam.bUsbDrvMount == TRUE )
			{
				if( g_RunParam.sdDetect == 1 )
				{
					system("rmmod /app/extdrv/g_file_storage.ko");
					DBG("111, rmmod /app/extdrv/g_file_storage.ko\n");
					g_RunParam.bUsbDrvMount = FALSE;
				}
			}
			else
			{
				if( g_RunParam.sdDetect == 0 && m_SDCardExsit == TRUE )
				{
					system("umount /media/mmcblk0p1");

					sleep(1);

					system("insmod /app/extdrv/g_file_storage.ko  file=/dev/mmcblk0 removable=1 stall=0");
					DBG("111, insmod /app/extdrv/g_file_storage.ko file=/dev/mmcblk0 removable=1 stall=0\n");
					g_RunParam.bUsbDrvMount = TRUE;
				}
			}
		}
	}
	else
	{
		if( m_bUSBCharge == true )
		{
			system("rmmod /app/extdrv/g_file_storage.ko");
			DBG("rmmod /app/extdrv/g_file_storage.ko\n");

			sleep(1);

			// Return to video
			ShowPageByID(DEFAULT_PAGE);

			system("mount /dev/mmcblk0p1 /media/mmcblk0p1");

			usleep(300000);

			rcrd_msg_event_req_strginfo();

			m_bUSBCharge = false;

			g_RunParam.bkStatus = 1;
			UpdateBacklight();
		}
	}
}

DFBResult CDesktop::CheckUSBCB(void *data)
{
	CDesktop *pDesktop = (CDesktop *)data;

	if( pDesktop )
	{
		pDesktop->DoCheckUSB();
	}

	return DFB_OK;
}

void CDesktop::DoCheckUSB(void)
{
	//DBG("DoCheckUSB\n");

	if( m_curPage == Page_ConnecttoPC && g_RunParam.dlyUsbKeyDown > 0 )
	{
		g_RunParam.dlyUsbKeyDown--;
		if( g_RunParam.dlyUsbKeyDown <= 0 )
		{
			g_RunParam.bkStatus = 0;
			UpdateBacklight();
		}
	}

#if 0
	// Check USB connect
	CheckUsbConnect();
#endif

	// Update video record timestamp
	if( m_pagePtr != NULL && m_curPage == Page_Video )
	{
		((CVideoPage*)m_pagePtr)->UpdateRecTimeStamp();
	}
}

//������ʱ����
void CDesktop::delayKeyOperate(unsigned int utime)
{
	int ret;

	ret = lite_enqueue_window_timeout(utime, TimeOut_EveryTime, delayKeyTimeCB, this, &m_delayKeyTimeID);

	if( ret == DFB_OK)
	{
		CDesktop::deskKeyLock(FALSE);
		DBG("[DirectFBUI-Desktop] create delayKey timer succes, id = %d\n", m_delayKeyTimeID);
	}
}

static DFBResult CDesktop::delayKeyTimeCB(void *data)
{
	CDesktop *pDesktop = (CDesktop *)data;

	if (pDesktop)
	{
		pDesktop->delayKeyTimeout();
	}

	return DFB_OK;
}

void CDesktop::delayKeyTimeout()
{
	if(m_waitpassTime > 0 && m_waitsetTime >= 1)
	{
		m_waitpassTime --;

		float v = (float)(m_waitsetTime - m_waitpassTime)/m_waitsetTime;
		lite_set_progressbar_value(m_waitingBar, v);
	}
	else
	{
		CDesktop::deskKeyLock(TRUE);

		lite_remove_window_timeout(m_delayKeyTimeID);
		DBG("[DirectFBUI-Desktop] delete delayKey timer succes, id = %d\n", m_delayKeyTimeID);

		if(m_waitingLabel)
		{
			DBG("[DirectFBUI-Desktop] delete  m_waitingLabel\n");

			int res = lite_destroy_box(LITE_BOX(m_waitingLabel));
			if (res != DFB_OK)
			{
				ERR("[DirectFBUI-Desktop] destory label failure\n");
			}

			lite_destroy_box(LITE_BOX(m_waitingBar));

			m_waitsetTime = 0;
			m_waitingLabel = NULL;
			m_waitingBar = NULL;

			if( m_waitfunCb )
			{
				(*m_waitfunCb)(m_waitfunData);
				m_waitfunCb = NULL;
				m_waitfunData = NULL;
		 	}
		}
	}
}

void CDesktop::waitingOperate(unsigned int utime, UI_FUNCTIONCB funCb,void *data)
{
	int ret;

	DFBRectangle 		dfbRect;
	int					screenWidth 	= 0;
	int					screenHeight 	= 0;
	LiteLabelTheme 		theme;
	DFBColor 			fontColor;

	if (m_waitingLabel == NULL)
	{
		lite_get_window_size(m_liteWin, &screenWidth, &screenHeight);
		dfbRect.x 		= 0;
		dfbRect.y 		= TOP_LABEL_H;
		dfbRect.w 		= screenWidth;
		dfbRect.h 		= screenHeight - TOP_LABEL_H;

		memset(&theme, 0, sizeof(LiteLabelTheme));
		theme.theme.bg_color.a = SETTING_TXT_BG_COLOR_A;
		theme.theme.bg_color.r = SETTING_TXT_BG_COLOR_R;
		theme.theme.bg_color.g = SETTING_TXT_BG_COLOR_G;
		theme.theme.bg_color.b = SETTING_TXT_BG_COLOR_B;

		fontColor.a = SETTING_TEXT_COLOR_A;
		fontColor.r = SETTING_TEXT_COLOR_R;
		fontColor.g = SETTING_TEXT_COLOR_G;
		fontColor.b = SETTING_TEXT_COLOR_B;

		if (DFB_OK != lite_new_label(LITE_BOX(m_liteWin), &dfbRect, &theme, 20, &fontColor, &m_waitingLabel))
		{
			DBG("[DirectFBUI-Desktop] create lite label failure\n");
			return FALSE;
		}
	}

	m_waitsetTime = utime/1000;
	m_waitpassTime = m_waitsetTime;
	m_waitfunCb = funCb;
	m_waitfunData = data;

	if(m_waitsetTime >= 1)
	{
		if (m_waitingLabel != NULL)
		{
			//lite_set_label_text(m_waitingLabel, "PLEASE WAITING...");
			lite_set_label_text(m_waitingLabel, g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3.c_str());
			lite_set_label_alignment(m_waitingLabel, LITE_LABEL_CENTER);
		}

		dfbRect.x = screenWidth /6;
		dfbRect.y = screenHeight/2 + 15 + TOP_LABEL_H;
		dfbRect.w = screenWidth *2/3 ;
		dfbRect.h = 10;

		if(m_waitingBar == NULL)
		{
			if (DFB_OK != lite_new_progressbar(LITE_BOX(m_liteWin), &dfbRect, liteNoProgressBarTheme, &m_waitingBar))
			{
				DBG("[DirectFBUI-Desktop] create update progressbar error\n");
			}
		}

		lite_set_progressbar_value(m_waitingBar, 0);
		//lite_set_progressbar_property(m_waitingBar,fontColor,fontColor);

		if (m_waitingBar != NULL)
			lite_update_box(LITE_BOX(m_waitingBar), NULL);

		if (m_waitingLabel != NULL)
			lite_update_box(LITE_BOX(m_waitingLabel), NULL);

		ret = lite_enqueue_window_timeout(1000, TimeOut_EveryTime, delayKeyTimeCB, this, &m_delayKeyTimeID);
	}
	else
	{
		if (m_waitingLabel != NULL)
			lite_update_box(LITE_BOX(m_waitingLabel), NULL);

		ret = lite_enqueue_window_timeout(utime, TimeOut_EveryTime, delayKeyTimeCB, this, &m_delayKeyTimeID);
	}

	if( ret == DFB_OK)
	{
		CDesktop::deskKeyLock(FALSE);
		DBG("[DirectFBUI-Desktop] create delayKey timer succes, id = %d\n", m_delayKeyTimeID);
	}
}

int CDesktop::GetJPEGFileSize(char *filePath,int *width ,int *height )
{
	int ret = FALSE;
	FILE *input_file;
	unsigned char str;
	int first = 0;
    int w = 0,h = 0;

	if(width == NULL || height == NULL || filePath == NULL)
		return ret;

	//DBG("szFilePath = %s  \n",filePath);

    input_file = fopen(filePath,"rb");

	if(input_file == NULL)
	{
		DBG("get File %s SIZE FAILE! \n",filePath);

		 *width = 640;
		 *height = 480;

		 return 0;
	}

	do {
 		fread(&str,sizeof(str),1,input_file);

		if(str == 0xFF )
		{
			fread(&str,sizeof(str),1,input_file);

			if(str == 0xC0)
			{
				fseek(input_file,3,SEEK_CUR);


				fread(&str,sizeof(str),1,input_file);
				h = str;
				fread(&str,sizeof(str),1,input_file);
				h = (h<<8)|str;

				fread(&str,sizeof(str),1,input_file);
				w = str;
				fread(&str,sizeof(str),1,input_file);
				w = (w<<8)|str;
				break;

			}
			else if(str == 0xD9)
			{
				break;
			}
			else{
				first = 1;
				continue;
			}
		}
		else
		{//not jpeg
			if(!first){
				DBG("pic head : %x \n",str);
				fclose(input_file);
				return FALSE;
			}
		}
	} while(1);

	*width = w;
	*height = h;

    fclose(input_file);

	return TRUE;
}

int CDesktop::CheckCanRecordCapture()
{
	int bRet = 0;

	if( g_RunParam.m_bVideoLost == TRUE )
	{
		bRet = 1;
		goto ERR_EXIT;
	}

	if( g_RunParam.sd_disk == DISK_INVALID || \
		g_RunParam.sd_disk == DISK_FULL )
	{
		bRet = 2;
		goto ERR_EXIT;
	}

#if 0
	DBG("iChargeMode: %d, HandleBat.bat_val: %d, LcdBat.bat_val: %d\n",
		g_SysBatCtrl.iChargeMode, g_SysBatCtrl.HandleBat.bat_val, \
		g_SysBatCtrl.LcdBat.bat_val);
#endif

	if( g_SysBatCtrl.iChargeMode == CM_CHARGE_HANDLE || \
		g_SysBatCtrl.iChargeMode == CM_CHARGE_HU )
	{
		// Docked mode
		if( g_SysBatCtrl.HandleBat.bat_val == 0 )
		{
			// Shut down 1min
			bRet = 3;
			goto ERR_EXIT;
		}
	}
	else
	{
		// Undocked mode
		if( g_SysBatCtrl.LcdBat.bat_val == 0 )
		{
			// Shut down 1min
			bRet = 4;
			goto ERR_EXIT;
		}
	}

ERR_EXIT:
	//bRet = 0; // xjf, for test
	DBG("CheckCanRecordCapture, bRet = %d\n", bRet);
	return bRet;
}

BOOL CDesktop::isSDCardExsit()
{
	BOOL bRet = FALSE;

	DBG("isSDCardExsit, g_RunParam.sdDetect: %d, m_SDCardExsit: %d\n", \
		g_RunParam.sdDetect, m_SDCardExsit);

	if( g_RunParam.sdDetect == 0 )
	{
		bRet = m_SDCardExsit;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}





