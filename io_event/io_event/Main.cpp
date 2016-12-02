#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif
#include <iostream>
#include <thread>
#include <event2/thread.h>
#include <event2/event-config.h>
#include "io_event.h"

using namespace std;

void run_io(io_event* io)
{
	io->run();
}

void init()
{
#ifdef _WIN32
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
	event_init();
#ifdef _WIN32
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif
}

void print1(int n)
{
	cout << n << endl;
}

void print2(int n1, int n2)
{
	cout << n1 << n2 << endl;
}

int main()
{

	init();

	io_event* io_event1 = new io_event();
	io_event* io_event2 = new io_event();

	int n = 10;

	for (int i = 0; i < n; ++i)
	{
		io_event1->post(std::bind(print1, i));
		io_event2->post(std::bind(print2, i, i));
	}

	std::thread t1(run_io, io_event1);
	std::thread t2(run_io, io_event2);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}