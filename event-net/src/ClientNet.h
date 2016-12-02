#pragma once
#include "EventNet.h"
#include <map>
#include <string>

using std::string;
using std::map;

struct bufferevent;
class ClientNet : public EventNet
{
public:
	ClientNet(const ClientNet&) = delete;
	ClientNet& operator=(const ClientNet&) = delete;

	ClientNet(event_base* evbase, const char* ip, int port);
	virtual			~ClientNet();
	virtual void	OnRecvMsg(bufferevent *bev, std::shared_ptr<char> pdata, int len);
	virtual void	OnClose(bufferevent *bev);
	
	void			Close();
	void			AddTimer(event* _ev);
	void			OnConnectSuccess(bufferevent* bev);
	void			OnConnectFail(bufferevent* bev);
	bool			Connect();

public:
	string			m_conn_ip;
	int				m_conn_port;

private:
	bufferevent*	m_bev;
	int				m_conn_fd;
	event*			m_ev;
};