#pragma once
#include <map>
#include <string>
#include <memory>
#include "Connection.h"
#include "MsgHeader.h"
#include <functional>
#include "io_event.h"

struct bufferevent;
class MessageDispatch;
class ClientNet
{
public:
	typedef Connection<ClientNet> connection;
	typedef std::shared_ptr<connection> connection_ptr;

	ClientNet(const ClientNet&) = delete;
	ClientNet& operator=(const ClientNet&) = delete;

	explicit ClientNet(io_event* netio, const char* ip, int port, MessageDispatch* dispatch);
	~ClientNet();

	void			OnClose(int fd);
	void			Close(int fd);
	void			Read(int fd, int msgid, std::shared_ptr<char> pdata, int len);

	bool			Connect();
	void			ReConnect();
	void			ConnectSuccess(connection_ptr pConn);

	template<typename MSG_T>
	void SendPBMsg(int msgid, const MSG_T& msg)
	{
		int header_size = sizeof(MsgHeader);
		int sz = msg.ByteSize();
		std::shared_ptr<char> p(new char[sz + header_size]);
		char* pdata = p.get();
		MsgHeader* head = (MsgHeader*)pdata;
		head->m_len = htonl(sz + sizeof(head->m_msgid));
		head->m_msgid = htonl(msgid);
		msg.SerializeToArray(pdata + header_size, sz);
		m_netio->post(std::bind(&ClientNet::Send, this, p, sz + header_size));
	}
	
	inline event_base* GetEvBase()
	{
		return m_pEvBase;
	}
private:
	void AddTimer(event* pEv);
	void Send(std::shared_ptr<char> pdata, int len)
	{
		if (!m_pConn)
			return;
		bool success = m_pConn->Write(pdata, len);
	}
private:
	io_event*								m_netio;
	MessageDispatch*						m_dispatch;
	std::string								m_ip;
	int										m_port;
	event*									m_pEv;
	event_base*								m_pEvBase;
	std::shared_ptr<Connection<ClientNet>>	m_pConn;
};