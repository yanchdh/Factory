#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif /* _WIN32 */
#include "ServerNet.h"
#include <memory>
#include <event2/listener.h>
#include <functional>
#include "MsgHeader.h"
#include "io_event.h"
#include "MessageDispatch.h"
#include <string.h>

ServerNet::ServerNet(io_event* netio, const char* ip, int port, MessageDispatch* dispatch)
	: m_netio(netio)
	, m_dispatch(dispatch)
	, m_ip(ip)
	, m_port(port)
	, m_fd(-1)
	, m_pEvBase(netio->get_evbase())
	, m_pListener(NULL)
{
}

ServerNet::~ServerNet()
{
	if (m_pListener)
	{
		evconnlistener_free(m_pListener);
		m_pListener = NULL;
	}
}

void ServerNet::Read(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	m_dispatch->OnConnectionRead(fd, msgid, pdata, len);
}

bool ServerNet::Listen()
{
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
	evconnlistener *listener = evconnlistener_new_bind(GetEvBase(),
		[](evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sock, int socklen, void *arg) //evconnlistener_cb
		{
			ServerNet* pServerNet = static_cast<ServerNet*>(arg);
			struct bufferevent *bev = bufferevent_socket_new(pServerNet->GetEvBase(), fd, BEV_OPT_CLOSE_ON_FREE);
			bufferevent_setcb(bev, Connection<ServerNet>::Read, NULL, 
				[](bufferevent* bev, short event, void *arg) //bufferevent_event_cb
				{
					ServerNet* pServerNet = static_cast<ServerNet*>(arg);
					int fd = bufferevent_getfd(bev);
					if (event & BEV_EVENT_CONNECTED)
					{
						fprintf(stderr, "Listener Connected[%d]\n", fd);
					}
					else
					{
						fprintf(stderr, "Listener Closed[%d], Error Code[%x]\n", fd, event);
						pServerNet->OnClose(fd);
						bufferevent_free(bev);
					}
				}
			, pServerNet);
			bufferevent_enable(bev, EV_READ | EV_PERSIST);
			fprintf(stdout, "OnAccept %d\n", fd);
			connection_ptr pConn(new connection(pServerNet->GetEvBase(), bev));
			pServerNet->Accept(pConn);
		}
	, this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 10, (sockaddr*)&addr, sizeof(addr));
	if (!listener)
	{
		fprintf(stdout, "Listen %s:%d Fail\n", m_ip.c_str(), m_port);
		return false;
	}
	m_fd = evconnlistener_get_fd(listener);
	fprintf(stdout, "Listen %s:%d Success[%d]\n", m_ip.c_str(), m_port, m_fd);
	return true;
}

void ServerNet::Accept(connection_ptr pConn)
{
	int fd = pConn->GetFd();
	m_fd2connection[fd] = pConn;
	m_dispatch->OnConnectionSuccess(fd);
}

void ServerNet::OnClose(int fd)
{
	m_fd2connection.erase(fd);
	m_dispatch->OnConnectionClose(fd);
}

void ServerNet::Close(int fd)
{
	auto it = m_fd2connection.find(fd);
	if (it != m_fd2connection.end())
	{
		it->second->Close();
		m_fd2connection.erase(it);
	}
}