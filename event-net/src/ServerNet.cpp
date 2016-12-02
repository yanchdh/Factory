#ifdef _WIN32
#include <WinSock2.h>
#else

#endif /* _WIN32 */
#include "ServerNet.h"
#include <memory>
#include <event2/listener.h>
#include <functional>

ServerNet::ServerNet(event_base* evbase, const char* ip, int port)
	: EventNet(evbase)
	, m_listen_ip(ip)
	, m_listen_port(port)
	, m_listen_fd(-1)
	, m_listener(NULL)
{
}

ServerNet::~ServerNet()
{
	if (m_listener)
	{
		evconnlistener_free(m_listener);
		m_listener = NULL;
	}
}

void ServerNet::OnRecvMsg(bufferevent *bev, std::shared_ptr<char> pdata, int len)
{
	char* data = (char*)pdata.get();
	uint32_t msgid = ntohl(*(uint32_t*)data);
	if (msgid == 0) 
	{
		fprintf(stdout, "RecvMsg: %s\n", data + sizeof((MsgHeader*)(NULL))->m_msgid);
	}
}

bool ServerNet::Listen()
{
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_listen_port);
	addr.sin_addr.s_addr = inet_addr(m_listen_ip.c_str());
	evconnlistener *listener = evconnlistener_new_bind(GetEventBase(), 
		[](evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg) //evconnlistener_cb
		{
			ServerNet* psn = (ServerNet*)(arg);
			struct bufferevent *bev = bufferevent_socket_new(psn->GetEventBase(), fd, BEV_OPT_CLOSE_ON_FREE);
			bufferevent_setcb(bev, EventNet::ReadCallBack, NULL, 
				[](bufferevent* bev, short event, void *arg) //bufferevent_event_cb
				{
					ServerNet* psn = (ServerNet*)(arg);
					int fd = bufferevent_getfd(bev);
					if (event & BEV_EVENT_CONNECTED)
					{
						fprintf(stderr, "Listener Connected[%d]\n", fd);
					}
					else
					{
						fprintf(stderr, "Listener Closed[%d], Error Code[%x]\n", fd, event);
						psn->OnClose(bev);
					}
				}
			, psn);
			bufferevent_enable(bev, EV_READ | EV_PERSIST);
			fprintf(stdout, "OnAccept %d\n", fd);
			psn->OnAccept(bev);
		}
	, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 10, (sockaddr*)&addr, sizeof(addr));
	if (!listener)
	{
		fprintf(stdout, "Listen %s:%d Fail\n", m_listen_ip.c_str(), m_listen_port);
		return false;
	}
	m_listen_fd = evconnlistener_get_fd(listener);
	fprintf(stdout, "Listen %s:%d Success[%d]\n", m_listen_ip.c_str(), m_listen_port, m_listen_fd);
	return true;
}

void ServerNet::OnAccept(bufferevent *bev)
{
	int fd = bufferevent_getfd(bev);

	m_fd2bev[fd] = bev;
}

void ServerNet::OnClose(bufferevent *bev)
{
	int fd = bufferevent_getfd(bev);
	m_fd2bev.erase(fd);
	bufferevent_free(bev);
}

void ServerNet::Close(int fd)
{
	auto it = m_fd2bev.find(fd);
	if (it != m_fd2bev.end())
	{
		bufferevent* bev = it->second;
		m_fd2bev.erase(fd);
		bufferevent_free(bev);
	}
}