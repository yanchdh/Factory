#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <event2/listener.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <memory>
#include <assert.h>
#include "EventNet.h"

EventNet::EventNet(event_base* evbase)
	: m_evbase(evbase)
{
}

EventNet::~EventNet()
{
}

//static
void EventNet::ReadCallBack(bufferevent* bev, void *arg)
{
	EventNet* pev = (EventNet*)arg;
	evbuffer *input = bufferevent_get_input(bev);
	int n = evbuffer_get_length(input);
	while (n > 4)
	{
		char header[4];
		evbuffer_copyout(input, header, 4);
		int len = ntohl(*(int*)header);
		if (len <= 0)
		{
			pev->OnClose(bev);
			return;
		}
		if (n >= len)
		{
			evbuffer_drain(input, 4);
			n -= 4;
			std::shared_ptr<char> pdata(new char[len]);
			char *data = (char*)pdata.get();
			evbuffer_remove(input, data, len);
			pev->OnRecvMsg(bev, pdata, len);
			n -= len;
		}
		else
		{
			break;
		}
	}
}

void EventNet::Run()
{
	int ret = event_base_dispatch(m_evbase);
}

event_base* EventNet::GetEventBase()
{
	return m_evbase;
}

void EventNet::SetEventBase(event_base* evbase)
{
	m_evbase = evbase;
}