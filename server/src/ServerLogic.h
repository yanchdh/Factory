#pragma once
#include <unordered_map>
#include "MessageDispatch.h"
#include "Player.h"
#include "ClassicDouDiZhu/server/Logic.h"

class ServerLogic
{
public:
	explicit ServerLogic();
	~ServerLogic();

	void ProcessConncetionClose(int fd);
	void ProcessMsg(int fd, int msgid, std::shared_ptr<char> pdata, int len);

private:

	void ProcessReqEnterGame(int fd, std::shared_ptr<char> pdata, int len);

///////////////////////////////////////////////////////////////////////////

	void AddPlayer(PlayerPtr player);
	void RemovePlayer(PlayerPtr player);
	PlayerPtr GetPlayerByFd(int fd);
	PlayerPtr GetPlayerByUid(int64_t uid);

///////////////////////////////////////////////////////////////////////////

	std::unordered_map<int, PlayerPtr>			m_fd2Player;
	std::unordered_map<int64_t, PlayerPtr>		m_uid2Player;

	ClassicDouDiZhu::server::Logic				m_classicDouDiZhuLogic;
};