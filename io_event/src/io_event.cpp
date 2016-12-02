#include "io_event.h"

io_event::io_event()
	: m_evbase(event_base_new())
	, m_tv({ 0, DEFAULT_IO_EVENT_TURN_TM })
	, m_ev(NULL)
{
	m_vtfunc.reserve(DEFAULT_IO_EVENT_NUM);
}
	
io_event::~io_event()
{
	event_base_loopexit(m_evbase, NULL);
}

void io_event::post(std::function<void()>&& func)
{
	m_mutex.lock();
	m_vtfunc.push_back(std::make_shared<std::function<void()>>(std::forward<std::function<void()>>(func)));
	m_mutex.unlock();
}

void io_event::run()
{
	m_ev = evtimer_new(m_evbase, &io_event::ontimeout, this);
	event_add(m_ev, &m_tv);
	event_base_dispatch(m_evbase);
}

void io_event::set_timeval(int ms)
{
	m_tv.tv_usec = ms * 1000;
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
	if (m_vtfunc.empty()) 
	{
		m_mutex.unlock();
		return;
	}
	auto vtfunc = m_vtfunc;
	m_vtfunc.clear();
	m_mutex.unlock();
	for (const auto& func : vtfunc)
	{
		(*func)();
	}
	vtfunc.clear();
}
