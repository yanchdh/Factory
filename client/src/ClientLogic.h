#pragma once
#include "MessageDispatch.h"
#include "ClassicDouDiZhu/client/Logic.h"
#include "Player.h"

class ClientLogic
{
public:
	explicit ClientLogic();
	~ClientLogic();

	void ProcessMsg(int fd, int msgid, std::shared_ptr<char> pdata, int len);
	void ProcessReqEnterGame();
	void ProcessRspEnterGame(int fd, std::shared_ptr<char> pdata, int len);
	PlayerPtr GetPlayer();

private:
	PlayerPtr								m_player;
	ClassicDouDiZhu::client::Logic			m_classicDouDiZhuLogic;
	
};