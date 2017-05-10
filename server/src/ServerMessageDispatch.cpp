#include "ServerMessageDispatch.h"
#include "Global.h"
#include <functional>
#include "ServerLogic.h"

void ServerMessageDispatch::OnConnectionRead(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	GLOBAL.m_logicio.post(std::bind(&ServerLogic::ProcessMsg, GLOBAL.m_pLogic, fd, msgid, pdata, len));
}

void ServerMessageDispatch::OnConnectionSuccess(int fd)
{

}

void ServerMessageDispatch::OnConnectionClose(int fd)
{
	GLOBAL.m_logicio.post(std::bind(&ServerLogic::ProcessConncetionClose, GLOBAL.m_pLogic, fd));
}