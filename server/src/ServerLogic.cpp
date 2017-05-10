#include "ServerLogic.h"
#include "Global.h"
#include "login.pb.h"
#include "chat.pb.h"
#include "classicddz.pb.h"

ServerLogic::ServerLogic()
{
	
}

ServerLogic::~ServerLogic()
{

}

void ServerLogic::ProcessMsg(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	if (msgid == request_enter_game)
	{
		ProcessReqEnterGame(fd, pdata, len);
		return;
	}
	PlayerPtr player = GetPlayerByFd(fd);
	if (!player)
		return;
	switch (msgid)
	{
	case request_classicddz_start:
		m_classicDouDiZhuLogic.ProcessReqStart(player, pdata, len);
		break;
	case request_classicddz_call:
		m_classicDouDiZhuLogic.ProcessReqCall(player, pdata, len);
		break;
	case request_classicddz_discard:
		m_classicDouDiZhuLogic.ProcessReqDiscard(player, pdata, len);
		break;
	default:
		break;
	}
}

void ServerLogic::ProcessConncetionClose(int fd)
{
	PlayerPtr player = GetPlayerByFd(fd);
	if (player)
	{
		m_classicDouDiZhuLogic.ProcessConncetionClose(player);
		RemovePlayer(player);
	}
}

void ServerLogic::ProcessReqEnterGame(int fd, std::shared_ptr<char> pdata, int len)
{
	msg_request_enter_game req;
	if (!req.ParseFromArray(pdata.get(), len))
	{
		fprintf(stderr, "Parse msg_request_enter_game Failed\n");
		return;
	}
	int64_t uid = req.uid();
	PlayerPtr player = GetPlayerByUid(uid);
	if (player)
	{
		player->Close();
		RemovePlayer(player);
		player->SetFd(fd);
	}
	else
	{
		player = PlayerPtr(new Player(fd, uid));
	}
	AddPlayer(player);
	msg_response_enter_game rsp;
	rsp.set_err(0);
	rsp.set_uid(uid);
	player->SendPBMsg(response_enter_game, rsp);
}

///////////////////////////////////////////////////////////////////////////

void ServerLogic::AddPlayer(PlayerPtr player)
{
	fprintf(stdout, "AddPlayer fd[%d] uid[%lld]\n", player->GetFd(), player->GetUid());
	player->SetOnline();
	m_fd2Player.insert(std::make_pair(player->GetFd(), player));
	m_uid2Player.insert(std::make_pair(player->GetUid(), player));
}
void ServerLogic::RemovePlayer(PlayerPtr player)
{
	fprintf(stdout, "RemovePlayer fd[%d] uid[%lld]\n", player->GetFd(), player->GetUid());
	player->SetOffline();
	m_fd2Player.erase(player->GetFd());
	m_uid2Player.erase(player->GetUid());
}
PlayerPtr ServerLogic::GetPlayerByFd(int fd)
{
	auto it = m_fd2Player.find(fd);
	if (it != m_fd2Player.end())
		return it->second;
	return NULL;
}
PlayerPtr ServerLogic::GetPlayerByUid(int64_t uid)
{
	auto it = m_uid2Player.find(uid);
	if (it != m_uid2Player.end())
		return it->second;
	return NULL;
}

///////////////////////////////////////////////////////////////////////////