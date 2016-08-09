/*
 * CStockOperation.h
 *
 *  Created on: 2015年10月16日
 *      Author: lixiang
 */

#ifndef STOCKMODULE_CSTOCKOPERATION_H_
#define STOCKMODULE_CSTOCKOPERATION_H_
#include <string>
#include "COperaMaridb.h"
using std::string;
namespace myselfstock
{

class CStockOperation
{
public:
    CStockOperation();
    ~CStockOperation();
    int SetStockName(const char* stockname);
    int SetValue(string &str);
    int CheckHammerEnd();
    int CheckYerDayHammerOK(bool islast);
//    int WriteDb(char* query);
private:
    COperaMaridb* m_pOpMaridb;
    char m_Query[1024];
    char m_StockCode[10];
    float m_OpenPriceToday;
    float m_ClosePriceYesday;
    float m_CurPrice;
    float m_TopPriceToday;
    float m_LowPriceToday;
    float m_BidBuy;//竞买加即买一 省略
    float m_BidSell;//竞卖价即卖一 省略
    int m_TradeVol;
    int m_TradeMoney;
    //
    int m_BuyOneVol;
    float m_BuyOnePrice;
    int m_BuyTwoVol;
    float m_BuyTwoPrice;
    int m_BuyThreeVol;
    float m_BuyThreePrice;
    int m_BuyFourVol;
    float m_BuyFourPrice;
    int m_BuyFiveVol;
    float m_BuyFivePrice;
    //
    int m_SellOneVol;
    float m_SellOnePrice;
    int m_SellTwoVol;
    float m_SellTwoPrice;
    int m_SellThreeVol;
    float m_SellThreePrice;
    int m_SellFourVol;
    float m_SellFourPrice;
    int m_SellFiveVol;
    float m_SellFivePrice;
//
    char date[16];
    char time[16];
};

} /* namespace myselfstock */

#endif /* STOCKMODULE_CSTOCKOPERATION_H_ */
