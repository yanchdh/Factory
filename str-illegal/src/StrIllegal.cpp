#include "StrIllegal.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <assert.h>

using std::endl;
using std::cout;

void StrIllegal::Init(const char* illegal_filepath)
{
	std::ifstream fin(illegal_filepath);
	if (fin)
	{
		string illegal_str;
		while (getline(fin, illegal_str))
		{
			TrimStr(illegal_str);
			if (!illegal_str.empty())
			{
				m_vtIllegalStr.push_back(illegal_str);
				illegal_str.clear();
			}
		}
	}
	sort(m_vtIllegalStr.begin(), m_vtIllegalStr.end(), [](const string& str1, const string& str2){return str1.size() < str2.size();});
	//for (const auto& illegal_str : m_vtIllegalStr)
	//{
	//	cout << illegal_str << endl;
	//}
}

bool StrIllegal::IsStrLegal(const string& check_str)
{
	int check_strsize = check_str.size();
	for (const auto& illegal_str : m_vtIllegalStr)
	{
		int illegal_strsize = illegal_str.size();
		if (illegal_strsize > check_strsize)
			break;
		//if (illegal_strsize == check_strsize && illegal_str == check_str)
		//	return false;
		//else
		//	continue;
		if (IsMatch(illegal_str, check_str))
			return false;
	}
	assert(0);
	return true;
}

int StrIllegal::GetUtf8Size(unsigned char ch)
{
	int temp = 128;
	int sz = 0;
	while (ch > 0 && ch >= temp)
	{
		ch -= temp;
		temp >>= 1;
		sz++;
	}
	return sz;
}

//1111111011111111
//0100111011100100
bool StrIllegal::HasPunctuation(const string& check_str)
{
	//0x4E00 - 0x9FA5
	int strsize = check_str.size();
	for (int i = 0; i < strsize;)
	{
		if (check_str[i] > 0)
		{
			if (!isalpha(check_str[i]) && !isdigit(check_str[i])) //非英文字母
			{
				return true;
			}
			i++;
			continue;
		}
		int sz = GetUtf8Size((unsigned char)(check_str[i]));
		if (sz < 2 || sz > 6 || strsize - i < sz)
			return true;
		int unicode = (check_str[i] & (((1 << (7 - sz)) - 1)));
		int j = 1;
		while (j < sz)
		{
			unicode = ((unicode << 6) | (check_str[i + j] & 0x3F));
			j++;
		}
		if (unicode < 0x4E00 || unicode > 0x9FA5) //非中文字符
			return true;
		i += sz;
	}
	return false;
}

bool StrIllegal::IsMatch(const string& illegal_str, const string& check_str)
{
	int illegal_strsize = illegal_str.size();
	int check_strsize = check_str.size();
	int i = 0, j = 0;
	while (i < illegal_strsize)
	{
		while (j < check_strsize && illegal_str[i] != check_str[j])
			++j;
		if (j >= check_strsize)
			return false;
		++i;
		++j;
	}
	return (i >= illegal_strsize);
}

void StrIllegal::TrimStr(std::string& illegal_str)
{
	int len = illegal_str.size();
	int i = len - 1;
	while (i >= 0 && isspace((unsigned char)(illegal_str[i])))
		i--;
	illegal_str.erase(i + 1);
	len = illegal_str.size();
	i = 0;
	while (i < len && isspace((unsigned char)(illegal_str[i])))
		++i;
	if (i > 0)
		illegal_str.erase(0, i);
}