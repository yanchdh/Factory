#pragma once
#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif
#include <event.h>
#include <memory>
#include <assert.h>

struct bufferevent;
struct event_base;
struct event;

template<typename T>
class Connection
{
public:
	Connection(const Connection&) = delete;
	Connection& operator = (const Connection&) = delete;

	explicit Connection(event_base* evbase, bufferevent* bev)
		: m_evbase(evbase)
		, m_bev(bev)
	{
	}

	~Connection()
	{
		m_bev = NULL;
		m_evbase = NULL;
	}

	void Close()
	{
		if (m_bev)
		{
			bufferevent_free(m_bev);
			m_bev = NULL;
		}
	}

	static void Read(bufferevent* bev, void *arg)
	{
		T* pNet = static_cast<T*>(arg);
		int fd = bufferevent_getfd(bev);
		evbuffer *input = bufferevent_get_input(bev);
		int n = evbuffer_get_length(input);
		char header[8];
		while (n >= 8)
		{
			evbuffer_copyout(input, header, 8);
			int msglen = ntohl(*(int*)header);
			int msgid = ntohl(*(int*)(header + 4));
			if (msglen <= 0)
			{
				pNet->OnClose(fd);
				return;
			}
			if (n >= msglen)
			{
				evbuffer_drain(input, 8);
				n -= 8;
				msglen -= 4;
				std::shared_ptr<char> pdata(new char[msglen]);
				char *data = (char*)pdata.get();
				evbuffer_remove(input, data, msglen);
				pNet->Read(fd, msgid, pdata, msglen);
				n -= msglen;
			}
			else
			{
				break;
			}
		}
	}

	inline bool Write(std::shared_ptr<char> pdata, int len)
	{
		assert(m_bev);
		int n = bufferevent_write(m_bev, pdata.get(), len);
		return (0 == n);
	}

	inline int GetFd()
	{
		return m_bev ? bufferevent_getfd(m_bev) : 0;
	}

private:
	event_base*		m_evbase;
	bufferevent*	m_bev;
};