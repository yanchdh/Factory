#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "ClassicDouDiZhu/Numerical.h"
#include "ClassicDouDiZhu/Rule.h"
#include <memory>

namespace ClassicDouDiZhu {

	namespace server {

		struct CardTableInfo
		{
			int32_t						m_dzPos;
			int32_t						m_dzCardId;
			int32_t						m_dzSeat;
			std::string					m_dzHideCard;
			std::vector<std::string>	m_vecPlayerCard;
			std::vector<int32_t>		m_vecPlayerCardNum;
			std::vector<int64_t>		m_vecPlayerBitCard;
			
			explicit CardTableInfo()
				: m_dzPos(0)
				, m_dzCardId(0)
				, m_dzSeat(-1)
				, m_dzHideCard(g_hide_card_num, 0)
				, m_vecPlayerCard(g_seat_num, std::string(g_seat_card_num, 0))
				, m_vecPlayerCardNum(g_seat_num, g_seat_card_num)
				, m_vecPlayerBitCard(g_seat_num, 0)
			{
			}

			inline void SetHideCard(int32_t seat)
			{
				m_vecPlayerCardNum[seat] += g_hide_card_num;
				for (int i = 0; i < g_hide_card_num; ++i)
				{
					m_vecPlayerBitCard[seat] |= (1LL << m_dzHideCard[i]);
				}
			}

			inline bool DisCard(int32_t seat, const card_info& info)
			{
				card_info_print(info);
				if (info.card_type() == e_card_0)
					return true;
				int64_t bitCard = m_vecPlayerBitCard[seat];
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
				m_vecPlayerBitCard[seat] = bitCard;
				m_vecPlayerCardNum[seat] -= (data.length() - 2);
				return true;
			}

			inline int32_t GetCardNum(int32_t seat)
			{
				return m_vecPlayerCardNum[seat];
			}

		};
		typedef std::shared_ptr<CardTableInfo>	CardTableInfoPtr;

	};

};