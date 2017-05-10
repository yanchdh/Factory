#pragma once
#include <stdint.h>
#include <memory>
#include "Global.h"
#include "ServerNet.h"
#include "ClassicDouDiZhu/server/PlayerInfo.h"

class Player
{
public:
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	explicit Player(int32_t fd, int64_t uid);
	inline int32_t GetFd() { return m_fd; }
	inline int64_t GetUid() { return m_uid; }
	inline bool    IsOnline() { return m_online; }
	inline void SetFd(int32_t fd){ m_fd = fd; }
	inline void SetOnline() { m_online = true; }
	inline void SetOffline() { m_online = false; }
	void Close();
	template<typename MSG_T>
	void SendPBMsg(int msgid, const MSG_T& msg)
	{
		GLOBAL.m_pServerNet->SendPBMsg(m_fd, msgid, msg);
	}
	void SetClassicDDZPlayerInfo(std::shared_ptr<ClassicDouDiZhu::server::PlayerInfo> pPlayerInfo);
	std::shared_ptr<ClassicDouDiZhu::server::PlayerInfo> GetClassicDDZPlayerInfo(void);
	void ClearClassicDDZPlayerInfo(void);
	
private:
	int32_t			m_fd;
	int64_t			m_uid;
	bool			m_online;

	std::shared_ptr<ClassicDouDiZhu::server::PlayerInfo> m_pClassicDDZPlayerInfo;
};
typedef std::shared_ptr<Player> PlayerPtr;