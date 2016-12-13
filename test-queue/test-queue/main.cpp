#include <iostream>
#include "queue.h"

using namespace std;

void test_list()
{
	fprintf(stdout, "test_list\n");
	struct element 
	{
		int id;
		LIST_ENTRY(element) field;
	};
	LIST_HEAD(listhead, element) head;
	LIST_INIT(&head);

	element* elm0 = new element();
	elm0->id = -1;
	LIST_INSERT_HEAD(&head, elm0, field);

	for (int i = 1; i < 10; ++i) 
	{
		element* elm = new element();
		elm->id = i;
		//LIST_INSERT_HEAD(&head, elm, field);
		LIST_INSERT_AFTER(elm0, elm, field);
		//LIST_INSERT_BEFORE(elm0, elm, field);
	}
	element* var = NULL;
	LIST_FOREACH(var, &head, field)
	{
		cout << var->id << endl;
	}
}

void test_single_list()
{
	fprintf(stdout, "test_single_list\n");
	struct element
	{
		int id;
		SLIST_ENTRY(element) field;
	};
	SLIST_HEAD(slisthead, element) head;
	SLIST_INIT(&head);
	for (int i = 0; i < 10; ++i) 
	{
		element* elm = new element();
		elm->id = i;
		SLIST_INSERT_HEAD(&head, elm, field);
	}

	element* var = NULL;
	SLIST_FOREACH(var, &head, field)
	{
		cout << var->id << endl;
	}
}

void test_simple_queue()
{
	fprintf(stdout, "test_simple_queue\n");
	struct element
	{
		int id;
		SIMPLEQ_ENTRY(element) field;
	};
	SIMPLEQ_HEAD(sqhead, element) head;
	SIMPLEQ_INIT(&head);

	for (int i = 0; i < 10; ++i)
	{
		element* elm = new element();
		elm->id = i;
		SIMPLEQ_INSERT_TAIL(&head, elm, field);
	}

	element* var = NULL;
	SIMPLEQ_FOREACH(var, &head, field)
	{
		cout << var->id << endl;
	}
}

void test_tail_queue()
{
	fprintf(stdout, "test_tail_queue\n");
	struct element
	{
		int id;
		TAILQ_ENTRY(element) field;
	};
	TAILQ_HEAD(tqhead, element) head;
	TAILQ_INIT(&head);

	for (int i = 0; i < 10; ++i)
	{
		element* elm = new element();
		elm->id = i;
		TAILQ_INSERT_TAIL(&head, elm, field);
	}

	element* var = NULL;
	TAILQ_FOREACH_REVERSE(var, &head, tqhead, field)
	{
		cout << var->id << endl;
	}
}

int main(int args, char *argv[])
{
	test_single_list();

	test_list();

	test_simple_queue();

	test_tail_queue();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}