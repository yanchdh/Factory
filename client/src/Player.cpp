#include "Player.h"
#include "ClientNet.h"

Player::Player(int64_t uid, int32_t fd)
	: m_fd(fd)
	, m_uid(uid)
	, m_online(false)
	, m_pClassicDDZPlayerInfo(NULL)
{
}

void Player::Close()
{
	GLOBAL.m_netio.post(std::bind(&ClientNet::Close, GLOBAL.m_pClientNet, m_fd));
}

void Player::SetClassicDDZPlayerInfo(std::shared_ptr<ClassicDouDiZhu::client::PlayerInfo> pPlayerInfo)
{
	m_pClassicDDZPlayerInfo = pPlayerInfo;
}

std::shared_ptr<ClassicDouDiZhu::client::PlayerInfo> Player::GetClassicDDZPlayerInfo(void)
{
	return m_pClassicDDZPlayerInfo;
}

void Player::ClearClassicDDZPlayerInfo(void)
{
	m_pClassicDDZPlayerInfo.reset();
}