#include "PriorityQueue.h"
#include <stdio.h>

std::set<int> st; //10228
bool vis[10240] = { 0 };

void dfs(int s)
{
	while (true)
	{

	}
}

int main()
{

	PriorityQueue Q(10240);
	
	int i = 0;
	while (true)
	{
		Node* pNode = new Node(i++, rand() % 5120);
		Q.push(pNode);

		if ((rand() % 11) != 0)
			continue;
		int j = 0;
		while (!Q.empty())
		{
			Node* pNode = Q.pop();
			printf("%d %d %d %d\n", j++, pNode->m_g, pNode->m_h, pNode->m_g + pNode->m_h);
		}
		printf("[%d]-------------------------------------------------\n", Q.size());
		getchar();
	}

	getchar();

	return 0;
}