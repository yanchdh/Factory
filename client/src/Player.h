#pragma once
#include <stdint.h>
#include <memory>
#include "Global.h"
#include "ClientNet.h"
#include "ClassicDouDiZhu/client/PlayerInfo.h"

class Player
{
public:
	explicit Player(int64_t uid, int32_t fd);
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
		GLOBAL.m_pClientNet->SendPBMsg(msgid, msg);
	}
	void SetClassicDDZPlayerInfo(std::shared_ptr<ClassicDouDiZhu::client::PlayerInfo> pPlayerInfo);
	std::shared_ptr<ClassicDouDiZhu::client::PlayerInfo> GetClassicDDZPlayerInfo(void);
	void ClearClassicDDZPlayerInfo(void);
private:
	int32_t			m_fd;
	int64_t			m_uid;
	bool			m_online;

	std::shared_ptr<ClassicDouDiZhu::client::PlayerInfo>	m_pClassicDDZPlayerInfo;
};

typedef std::shared_ptr<Player> PlayerPtr;
