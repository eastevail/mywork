/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: desktop.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.05.22
	Description	:
	History		:
					Create by hul
******************************************************************************/

#ifndef __CHARGEMANAGER_H__
#define __CHARGEMANAGER_H__

#include "common.h"


enum{
	CHARGE_IDLE = 0,
	CHARGE_DONE ,
	CHARGE_PROGRESS ,
	CHARGE_USB_PROGRESS ,
};

#define STATISTIC_NUM    6
#define BASE_USB_VOLTAGE  150


class BQ2425Control{

public :

	BQ2425Control();
	virtual ~BQ2425Control();

	void BQ2425ChargeON();

	void BQ2425ChargeOFF();

	int getChargeStatus();



private:

	int readBQ2425Reg(unsigned char regAddr,unsigned char *value);
	int writeBQ2425Reg(unsigned char regAddr,unsigned char *value);




};


class ChargeManager {


public:

	ChargeManager();
	virtual ~ChargeManager();

	void setAGCfd( int fd){m_AGCDevFd = fd;};

	int Start();

	int isChargeDone();

	int isboardCharge() ;

	int getCurrentVoltage();

	static DFBResult OnTimeOutEvent(void *data);

	void TimeOutEvent();

	void GIO60_Ctrl(int f);
	void GIO25_Ctrl(int out_val);

private:

	void statisticUSBVoltage();



	void toChargeStatus();

	void toUSBChargeStatus();

	void toIdleStatus();


	BQ2425Control m_batteryCtrl;

	int m_chargestatus;

	int m_TimerId;

	int m_AGCDevFd;
	int m_GPioDevFd;

	int m_USBCharge;
	int m_USBVoltage[STATISTIC_NUM];
	int m_vIndex;

};

#endif

