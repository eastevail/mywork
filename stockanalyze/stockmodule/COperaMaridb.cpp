/*
 * COperaMarodb.cpp
 *
 *  Created on: 2015年10月20日
 *      Author: lixiang
 */

#include "COperaMaridb.h"
#include "com_log.h"
#include <string.h>
namespace myselfstock
{
COperaMaridb* COperaMaridb::m_pInstance=NULL;
int COperaMaridb::m_ReferCount=0;
COperaMaridb::COperaMaridb()
{
    // TODO Auto-generated constructor stub
    m_ReferCount=0;
    INIT_TS_LOG;
    TSever_LOG_Error("**************************************************************************************************************");

/*    mysql_init(&m_Msqlconnection);
    if (mysql_real_connect(&m_Msqlconnection, NULL, "root", "lixiang6", "stock", 3306, NULL, 0))
    {
        TSever_LOG_Error("success");
    }else{
        TSever_LOG_Error("error:%s",mysql_error(&m_Msqlconnection));
    }*/
}

COperaMaridb::~COperaMaridb()
{
    // TODO Auto-generated destructor stub
    mysql_close(&m_Msqlconnection);
    CLOSE_TS_LOG;
}
COperaMaridb* COperaMaridb::GetInstance(const char* table)
{
    if (!m_pInstance)
    {
        m_pInstance = new COperaMaridb();
        m_pInstance->Init();
    }
    m_ReferCount++;
    return m_pInstance;
}
int COperaMaridb::Init()
{
    mysql_init(&m_Msqlconnection);
    if (mysql_real_connect(&m_Msqlconnection, NULL,NULL,NULL,"stock",3306,NULL,0))
    {
        TSever_LOG_Error("success");
/*        char value=1;
        mysql_options(&m_Msqlconnection, MYSQL_OPT_RECONNECT, (char*)&value);*/
    }
    else
    {
        TSever_LOG_Error("error:%s", mysql_error(&m_Msqlconnection));
    }
    return 0;
}
int COperaMaridb::SelectDb(char* query_onecol,char* buf,int buf_len)
{
    int res=0,row=0;
    MYSQL_RES *res_ptr;
    MYSQL_ROW result_row;
    res = mysql_query(&m_Msqlconnection,query_onecol);
    if (!res)
    {
        res_ptr = mysql_store_result(&m_Msqlconnection);
        /*如果结果不为空，就把结果print*/
        if (res_ptr)
        {
            row = mysql_num_rows(res_ptr);
            for (int i = 1; i < row + 1; i++)
            {
                result_row = mysql_fetch_row(res_ptr);
//                TSever_LOG_Error("name:%s",result_row[0]);
                strcat(buf,result_row[0]);
                strcat(buf,",");

            }
        }
        TSever_LOG_Error("%s",buf);
    }
    else
    {
        TSever_LOG_Error("write db error:%s",mysql_error(&m_Msqlconnection));
    }
    return res;
}
int COperaMaridb::WriteDb(char* query)
{
    int res=0;
 //   mysql_init(&m_Msqlconnection);
//    if (mysql_real_connect(&m_Msqlconnection, "127.0.0.1", "root", "lixiang6", "stock", 3306, NULL, 0))
//    if (mysql_real_connect(&m_Msqlconnection, NULL,NULL,NULL,"stock",3306,NULL,0))
//    {
        res = mysql_query(&m_Msqlconnection,query);
        if (!res)
        {

        }
        else
        {
            TSever_LOG_Error("write db error:%s",mysql_error(&m_Msqlconnection));
        }
  //  }
 //   mysql_close(&m_Msqlconnection);
    return res;
}
int COperaMaridb::Destroy(const char* table)
{
    m_ReferCount--;
    if (0 == m_ReferCount)
    {
        TSever_LOG_Error("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
        delete m_pInstance;
    }
    return 0;
}
} /* namespace myselfstock */
