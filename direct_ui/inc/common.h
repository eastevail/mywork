/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: common.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

//------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <algorithm>
using namespace std;

#include "type_common.h"
#include "msg_common.h"
#include "sys_msg_drv.h"
#include "lang.h"

//------------------------------------------------------------------
#include "directfb.h"
#include "direct/list.h"
#include "direct/types.h"
#include "lite/window.h"
#include "lite/lite.h"
#include "lite/util.h"
#include "leck/image.h"
#include "leck/label.h"
#include "leck/textbutton.h"
#include "leck/button.h"
#include "leck/list.h"
#include "leck/progressbar.h"

#ifndef BOOL
typedef unsigned int BOOL;
#endif

#ifndef SUCCESS
#define SUCCESS         0
#endif

#ifndef FAILURE
#define FAILURE         (-1)
#endif

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif
#include"../../common/debug.h"
/*
#define DBG(fmt, args...) \
    do{  \
        fprintf(stdout, "[-DBG-] [%s:%5d] --- " fmt"\n", __FILE__, __LINE__, ## args);  \
    }while(0)
*/

#define ERR(fmt, args...) \
    do{ \
        fprintf(stderr, "[*ERR*] [%s:%5d] *** " fmt, __FILE__, __LINE__, ## args);  \
    }while(0)

#define DFBCHECK(x...)                                                  \
    if (x != DFB_OK) {                                                  \
        fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );          \
        DirectFBErrorFatal( #x, x );                                    \
    }

#define SAFE_DEL_LITE(l)                                                \
    if (l != NULL) {                                                    \
        lite_destroy_box(LITE_BOX(l));                                  \
        l = NULL;                                                       \
    }

#define SAFE_DELETE(o)                                                  \
    if (o != NULL) {                                                    \
        delete o;                                                       \
        o = NULL;                                                       \
    }




typedef enum
{
    Page_Invalid = -1,
    Page_Video,             // ��Ƶҳ��
    Page_Record,            // ¼��ҳ��
    Page_Setting,           // ����ҳ��
    Page_Review,            // ��Ƶ��ͼƬ�鿴ҳ��
    Page_ConnecttoPC,       // ���ӵ�PC
    Page_End
} Page_E;

typedef enum
{
    Zoom_Normal = 0,
    Zoom_X1,
    Zoom_X2,
//    Zoom_X3,
    Zoom_X4,
} Zoom_E;

typedef enum
{
    Rotate_Normal = 1,      // ͼ������
    Rotate_90,              // ͼ����ת90��
    Rotate_180,             // ͼ����ת180��
    Rotate_270              // ͼ����ת270��
} Rotate_E;

typedef enum
{
    Record_Init = 0,        // init
    Record_Stop,            // ¼��ֹͣ
    Record_Ing,             // ����¼��
    Record_cap,            	// ץ��
} Record_E;

typedef enum
{
    MSG_DECODE_PLAY_CHANGE = 0,
    MSG_DECODE_GET_CURTIME,
    MSG_DECODE_PARAM,
    MSG_RECORD_STOP,
    MSG_RECORD_REPLAY,
} Window_Msg_Type;

typedef struct
{
    Window_Msg_Type     msgType;        //   ��Ϣ����
    char                buffer[512];    //
} Window_Msg_S;

#define ARRAY_SIZE(array)               ((int)(sizeof(array) / sizeof(array[0])))
#define MAX_VOLTAGE                     80
#define MIN_VOLTAGE                     0

//lcd defind
#define LCD_DEV                 "/dev/ssd2119"
#define	LIGHT_CONTROL  			1
#define	SLEEP_MODE				2
#define	SCREEN_TURN				3
#define	DISPLAY_CONTROL			4
#define GET_BAT_TAG             5       // ADC��ȡ��ص���

#define BRIGHTNESS_0			0
#define BRIGHTNESS_1			1
#define BRIGHTNESS_2			2
#define BRIGHTNESS_3			3
#define BRIGHTNESS_4			4
#define BRIGHTNESS_5			5
#define BRIGHTNESS_6			6
#define BRIGHTNESS_7			7
#define BRIGHTNESS_8			8
#define BRIGHTNESS_9			9
#define BRIGHTNESS_10			10


#define VIDEO_DETECT_COUNT              3

#define MAX_DEVPATH_LEN                 64
#define UPDATE_FONT_FILE                "font01.tar.gz"
#define UPDATE_FILE                     "update.tar.gz"
#define DEV_TTYS1                       "/dev/ttyS1"
#define DEV_BAUD                        9600

#define LEFT_BTN                        0x03
#define RIGHT_BTN                       0x04
#define UP_BTN                          0x01
#define DOWN_BTN                        0x02
#define OK_BTN                          0x05
#define SETTING_BTN                     0x07
////#define REPLAY_BTN                      0x08
////#define VIDEO_CAP_BTN                   0x06
#define REPLAY_BTN                      0x06            // Ӳ����ӦPCB�Ķ��巴��, Ӧ�ò����µ���
#define VIDEO_CAP_BTN                   0x08

#define UPDATE_BOARD_CELL_TIME          (1 * 1000)        // milliseconds
#define UPDATE_BAR_CELL_TIME            (1 * 1000)        // milliseconds
#define UPDATE_HB_TIME                  (35 * 1000)       // milliseconds
#define CHECK_CONNECTTOPC               (1 * 1000)        // milliseconds

#define DEFAULT_PAGE                    Page_Video

#define KEY_COLOR_A                     0x00
#define KEY_COLOR_R                     0x00
#define KEY_COLOR_G                     0x00
#define KEY_COLOR_B                     0x00

#define MAX_LANG_LEN                    64

#define MAJOR_VERSION                   0
#define MINOR_VERSION                   9
#define UI_SSID                         0xFFFF0569

//#define PIC_PATH                        "/app/vs/tuobang"
#define PIC_PATH                        "./tuobang"
#define NO_PREVIEW_FILE                 "Preview-no.png"
/*******************************���㱳��*******************************************/
#define TOP_LABEL_H                     30
#if 0
#define TOP_LABEL_COLOR_A               0xff
#define TOP_LABEL_COLOR_R               0x3f
#define TOP_LABEL_COLOR_G               0x3f
#define TOP_LABEL_COLOR_B               0x3f
#else
#define TOP_LABEL_COLOR_A               0xff
#define TOP_LABEL_COLOR_R               0x10
#define TOP_LABEL_COLOR_G               0x10
#define TOP_LABEL_COLOR_B               0x10
#endif
#define TOP_LABEL_SIZE                  12
/*******************************���㱳��*******************************************/

/*******************************ʵʱ��Ƶҳ��***************************************/
#define VIDEO_IMG_X                     2
#define VIDEO_IMG_Y                     6
#define VIDEO_IMG_W                     28
#define VIDEO_IMG_H                     16
#define VIDEO_FILE                      "video.png"
#define VIDEO_FILE_1                      "video_1.png"


#define CAPTURE_IMG_X                     3
#define CAPTURE_IMG_Y                     5
#define CAPTURE_IMG_W                     25
#define CAPTURE_IMG_H                     18
#define CAPTURE_FILE                    "camera.png"
#define CAPTURE_FILE_1                    "camera_1.png"


#define ROCK_BRIGHT_IMG_X               VIDEO_IMG_X + VIDEO_IMG_W + 7
#define ROCK_BRIGHT_IMG_Y               4
#define ROCK_BRIGHT_IMG_W               14
#define ROCK_BRIGHT_IMG_H               18
#define ROCK_BRIGHT_FILE                "light0.png"

#define RECORD_TIMER                    1 * 1000            // milliseconds
#define RECORD_IMG_X                    6
#define RECORD_IMG_Y                    VIDEO_IMG_Y
#define RECORD_IMG_W                    15
#define RECORD_IMG_H                    15
#define RECORD_FILE                     "record.png"
#define MIC_FILE                        "mic.png"

#define ROTATE_TXT_IMG_X                VIDEO_IMG_X + VIDEO_IMG_W + 28
//#define ROTATE_TXT_IMG_Y                VIDEO_IMG_Y
#define ROTATE_TXT_IMG_Y                8
#define ROTATE_TXT_IMG_W                54
#define ROTATE_TXT_IMG_H                VIDEO_IMG_H
#define ROTATE_TXT_FILE                 "rotate_word0.png"
#define ROTATE_TXT_FILE_1                 "rotate_word.png"


#define ROTATE_ACTION_X                 ROTATE_TXT_IMG_X + ROTATE_TXT_IMG_W + 8
#define ROTATE_ACTION_Y                 VIDEO_IMG_Y + 4
#define ROTATE_ACTION_W                 14
#define ROTATE_ACTION_H                 7

#define ROTATE_FLG_IMG_X                ROTATE_ACTION_X + ROTATE_ACTION_W + 5
#define ROTATE_FLG_IMG_Y                VIDEO_IMG_Y
#define ROTATE_FLG_IMG_W                8
#define ROTATE_FLG_IMG_H                8
#define ROTATE_FLG_FILE                 "rotate.png"

#define ZOOM_TXT_IMG_X                  ROTATE_FLG_IMG_X + ROTATE_FLG_IMG_W + 15
//#define ZOOM_TXT_IMG_Y                  VIDEO_IMG_Y
#define ZOOM_TXT_IMG_Y                  8
#define ZOOM_TXT_IMG_W                  44
#define ZOOM_TXT_IMG_H                  VIDEO_IMG_H
#define ZOOM_TXT_FILE                   "zoom_word0.png"
#define ZOOM_TXT_FILE_1                   "zoom_word.png"


#define ZOOM_ACTION_X                   ZOOM_TXT_IMG_X + ZOOM_TXT_IMG_W + 8
#define ZOOM_ACTION_Y                   VIDEO_IMG_Y
#define ZOOM_ACTION_W                   7
#define ZOOM_ACTION_H                   14

#define ZOOM_FLG_IMG_X                  ZOOM_ACTION_X + ZOOM_ACTION_W + 5
#define ZOOM_FLG_IMG_Y                  VIDEO_IMG_Y + 1;
#define ZOOM_FLG_IMG_W                  8
#define ZOOM_FLG_IMG_H                  8
#define ZOOM_FLG_FILE                   "zoom.png"

#define ZOOM_TEXT_SIZE                  14
#define ZOOM_NUM_W                      30
#define ZOOM_NUM_H                      20
#define ZOOM_TOP                        TOP_LABEL_H
#define ZOOM_TEXT_COLOR_A               0xFF
#define ZOOM_TEXT_COLOR_R               0xFF
#define ZOOM_TEXT_COLOR_G               0xFF
#define ZOOM_TEXT_COLOR_B               0xFF
#define ZOOM_BG_COLOR_A                 TOP_LABEL_COLOR_A
#define ZOOM_BG_COLOR_R                 TOP_LABEL_COLOR_R
#define ZOOM_BG_COLOR_G                 TOP_LABEL_COLOR_G
#define ZOOM_BG_COLOR_B                 TOP_LABEL_COLOR_B

#define RECORD_TIME_TEXT_X              28
#define RECORD_TIME_TEXT_Y_SPACE        40
#define RECORD_TIME_TEXT_W              100
#define RECORD_TIME_TEXT_H              24
#define RECORD_TIME_FONT_SIZE           24
#define RECORD_TIME_COLOR_A             0xFF
#define RECORD_TIME_COLOR_R             0xFF
#define RECORD_TIME_COLOR_G             0xFF
#define RECORD_TIME_COLOR_B             0xFF

#define BATTERY_HAND_X                  258
#define BATTERY_HAND_Y                  6
#define BATTERY_HAND_H                  24


/*******************************ʵʱ��Ƶҳ��***************************************/


/*******************************  PLAY  ҳ��***************************************/

#define PLAY_REVIEW_IMG_X                     4
#define PLAY_REVIEW_IMG_Y                     BATTERY_HAND_Y
#define PLAY_REVIEW_IMG_W                     16
#define PLAY_REVIEW_IMG_H                     18
#define PLAY_REVIEW_FILE                      "review.png"

#define PLAY_PAUSEOK_IMG_X                     2+PLAY_REVIEW_IMG_W*2+ 12*5
#define PLAY_PAUSEOK_IMG_Y                     PLAY_REVIEW_IMG_Y
#define PLAY_PAUSEOK_IMG_W                     16
#define PLAY_PAUSEOK_IMG_H                     16
#define PLAY_PAUSEOK_ON_FILE                      "pause_ok_on.png"
#define PLAY_PAUSEOK_OFF_FILE                      "pause_ok_off.png"


#define PLAY_EXIT_IMG_X                     2
#define PLAY_EXIT_IMG_Y                     PLAY_REVIEW_IMG_Y
//#define PLAY_EXIT_IMG_W                     20
#define PLAY_EXIT_IMG_W                     16
#define PLAY_EXIT_IMG_H                     16
#define PLAY_EXIT_FILE                      "exit.png"


#define PLAY_FONT_WIDTH                     10
#define PLAY_FONT_Y                    PLAY_REVIEW_IMG_Y






/*******************************  PLAY  ҳ��*******************************************/


/**********************************��ͷ********************************************/
#define ARROW_COLOR_A                   0xFF
#define ARROW_COLOR_R                   0xFF
#define ARROW_COLOR_G                   0xFF
#define ARROW_COLOR_B                   0xFF
#define ARROW_ACTIVE_COLOR_A            0xFF
#define ARROW_ACTIVE_COLOR_R            0xFF
#define ARROW_ACTIVE_COLOR_G            0x00
#define ARROW_ACTIVE_COLOR_B            0x00

#define ARROW_IMG_W                     8
#define ARROW_IMG_H                     8
#define ARROW_IMG_UP1                  "up_0.png"
#define ARROW_IMG_UP2                  "up_1.png"
#define ARROW_IMG_UP3                  "up_3.png"
#define ARROW_IMG_DOWN1                "down_0.png"
#define ARROW_IMG_DOWN2                "down_1.png"
#define ARROW_IMG_DOWN3                "down_3.png"
#define ARROW_IMG_LEFT1                 "left_0.png"
#define ARROW_IMG_LEFT2                 "left_1.png"
#define ARROW_IMG_LEFT3                 "left_3.png"
#define ARROW_IMG_RIGHT1               "right_0.png"
#define ARROW_IMG_RIGHT2               "right_1.png"
#define ARROW_IMG_RIGHT3               "right_3.png"


/**********************************��ͷ********************************************/

/**********************************�Ի���******************************************/
#define DIALOG_BG_COLOR_A               0xFF
#define DIALOG_BG_COLOR_R               0x3f
#define DIALOG_BG_COLOR_G               0x3f
#define DIALOG_BG_COLOR_B               0x3f
#define DIALOG_BTN_BG_COLOR_A           0xFF
#define DIALOG_BTN_BG_COLOR_R           0x70
#define DIALOG_BTN_BG_COLOR_G           0x70
#define DIALOG_BTN_BG_COLOR_B           0x70
#define DIALOG_BTN_ACTIIVE_COLOR_A      0xFF
#define DIALOG_BTN_ACTIIVE_COLOR_R      0xdb
#define DIALOG_BTN_ACTIIVE_COLOR_G      0x01
#define DIALOG_BTN_ACTIIVE_COLOR_B      0x1c
#define DIALOG_BTN_TEXT_COLOR_A         0xFF
#define DIALOG_BTN_TEXT_COLOR_R         0xFF
#define DIALOG_BTN_TEXT_COLOR_G         0xFF
#define DIALOG_BTN_TEXT_COLOR_B         0xFF
#define DIALOG_BTN_TEXT_SIZE            20


#define BTN_SPACE                       18
#define BTN_BOTTOM_SPACE                10


#define DIALOG_TEXT_LINE_H              30

#define DIALOG_BUTTON_H                 36
#define DIALOG_BUTTON_W                 138


/**********************************�Ի���******************************************/

/*******************************ϵͳ����ҳ��***************************************/
#define SETTING_IMG_X                   5
#define SETTING_IMG_Y                   3
#define SETTING_IMG_W                   18
#define SETTING_IMG_H                   18
#define SETTING_FILE                    "settiings.png"
//#define SETTING_TXT_W                   200
#define SETTING_TXT_W                   180
#define SETTING_TXT_H                   TOP_LABEL_H
#define SETTING_TXT_SIZE                18
#define SETTING_TXT_BG_COLOR_A          0xFF
#define SETTING_TXT_BG_COLOR_R          0x3f
#define SETTING_TXT_BG_COLOR_G          0x3f
#define SETTING_TXT_BG_COLOR_B          0x3f
#define SETTING_TEXT_COLOR_A            0xFF;
#define SETTING_TEXT_COLOR_R            0xFF;
#define SETTING_TEXT_COLOR_G            0xFF;
#define SETTING_TEXT_COLOR_B            0xFF;
#define SETTING_LIST_TOP                TOP_LABEL_H
#define LIST_MAIN_FONT_SIZE             28
#define LIST_SUB_FONT_SIZE              16
#define LIST_STR_COLOR_A                0xFF
#define LIST_STR_COLOR_R                0xFF
#define LIST_STR_COLOR_G                0xFF
#define LIST_STR_COLOR_B                0xFF
#define LIST_BG_COLOR_A                 0xFF
#define LIST_BG_COLOR_R                 0x3f
#define LIST_BG_COLOR_G                 0x3f
#define LIST_BG_COLOR_B                 0x3f
#define LIST_LEFT_MARGIN                10
#define LIST_BG_ACTIVE_COLOR_A          0xFF
#define LIST_BG_ACTIVE_COLOR_R          0xFF
#define LIST_BG_ACTIVE_COLOR_G          0x00
#define LIST_BG_ACTIVE_COLOR_B          0x00
#define SHOW_LIST_COUNT                 4
#define SCROLL_BAR_FILE                 "scrollbar.png"
/*******************************ϵͳ����ҳ��***************************************/

/*******************************����ʱ��ҳ��***************************************/
#define DAT_FONT_SIZE                   12

#define DAT_BOTTOM                      10
#define DAT_H                           20

#define DAT_LR_X                        30
#define DAT_LR_W                        70

#define DAT_LR_ARROW_X                  DAT_LR_X + DAT_LR_W + 5
#define DAT_LR_ARROW_W                  14
#define DAT_LR_ARROW_H                  10

#define DAT_UD_X                        DAT_LR_ARROW_X + DAT_LR_ARROW_W + 15
#define DAT_UD_W                        60

#define DAT_UD_ARROW_X                  DAT_UD_X + DAT_UD_W + 5
#define DAT_UD_ARROW_W                  8
#define DAT_UD_ARROW_H                  15

#define DAT_DONE_X                      DAT_UD_ARROW_X + DAT_UD_ARROW_W + 20
#define DAT_DONE_W                      40

#define DAT_DONE_IMAGE                  "done_ok.png"

#define DAT_DONE_IMAGE_X                DAT_UD_ARROW_X + DAT_UD_ARROW_W + 60
#define DAT_DONE_IMAGE_Y                214
#define DAT_DONE_IMAGE_W                12
#define DAT_DONE_IMAGE_H                12





#define DAT_BG_COLOR_A                  0xFF
#define DAT_BG_COLOR_R                  0x70
#define DAT_BG_COLOR_G                  0x70
#define DAT_BG_COLOR_B                  0x70
#define DAT_BG_ACTIVE_COLOR_A           0xFF
#define DAT_BG_ACTIVE_COLOR_R           0xFF
#define DAT_BG_ACTIVE_COLOR_G           0x00
#define DAT_BG_ACTIVE_COLOR_B           0x00

#define DAT_FONT_COLOR_A                0xFF
#define DAT_FONT_COLOR_R                0xFF
#define DAT_FONT_COLOR_G                0xFF
#define DAT_FONT_COLOR_B                0xFF

#define DAT_L_W                         60
#define DAT_L_H                         60
#define DAT_L_Y                         30

#define DAT_FIRST_L_X                   40
#define DAT_SECOND_L_X                  DAT_FIRST_L_X + DAT_L_W + 25
#define DAT_THIRD_L_X                   DAT_SECOND_L_X + DAT_L_W + 25
#define DAT_TIME_FONT_SIZE              32
#define DAT_TIME_SUB_FONT_SIZE          14
/*******************************����ʱ��ҳ��***************************************/

/*******************************�鿴ͼƬ����Ƶҳ��*********************************/
#define REVIEW_TR_W                     200
#define REVIEW_TR_H                     TOP_LABEL_H
#define REVIEW_TOP_BG_COLOR_A           0xFF
#define REVIEW_TOP_BG_COLOR_R           0x3f
#define REVIEW_TOP_BG_COLOR_G           0x3f
#define REVIEW_TOP_BG_COLOR_B           0x3f
#define REVIEW_POST_LEFT                10
#define REVIEW_OUT_POST_W               20
#define REVIEW_IN_POST_W                6
#define REVIEW_POST_H                   30
#define REVIEW_FILE_TYPE_IMAGE_W        28
#define REVIEW_FILE_TYPE_IMAGE_H        18
#define REVIEW_FLAG_IMAGE_W             25
#define REVIEW_FLAG_IMAGE_H             18
#define REVIEW_FLAG_FILE                "flag.png"
#define REVIEW_IMG_FILE                 "camera_1.png"
#define REVIEW_REC_FILE                 "video_1.png"
/*******************************�鿴ͼƬ����Ƶҳ��*********************************/


//------------------------------------------------------------------
//#define SOFT_VERSION  "TB_AV03_T_160226A"
#define SOFT_VERSION  "MILWAUKEE_MSFLEX_160606A"

#define sys_msg_cmd_get_tuobang_DEF		0	// xjf

#define NO_CARD_FILE_FONT_SIZE			26
#define DEVICE_CONNECTED_FONT_SIZE		26

#define TOPLABEL_FONT_SIZE	14

#define REMOVE_BAD_FILE		0

//-----------------------------------------------------
#define MAIN_TEXT_HEIGHT	24
#define MAIN_FONT_SIZE		20
#define MAIN_TEXT1_POS		84
#define MAIN_TEXT2_POS		(MAIN_TEXT1_POS+MAIN_TEXT_HEIGHT+4)

#define SUB_TEXT_HEIGHT		16
#define SUB_FONT_SIZE		14
#define SUB_TEXT_YOFF		26
#define SUB_TEXT1_POS		140
#define SUB_TEXT2_POS		(SUB_TEXT1_POS+SUB_TEXT_YOFF)
#define SUB_TEXT3_POS		(SUB_TEXT2_POS+SUB_TEXT_YOFF)
#define SUB_TEXT_LEFT		80

//--------------------------------------------
typedef enum
{
	FLAT_HIDE = 0,
	FLAT_NORMAL,
	FLAT_LOW,
	FLAT_FLASH
} teShowFlat;

typedef struct
{
	int show_flat; // 0:hide, 1:normal, 2:low, 3:flash
	int flash_flat;
	int bat_val; // f:hide, 0 to 6
	int flash_bar;
} tsBatProp;

typedef struct
{
	int iChargeMode; // Charge mode
	int iErrCode; // Error code
	tsBatProp LcdBat;
	tsBatProp HandleBat;
	int iLedBright;
	volatile int iTriggerKey;
} tsSysBatCrtl;

//------------------------------------------------------------------
#define SDCARD_ALMOST_FULL	100 // 100M
#define SDCARD_FULL			20 	// 20M

typedef enum
{
	DISK_INVALID = -1,
	DISK_FULL,
	DISK_ALMOST_FULL,
	DISK_ENOUGH
} SDCARD_DISK;

//------------------------------------------------------------------
typedef enum
{
	TIME_FMT_12HR = 0,
	TIME_FMT_24HR
} teTimeFmt;

typedef enum
{
	ARROW_RED = 0,
	ARROW_NORMAL
} teArrowStatus;

//------------------------------------------------------------------
typedef struct
{
	volatile BOOL bAdd;
	list<string> lstFiles;
} tsFileRemove;

//------------------------------------------------------------------
typedef struct
{
	volatile int cntDispStamp;

	SDCARD_DISK sd_disk;
	volatile int sdDetect; // CARD_DET_GIO43, 1:no card, 0:with card
	BOOL bUsbDrvMount;

	volatile int m_cntCommLostDetect;
	volatile int m_cntVideoLostDetect;
	BOOL m_bVideoLost;
	BOOL m_bCommLost;

	volatile int m_cntSDCardDetect;
	volatile int m_cntHeartbeat;

	TOP_BAND_PARA_S tuobangPara; // Save in ini file

	int screenWidth;
	int screenHeight;

	volatile int HalfSec;
	volatile int cntHalf1;
	volatile int cntHalf2;

	volatile int dlyKey;
	volatile BOOL keyValid;

	BOOL bShutDown;

	volatile int dlyUsbKeyDown;
	int bkStatus; // 0:Off, 1:On

	tsFileRemove fileRemove;
	TOP_BAND_PIC_S pictProp;

} tsRunParam;

//------------------------------------------------------------------
extern struct tm g_curLocalTime;
extern BOOL g_isFirstTimeSetting;

extern tsRunParam g_RunParam;
extern tsSysBatCrtl g_SysBatCtrl;

#endif




