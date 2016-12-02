#include "ForbiddenWords.h"

ForbiddenWords::ForbiddenWords()
{
	m_wordid = -1;
	m_fail = NULL;
	for (int i = 0; i < MAX_ID; ++i)
		m_next[i] = NULL;
}

void ForbiddenWords::Init(const std::string& respath)
{
	std::ifstream fin(respath.c_str());
	std::string str;
	int id = 0;
	if (fin)
	{
		while (getline(fin, str))
		{
			TrimStr(str);
			if (!str.empty())
			{
				Insert(str, id++);
				str.clear();
			}
		}
	}
	InitFail(); //初始化m_fail指针
	fin.close();
}

bool ForbiddenWords::IsStrLegal(const std::string& str)
{
	int sz = str.size();
	ForbiddenWords *p = this;
	for (int i = 0; i < sz; ++i)
	{
		int id = GetId(str[i]);
		while (NULL == p->m_next[id] && p != this)
			p = p->m_fail;
		p = (NULL == p->m_next[id] ? this : p->m_next[id]);
		ForbiddenWords *temp = p;
		while (temp != this)
		{
			if (temp->m_wordid >= 0)
				return false;
			temp = temp->m_fail;
		}
	}
	return true;
}

bool ForbiddenWords::HasPunctuation(const std::string& str)
{
	//0x4E00 - 0x9FA5
	int strsize = str.size();
	for (int i = 0; i < strsize;)
	{
		if (str[i] > 0)
		{
			if (!isalpha(str[i]) && !isdigit(str[i])) //非英文字母
			{
				return true;
			}
			i++;
			continue;
		}
		int sz = GetUtf8Size((unsigned char)(str[i]));
		if (sz < 2 || sz > 6 || strsize - i < sz)
			return true;
		int unicode = (str[i] & (((1 << (7 - sz)) - 1)));
		int j = 1;
		while (j < sz)
		{
			unicode = ((unicode << 6) | (str[i + j] & 0x3F));
			j++;
		}
		if (unicode < 0x4E00 || unicode > 0x9FA5) //非中文字符
			return true;
		i += sz;
	}
	return false;
}

void ForbiddenWords::Clear()
{
	for (int i = 0; i < MAX_ID; ++i)
	{
		if (NULL != m_next[i])
		{
			m_next[i]->Clear();
			delete m_next[i]; 
			m_next[i] = NULL;
		}
	}
}

int ForbiddenWords::GetId(char ch)
{
	return (int)((unsigned char)(ch));
}

void ForbiddenWords::Insert(const std::string& word, int wrodid)
{
	ForbiddenWords *p = this;
	int sz = word.size();
	for (int i = 0; i < sz; ++i)
	{
		int id = GetId(word[i]);
		if (NULL == p->m_next[id])
			p->m_next[id] = new ForbiddenWords();
		p = p->m_next[id];
	}
	p->m_wordid = wrodid;
}

void ForbiddenWords::InitFail()
{
	std::queue<ForbiddenWords*> que;
	que.push(this);
	ForbiddenWords *p, *temp;
	while (!que.empty())
	{
		p = que.front(); 
		que.pop();
		for (int i = 0; i < MAX_ID; ++i)
		{
			if (NULL == p->m_next[i])
				continue;
			if (p == this)
			{
				p->m_next[i]->m_fail = this;
			}
			else
			{
				temp = p->m_fail;
				while (NULL != temp)
				{
					if (NULL != temp->m_next[i])
					{
						p->m_next[i]->m_fail = temp->m_next[i];
						break;
					}
					temp = temp->m_fail;
				}
				if (NULL == temp)
				{
					p->m_next[i]->m_fail = this;
				}
			}
			que.push(p->m_next[i]);
		}
	}
}

void ForbiddenWords::TrimStr(std::string& str)
{
	int len = str.size();
	int i = len - 1;
	while (i >= 0 && isspace((unsigned char)(str[i])))
		i--;
	str.erase(i + 1);
	len = str.size();
	i = 0;
	while (i < len && isspace((unsigned char)(str[i])))
		++i;
	if (i > 0)
		str.erase(0, i);
}

int ForbiddenWords::GetUtf8Size(unsigned char ch)
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