/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: chargeManager.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2015.05.22
	Description	:
	History		:
					Create by hul
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <linux/i2c.h>
//#include <linux/i2c-dev.h>

#include "chargeManager.h"

ChargeManager::ChargeManager()
{

	m_TimerId = -1;
	m_AGCDevFd = -1;
	m_GPioDevFd = -1;
	m_vIndex = 0;
	m_USBCharge = FALSE;
	m_chargestatus = CHARGE_IDLE;
	memset(m_USBVoltage,0,sizeof(m_USBVoltage));

}

ChargeManager::~ChargeManager()
{

	m_TimerId = -1;
	m_AGCDevFd = -1;
	m_GPioDevFd = -1;


}

int ChargeManager::Start()
{

	if (0 > (m_GPioDevFd = open("/dev/dvs_gpio", O_RDWR | O_NOCTTY)))
	{
		printf("open gpio fail\n");
		return -1;
	}
/*
	if (DFB_OK != lite_enqueue_window_timeout(1 *1000, TimeOut_EveryTime, OnTimeOutEvent, this, &m_TimerId))//TimeOut_EveryTime
	{
		DBG("create timer error\n");
	}

	DBG(" create ChargeManager timer succes, id = %d\n", m_TimerId);
*/

	return 0;
}

void ChargeManager::statisticUSBVoltage()
{
	int val = 0;
	int i;
	int flag = 1;  // USB in


	if (0 > ioctl(m_AGCDevFd, 6, &val))
	{
		DBG("read 3 vat fail\n");
	}


	m_USBVoltage[m_vIndex%STATISTIC_NUM] = val;

	m_vIndex++;


/*	DBG("val = %d\n",val);

	for(i=0;i<STATISTIC_NUM;i++)
	{
		printf("%d ",m_USBVoltage[i]);

	}

	printf("\n\n");*/



	for(i=0;i<STATISTIC_NUM;i++)
	{

//��ѹ�ȶ���ֵ�Ƚϴ�
		if(abs(val - m_USBVoltage[i]) > 3 || val < BASE_USB_VOLTAGE)
		{
			flag = 0;
			break;
		}

	}





	if(flag > 0)
		m_USBCharge = TRUE;
	else
		m_USBCharge = FALSE;

}

void ChargeManager::GIO60_Ctrl(int f)
{
	int gio_value = 60;

	if (0 > ioctl(m_GPioDevFd, 0x04, &gio_value))
	{
		DBG("config GIO 60  fail\n");
	}

	if(f)
	{
		if (0 > ioctl(m_GPioDevFd, 0x02, &gio_value))
		{
			DBG("set GIO 60  high fail\n");
		}
	}
	else
	{
		if (0 > ioctl(m_GPioDevFd, 0x01, &gio_value))
		{
			DBG("set GIO 60  low fail\n");
		}
	}
}

// LCD backlight control
void ChargeManager::GIO25_Ctrl(int out_val)
{
	int gio_value = 25;

	if( 0 > ioctl(m_GPioDevFd, 0x04, &gio_value) )
	{
		DBG("config GIO 25 fail\n");
	}

	if( out_val )
	{
		if( 0 > ioctl(m_GPioDevFd, 0x02, &gio_value) )
		{
			DBG("set GIO 25 high fail\n");
		}
	}
	else
	{
		if( 0 > ioctl(m_GPioDevFd, 0x01, &gio_value) )
		{
			DBG("set GIO 25 low fail\n");
		}
	}
}

void ChargeManager::toChargeStatus()
{
	if(m_chargestatus != CHARGE_PROGRESS)
	{
		DBG("%s()\n",__FUNCTION__);

		//USBChargeCtrl(FALSE);

		//m_batteryCtrl.BQ2425ChargeON();

		m_chargestatus = CHARGE_PROGRESS;
	}
}

void ChargeManager::toUSBChargeStatus()
{

	if(m_chargestatus != CHARGE_USB_PROGRESS)
	{
		DBG("%s()\n",__FUNCTION__);

		//USBChargeCtrl(TRUE);

		m_chargestatus = CHARGE_USB_PROGRESS;
	}

}


void ChargeManager::toIdleStatus()
{

	if(m_chargestatus != CHARGE_IDLE)
	{
		DBG("%s()\n",__FUNCTION__);

		//m_batteryCtrl.BQ2425ChargeOFF();

		m_chargestatus = CHARGE_IDLE;
	}

}


DFBResult ChargeManager::OnTimeOutEvent(void *data)
{
	ChargeManager *p = (ChargeManager *)data;

	if (p)
	{
		p->TimeOutEvent();
	}

	return DFB_OK;
}


void ChargeManager::TimeOutEvent()
{
	int isboardCharging = -1;
	int isUSBCharging = -1;

	statisticUSBVoltage();

	isboardCharging = isboardCharge();

	isUSBCharging = m_USBCharge;


	if( isboardCharging < 0){
		return ;
	}


	//int status = m_batteryCtrl.getChargeStatus();

	//DBG("status = %d ; isboard = %d\n",status,ret);


	if(isboardCharging == TRUE && isUSBCharging == FALSE )
	{
		toChargeStatus();
	}
	else if(isboardCharging == FALSE && isUSBCharging == TRUE)
	{
		toUSBChargeStatus();

	}
	else if(isboardCharging == TRUE && isUSBCharging == TRUE)
	{
		toUSBChargeStatus();

	}else{
		toIdleStatus();
	}
}

//����߲���
int ChargeManager::isboardCharge()
{
	int gio_value = 43;

	if (0 > ioctl(m_GPioDevFd, 0x03, &gio_value))
	{
		DBG("set gpio 43 fail\n");
		return FAILURE;
	}

	gio_value = 43;

	if (0 > ioctl(m_GPioDevFd, 0x10, &gio_value))
	{
		DBG("read gpio 43 fail\n");
		return FAILURE;
	}

	return gio_value;
}

int ChargeManager::isChargeDone()
{
	return (m_batteryCtrl.getChargeStatus() == CHARGE_DONE)? TRUE : FALSE;
}


int ChargeManager::getCurrentVoltage()
{

}



/****************       BQ2425Control   **********************/


#define AV_FAIL -1
#define AV_OK 0
#define AV_ERROR DBG
#define AV_DEBUG DBG
#define SET_REG_BIT(reg,pos) reg |= (0x1<<pos)
#define CLEAR_REG_BIT(reg,pos) reg &= (~(0x1<<pos))


#define I2C_DEV  "/dev/i2c-0"

const static unsigned char device_addr = 0x6a;


BQ2425Control::BQ2425Control()
{



}


BQ2425Control::~BQ2425Control()
{


}


void BQ2425Control::BQ2425ChargeON()
{

		printf("-- BQ2425ChargeON\n");

/*
		unsigned char reg_value = 0;


		readBQ2425Reg(0,&reg_value);
		CLEAR_REG_BIT(reg_value,6);
		writeBQ2425Reg(0,&reg_value);


		readBQ2425Reg(1,&reg_value);
		CLEAR_REG_BIT(reg_value,7);
		SET_REG_BIT(reg_value,6);
		CLEAR_REG_BIT(reg_value,5);
		CLEAR_REG_BIT(reg_value,4);
		SET_REG_BIT(reg_value,2);
		writeBQ2425Reg(1,&reg_value);


		readBQ2425Reg(3,&reg_value);
		SET_REG_BIT(reg_value,7);
		SET_REG_BIT(reg_value,6);
		CLEAR_REG_BIT(reg_value,5);
		CLEAR_REG_BIT(reg_value,4);
		writeBQ2425Reg(3,&reg_value);
*/

}

void BQ2425Control::BQ2425ChargeOFF()
{


		printf("-- BQ2425ChargeOFF\n");

/*		unsigned char reg_value = 0;

		readBQ2425Reg(1,&reg_value);
		SET_REG_BIT(reg_value,7);

		writeBQ2425Reg(1,&reg_value);
*/

}


int BQ2425Control::getChargeStatus()
{

int chargestatus = 0;

/*
	unsigned char reg_value = 0;
	unsigned char flag = 0;


	readBQ2425Reg(0,&reg_value);


	//DBG(" reg_value : 0x%x\n" , reg_value);

	flag = (reg_value & 0x30)>>4;

	if( flag == 0x1)
	{
		chargestatus = CHARGE_PROGRESS;
	}
	else if( flag == 0x2)
	{
		chargestatus = CHARGE_DONE;
	}
	else
	{
		chargestatus = CHARGE_IDLE;
	}
	*/

	return chargestatus;
}

int BQ2425Control::readBQ2425Reg(unsigned char regAddr, unsigned char *value)
{
/*

	int fd,ret;
	unsigned char buftmp[32];
	struct i2c_rdwr_ioctl_data i2c_data;
	const char		*i2c_dev = I2C_DEV;
	//----------------------------------

	//device_addr>>=1;
	//init
	fd=open(i2c_dev, O_RDWR);
	if(fd<0)
	{
		AV_ERROR("not have /dev/i2c t\r\n");
		return AV_FAIL;
	}

	i2c_data.nmsgs=2;
	i2c_data.msgs= (struct i2c_msg *)malloc(i2c_data.nmsgs *sizeof(struct i2c_msg));
	if(i2c_data.msgs == NULL)
	{
		AV_ERROR("malloc error");
		close(fd);
		return AV_FAIL;
	}
	ioctl(fd,I2C_TIMEOUT, 1);
	ioctl(fd,I2C_RETRIES, 2);

	//write reg
	buftmp[0]=regAddr;
	i2c_data.msgs[0].len= 1;
	i2c_data.msgs[0].addr= device_addr;
	i2c_data.msgs[0].flags= 0;	   // 0: write 1:read
	i2c_data.msgs[0].buf= buftmp;
	//read data
	i2c_data.msgs[1].len= 1;
	i2c_data.msgs[1].addr= device_addr;
	i2c_data.msgs[1].flags= 1;	   // 0: write 1:read
	i2c_data.msgs[1].buf= value;
	ret= ioctl(fd, I2C_RDWR, (unsigned long)&i2c_data);
	if(ret < 0)
	{
		AV_ERROR("read data %x %x error\r\n",device_addr,regAddr);
		close(fd);
		free(i2c_data.msgs);
		return AV_FAIL; // 1
	}
	free(i2c_data.msgs);
	close(fd);


	//printf(" 0x%02x \n",value[0]);
*/

	return AV_OK;

}

int BQ2425Control::writeBQ2425Reg(unsigned char regAddr,unsigned char *value)
{
/*

	int fd,ret;
	unsigned char buftmp[32];
	struct i2c_rdwr_ioctl_data i2c_data;
	const char		  *i2c_dev = I2C_DEV;

	int ByteNo = 1;
	//----------------------------------

	//device_addr>>=1;
	//init
	fd=open(i2c_dev, O_RDWR);
	if(fd<0)
	{
		AV_ERROR("not have /dev/i2c\r\n");
		return -1;
	}

	i2c_data.nmsgs=1;
	i2c_data.msgs= (struct i2c_msg *)malloc(i2c_data.nmsgs *sizeof(struct i2c_msg));
	if(i2c_data.msgs == NULL)
	{
	  AV_ERROR("malloc error");
	  close(fd);
	  return -1;
	}
	ioctl(fd,I2C_TIMEOUT, 1);
	ioctl(fd,I2C_RETRIES, 2);

	buftmp[0] = regAddr;
	memcpy(buftmp+1,value,1);
	i2c_data.msgs[0].len= ByteNo+1;;
	i2c_data.msgs[0].addr= device_addr;
	i2c_data.msgs[0].flags= 0;	   // 0: write 1:read
	i2c_data.msgs[0].buf = buftmp;
	ret= ioctl(fd, I2C_RDWR, (unsigned long)&i2c_data);
	if(ret < 0)
	{
	  AV_ERROR("write reg %x %x error\r\n",device_addr,regAddr);
	  close(fd);
	  free(i2c_data.msgs);
	  return 1;
	}
	free(i2c_data.msgs);
	close(fd);

	//printf("i2c_write %d:",buftmp[0]);
*/

	return 0;

}

