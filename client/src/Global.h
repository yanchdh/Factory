#pragma once
#include "io_event.h"

class ClientNet;
class ClientLogic;
class Global
{
public:
	int64_t			m_uid;
	io_event		m_netio;
	io_event		m_logicio;
	ClientNet*		m_pClientNet;
	ClientLogic*	m_pLogic;

	static Global& Instance()
	{
		static Global oInst;
		return oInst;
	}
};

#define GLOBAL Global::Instance()