#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <fstream>

#define MAX_ID (256)
// 屏蔽字过滤 AC自动机
class ForbiddenWords
{

public:
	ForbiddenWords();
	void Init(const std::string& respath);
	bool IsStrLegal(const std::string& str);
	bool HasPunctuation(const std::string& str);
private:
	void Clear();
	int  GetId(char ch);
	void Insert(const std::string& word, int wrodid);
	void InitFail();
	void TrimStr(std::string& str);
	int  GetUtf8Size(unsigned char ch);
private:
	ForbiddenWords	*m_next[MAX_ID];
	ForbiddenWords  *m_fail;
	int 			m_wordid;

};
