#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif /* _WIN32 */
#include "ClientNet.h"
#include <memory>
#include <assert.h>
#include "io_event.h"
#include "MessageDispatch.h"
#include <string.h>

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

ClientNet::ClientNet(io_event* netio, const char* ip, int port, MessageDispatch* dispatch)
	: m_netio(netio)
	, m_dispatch(dispatch)
	, m_ip(ip)
	, m_port(port)
	, m_pEv(NULL)
	, m_pEvBase(netio->get_evbase())
	, m_pConn(NULL)
{
}

ClientNet::~ClientNet()
{
	if (m_pEv)
	{
		event_free(m_pEv);
		m_pEv = NULL;
	}
	if (m_pConn)
	{
		OnClose(m_pConn->GetFd());
	}
}

void ClientNet::Close(int fd)
{	
	if (m_pConn)
	{
		m_pConn->Close();
		m_pConn.reset();
	}
}

void ClientNet::OnClose(int fd)
{
	if (m_pConn)
	{
		m_pConn.reset();
		m_dispatch->OnConnectionClose(fd);
	}
}

void ClientNet::AddTimer(event* pEv)
{
	if (!pEv)
	{
		return;
	}
	if (m_pEv)
	{
		event_free(m_pEv);
		m_pEv = NULL;
	}
	m_pEv = pEv;
	timeval tv = { 0, 300 * 1000 };
	event_add(m_pEv, &tv);
}

bool ClientNet::Connect()
{
	bufferevent* bev = bufferevent_socket_new(GetEvBase(), -1, BEV_OPT_CLOSE_ON_FREE);
	struct sockaddr_in addr;
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);
	addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

	if ((bufferevent_socket_connect(bev, (sockaddr*)&addr, sizeof(addr))) < 0)
	{
		fprintf(stdout, "Connect %s:%d Fail[bufferevent_socket_connect()]\n", m_ip.c_str(), m_port);
		return false;
	}

	bufferevent_setcb(bev, Connection<ClientNet>::Read, NULL,
		[](bufferevent* bev, short event, void *arg) //bufferevent_event_cb
		{
			int fd = bufferevent_getfd(bev);
			ClientNet* pClientNet = (ClientNet*)arg;
			if (event & BEV_EVENT_CONNECTED)
			{
				connection_ptr pConn(new connection(pClientNet->GetEvBase(), bev));
				pClientNet->ConnectSuccess(pConn);
			}
			else
			{
				pClientNet->OnClose(fd);
				pClientNet->ReConnect();
				bufferevent_free(bev);
			}
		}
	, this);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);
	return 0;
}

void ClientNet::ReConnect()
{
	AddTimer(evtimer_new(GetEvBase(),
		[](evutil_socket_t fd, short events, void* arg)
	{
		ClientNet* pClientNet = static_cast<ClientNet*>(arg);
		fprintf(stdout, "Reconnect %s:%d\n", pClientNet->m_ip.c_str(), pClientNet->m_port);
		pClientNet->Connect();
	}
	, this));
}

void ClientNet::Read(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	m_dispatch->OnConnectionRead(fd, msgid, pdata, len);
}

void ClientNet::ConnectSuccess(connection_ptr pConn)
{
	int fd = pConn->GetFd();
	m_pConn = pConn;
	fprintf(stdout, "ConnectSuccess %s:%d\n", m_ip.c_str(), m_port);
	m_dispatch->OnConnectionSuccess(fd);
}