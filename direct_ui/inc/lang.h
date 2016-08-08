/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   : lang.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.16
******************************************************************************/

#ifndef __LANG_H__
#define __LANG_H__

//-------------------------------------------------
// Language define
//-------------------------------------------------
typedef enum
{
	LANG_BEGIN = -1,
	LANG_ENGLISH,
	LANG_FRENCH,
	LANG_SPANISH,
	LANG_GERMAN,
	LANG_DANISH,
	LANG_DUTCH,
	LANG_END
} teLang;

//-------------------------------------------------
// Notification message
//-------------------------------------------------
typedef enum
{
	Notifiction_Begin = -1,
    Notifiction_MemeryFull, // Memory card is full
    Notifiction_MemeryAlmostFull, // Memory card is 90% full
    Notifiction_HandleNotFound,
    Notifiction_BatteryLifeLow, // Battery charge low, Only show handle battery icon
    Notifiction_HandleBatteryLow, // M12 Battery Charge Low
    Notifiction_ScreenBatteryLow, // Display Charge Low
    Notifiction_BatteryFault, // Battery Fault
    Notifiction_CameraCableDisconnect,
    Notifiction_HandleBatteryLowOff, // M12 Battery Charge Low, Recharge Required
    Notifiction_ScreenBatteryLowOff, // Display Charge Low, Recharge Required
    Notifiction_NoSDCard,
    Notifiction_NoImagesSaved,
    Notifiction_NoFile,
    Notifiction_NoImageSignal,
    Notifiction_DeviceConnected,
    Notifiction_End
} NotifictionId_E;

//-------------------------------------------------
typedef struct
{
	string Logo; // Log file
	string ProductName; // M MSPECTOR FLEX
} tsLangPowerOnOff;

typedef struct
{
	string TIME_HR; // HR
	string TIME_MIN; // MIN
	string AM;
	string PM;
	string HOUR24; // 24
	string SUB_24; // HOUR
	string SUB_12; // 12 HR

	string ROTATE;
	string ZOOM;

	string Save;
	string Cancel;
	string Delete;
	string On;
	string Off;
	string DeleteAll;
	string DELETE;

	string DateTimeLeftRight;
	string DateTimeUpDown;
	string DateTimeSave;
	string DateTimeSavePict; // OK
	string DateTimeExit;
	string DateTimeExitPict; // Exit

	string IMG; // IMG
	string REC; // REC;
	string FIMG; // FIMG
	string FREC; // FREC

} tsLangPublic;

typedef struct
{
	string LongDesc;
	string ShortDesc;
} tsLangWeek;

typedef struct
{
	string LongDesc;
	string ShortDesc;
} tsLangMonth;

//-------------------------------------------------
// LiveView
/*typedef struct
{
	string Rotate;
	string Zoom;
	string Save;
	string Cancel;
} tsLangLiveView;*/

//-------------------------------------------------
// Review
typedef struct
{
	string Edit;
	string Play;
	string Delete;
	string Flag;
	string RemoveFlag;
	string Done;

	string PLAY;
	string PAUSE;
	string RW;
	string FF;
	string EXIT;
	string OkRed;
	string OkWhite;
} tsLangReview;

//-------------------------------------------------
// Setting
//-------------------------------------------------
// Microphone
typedef struct
{
	string Title;
	string Message;
} tsSubMicrophone;

typedef struct
{
	string MainMsg; // MICROPHONE
	string SubMsgOn; // ON
	string SubMsgOff; // OFF
	tsSubMicrophone SubMicrophone;
} tsMainMicrophone;

//-------------------------------------------------
// LCD Brightness
typedef struct
{
	string Title; // SCREEN BRIGHTNESS
	string Low; // LOW
	string Medium; // MEDIUM
	string High; // HIGH
} tsSubLcdBrightness;

typedef struct
{
	string MainMsg; // LCD BRIGHTNESS
	string SubMsgLow; // LOW
	string SubMsgMedium; // MEDIUM
	string SubMsgHigh; // HIGH
	tsSubLcdBrightness SubLcdBrightness;
} tsMainLcdBrightness;

//-------------------------------------------------
// Date and Time
typedef struct
{
	string TitleFirst; // SET DATE
	string Title; // DATE
} tsSubDate;

typedef struct
{
	string MainMsg; // DATE
	string SubMsg; // 11/20/12
	tsSubDate SubDate;
} tsMainDate;

typedef struct
{
	string TitleFirst; // SET TIME
	string Title; // TIME
} tsSubTime;

typedef struct
{
	string MainMsg; // TIME
	string SubMsg; // 12:00 PM
	tsSubTime SubTime;
} tsMainTime;

//-------------------------------------------------
// Language
typedef struct
{
	string English;
	string French;
	string Spanish;
	string Germain;
	string Danish;
	string Dutch;
} tsLang;

typedef struct
{
	string TitleFirst; // SET LANGUAGE
	string Title; // LANGUAGE
	tsLang Lang;
} tsSubLang;

typedef struct
{
	string MainMsg; // LANGUAGE
	string SubMsg; // ENGLISH
	tsSubLang SubLang;
} tsMainLang;

//-------------------------------------------------
// Delete all
typedef struct
{
	string Title;
	string Msg1; // DELETE ALL PHOTOS
	string Msg2; // AND RECORDINGS?
	string Msg3; // DELETING IAMGES
} tsSubDeleteAll;

typedef struct
{
	string MainMsg; // DELETE ALL
	string SubMsg; // -
	tsSubDeleteAll SubDeleteAll;
} tsMainDeleteAll;

//-------------------------------------------------
// Version
typedef struct
{
	string MainMsg; // VERSION
	string SubMsg; // TB_AV03_T_151215A
} tsMainVersion;

//-------------------------------------------------
// Manufacture
typedef struct
{
	string Title; // MANUFACTURE
	string TestColor; // TEST COLOR
	string RestoreParam; // RESTORE PARAM
} tsSubManufacture;

typedef struct
{
	string MainMsg; // Manufacture
	string SubMsg; // -
	tsSubManufacture SubManufacture;
} tsMainManufacture;

//-------------------------------------------------
typedef struct
{
	string Title;
	tsMainMicrophone MainMicrophone;
	tsMainLcdBrightness MainLcdBrightness;

	tsMainDate MainDate;
	tsMainTime MainTime;

	tsMainLang MainLang;
	tsMainDeleteAll MainDeleteAll;

	tsMainVersion MainVersion;

	tsMainManufacture MainManufacture;
} tsLangSetting;

//-------------------------------------------------
// Notification
typedef struct
{
	vector<string> vecMainText;
	vector<string> vecSubText;
	string imgPath;
} tsLangNotiMsg;

//-------------------------------------------------
typedef struct
{
	tsLangPowerOnOff PowerOnOff;

	tsLangPublic Public;

	tsLangWeek Week[7]; // 0 is sunday
	tsLangMonth Month[12];

	//tsLangLiveView LiveView;
	tsLangReview Review;
	tsLangSetting Setting;

	tsLangNotiMsg NotiMsg[Notifiction_End];

} tsLangDesc;

//-------------------------------------------------
extern tsLangDesc g_CurLangDesc;

extern void InitLangDesc(int LangID);
extern void LoadTimeStamp(struct tm *ptm, char *pszStamp);
extern void LoadCurDay(char *pszDay);
extern void LoadCurTime(char *pszTime);
extern void LoadDelMsg(char *pszFile, char *pszBuf);

#endif

