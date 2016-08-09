/*
 * CStockOperation.cpp
 *
 *  Created on: 2015骞�10鏈�16鏃�
 *      Author: lixiang
 */

#include "CStockOperation.h"
#include "com_log.h"
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
namespace myselfstock
{

CStockOperation::CStockOperation()
{
    // TODO Auto-generated constructor stub
    INIT_TS_LOG;
    bzero(m_StockCode,sizeof(m_StockCode));
    bzero(date,sizeof(date));
    bzero(time,sizeof(time));
    bzero(m_Query,sizeof(m_Query));
    m_pOpMaridb=COperaMaridb::GetInstance("sfsdf");

}

CStockOperation::~CStockOperation()
{
    // TODO Auto-generated destructor stub
    CLOSE_TS_LOG;
    COperaMaridb::Destroy("sfsdf");
}
int CStockOperation::SetStockName(const char* stockname)
{
    strncpy(m_StockCode,stockname,sizeof(m_StockCode));
//    snprintf(m_Query,sizeof(m_Query),"insert into yesterday_hammer_stock values(\"%s\",NULL,NULL)",m_StockCode);
    return 0;
}
/*int CStockOperation::WriteDb(char* query)
{
    int res=0;
    mysql_init(&m_Msqlconnection);
    if (mysql_real_connect(&m_Msqlconnection, "127.0.0.1", "root", "lixiang6", "stock", 3306, NULL, 0))
    {
        res = mysql_query(&m_Msqlconnection,query);
        if (!res)
        {
        }
        else
        {
            TSever_LOG_Error("select");
        }
//    }
//    mysql_close(&m_Msqlconnection);
    return res;
}*/
int CStockOperation::SetValue(string &str)
{
    std::size_t pos1 = str.find(",");
    std::size_t pos2 = str.rfind(",");
    string data = str.substr(pos1 + 1, pos2 - pos1 - 1);
//    TSever_LOG_Error("%s", data.c_str());
    sscanf(data.c_str(), "%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%s,%s",
            &m_OpenPriceToday,&m_ClosePriceYesday,&m_CurPrice,&m_TopPriceToday,&m_LowPriceToday,&m_BidBuy,&m_BidSell,&m_TradeVol,&m_TradeMoney,
            &m_BuyOneVol,&m_BuyOnePrice,&m_BuyTwoVol,&m_BuyTwoPrice,&m_BuyThreeVol,&m_BuyThreePrice,&m_BuyFourVol,&m_BuyFourPrice
            ,&m_BuyFiveVol,&m_BuyFivePrice,&m_SellOneVol,&m_SellOnePrice,&m_SellTwoVol,&m_SellTwoPrice,&m_SellThreeVol
            ,&m_SellThreePrice,&m_SellFourVol,&m_SellFourPrice,&m_SellFiveVol,&m_SellFivePrice,date,time);
/*    TSever_LOG_Error("%f,%f,%f,%f,%f,%f,%f,%d,%d,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%d,%f,%s,%s",
            m_OpenPriceToday,m_ClosePriceYesday,m_CurPrice,m_TopPriceToday,m_LowPriceToday,m_BidBuy,m_BidSell,m_TradeVol,m_TradeMoney,
            m_BuyOneVol,m_BuyOnePrice,m_BuyTwoVol,m_BuyTwoPrice,m_BuyThreeVol,m_BuyThreePrice,m_BuyFourVol,m_BuyFourPrice
            ,m_BuyFiveVol,m_BuyFivePrice,m_SellOneVol,m_SellOnePrice,m_SellTwoVol,m_SellTwoPrice,m_SellThreeVol
            ,m_SellThreePrice,m_SellFourVol,m_SellFourPrice,m_SellFiveVol,m_SellFivePrice,date,time);*/

//    TSever_LOG_Error("%s",m_Query);
    return 0;
}
int CStockOperation::CheckYerDayHammerOK(bool islast){
    bzero(m_Query,sizeof(m_Query));
    snprintf(m_Query,sizeof(m_Query),"insert into yesterday_hammer_stock values(\"%s\",\"%f\",NULL,\"%f\",\"%f\",NULL,NULL)"
            "ON DUPLICATE KEY UPDATE curprice=\"%f\",dailyKurl=\"http://image.sinajs.cn/newchart/daily/n/%s.gif\"",
            m_StockCode,m_ClosePriceYesday,m_TopPriceToday,m_CurPrice,m_CurPrice,m_StockCode);
    TSever_LOG_Error("%s",m_Query);
    m_pOpMaridb->WriteDb(m_Query);
    if (islast)
    {
        m_pOpMaridb->WriteDb("update yesterday_hammer_stock set increase_percent=((curprice-topprice)*100/topprice);");
    }
    char commond[1024]={0};
    char filename[128]={0};
    snprintf(filename,sizeof(filename),"/home/share/gif/%s.gif",m_StockCode);
    snprintf(commond,sizeof(commond),"curl http://image.sinajs.cn/newchart/daily/n/%s.gif > /home/share/gif/%s.gif",m_StockCode,m_StockCode);
	if (!access(filename, F_OK))
	{
		TSever_LOG_Error("%s already exist",filename);
	}
	else
	{
		int ret = system(commond);
		if (ret < 0)
		{
			TSever_LOG_Error("system() error");
		}
	}
    return 1;
}
//判断是否是锤子形态结束后
//return 1 ok
int CStockOperation::CheckHammerEnd()
{
	float tmp1=0,tmp2=0,tmp3=0;
    bzero(m_Query,sizeof(m_Query));
    snprintf(m_Query,sizeof(m_Query),"insert into yesterday_hammer_stock values(\"%s\",\"%f\",NULL,\"%f\",\"%f\",NULL,NULL)"
            "ON DUPLICATE KEY UPDATE closepriceyester=\"%f\" ,topprice=\"%f\",curprice=\"%f\"",m_StockCode,m_ClosePriceYesday,
            m_TopPriceToday,m_CurPrice,m_ClosePriceYesday,m_TopPriceToday,m_CurPrice);
//	TSever_LOG_Error("%s",m_Query);
	if ((m_OpenPriceToday > m_LowPriceToday)&&(m_CurPrice>=m_OpenPriceToday))//&&(m_TopPriceToday==m_CurPrice))
	{
//	    TSever_LOG_Error("maybe========%s", m_StockCode);
		tmp1 = m_TopPriceToday - m_CurPrice;
		tmp2 = m_CurPrice - m_OpenPriceToday;
		tmp3 = m_OpenPriceToday - m_LowPriceToday;
//		TSever_LOG_Error("tmp1=%f,tmp2=%f,tmp3=%f",tmp1,tmp2,tmp3);
		if (0 != tmp2)
		{
			if (2 < (tmp3 / tmp2))
			{
				if (0 != tmp1)
				{
					if (5 < (tmp3 / tmp1))
					{
						TSever_LOG_Error("!!!!!!!!!!!!!!!!!!!!!!%s", m_StockCode);
						m_pOpMaridb->WriteDb(m_Query);
						return 1;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					TSever_LOG_Error("!!!!!!!!!!!!!!!!!!!!!!%s", m_StockCode);
                    m_pOpMaridb->WriteDb(m_Query);
					return 1;
				}
			}
		}
		else
		{
			if (0 != tmp1)
			{
//				TSever_LOG_Error("tmp1=%f,tmp3=%f stock=%s ",m_StockCode,tmp1,tmp3);
				if (2 < (tmp3 / tmp1))
				{
					TSever_LOG_Error("!!!!!!!!!!!!!!!!!!!!!!%s", m_StockCode);
                    m_pOpMaridb->WriteDb(m_Query);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
/*				TSever_LOG_Error("!!!!!!!!!!!!!!!!!!!!!!%s", m_StockCode);
                m_pOpMaridb->WriteDb(m_Query);*/
				return 0;
			}
		}
	}
    return 0;
}























} /* namespace myselfstock */
