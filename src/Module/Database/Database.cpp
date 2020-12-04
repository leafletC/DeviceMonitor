#include "Database.h"
#include <cstdio>
#include <stdint.h>
#include <iostream>

void DBConnector::Option::operator=(const Option &option)
{
    this->host = option.host;
    this->port = option.port;
    this->user = option.user;
    this->password = option.password;
    this->name = option.name;
    this->reconnect = option.reconnect;
    this->timeout = option.timeout;
}

DBConnector::DBConnector()
{
    mysql_init(&sock);
    mysql_set_character_set(&sock, "utf8");
}

DBConnector::~DBConnector()
{
    mysql_close(&sock);
}

void DBConnector::setOptions(const Option& option)
{
    this->option = option;
}

int DBConnector::run()
{
    mysql_options(&sock, MYSQL_OPT_RECONNECT, &option.reconnect);
    mysql_options(&sock, MYSQL_OPT_CONNECT_TIMEOUT, &option.timeout);
    if(!mysql_real_connect(&sock, option.host.c_str(), option.user.c_str(), option.password.c_str(), option.name.c_str(), option.port, nullptr, 0))
    {
        printf("Failed to connect to database: Error: %s\n", mysql_error(&sock));
        cout << "host:" << option.host << " user:" << option.user << " password:" << option.password << " name:" << option.name << " port:" << option.port << endl;
        return -1;
    }
    return 0;
}

void *DBConnector::onQuery(void *arg)
{
    string sql = *(string *)arg;
    Table *tbl = new Table;
    if(0 != mysql_query(&sock, sql.c_str()))
    {
        printf("Failed to query to t_slestate_event_current: Error: %s\n", mysql_error(&sock));
        return nullptr;
    }
    MYSQL_RES * result = mysql_use_result(&sock);
    for( ; ; )
    {
        MYSQL_ROW mysqlRow = mysql_fetch_row(result);
        uint32_t fields = mysql_num_fields(result);
        if(mysqlRow == nullptr)
        {
            return tbl;
        }
        Row row;
        for(uint32_t i = 0; i < fields; i++)
        {
            row.push_back(mysqlRow[i]);
        }
        tbl->push_back(row);
    }
    return tbl;
}