#pragma once
#include <map>
#include <string>
#include <memory>
#include "Connection.h"
#include "MsgHeader.h"
#include "io_event.h"


struct bufferevent;
struct event_base;
struct evconnlistener;
class io_event;
class MessageDispatch;

class ServerNet
{
public:
	typedef Connection<ServerNet> connection;
	typedef std::shared_ptr<connection> connection_ptr;

	ServerNet(const ServerNet&) = delete;
	ServerNet& operator=(const ServerNet&) = delete;

	ServerNet(io_event* netio, const char* ip, int port, MessageDispatch* dispatch);
	~ServerNet();

	void			OnClose(int fd);
	void			Close(int fd);
	void			Read(int fd, int msgid, std::shared_ptr<char> pdata, int len);
	
	bool			Listen();
	void			Accept(connection_ptr pConn);
	
	//for other thread call
	template<typename MSG_T>
	void SendPBMsg(int fd, int msgid, const MSG_T& msg)
	{
		int header_size = sizeof(MsgHeader);
		int sz = msg.ByteSize();
		std::shared_ptr<char> p(new char[sz + header_size]);
		char* pdata = p.get();
		MsgHeader* head = (MsgHeader*)pdata;
		head->m_len = htonl(sz + sizeof(head->m_msgid));
		head->m_msgid = htonl(msgid);
		msg.SerializeToArray(pdata + header_size, sz);
		m_netio->post(std::bind(&ServerNet::Send, this, fd, p, sz + header_size));
	}

	inline event_base* GetEvBase()
	{
		return m_pEvBase;
	}
private:
	void Send(int fd, std::shared_ptr<char> pdata, int len)
	{
		auto it = m_fd2connection.find(fd);
		if (it == m_fd2connection.end())
			return;
		bool success = it->second->Write(pdata, len);
	}
private:
	io_event*						m_netio;
	MessageDispatch*				m_dispatch;
	std::string						m_ip;
	int								m_port;
	int								m_fd;
	event_base*						m_pEvBase;
	evconnlistener*					m_pListener;
	std::map<int, connection_ptr>	m_fd2connection;
};