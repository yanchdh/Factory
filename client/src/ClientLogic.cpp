#include "ClientLogic.h"
#include "Global.h"
#include "ClientLogic.h"
#include "login.pb.h"
#include "classicddz.pb.h"
#include "ClientNet.h"

ClientLogic::ClientLogic()
	: m_player(NULL)
{
}

ClientLogic::~ClientLogic()
{

}

void ClientLogic::ProcessMsg(int fd, int msgid, std::shared_ptr<char> pdata, int len)
{
	switch (msgid)
	{
	case response_enter_game:
		ProcessRspEnterGame(fd, pdata, len);
		break;
	case response_classicddz_start:
		m_classicDouDiZhuLogic.ProcessRspClassicDouDiZhuStart(fd, pdata, len);
		break;
	case response_classicddz_call:
		m_classicDouDiZhuLogic.ProcessRspClassicDouDiZhuCall(fd, pdata, len);
		break;
	case response_classicddz_start_discard:
		m_classicDouDiZhuLogic.ProcessRspClassicDouDiZhuStartDiscard(fd, pdata, len);
		break;
	case response_classicddz_discard:
		m_classicDouDiZhuLogic.ProcessRspClassicDouDiZhuDiscard(fd, pdata, len);
		break;
	case response_classicddz_end:
		m_classicDouDiZhuLogic.ProcessRspClassicDouDiZhuEnd(fd, pdata, len);
		break;
	default:
		break;
	}
}

void ClientLogic::ProcessReqEnterGame()
{
	msg_request_enter_game req;
	req.set_uid(GLOBAL.m_uid);
	GLOBAL.m_pClientNet->SendPBMsg(request_enter_game, req);
}
void ClientLogic::ProcessRspEnterGame(int fd, std::shared_ptr<char> pdata, int len)
{
	msg_response_enter_game rsp;
	if (!rsp.ParseFromArray(pdata.get(), len))
	{
		fprintf(stderr, "Parse msg_response_enter_game Failed\n");
		return;
	}
	int err = rsp.err();
	int64_t uid = rsp.uid();
	if (GLOBAL.m_uid != uid)
	{
		fprintf(stderr, "GLOBAL.m_uid[%lld] != uid[%lld], err[%d]!\n", GLOBAL.m_uid, uid, err);
		return;
	}
	if (0 == err)
	{
		fprintf(stdout, "uid[%lld] enter_game success!\n", uid);
		m_player = PlayerPtr(new Player(uid, fd));
		m_classicDouDiZhuLogic.ProcessReqClassicDouDiZhuStart();
	}
	else
	{
		fprintf(stderr, "uid[%lld] enter_game failed, err[%d]!\n", uid, err);
	}
}

PlayerPtr ClientLogic::GetPlayer()
{
	return m_player;
}