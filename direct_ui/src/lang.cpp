/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: lang.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.16
******************************************************************************/
#include <stdio.h>
#include "common.h"
#include "lang.h"

//-------------------------------------------------
// Language desc
//-------------------------------------------------
tsLangDesc g_CurLangDesc;

//-------------------------------------------------
void LoadLangEnglish()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "HR";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "HOUR";
	g_CurLangDesc.Public.SUB_12 = "12 HR";


	g_CurLangDesc.Public.ROTATE = "ROTATE";
	g_CurLangDesc.Public.ZOOM = "ZOOM";
	g_CurLangDesc.Public.Save = "Save";
	g_CurLangDesc.Public.Cancel = "Cancel";
	g_CurLangDesc.Public.Delete = "Delete";
	g_CurLangDesc.Public.On = "On";
	g_CurLangDesc.Public.Off = "Off";
	g_CurLangDesc.Public.DeleteAll = "Delete All";
	g_CurLangDesc.Public.DELETE = "DELETE";

	g_CurLangDesc.Public.DateTimeLeftRight = "LEFT/RIGHT";
	g_CurLangDesc.Public.DateTimeUpDown = "UP/DOWN";
	g_CurLangDesc.Public.DateTimeSave = "SAVE";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "EXIT";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "REC";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FREC";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "SUNDAY";
	g_CurLangDesc.Week[0].ShortDesc = "SUN";
	g_CurLangDesc.Week[1].LongDesc = "MONDAY";
	g_CurLangDesc.Week[1].ShortDesc = "MON";
	g_CurLangDesc.Week[2].LongDesc = "TUESDAY";
	g_CurLangDesc.Week[2].ShortDesc = "TUES";
	g_CurLangDesc.Week[3].LongDesc = "WEDNESDAY";
	g_CurLangDesc.Week[3].ShortDesc = "WED";
	g_CurLangDesc.Week[4].LongDesc = "THURSDAY";
	g_CurLangDesc.Week[4].ShortDesc = "THURS";
	g_CurLangDesc.Week[5].LongDesc = "FRIDAY";
	g_CurLangDesc.Week[5].ShortDesc = "FRI";
	g_CurLangDesc.Week[6].LongDesc = "SATURDAY";
	g_CurLangDesc.Week[6].ShortDesc = "SAT";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "JANUARY";
	g_CurLangDesc.Month[0].ShortDesc = "JAN";
	g_CurLangDesc.Month[1].LongDesc = "FEBRUARY";
	g_CurLangDesc.Month[1].ShortDesc = "FEB";
	g_CurLangDesc.Month[2].LongDesc = "MARCH";
	g_CurLangDesc.Month[2].ShortDesc = "MAR";
	g_CurLangDesc.Month[3].LongDesc = "APRIL";
	g_CurLangDesc.Month[3].ShortDesc = "APRIL";
	g_CurLangDesc.Month[4].LongDesc = "MAY";
	g_CurLangDesc.Month[4].ShortDesc = "MAY";
	g_CurLangDesc.Month[5].LongDesc = "JUNE";
	g_CurLangDesc.Month[5].ShortDesc = "JUNE";
	g_CurLangDesc.Month[6].LongDesc = "JULY";
	g_CurLangDesc.Month[6].ShortDesc = "JULY";
	g_CurLangDesc.Month[7].LongDesc = "AUGUST";
	g_CurLangDesc.Month[7].ShortDesc = "AUG";
	g_CurLangDesc.Month[8].LongDesc = "SEPTEMBER";
	g_CurLangDesc.Month[8].ShortDesc = "SEPT";
	g_CurLangDesc.Month[9].LongDesc = " OCTOBER";
	g_CurLangDesc.Month[9].ShortDesc = "OCT";
	g_CurLangDesc.Month[10].LongDesc = " NOVEMBER";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DECEMBER";
	g_CurLangDesc.Month[11].ShortDesc = "DEC";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Edit";
	g_CurLangDesc.Review.Play = "Play";
	g_CurLangDesc.Review.Delete = "Delete";
	g_CurLangDesc.Review.Flag = "Flag";
	g_CurLangDesc.Review.RemoveFlag = "Remove Flag";
	g_CurLangDesc.Review.Done = "Done";

	g_CurLangDesc.Review.PLAY = "PLAY";
	g_CurLangDesc.Review.PAUSE = "PAUSE";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	g_CurLangDesc.Review.EXIT = "EXIT";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "SETTINGS";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MICROPHONE";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "ON";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "OFF";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MICROPHONE";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MICROPHONE";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "LCD BRIGHTNESS";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "LOW";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "HIGH";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "SCREEN BRIGHTNESS";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "LOW";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "HIGH";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "DATE";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12";
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "SET DATE";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "DATE";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "TIME";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "SET TIME";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "TIME";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "LANGUAGE";
	g_CurLangDesc.Setting.MainLang.SubMsg = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "SET LANGUAGE";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "LANGUAGE";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "DELETE ALL";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "DELETE ALL";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "DELETE ALL PHOTOS";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "AND RECORDINGS?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "DELETING IMAGES...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSION";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "MEMORY CARD IS FULL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "MEMORY CARD IS 90% FULL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO IMAGE SIGNAL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "Is handle unit powered on?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Is handle unit within range?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Is camera cable connected?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "BATTERY CHARGE LOW";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "M12 BATTERY CHARGE LOW";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DISPLAY CHARGE LOW";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "BATTERY FAULT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "M12 BATTERY CHARGE LOW,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "RECHARGE REQUIRED";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DISPLAY CHARGE LOW,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "RECHARGE REQUIRED";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO SD CARD";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO IMAGES SAVED";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DEVICE CONNECTED";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void LoadLangFrench()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "HR";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "HEURE";
	g_CurLangDesc.Public.SUB_12 = "12 HR";


	g_CurLangDesc.Public.ROTATE = "TOURNER";
	g_CurLangDesc.Public.ZOOM = "ZOOM";
	g_CurLangDesc.Public.Save = "Enregistrer";
	g_CurLangDesc.Public.Cancel = "Annuler";
	g_CurLangDesc.Public.Delete = "Effacer";
	g_CurLangDesc.Public.On = "Marche";
	g_CurLangDesc.Public.Off = "Arrêt";
	g_CurLangDesc.Public.DeleteAll = "Effacer tout";
	g_CurLangDesc.Public.DELETE = "EFFACER";

	g_CurLangDesc.Public.DateTimeLeftRight = "GAU./DROITE";
	g_CurLangDesc.Public.DateTimeUpDown = "HAUT/BAS";
	g_CurLangDesc.Public.DateTimeSave = "GARDER";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "QUIT.";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "ENR";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FENR";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "DIMANCHE";
	g_CurLangDesc.Week[0].ShortDesc = "DIM";
	g_CurLangDesc.Week[1].LongDesc = "LUNDI";
	g_CurLangDesc.Week[1].ShortDesc = "LUN";
	g_CurLangDesc.Week[2].LongDesc = "MARDI";
	g_CurLangDesc.Week[2].ShortDesc = "MAR";
	g_CurLangDesc.Week[3].LongDesc = "MERCREDI";
	g_CurLangDesc.Week[3].ShortDesc = "MER";
	g_CurLangDesc.Week[4].LongDesc = "JEUDI";
	g_CurLangDesc.Week[4].ShortDesc = "JEUDI";
	g_CurLangDesc.Week[5].LongDesc = "VENDREDI";
	g_CurLangDesc.Week[5].ShortDesc = "VEN";
	g_CurLangDesc.Week[6].LongDesc = "SAMEDI";
	g_CurLangDesc.Week[6].ShortDesc = "SAM";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "JANVIER";
	g_CurLangDesc.Month[0].ShortDesc = "JAN";
	g_CurLangDesc.Month[1].LongDesc = "FÉVRIER";
	g_CurLangDesc.Month[1].ShortDesc = "FÉV";
	g_CurLangDesc.Month[2].LongDesc = "MARS";
	g_CurLangDesc.Month[2].ShortDesc = "MARS";
	g_CurLangDesc.Month[3].LongDesc = "AVRIL";
	g_CurLangDesc.Month[3].ShortDesc = "AVRIL";
	g_CurLangDesc.Month[4].LongDesc = "MAI";
	g_CurLangDesc.Month[4].ShortDesc = "MAI";
	g_CurLangDesc.Month[5].LongDesc = "JUIN";
	g_CurLangDesc.Month[5].ShortDesc = "JUIN";
	g_CurLangDesc.Month[6].LongDesc = "JUILLET";
	g_CurLangDesc.Month[6].ShortDesc = "JUIL";
	g_CurLangDesc.Month[7].LongDesc = "AOÛT";
	g_CurLangDesc.Month[7].ShortDesc = "AOÛT";
	g_CurLangDesc.Month[8].LongDesc = "SEPTEMBRE";
	g_CurLangDesc.Month[8].ShortDesc = "SEPT";
	g_CurLangDesc.Month[9].LongDesc = " OCTOBRE";
	g_CurLangDesc.Month[9].ShortDesc = "OCT";
	g_CurLangDesc.Month[10].LongDesc = " NOVEMBRE";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DÉCEMBRE";
	g_CurLangDesc.Month[11].ShortDesc = "DÉC";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Éditer";
	g_CurLangDesc.Review.Play = "Ir";
	g_CurLangDesc.Review.Delete = "Effacer";
	g_CurLangDesc.Review.Flag = "Drapeau";
	g_CurLangDesc.Review.RemoveFlag = "Enlever le drapeau";
	g_CurLangDesc.Review.Done = "Terminé";

	g_CurLangDesc.Review.PLAY = "IR";
	g_CurLangDesc.Review.PAUSE = "PAUSE";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	g_CurLangDesc.Review.EXIT = "QUIT.";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "RÉGLAGES";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MICROPHONE";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "MARCHE";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "ARRÊT";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MICROPHONE";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MICROPHONE";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "LUMINOSITÉ ACL";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "BAS";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MOYEN";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "ÉLEVÉ";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "LUMINOSITÉ ACL";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "BAS";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MOYEN";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "ÉLEVÉ";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "DATE";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12"; // 2016-1-16
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "RÉGLAGE DE LA DATE";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "DATE";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "HEURE";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "RÉGLAGE DE L'HEURE";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "HEURE";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "LANGUE";
	g_CurLangDesc.Setting.MainLang.SubMsg = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "RÉGLAGE DE LA LANGUE";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "LANGUE";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "EFFACER TOUT";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "EFFACER TOUT";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "EFFACER TOUTES LES PHOTOS";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "ET LES VIDÉOS?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "SUPPRESSION DES IMAGES...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSION";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "IL N'Y A PLUS D'ESPACE LIBRE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "SUR LA CARTE MÉMOIRE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "10 % D'ESPACE LIBRE SUR LA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "CARTE MÉMOIRE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "AUCUN SIGNAL D'IMAGE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "La poignée est allumée?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "La poignée est de l'ordre?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Le câble est connecté?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "PILE FAIBLE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "PILE M12 FAIBLE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "AFFICHAGE PILE FAIBLE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DÉFAUT DE PILE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Pile M12 faible,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "recharge requise";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Affichage pile faible,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "recharge requise";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "AUCUNE CARTE SD";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "AUCUNE IMAGE ENREGISTRÉE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DISPOSITIF CONNECTÉ";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void LoadLangSpanish()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "HR";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "HORA";
	g_CurLangDesc.Public.SUB_12 = "12 HR";

	g_CurLangDesc.Public.ROTATE = "GIRAR";
	g_CurLangDesc.Public.ZOOM = "ACERCAR";
	g_CurLangDesc.Public.Save = "Guardar";
	g_CurLangDesc.Public.Cancel = "Cancelar";
	g_CurLangDesc.Public.Delete = "Eliminar";
	g_CurLangDesc.Public.On = "Encendido";
	g_CurLangDesc.Public.Off = "Apagado";
	g_CurLangDesc.Public.DeleteAll = "Eliminar todo";
	g_CurLangDesc.Public.DELETE = "ELIMINAR";

	g_CurLangDesc.Public.DateTimeLeftRight = "IZQU./DER.";
	g_CurLangDesc.Public.DateTimeUpDown = "ARR./ABAJO";
	g_CurLangDesc.Public.DateTimeSave = "GUARD";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "SALIR";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "REC";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FREC";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "DOMINGO";
	g_CurLangDesc.Week[0].ShortDesc = "DOM";
	g_CurLangDesc.Week[1].LongDesc = "LUNES";
	g_CurLangDesc.Week[1].ShortDesc = "LUN";
	g_CurLangDesc.Week[2].LongDesc = "MARTES";
	g_CurLangDesc.Week[2].ShortDesc = "MAR";
	g_CurLangDesc.Week[3].LongDesc = "MIÉRCOLES";
	g_CurLangDesc.Week[3].ShortDesc = "MIER";
	g_CurLangDesc.Week[4].LongDesc = "JUEVES";
	g_CurLangDesc.Week[4].ShortDesc = "JUE";
	g_CurLangDesc.Week[5].LongDesc = "VIERNES";
	g_CurLangDesc.Week[5].ShortDesc = "VIE";
	g_CurLangDesc.Week[6].LongDesc = "SÁBADO";
	g_CurLangDesc.Week[6].ShortDesc = "SÁB";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "ENERO";
	g_CurLangDesc.Month[0].ShortDesc = "ENE";
	g_CurLangDesc.Month[1].LongDesc = "FEBRERO";
	g_CurLangDesc.Month[1].ShortDesc = "FEB";
	g_CurLangDesc.Month[2].LongDesc = "MARZO";
	g_CurLangDesc.Month[2].ShortDesc = "MAR";
	g_CurLangDesc.Month[3].LongDesc = "ABRIL";
	g_CurLangDesc.Month[3].ShortDesc = "ABRIL";
	g_CurLangDesc.Month[4].LongDesc = "MAYO";
	g_CurLangDesc.Month[4].ShortDesc = "MAYO";
	g_CurLangDesc.Month[5].LongDesc = "JUNIO";
	g_CurLangDesc.Month[5].ShortDesc = "JUNIO";
	g_CurLangDesc.Month[6].LongDesc = "JULIO";
	g_CurLangDesc.Month[6].ShortDesc = "JULIO";
	g_CurLangDesc.Month[7].LongDesc = "AGOSTO";
	g_CurLangDesc.Month[7].ShortDesc = "AGO";
	g_CurLangDesc.Month[8].LongDesc = "SEPTIEMBRE";
	g_CurLangDesc.Month[8].ShortDesc = "SEP";
	g_CurLangDesc.Month[9].LongDesc = " OCTUBRE";
	g_CurLangDesc.Month[9].ShortDesc = "OCT";
	g_CurLangDesc.Month[10].LongDesc = " NOVIEMBRE";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DICIEMBRE";
	g_CurLangDesc.Month[11].ShortDesc = "DIC";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Editar";
	g_CurLangDesc.Review.Play = "Lire";
	g_CurLangDesc.Review.Delete = "Eliminar";
	g_CurLangDesc.Review.Flag = "Marcar";
	g_CurLangDesc.Review.RemoveFlag = "Eliminar marca";
	g_CurLangDesc.Review.Done = "Salir";

	g_CurLangDesc.Review.PLAY = "LIRE";
	g_CurLangDesc.Review.PAUSE = "PAUSA";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	g_CurLangDesc.Review.EXIT = "SALIR";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "CONFIGURACIÓN";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MICRÓFONO";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "ENCENDIDO";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "APAGADO";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MICRÓFONO";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MICRÓFONO";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "INTENSIDAD LCD";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "BAJA";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MEDIA";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "ALTA";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "INTENSIDAD LCD";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "BAJA";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MEDIA";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "ALTA";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "FECHA";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12";
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "AJUSTAR FECHA";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "FECHA";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "HORA";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "AJUSTAR HORA";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "HORA";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "IDIOMA";
	g_CurLangDesc.Setting.MainLang.SubMsg = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "AJUSTAR IDIOMA";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "IDIOMA";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "ELIMINAR TODO";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "ELIMINAR TODO";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "¿ELIMINAR TODAS LAS";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "FOTOGRAFÍAS Y VIDEOS?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "ELIMINANDO IMÁGENES...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSIÓN";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LA TARJETA DE MEMORIA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "ESTÁ LLENA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LA TARJETA DE MEMORIA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "ESTÁ 90% LLENA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO HAY SEÑAL DE IMAGEN";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "¿Está encendida la unidad del asa?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "¿Está la el asa dentro de rango?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "¿Está conectado el cable?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "CARGA DE BATERÍA BAJA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "CARGA DE BATERÍA M12 BAJA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "CARGA DE PANTALLA BAJA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "FALLA DE BATERÍA";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Carga de batería M12 baja,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "se requiere recargar";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Carga de pantalla baja,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "se requiere recargar";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO HAY TARJETA SD";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "NO HAY IMÁGENES GUARDADAS";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "DISPOSITIVO CONECTADO";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void LoadLangGermain()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "STD";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "STUNDE";
	g_CurLangDesc.Public.SUB_12 = "12 STD";

	g_CurLangDesc.Public.ROTATE = "DREHEN";
	g_CurLangDesc.Public.ZOOM = "ZOOM";
	g_CurLangDesc.Public.Save = "Speichern";
	g_CurLangDesc.Public.Cancel = "Abbrechen";
	g_CurLangDesc.Public.Delete = "Löschen";
	g_CurLangDesc.Public.On = "Ein";
	g_CurLangDesc.Public.Off = "Aus";
	g_CurLangDesc.Public.DeleteAll = "Alle löschen";
	g_CurLangDesc.Public.DELETE = "LÖSCHEN";

	g_CurLangDesc.Public.DateTimeLeftRight = "LINKS/RECHTS";
	g_CurLangDesc.Public.DateTimeUpDown = "HOCH/RUN.";
	g_CurLangDesc.Public.DateTimeSave = "HALTEN";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "STOP";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "REC";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FREC";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "SONNTAG";
	g_CurLangDesc.Week[0].ShortDesc = "SO";
	g_CurLangDesc.Week[1].LongDesc = "MONTAG";
	g_CurLangDesc.Week[1].ShortDesc = "MO";
	g_CurLangDesc.Week[2].LongDesc = "DIENSTAG";
	g_CurLangDesc.Week[2].ShortDesc = "DI";
	g_CurLangDesc.Week[3].LongDesc = "MITTWOCH";
	g_CurLangDesc.Week[3].ShortDesc = "MI";
	g_CurLangDesc.Week[4].LongDesc = "DONNERSTAG";
	g_CurLangDesc.Week[4].ShortDesc = "DO";
	g_CurLangDesc.Week[5].LongDesc = "FREITAG";
	g_CurLangDesc.Week[5].ShortDesc = "FR";
	g_CurLangDesc.Week[6].LongDesc = "SAMSTAG";
	g_CurLangDesc.Week[6].ShortDesc = "SA";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "JANUAR";
	g_CurLangDesc.Month[0].ShortDesc = "JAN";
	g_CurLangDesc.Month[1].LongDesc = "FEBRUAR";
	g_CurLangDesc.Month[1].ShortDesc = "FEB";
	g_CurLangDesc.Month[2].LongDesc = "MARZ";
	g_CurLangDesc.Month[2].ShortDesc = "MÄR";
	g_CurLangDesc.Month[3].LongDesc = "APRIL";
	g_CurLangDesc.Month[3].ShortDesc = "APRIL";
	g_CurLangDesc.Month[4].LongDesc = "MAI";
	g_CurLangDesc.Month[4].ShortDesc = "MAI";
	g_CurLangDesc.Month[5].LongDesc = "JUNI";
	g_CurLangDesc.Month[5].ShortDesc = "JUNI";
	g_CurLangDesc.Month[6].LongDesc = "JULI";
	g_CurLangDesc.Month[6].ShortDesc = "JULI";
	g_CurLangDesc.Month[7].LongDesc = "AUGUST";
	g_CurLangDesc.Month[7].ShortDesc = "AUG";
	g_CurLangDesc.Month[8].LongDesc = "SEPTEMBER";
	g_CurLangDesc.Month[8].ShortDesc = "SEPT";
	g_CurLangDesc.Month[9].LongDesc = " OKTOBER";
	g_CurLangDesc.Month[9].ShortDesc = "OKT";
	g_CurLangDesc.Month[10].LongDesc = " NOVEMBER";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DEEMBER";
	g_CurLangDesc.Month[11].ShortDesc = "DEZ";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Bearbeiten";
	g_CurLangDesc.Review.Play = "Gehen";
	g_CurLangDesc.Review.Delete = "Löschen";
	g_CurLangDesc.Review.Flag = "Kennzeichnen";
	g_CurLangDesc.Review.RemoveFlag = "Kennzeichnung entfernen";
	g_CurLangDesc.Review.Done = "Abgeschlossen";

	g_CurLangDesc.Review.PLAY = "GEHEN";
	g_CurLangDesc.Review.PAUSE = "PAUSE";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	g_CurLangDesc.Review.EXIT = "STOP";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "EINSTELLUNGEN";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MIKROFON";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "EIN";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "AUS";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MIKROFON";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MIKROFON";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "LCD-HELLIGKEIT";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "NIEDRIG";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MITTEL";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "HOCH";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "LCD-HELLIGKEIT";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "NIEDRIG";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MITTEL";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "HOCH";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "DATUM";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12";
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "DATUM EINSTELLEN";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "DATUM";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "UHRZEIT";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "UHRZEIT EINSTELLEN";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "UHRZEIT";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "SPRACHE";
	g_CurLangDesc.Setting.MainLang.SubMsg = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "SPRACHE EINSTELLEN";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "SPRACHE";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "ALLE LÖSCHEN";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "ALLE LÖSCHEN";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "ALLE FOTOS UND";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "AUFNAHMEN LÖSCHEN?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "BILDER WERDEN GELÖSCHT...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSION";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "SPEICHERKARTE IST VOLL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "SPEICHERKARTE ZU 90 % VOLL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "KEIN BILDSIGNAL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "Ist das Handle eingeschaltet?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Ist das Handle in der Gegend?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Ist das kabel angeschlossen?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "BATTERIESPANNUNG NIEDRIG";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "M12-BATTERIESPANNUNG NIEDRIG";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "ANZEIGELADUNG NIEDRIG";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "BATTERIEFEHLER";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = " M12-Batteriespannung niedrig,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "Wiederaufladung erforderlich";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Anzeigeladung niedrig,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "Wiederaufladung erforderlich";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "KEINE SD-KARTE";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "KEINE BILDER GESPEICHERT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "GERÄT ANGESCHLOSSEN";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void LoadLangDanish()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "T";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "TIME";
	g_CurLangDesc.Public.SUB_12 = "12 TIMER";

	g_CurLangDesc.Public.ROTATE = "ROTER";
	g_CurLangDesc.Public.ZOOM = "ZOOM";
	g_CurLangDesc.Public.Save = "Gem";
	g_CurLangDesc.Public.Cancel = "Annullér";
	g_CurLangDesc.Public.Delete = "Slet";
	g_CurLangDesc.Public.On = "Til";
	g_CurLangDesc.Public.Off = "Fra";
	g_CurLangDesc.Public.DeleteAll = "Slet alt";
	g_CurLangDesc.Public.DELETE = "SLET";

	g_CurLangDesc.Public.DateTimeLeftRight = "VEN./HØJRE";
	g_CurLangDesc.Public.DateTimeUpDown = "OP/NED";
	g_CurLangDesc.Public.DateTimeSave = "GEM";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "AFSLUT";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "REC";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FREC";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "SØNDAG";
	g_CurLangDesc.Week[0].ShortDesc = "SØN";
	g_CurLangDesc.Week[1].LongDesc = "MANDAG";
	g_CurLangDesc.Week[1].ShortDesc = "MAN";
	g_CurLangDesc.Week[2].LongDesc = "TIRSDAG";
	g_CurLangDesc.Week[2].ShortDesc = "TIRS";
	g_CurLangDesc.Week[3].LongDesc = "ONSDAG";
	g_CurLangDesc.Week[3].ShortDesc = "ONS";
	g_CurLangDesc.Week[4].LongDesc = "TORSDAG";
	g_CurLangDesc.Week[4].ShortDesc = "TORS";
	g_CurLangDesc.Week[5].LongDesc = "FREDAG";
	g_CurLangDesc.Week[5].ShortDesc = "FRE";
	g_CurLangDesc.Week[6].LongDesc = "LØRDAG";
	g_CurLangDesc.Week[6].ShortDesc = "LØR";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "JANUAR";
	g_CurLangDesc.Month[0].ShortDesc = "JAN";
	g_CurLangDesc.Month[1].LongDesc = "FEBRUAR";
	g_CurLangDesc.Month[1].ShortDesc = "FEB";
	g_CurLangDesc.Month[2].LongDesc = "MARTS";
	g_CurLangDesc.Month[2].ShortDesc = "MAR";
	g_CurLangDesc.Month[3].LongDesc = "APRIL";
	g_CurLangDesc.Month[3].ShortDesc = "APRIL";
	g_CurLangDesc.Month[4].LongDesc = "MAJ";
	g_CurLangDesc.Month[4].ShortDesc = "MAJ";
	g_CurLangDesc.Month[5].LongDesc = "JUNI";
	g_CurLangDesc.Month[5].ShortDesc = "JUNI";
	g_CurLangDesc.Month[6].LongDesc = "JULI";
	g_CurLangDesc.Month[6].ShortDesc = "JULI";
	g_CurLangDesc.Month[7].LongDesc = "AUGUST";
	g_CurLangDesc.Month[7].ShortDesc = "AUG";
	g_CurLangDesc.Month[8].LongDesc = "SEPTEMBER";
	g_CurLangDesc.Month[8].ShortDesc = "SEPT";
	g_CurLangDesc.Month[9].LongDesc = " OKTOBER";
	g_CurLangDesc.Month[9].ShortDesc = "OKT";
	g_CurLangDesc.Month[10].LongDesc = " NOVEMBER";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DECEMBER";
	g_CurLangDesc.Month[11].ShortDesc = "DEC";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Rediger";
	g_CurLangDesc.Review.Play = "Afspil";
	g_CurLangDesc.Review.Delete = "Slet";
	g_CurLangDesc.Review.Flag = "Marker";
	g_CurLangDesc.Review.RemoveFlag = "Fjern markering";
	g_CurLangDesc.Review.Done = "Udført";

	g_CurLangDesc.Review.PLAY = "AFSPIL";
	g_CurLangDesc.Review.PAUSE = "PAUSE";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	//g_CurLangDesc.Review.EXIT = "AFSLUT"; // too long
	g_CurLangDesc.Review.EXIT = "AFSLUT";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "INDSTILLINGER";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MIKROFON";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "TIL";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "FRA";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MIKROFON";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MIKROFON";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "LCD SKARPHED";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "LAV";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "HØJ";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "LCD SKARPHED";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "LAV";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "HØJ";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "DATO";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12";
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "INDSTIL DATO";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "DATO";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "TID";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "INDSTIL TID";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "TID";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "SPROG";
	g_CurLangDesc.Setting.MainLang.SubMsg = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "INDSTIL SPROG";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "SPROG";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "SLET ALT";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "SLET ALT";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "SLET ALLE BILLEDER OG";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "OPTAGELSER?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "SLETTER BILLEDER...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSION";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "HUKOMMELSESKORT ER FULDT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "HUKOMMELSESKORT ER";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "90 % FULDT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "INTET BILLEDESIGNAL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "Er håndtagsenheden tændt?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Er håndtagsenheden?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Er kamerakablet tilsluttet?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LAV BATTERIOPLADNING";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LAV M12 BATTERIOPLADNING";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LAV SKÆRMOPLADNING";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "BATTERIFEJL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Lav M12 batteriopladning,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "genopladning er nødvendig";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Lav skærmopladning,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "genopladning er nødvendig";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "INGEN SD-KORT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "INGEN BILLEDER GEMT";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "ENHED TILSLUTTET";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void LoadLangDutch()
{
	// Power ON/OFF
	g_CurLangDesc.PowerOnOff.Logo = PIC_PATH"/logo-red.png";
	g_CurLangDesc.PowerOnOff.ProductName = "M MSPECTOR FLEX";

	// Public
	g_CurLangDesc.Public.TIME_HR = "U";
	g_CurLangDesc.Public.TIME_MIN = "MIN";
	g_CurLangDesc.Public.AM = "AM";
	g_CurLangDesc.Public.PM = "PM";
	g_CurLangDesc.Public.HOUR24 = "24";
	g_CurLangDesc.Public.SUB_24 = "UUR";
	g_CurLangDesc.Public.SUB_12 = "12 U";

	g_CurLangDesc.Public.ROTATE = "DRAAIEN";
	g_CurLangDesc.Public.ZOOM = "ZOOMEN";
	g_CurLangDesc.Public.Save = "Opslaan";
	g_CurLangDesc.Public.Cancel = "Annuleren";
	g_CurLangDesc.Public.Delete = "Verwijderen";
	g_CurLangDesc.Public.On = "Aan";
	g_CurLangDesc.Public.Off = "Uit";
	g_CurLangDesc.Public.DeleteAll = "Alles verw.";
	g_CurLangDesc.Public.DELETE = "VERWIJDEREN";

	g_CurLangDesc.Public.DateTimeLeftRight = "LINKS/RECHTS";
	g_CurLangDesc.Public.DateTimeUpDown = "OMH./OML.";
	g_CurLangDesc.Public.DateTimeSave = "OPSL.";
	g_CurLangDesc.Public.DateTimeSavePict = PIC_PATH"/"DAT_DONE_IMAGE;
	g_CurLangDesc.Public.DateTimeExit = "VERLOF";
	g_CurLangDesc.Public.DateTimeExitPict = PIC_PATH"/"SETTING_FILE;

	g_CurLangDesc.Public.IMG = "IMG";
	g_CurLangDesc.Public.REC = "REC";
	g_CurLangDesc.Public.FIMG = "FIMG";
	g_CurLangDesc.Public.FREC = "FREC";

	// Week
	g_CurLangDesc.Week[0].LongDesc = "ZONDAG";
	g_CurLangDesc.Week[0].ShortDesc = "ZON";
	g_CurLangDesc.Week[1].LongDesc = "MAANDAG";
	g_CurLangDesc.Week[1].ShortDesc = "MAA";
	g_CurLangDesc.Week[2].LongDesc = "DINSDAG";
	g_CurLangDesc.Week[2].ShortDesc = "DIN";
	g_CurLangDesc.Week[3].LongDesc = "WOENSDAG";
	g_CurLangDesc.Week[3].ShortDesc = "WOE";
	g_CurLangDesc.Week[4].LongDesc = "DONDERDAG";
	g_CurLangDesc.Week[4].ShortDesc = "DON";
	g_CurLangDesc.Week[5].LongDesc = "VRIJDAG";
	g_CurLangDesc.Week[5].ShortDesc = "VRIJ";
	g_CurLangDesc.Week[6].LongDesc = "ZATERDAG";
	g_CurLangDesc.Week[6].ShortDesc = "ZAT";

	// Month
	g_CurLangDesc.Month[0].LongDesc = "JANUARI";
	g_CurLangDesc.Month[0].ShortDesc = "JAN";
	g_CurLangDesc.Month[1].LongDesc = "FEBRUARI";
	g_CurLangDesc.Month[1].ShortDesc = "FEB";
	g_CurLangDesc.Month[2].LongDesc = "MAART";
	g_CurLangDesc.Month[2].ShortDesc = "MRT";
	g_CurLangDesc.Month[3].LongDesc = "APRIL";
	g_CurLangDesc.Month[3].ShortDesc = "APRIL";
	g_CurLangDesc.Month[4].LongDesc = "MEI";
	g_CurLangDesc.Month[4].ShortDesc = "MEI";
	g_CurLangDesc.Month[5].LongDesc = "JUNI";
	g_CurLangDesc.Month[5].ShortDesc = "JUNI";
	g_CurLangDesc.Month[6].LongDesc = "JULI";
	g_CurLangDesc.Month[6].ShortDesc = "JULI";
	g_CurLangDesc.Month[7].LongDesc = "AUGUSTUS";
	g_CurLangDesc.Month[7].ShortDesc = "AUG";
	g_CurLangDesc.Month[8].LongDesc = "SEPTEMBER";
	g_CurLangDesc.Month[8].ShortDesc = "SEPT";
	g_CurLangDesc.Month[9].LongDesc = " OKTOBER";
	g_CurLangDesc.Month[9].ShortDesc = "OKT";
	g_CurLangDesc.Month[10].LongDesc = " NOVEMBER";
	g_CurLangDesc.Month[10].ShortDesc = "NOV";
	g_CurLangDesc.Month[11].LongDesc = " DECEMBER";
	g_CurLangDesc.Month[11].ShortDesc = "DEC";

	// LiveView
	/*g_CurLangDesc.LiveView.Rotate = "ROTATE";
	g_CurLangDesc.LiveView.Zoom = "ZOOM";
	g_CurLangDesc.LiveView.Save = "Save";
	g_CurLangDesc.LiveView.Cancel = "Cancel";*/

	// Review
	g_CurLangDesc.Review.Edit = "Bewerken";
	g_CurLangDesc.Review.Play = "Gaan";
	g_CurLangDesc.Review.Delete = "Verwijderen";
	g_CurLangDesc.Review.Flag = "Vlag";
	g_CurLangDesc.Review.RemoveFlag = "Vlag verwijderen";
	g_CurLangDesc.Review.Done = "Gereed";

	g_CurLangDesc.Review.PLAY = "GAAN";
	g_CurLangDesc.Review.PAUSE = "PAUZE";
	g_CurLangDesc.Review.RW = "RW";
	g_CurLangDesc.Review.FF = "FF";
	//g_CurLangDesc.Review.EXIT = "VERLOF"; // too long
	g_CurLangDesc.Review.EXIT = "VERLOF";
	g_CurLangDesc.Review.OkRed = PIC_PATH"/OkRed.png";
	g_CurLangDesc.Review.OkWhite = PIC_PATH"/OkWhite.png";

	// Setting
	g_CurLangDesc.Setting.Title = "INSTELLINGEN";
	// Microphone
	g_CurLangDesc.Setting.MainMicrophone.MainMsg = "MICROFOON";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOn = "AAN";
	g_CurLangDesc.Setting.MainMicrophone.SubMsgOff = "UIT";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Title = "MICROFOON";
	g_CurLangDesc.Setting.MainMicrophone.SubMicrophone.Message = "MICROFOON";
	// LCD Brightness
	g_CurLangDesc.Setting.MainLcdBrightness.MainMsg = "HELDERHEID LCD";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgLow = "LAAG";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgMedium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubMsgHigh = "HOOG";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Title = "HELDERHEID LCD";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Low = "LAAG";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.Medium = "MEDIUM";
	g_CurLangDesc.Setting.MainLcdBrightness.SubLcdBrightness.High = "HOOG";
	// Date
	g_CurLangDesc.Setting.MainDate.MainMsg = "DATUM";
	g_CurLangDesc.Setting.MainDate.SubMsg = "11/20/12";
	g_CurLangDesc.Setting.MainDate.SubDate.TitleFirst = "DATUM INSTELLEN";
	g_CurLangDesc.Setting.MainDate.SubDate.Title = "DATUM";
	// Time
	g_CurLangDesc.Setting.MainTime.MainMsg = "TIJD";
	g_CurLangDesc.Setting.MainTime.SubMsg = "12:00 PM";
	g_CurLangDesc.Setting.MainTime.SubTime.TitleFirst = "TIJD INSTELLEN";
	g_CurLangDesc.Setting.MainTime.SubTime.Title = "TIJD";
	// Language
	g_CurLangDesc.Setting.MainLang.MainMsg = "TAAL";
	g_CurLangDesc.Setting.MainLang.SubMsg = "NEDERLANDS";
	g_CurLangDesc.Setting.MainLang.SubLang.TitleFirst = "TAAL INSTELLEN";
	g_CurLangDesc.Setting.MainLang.SubLang.Title = "TAAL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.English = "ENGLISH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.French = "FRANÇAIS";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Spanish = "ESPAÑOL";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Germain = "DEUTSCH";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Danish = "DANSK";
	g_CurLangDesc.Setting.MainLang.SubLang.Lang.Dutch = "NEDERLANDS";
	// Delete All
	g_CurLangDesc.Setting.MainDeleteAll.MainMsg = "ALLES VERW.";
	g_CurLangDesc.Setting.MainDeleteAll.SubMsg = "-";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Title = "ALLES VERW.";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg1 = "ALLE FOTO'S EN";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg2 = "OPNAMES VERWIJDEREN?";
	g_CurLangDesc.Setting.MainDeleteAll.SubDeleteAll.Msg3 = "AFB. VERWIJDEREN...";
	// Version
	g_CurLangDesc.Setting.MainVersion.MainMsg = "VERSIE";
	g_CurLangDesc.Setting.MainVersion.SubMsg = SOFT_VERSION;
	// Manufacture
	g_CurLangDesc.Setting.MainManufacture.MainMsg = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubMsg = "-";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.Title = "MANUFACTURE";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.TestColor = "TEST COLOR";
	g_CurLangDesc.Setting.MainManufacture.SubManufacture.RestoreParam = "RESTORE PARAM";

	// Notification
	string tp;
	int nt;
	for( nt = Notifiction_MemeryFull; nt < Notifiction_End; nt++ )
	{
		switch( nt )
		{
		case Notifiction_MemeryFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "GEHEUGENKAART VOL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_MemeryAlmostFull:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "GEHEUGENKAART 90% VOLL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card.png";
			break;
		case Notifiction_HandleNotFound:
		case Notifiction_NoImageSignal:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "INTET BILLEDESIGNAL";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);

			g_CurLangDesc.NotiMsg[nt].vecSubText.clear();
			tp = "Is bedieningstoestel aan?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Bedieningstoestel binnen bereik?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			tp = "Camerakabel aangesloten?";
			g_CurLangDesc.NotiMsg[nt].vecSubText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-no-image-signal.png";
			break;
		case Notifiction_BatteryLifeLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "ACCU BIJNA LEEG";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-charge-low.png";
			break;
		case Notifiction_HandleBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "M12-ACCU BIJNA LEEG";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLow:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "LAGE DISPLAYLADING";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_BatteryFault:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "FOUT ACCU";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-battery-fault.png";
			break;
		case Notifiction_CameraCableDisconnect: // not use
			break;
		case Notifiction_HandleBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "M12 accu bijna leeg,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "graag opladen";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-m12-battery-charge-low.png";
			break;
		case Notifiction_ScreenBatteryLowOff:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "Toon Accu bijna leeg,";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			tp = "graag opladen";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-display-charge-low.png";
			break;
		case Notifiction_NoSDCard:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "GEEN SD-KAART";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_NoImagesSaved:
		case Notifiction_NoFile:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "GEEN AFB. OPGESLAGEN";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-card-black.png";
			break;
	    case Notifiction_DeviceConnected:
			g_CurLangDesc.NotiMsg[nt].vecMainText.clear();
			tp = "APPARAAT AANGESLOTEN";
			g_CurLangDesc.NotiMsg[nt].vecMainText.push_back(tp);
			g_CurLangDesc.NotiMsg[nt].imgPath = PIC_PATH"/bk-usb-black.png";
			break;
		default:
			break;
		}
	}
}

//-------------------------------------------------
void InitLangDesc(int LangID)
{
	teLang id = (teLang)LangID;

	switch( id )
	{
	case LANG_ENGLISH:
		LoadLangEnglish();
		break;
	case LANG_FRENCH:
		LoadLangFrench();
		break;
	case LANG_SPANISH:
		LoadLangSpanish();
		break;
	case LANG_GERMAN:
		LoadLangGermain();
		break;
	case LANG_DANISH:
		LoadLangDanish();
		break;
	case LANG_DUTCH:
		LoadLangDutch();
		break;
	default:
		LoadLangEnglish();
		break;
	}
}

//-------------------------------------------------
void LoadTimeStamp(struct tm *ptm, char *pszStamp)
{
	teLang id = (teLang)g_RunParam.tuobangPara.lang;

	switch( id )
	{
	case LANG_ENGLISH:
		sprintf(pszStamp, "%s %02d, %04d", g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), \
			ptm->tm_mday, 1900+ptm->tm_year);
		break;
	case LANG_FRENCH:
		sprintf(pszStamp, "%d %s %04d", ptm->tm_mday, \
			g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), 1900+ptm->tm_year);
		break;
	case LANG_SPANISH:
		sprintf(pszStamp, "%d DE %s DE %04d", ptm->tm_mday, \
			g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), 1900+ptm->tm_year);
		break;
	case LANG_GERMAN:
		sprintf(pszStamp, "%d %s %04d", ptm->tm_mday, \
			g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), 1900+ptm->tm_year);
		break;
	case LANG_DANISH:
		sprintf(pszStamp, "%d.%s %04d", ptm->tm_mday, \
			g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), 1900+ptm->tm_year);
		break;
	case LANG_DUTCH:
		sprintf(pszStamp, "%d %s %04d", ptm->tm_mday, \
			g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), 1900+ptm->tm_year);
		break;
	default: // English
		sprintf(pszStamp, "%s %02d, %04d", g_CurLangDesc.Month[ptm->tm_mon].LongDesc.c_str(), \
			ptm->tm_mday, 1900+ptm->tm_year);
		break;
	}
}

//-------------------------------------------------
void LoadCurDay(char *pszDay)
{
	teLang id = (teLang)g_RunParam.tuobangPara.lang;
	struct tm *ptm;

	time_t now = time(NULL);
	ptm = localtime(&now);

	switch( id )
	{
	case LANG_ENGLISH:
		sprintf(pszDay, "%02d/%02d/%04d", ptm->tm_mon+1, \
			ptm->tm_mday, 1900+ptm->tm_year);
		break;
	case LANG_FRENCH:
		sprintf(pszDay, "%04d-%d-%d", 1900+ptm->tm_year, \
			ptm->tm_mon+1, ptm->tm_mday);
		break;
	case LANG_SPANISH:
		sprintf(pszDay, "%04d-%d-%d", 1900+ptm->tm_year, \
			ptm->tm_mday, ptm->tm_mon+1);
		break;
	case LANG_GERMAN:
		sprintf(pszDay, "%02d/%02d/%04d", ptm->tm_mon+1, \
			ptm->tm_mday, 1900+ptm->tm_year);
		break;
	case LANG_DANISH:
		sprintf(pszDay, "%02d/%02d-%04d", ptm->tm_mday, \
			ptm->tm_mon+1, 1900+ptm->tm_year);
		break;
	case LANG_DUTCH:
		sprintf(pszDay, "%04d-%d-%d", 1900+ptm->tm_year, \
			ptm->tm_mday, ptm->tm_mon+1);
		break;
	default: // English
		sprintf(pszDay, "%02d/%02d/%04d", ptm->tm_mon+1, \
			ptm->tm_mday, 1900+ptm->tm_year);
		break;
	}
}

//-------------------------------------------------
void LoadCurTime(char *pszTime)
{
	teLang id = (teLang)g_RunParam.tuobangPara.lang;
	struct tm *ptm;
	int iHour;
	char *pAmPm;

	time_t now = time(NULL);
	ptm = localtime(&now);

	if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
	{
		if( ptm->tm_hour >= 12 )
		{
			if( ptm->tm_hour == 12 )
				iHour = ptm->tm_hour;
			else
				iHour = ptm->tm_hour - 12;

			pAmPm = g_CurLangDesc.Public.PM.c_str();
		}
		else
		{
			if( ptm->tm_hour == 0 )
				iHour = ptm->tm_hour + 12;
			else
				iHour = ptm->tm_hour;

			pAmPm = g_CurLangDesc.Public.AM.c_str();
		}
	}

	switch( id )
	{
	case LANG_ENGLISH:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	case LANG_FRENCH:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	case LANG_SPANISH:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	case LANG_GERMAN:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	case LANG_DANISH:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	case LANG_DUTCH:
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	default: // English
		if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
		{
			sprintf(pszTime, "%02d:%02d %s", iHour, ptm->tm_min, pAmPm);
		}
		else
		{
			sprintf(pszTime, "%02d:%02d", ptm->tm_hour, ptm->tm_min);
		}
		break;
	}
}

//-------------------------------------------------
void LoadDelMsg(char *pszFile, char *pszBuf)
{
	teLang id = (teLang)g_RunParam.tuobangPara.lang;

	char *pp = strrchr(pszFile, '.');
	int pos = pp - pszFile;
	pszFile[pos] = 0;

	switch( id )
	{
	case LANG_ENGLISH:
		sprintf(pszBuf, "%s %s?", g_CurLangDesc.Public.DELETE.c_str(), pszFile);
		break;
	case LANG_FRENCH:
		sprintf(pszBuf, "%s %s?", g_CurLangDesc.Public.DELETE.c_str(), pszFile);
		break;
	case LANG_SPANISH:
		{
			string DELETE = "¿ELIMINAR";
			sprintf(pszBuf, "%s %s?", DELETE.c_str(), pszFile);
		}
		break;
	case LANG_GERMAN:
		sprintf(pszBuf, "%s %s?", pszFile, g_CurLangDesc.Public.DELETE.c_str());
		break;
	case LANG_DANISH:
		sprintf(pszBuf, "%s %s?", g_CurLangDesc.Public.DELETE.c_str(), pszFile);
		break;
	case LANG_DUTCH:
		sprintf(pszBuf, "%s %s?", pszFile, g_CurLangDesc.Public.DELETE.c_str());
		break;
	default: // English
		sprintf(pszBuf, "%s %s?", g_CurLangDesc.Public.DELETE.c_str(), pszFile);
		break;
	}
}




