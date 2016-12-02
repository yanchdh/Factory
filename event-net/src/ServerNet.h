#pragma once
#include "EventNet.h"
#include <map>
#include <string>

using std::string;
using std::map;

struct bufferevent;
struct event_base;
struct evconnlistener;

class ServerNet : public EventNet
{
public:
	ServerNet(const ServerNet&) = delete;
	ServerNet& operator=(const ServerNet&) = delete;

	ServerNet(event_base* evbase, const char* ip, int port);
	virtual			~ServerNet();
	virtual void	OnRecvMsg(bufferevent *bev, std::shared_ptr<char> pdata, int len);
	virtual void	OnClose(bufferevent *bev);
	
	void			Close(int fd);
	void			OnAccept(bufferevent *bev);
	bool			Listen();
private:
	map<int, bufferevent*>	m_fd2bev;
	
	string					m_listen_ip;
	int						m_listen_port;
	int						m_listen_fd;
	evconnlistener*			m_listener;
};