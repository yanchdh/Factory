#pragma once

#include "io_event.h"

class ServerNet;
class ServerLogic;
class Global
{
public:
	io_event		m_netio;
	io_event		m_logicio;
	ServerNet*		m_pServerNet;
	ServerLogic*	m_pLogic;

	static Global& Instance()
	{
		static Global oInst;
		return oInst;
	}
};

#define GLOBAL Global::Instance()