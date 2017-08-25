#include "Global.h"
#include <thread>
#include "event2/thread.h"
#include "ClientMessageDispatch.h"
#include "ClientLogic.h"
#include "ClientNet.h"
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

int main(int args, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2, 1), &wsa);
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif
	event_init();
	if (args < 2)
	{
		srand(_getpid());
		GLOBAL.m_uid = rand();
	}
	else
	{
		GLOBAL.m_uid = atol(argv[1]);
	}
	GLOBAL.m_pLogic = new ClientLogic();
	GLOBAL.m_pClientNet = new ClientNet(&GLOBAL.m_netio, "127.0.0.1", 3333, new ClientMessageDispatch());
	GLOBAL.m_pClientNet->Connect();
	std::shared_ptr<std::thread> pNetThread(new std::thread([](io_event* io){io->run(); }, &GLOBAL.m_netio));
	GLOBAL.m_logicio.run();
	pNetThread->join();
	return 0;
}