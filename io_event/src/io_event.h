#ifndef IO_EVENT
#define IO_EVENT

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#include <WinSock2.h>
#else
#include <sys/time.h>
#endif

#include <event.h>
#include <vector>
#include <mutex>
#include <functional>
#include <memory>

#define DEFAULT_IO_EVENT_TURN_TM	(10 * 1000)
#define DEFAULT_IO_EVENT_NUM		(1024 * 1024 / 8)

class io_event final
{
public:
	typedef std::shared_ptr<std::function<void()>> func_ptr;

	io_event();
	
	~io_event();

	io_event(const io_event&) = delete;

	io_event& operator=(const io_event&) = delete;

	void post(std::function<void()>&& func);
	
	void run();

	void set_timeval(int ms);

private:
	void dispatch();

	static void ontimeout(evutil_socket_t fd, short events, void* arg);

private:
	std::vector<func_ptr>			m_vtfunc;
	std::mutex						m_mutex;
	event*							m_ev;
	timeval							m_tv;
	event_base*						m_evbase;
};

#endif /* IO_EVENT */