#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;

class StrIllegal
{
public:
	void Init(const char* illegal_filename);
	bool IsStrLegal(const string& check_str);
	bool HasPunctuation(const string& check_str);
	void TrimStr(string& illegal_str);
private:
	bool IsMatch(const string& illegal_str, const string& check_str);
	int  GetUtf8Size(unsigned char ch);
private:
	vector<string>	m_vtIllegalStr;
};