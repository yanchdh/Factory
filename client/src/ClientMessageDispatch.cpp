#include "ClientMessageDispatch.h"
#include "chat.pb.h"
#include "Global.h"
#include "login.pb.h"
#include "ClientLogic.h"

void ClientMessageDispatch::OnConnectionRead(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	GLOBAL.m_logicio.post(std::bind(&ClientLogic::ProcessMsg, GLOBAL.m_pLogic, fd, msgid, pdata, len));
}

void ClientMessageDispatch::OnConnectionSuccess(int fd)
{
	GLOBAL.m_logicio.post(std::bind(&ClientLogic::ProcessReqEnterGame, GLOBAL.m_pLogic));
}

void ClientMessageDispatch::OnConnectionClose(int fd)
{

}