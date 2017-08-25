#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "ClassicDouDiZhu/Numerical.h"
#include "ClassicDouDiZhu/Rule.h"
#include <memory>
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	namespace client {

		struct CardTableInfo
		{
			int32_t						m_dzPos;
			int32_t						m_dzCardId;
			int32_t						m_dzSeat;
			int32_t						m_playerCardNum;
			int64_t						m_playerBitCard;
			std::string					m_dzHideCard;
			std::string					m_playerCard;
			
			explicit CardTableInfo(int32_t dzPos, int32_t dzCardId, int32_t dzSeat, const std::string& playerCard)
				: m_dzPos(dzPos)
				, m_dzCardId(dzCardId)
				, m_dzSeat(dzSeat)
				, m_playerCardNum(g_seat_card_num)
				, m_playerBitCard(0)
				, m_dzHideCard(g_hide_card_num, 0)
				, m_playerCard(playerCard)
			{
				assert(playerCard.length() == g_seat_card_num);
				for (int i = 0; i < g_seat_card_num; ++i)
				{
					m_playerBitCard |= (1LL << m_playerCard[i]);
				}
			}

			inline void SetHideCard(const std::string& hideCard, bool isDiZhu)
			{
				m_dzHideCard = hideCard;
				if (isDiZhu)
				{
					for (int i = 0; i < g_hide_card_num; ++i)
					{
						m_playerBitCard |= (1LL << m_dzHideCard[i]);
					}
				}
			}

			inline bool DisCard(const card_info& info)
			{
				card_info_print(info);
				if (info.card_type() == e_card_0)
					return true;
				int64_t bitCard = m_playerBitCard;
				const std::string& data = info.data();
				for (int i = data.length() - 1; i >= 2; --i)
				{
					int64_t tmp = (1LL << data[i]);
					if (!(bitCard & tmp))
					{
						return false;
					}
					bitCard ^= tmp;
				}
				m_playerBitCard = bitCard;
				m_playerCardNum -= (data.length() - 2);
				return true;
			}

		};
		typedef std::shared_ptr<CardTableInfo>	CardTableInfoPtr;

	};

};