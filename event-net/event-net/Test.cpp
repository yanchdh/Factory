#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif
#include <event2/event-config.h>
#include <event2/thread.h>
#include <iostream>
#include "ServerNet.h"
#include "ClientNet.h"

using namespace std;

event_base* g_evbase = NULL;

int main(int args, char**argv) 
{
#ifdef _WIN32
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsa);
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif

	event_init();
	g_evbase = event_base_new();
	if (args == 1)
	{

		ClientNet *pcn = new ClientNet(g_evbase, "127.0.0.1", 3333);
		pcn->Connect();

		//struct bufferevent *ev_cmd = bufferevent_socket_new(g_evbase, STDIN_FILENO, BEV_OPT_CLOSE_ON_FREE);
		//bufferevent_setcb(ev_cmd, 
		//	[](bufferevent *bev, void *ctx) //read_cb
		//	{
		//		evbuffer *input = bufferevent_get_input(bev);
		//		int n = evbuffer_get_length(input);
		//		char* data = new char[n + 1];
		//		evbuffer_remove(input, data, n);
		//		data[n] = 0;
		//		fprintf(stdout, "Recv Msg: %s\n", data);
		//		delete[] data;
		//		data = NULL;
		//	}
		//, NULL, NULL, pcn);
		//bufferevent_enable(ev_cmd, EV_READ | EV_PERSIST);

		pcn->Run();
	}
	else
	{
		ServerNet *psn = new ServerNet(g_evbase, "0.0.0.0", 3333);
		psn->Listen();
		psn->Run();
	}
	system("pause");
	return 0;
}