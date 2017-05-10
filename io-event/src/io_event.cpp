#include "io_event.h"

io_event::io_event()
	: m_evbase(event_base_new())
	, m_tv({ 0, DEFAULT_IO_EVENT_TURN_MS })
	, m_ev(NULL)
	, m_tail(NULL)
	, m_elmnum(0)
{
	_SLIST_INIT(&m_head);
}
	
io_event::~io_event()
{
	event_base_loopexit(m_evbase, NULL);
}

void io_event::post(std::function<void()>&& func)
{
	element* elm = new element();
	elm->m_func = std::make_shared<std::function<void()>>(std::forward<std::function<void()>>(func));
	m_mutex.lock();
	if (!m_tail)
		_SLIST_INSERT_HEAD(&m_head, elm, field);
	else
		_SLIST_INSERT_AFTER(m_tail, elm, field);
	m_elmnum++;
	m_tail = elm;
	m_mutex.unlock();
}

void io_event::run()
{
	m_ev = evtimer_new(m_evbase, &io_event::ontimeout, this);
	event_add(m_ev, &m_tv);
	event_base_dispatch(m_evbase);
}

void io_event::set_timeout(int ms)
{
	m_tv.tv_usec = ms * 1000;
}

event_base* io_event::get_evbase()
{
	return m_evbase;
}

void io_event::ontimeout(evutil_socket_t fd, short events, void* arg)
{
	io_event* io = static_cast<io_event*>(arg);
	io->dispatch();
	event_add(io->m_ev, &io->m_tv);
}

void io_event::dispatch()
{
	m_mutex.lock();
	if (!m_tail) 
	{
		m_mutex.unlock();
		return;
	}
	decltype(m_head) head;
	_SLIST_INIT(&head);
	std::swap(m_head, head);
	m_tail = NULL;
	m_elmnum = 0;
	m_mutex.unlock();

	element* delelm = NULL;
	element* elm;
	_SLIST_FOREACH(elm, &head, field)
	{
		(*elm->m_func)();
		if (delelm)
			delete delelm;
		delelm = elm;
	}
	if (delelm)
		delete delelm;
}
