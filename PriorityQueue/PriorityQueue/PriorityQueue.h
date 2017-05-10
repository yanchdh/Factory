#pragma once
#include <set>
#include <vector>
#include "queue.h"

struct Node
{
	int					m_g;
	int					m_h;
	bool				m_free;
	TAILQ_ENTRY(Node)	m_field;

	Node(int g, int h)
		: m_g(g)
		, m_h(h)
		, m_free(true)
	{
	}
};

class PriorityQueue
{
public:
	PriorityQueue(int headSize)
	{
		m_headSize = headSize;
		m_nodeNum = 0;
		m_head.resize(m_headSize);
		for (int i = 0; i < m_headSize; ++i)
		{
			TAILQ_INIT(&m_head[i]);
		}
	}
	~PriorityQueue()
	{
		clear();
	}
	void clear()
	{
		for (auto gh : m_keys)
		{
			TAILQ_INIT(&m_head[gh]);
		}
		m_keys.clear();
		m_nodeNum = 0;
	}
	bool empty()
	{
		return m_keys.empty();
	}
	int size()
	{
		return m_nodeNum;
	}
	Node* pop()
	{
		if (m_keys.empty())
			return NULL;
		int gh = *m_keys.begin();
		if (TAILQ_EMPTY(&m_head[gh]))
		{
			fprintf(stderr, "error pop TAILQ_EMPTY gh[%d]\n", gh);
			return NULL;
		}
		Node* pNode = TAILQ_FIRST(&m_head[gh]);
		if (NULL == pNode)
		{
			fprintf(stderr, "error pop TAILQ_FIRST gh[%d]\n", gh);
			return NULL;
		}
		TAILQ_REMOVE(&m_head[gh], pNode, m_field);
		pNode->m_free = true;
		if (TAILQ_EMPTY(&m_head[gh]))
		{
			m_keys.erase(gh);
		}
		--m_nodeNum;
		return pNode;
	}
	bool erase(Node* pNode)
	{
		if (!pNode->m_free)
		{
			int gh = pNode->m_g + pNode->m_h;
			TAILQ_REMOVE(&m_head[gh], pNode, m_field);
			pNode->m_free = true;
			if (TAILQ_EMPTY(&m_head[gh]))
			{
				m_keys.erase(gh);
			}
			--m_nodeNum;
			return true;
		}
		return false;
	}
	bool push(Node* pNode)
	{
		if (!pNode->m_free)
			return false;
		int gh = pNode->m_g + pNode->m_h;
		if (gh >= m_headSize)
			return false;
		m_keys.insert(gh);
		TAILQ_INSERT_TAIL(&m_head[gh], pNode, m_field);
		pNode->m_free = false;
		++m_nodeNum;
		return true;
	}
	TAILQ_HEAD(tqh, Node);
private:
	std::set<int>		m_keys;
	std::vector<tqh>	m_head;
	int					m_headSize;
	int					m_nodeNum;
};