#ifndef _IO_EVENT_H_
#define _IO_EVENT_H_

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif

#include <event.h>
#include <mutex>
#include <functional>
#include <memory>
#include "slist.h"

#define DEFAULT_IO_EVENT_TURN_MS (5 * 1000)

class io_event final
{
public:
	typedef std::shared_ptr<std::function<void()>> func_ptr;

	struct element
	{
		func_ptr m_func;
		_SLIST_ENTRY(element) field;
	};

	io_event();
	
	~io_event();

	io_event(const io_event&) = delete;

	io_event& operator=(const io_event&) = delete;

	void post(std::function<void()>&& func);
	
	void run();

	void set_timeout(int ms);

	event_base* get_evbase();

	static void ontimeout(evutil_socket_t fd, short events, void* arg);

private:
	void dispatch();

	std::mutex						m_mutex;
	event*							m_ev;
	timeval							m_tv;
	event_base*						m_evbase;
	_SLIST_HEAD(slhead, element)	m_head;
	element*						m_tail;
	int								m_elmnum;
};

#endif /* _IO_EVENT_H_ */