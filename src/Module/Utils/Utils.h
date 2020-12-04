#pragma once

#include <string>
#include <vector>
#include <getopt.h>

using namespace std;

namespace OS
{
    string execShell(const string &sh);
};

namespace Utils
{
    string trim(string &s);
    string trim(string &&s);
    string trim(string &s, const string &symbol);
    string trim(string &&s, const string &symbol);
    vector<string> split(string &s, const string &symbol);
};
