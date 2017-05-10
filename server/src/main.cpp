#include "Global.h"
#include <thread>
#include "event2/thread.h"
#include "ServerMessageDispatch.h"
#include "ServerNet.h"
#include "ServerLogic.h"

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
	GLOBAL.m_pLogic = new ServerLogic();
	GLOBAL.m_pServerNet = new ServerNet(&GLOBAL.m_netio, "0.0.0.0", 3333, new ServerMessageDispatch());
	GLOBAL.m_pServerNet->Listen();
	std::shared_ptr<std::thread> pNetThread(new std::thread([](io_event* io){io->run(); }, &GLOBAL.m_netio));
	GLOBAL.m_logicio.run();
	pNetThread->join();
	return 0;
}