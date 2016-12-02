#ifdef _WIN32
#include <WinSock2.h>
#else

#endif /* _WIN32 */
#include "ClientNet.h"
#include <memory>
#include <assert.h>

void std_read_cb(struct bufferevent *bev, void *arg)
{
	//SocketData* pSocketData = (SocketData*)arg;
	//assert(pSocketData && pSocketData->pEventNet);
	//struct evbuffer *input = bufferevent_get_input(bev);
	//int n = evbuffer_get_length(input);
	//char *data = new char[n];
	//evbuffer_remove(input, data, n);
	//data[n - 1] = 0;
	//pSocketData->pEventNet->StdRecvMsg(NULL, data, n);
}

ClientNet::ClientNet(event_base* evbase, const char* ip, int port)
	: EventNet(evbase)
	, m_conn_ip(ip)
	, m_conn_port(port)
	, m_conn_fd(-1)
	, m_bev(NULL)
	, m_ev(NULL)
{
}

ClientNet::~ClientNet()
{
	if (m_bev) 
	{
		bufferevent_free(m_bev);
		m_bev = NULL;
	}
	if (m_ev)
	{
		event_free(m_ev);
		m_ev = NULL;
	}
}

void ClientNet::OnRecvMsg(bufferevent *bev, std::shared_ptr<char> pdata, int len)
{
	char* data = (char*)pdata.get();
	uint32_t iMsgId = ntohl(*(uint32_t*)data);
}

void ClientNet::Close()
{
	if (m_bev) 
	{
		bufferevent_free(m_bev);
		m_bev = NULL;
	}
}

void ClientNet::AddTimer(event* _ev)
{
	if (!_ev)
	{
		return;
	}
	if (m_ev)
	{
		event_free(m_ev);
		m_ev = NULL;
	}
	m_ev = _ev;
	timeval tv = { 1, 0 };
	event_add(m_ev, &tv);
}

bool ClientNet::Connect()
{
	bufferevent* bev = bufferevent_socket_new(GetEventBase(), -1, BEV_OPT_CLOSE_ON_FREE);
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_conn_port);
	addr.sin_addr.s_addr = inet_addr(m_conn_ip.c_str());

	if ((bufferevent_socket_connect(bev, (sockaddr*)&addr, sizeof(addr))) < 0)
	{
		fprintf(stdout, "Connect %s:%d Fail[bufferevent_socket_connect()]\n", m_conn_ip.c_str(), m_conn_port);
		return false;
	}

	bufferevent_setcb(bev, EventNet::ReadCallBack, NULL, 
		[](bufferevent* bev, short event, void *arg) //bufferevent_event_cb
		{
			int fd = bufferevent_getfd(bev);
			ClientNet* pcn = (ClientNet*)arg;
			if (event & BEV_EVENT_CONNECTED)
			{
				fprintf(stdout, "Connect Success[%d]!\n", fd);
				pcn->OnConnectSuccess(bev);
			}
			else
			{
				fprintf(stderr, "Connect Fail[%d]\n", fd);
				pcn->OnConnectFail(bev);
				pcn->AddTimer(evtimer_new(pcn->GetEventBase(),
					[](evutil_socket_t fd, short events, void* arg) //Reconnect
					{
						ClientNet* pcn = (ClientNet*)arg;
						fprintf(stdout, "Reconnect %s:%d\n", pcn->m_conn_ip.c_str(), pcn->m_conn_port);
						pcn->Connect();
					}
				, pcn));
			}
		}
	, this);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);
	return 0;
}

void ClientNet::OnClose(bufferevent *bev)
{
	if (m_bev && bev == m_bev)
	{
		bufferevent_free(m_bev);
		m_bev = NULL;
		m_conn_fd = -1;
	}
}

void ClientNet::OnConnectSuccess(bufferevent* bev)
{
	m_bev = bev;
	m_conn_fd = bufferevent_getfd(bev);
	SendMsg(bev, sizeof("hello world"), "hello world");
}

void ClientNet::OnConnectFail(bufferevent* bev)
{
	bufferevent_free(bev);
	m_bev = NULL;
	m_conn_fd = -1;
}