/*
 * COperaMarodb.h
 *
 *  Created on: 2015年10月20日
 *      Author: lixiang
 */

#ifndef CORE_COPERAMARIDB_H_
#define CORE_COPERAMARIDB_H_
#include<stdio.h>
#include<mysql/mysql.h>
namespace myselfstock
{

class COperaMaridb
{
public:
    static COperaMaridb* GetInstance(const char* table);
    static int Destroy(const char* table);
    int WriteDb(char* query);
    int Init();
    int SelectDb(char* query_onecol,char* buf,int buf_len);
private:
    static COperaMaridb* m_pInstance;
    static int m_ReferCount;
    MYSQL m_Msqlconnection;
    COperaMaridb();
    ~COperaMaridb();
};

} /* namespace myselfstock */

#endif /* CORE_COPERAMARIDB_H_ */
