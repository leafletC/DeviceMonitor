#pragma once

#include <mysql/mysql.h>
#include <vector>
#include <string>
#include <memory>
#include "MyModule.h"

using namespace std;

class DBConnector : public MyModule
{
public:
    using Row = vector<string>;
    using Table = vector<Row>;
    struct Option
    {
        string host;
        int32_t port;
        string user;
        string password;
        string name;
        bool reconnect;
        int32_t timeout;
        void operator=(const Option &option);
    };
    DBConnector();
    ~DBConnector();
    int run();
    void setOptions(const Option& option);
    void *onQuery(void *arg);
private:
    MYSQL sock;
    Option option;
};

