#include "Utils.h"

string OS::execShell(const string &sh)
{
    FILE *fp = NULL;
	char data[1024] = {};
	fp = popen(sh.c_str(), "r");
    string result;
	if(fp == NULL)
	{
		printf("popen error!\n");
		return result;
	}
	while(fgets(data, sizeof(data), fp) != NULL)
	{
		result.append(data);
	}
    return result;
}

string Utils::trim(string &s)
{
	return trim(move(s));
}

string Utils::trim(string &&s)
{
	trim(move(s), " ");
	trim(move(s), "\n");
	return s;
}

string Utils::trim(string &s, const string &symbol)
{
	return trim(move(s), symbol);
}

string Utils::trim(string &&s, const string &symbol)
{
	if (s.empty()) 
    {
        return s;
    }
    s.erase(0, s.find_first_not_of(symbol));
    s.erase(s.find_last_not_of(symbol) + 1);
	return s;
}

vector<string> Utils::split(string &s, const string &symbol)
{
	vector<string> words;
	if(s.empty())
		return words;
	auto pos = s.find_first_of(symbol);
	string::size_type posOld = 0;
	while(pos != string::npos)
	{
		words.push_back(s.substr(posOld, pos - posOld));
		posOld = pos + 1;
		pos = s.find_first_of(symbol, posOld);
	}
	words.push_back(s.substr(posOld));
	return words;
}