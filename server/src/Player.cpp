#include "Player.h"

Player::Player(int32_t fd, int64_t uid)
	: m_fd(fd)
	, m_uid(uid)
	, m_online(false)
	, m_pClassicDDZPlayerInfo(NULL)
{
}

void Player::Close()
{
	GLOBAL.m_netio.post(std::bind(&ServerNet::Close, GLOBAL.m_pServerNet, m_fd));
}

void Player::SetClassicDDZPlayerInfo(std::shared_ptr<ClassicDouDiZhu::server::PlayerInfo> pPlayerInfo)
{
	m_pClassicDDZPlayerInfo = pPlayerInfo;
}

std::shared_ptr<ClassicDouDiZhu::server::PlayerInfo> Player::GetClassicDDZPlayerInfo(void)
{
	return m_pClassicDDZPlayerInfo;
}

void Player::ClearClassicDDZPlayerInfo(void)
{
	m_pClassicDDZPlayerInfo.reset();
}