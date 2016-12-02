#pragma once
#include <event.h>
#include <memory>
#include "MsgHeader.h"

struct bufferevent;
struct event_base;
struct event;

class EventNet
{
public:
	EventNet(event_base* evbase);
	virtual			~EventNet();
	static  void	ReadCallBack(bufferevent* bev, void *arg);
	virtual void	OnRecvMsg(bufferevent *bev, std::shared_ptr<char> pdata, int iLen) = 0;
	virtual void	OnClose(bufferevent *bev) = 0;

	template<typename MSG_T>
	bool SendPBMsg(bufferevent* bev, int msgid, MSG_T& msg)
	{
		int header_size = sizeof(MsgHeader);
		int n = msg.ByteSize();
		std::shared_ptr<char> p(new char[n + header_size]);
		char* data = p.get();
		MsgHeader* head = (MsgHeader*)data;
		head->m_len = htonl(n + sizeof(head->iMsgId));
		head->m_msgid = htonl(msgid);
		msg.SerializeToArray(data + header_size, n);
		bufferevent_write(bev, data, n + header_size);
		return true;
	}

	bool SendMsg(bufferevent* bev, int len, char* msgdata)
	{
		if (len <= 0)
		{
			return false;
		}
		int header_size = sizeof(MsgHeader);
		std::shared_ptr<char> p(new char[len + header_size]);
		char* data = (char*)p.get();
		MsgHeader* head = (MsgHeader*)data;
		head->m_len = htonl(len + sizeof(head->m_msgid));
		head->m_msgid = 0;
		memcpy(data + header_size, msgdata, len);
		bufferevent_write(bev, data, len + header_size);
		return true;
	}

	void			Run();
	void			SetEventBase(event_base* evbase);
	event_base*		GetEventBase();

private:
	event_base*		m_evbase;
};