#include "CardTable.h"
#include "classicddz.pb.h"
#include "ClassicDouDiZhu/Numerical.h"
#include "ClassicDouDiZhu/Rule.h"
#include "Global.h"
#include "ClientLogic.h"
#include <assert.h>

namespace ClassicDouDiZhu {

	namespace client {

		CardTable::CardTable()
			: m_clientSeat(-1)
			, m_firstSeat(-1)
			, m_disCardSeat(-1)
			, m_tableId(0)
			, m_vecPlayer(g_seat_num, NULL)
			, m_pCardTableInfo(NULL)
		{
		}

		void CardTable::Init(const msg_response_classicddz_start& msg)
		{
			m_tableId = msg.tableid();
			int sz = msg.playerinfo_size();
			assert(sz == g_seat_num);
			for (int i = 0; i < g_seat_num; ++i)
			{
				int64_t uid = msg.playerinfo(i).uid();
				int32_t seat = msg.playerinfo(i).seat();
				if (uid == GLOBAL.m_uid)
				{
					m_vecPlayer[i] = GLOBAL.m_pLogic->GetPlayer();
					m_clientSeat = seat;
				}
				else
				{
					m_vecPlayer[i] = PlayerPtr(new Player(msg.playerinfo(i).uid(), 0));
				}
				std::shared_ptr<PlayerInfo> pPlayerInfo(new PlayerInfo(uid, m_tableId, seat));
				m_vecPlayer[i]->SetClassicDDZPlayerInfo(pPlayerInfo);
			}
			const classicddz_dizhu_info& dzinfo = msg.dizhuinfo();
			m_pCardTableInfo = CardTableInfoPtr(new CardTableInfo(dzinfo.pos(), dzinfo.cardid(), dzinfo.seat(), msg.data()));
		}

		void CardTable::Init(const msg_response_classicddz_start_discard& msg)
		{
			m_dzSeat = msg.seat();
			m_score = msg.score();
			m_pCardTableInfo->SetHideCard(msg.data(), m_dzSeat == m_clientSeat);
			m_firstSeat = m_disCardSeat = m_dzSeat;
		}

		bool CardTable::IsDiZhou()
		{
			//fprintf(stdout, "IsDiZhou(%d, %d)\n", m_clientSeat, m_pCardTableInfo->m_dzSeat);
			return m_clientSeat == m_pCardTableInfo->m_dzSeat;
		}

		bool CardTable::IsDisCard()
		{
			return m_clientSeat == m_disCardSeat;
		}

		bool CardTable::CanDisCard(const card_info& info)
		{
			int sz = m_vecHistoryCardInfo.size();
			if (info.card_type() == e_card_0)
			{
				if (sz == 0)
					return false;
				else if (sz > 2 && m_vecHistoryCardInfo[sz - 1].card_type() == e_card_0 
								&& m_vecHistoryCardInfo[sz - 2].card_type() == e_card_0)
					return false;
				return true;
			}
			if (sz == 0 || m_vecHistoryCardInfo.back().card_type() == e_card_0)
				return true;
			return card_info_compare(info, m_vecHistoryCardInfo.back());
		}

		void CardTable::RspDisCard(const msg_response_classicddz_discard& msg)
		{
			int32_t seat = msg.seat();
			if (m_disCardSeat != seat)
			{
				fprintf(stderr, "RspDisCard DisCard m_disCardSeat[%d] != seat[%d]");
				return;
			}
			const card_info& info = msg.cardinfo();
			if (m_disCardSeat == m_clientSeat && !m_pCardTableInfo->DisCard(info))
			{
				fprintf(stderr, "RspDisCard !m_pCardTableInfo->DisCard(info)");
				return;
			}
			m_disCardSeat = (m_disCardSeat + 1) % g_seat_num;
			m_vecHistoryCardInfo.push_back(info);
		}
		/////////////////////////////////////////////////////////////////////////
		void CardTable::DisPlay(void)
		{
			fprintf(stdout, "your cards: [ ");
			int64_t bitCard = m_pCardTableInfo->m_playerBitCard;
			if (bitCard & (1LL << 54))
				fprintf(stdout, "15 ");
			if (bitCard & (1LL << 53))
				fprintf(stdout, "14 ");
			for (int i = 8; i >= 1; --i) {
				if (bitCard & (1LL << i))
					fprintf(stdout, "%d ", (i - 1) / 4 + 1);
			}
			for (int i = 52; i >= 9; --i) {
				if (bitCard & (1LL << i))
					fprintf(stdout, "%d ", (i - 1) / 4 + 1);
			}
			fprintf(stdout, "]\n");
		}

		bool CardTable::TransformInput(const std::vector<int8_t>& vecInput, std::vector<int8_t>& vecOutput)
		{
			int64_t bitCard = m_pCardTableInfo->m_playerBitCard;
			for (const auto& val : vecInput)
			{
				if (val < 1 || val > 15)
					return false;
				if (val == 15)
				{
					int64_t tmp = (1LL << 54);
					if (bitCard & tmp)
					{
						bitCard ^= tmp;
						vecOutput.push_back(54);
					}
					else
						return false;
				}
				else if (val == 14)
				{
					int64_t tmp = (1LL << 53);
					if (bitCard & tmp)
					{
						bitCard ^= tmp;
						vecOutput.push_back(53);
					}
					else
						return false;
				}
				else
				{
					int j = -1;
					for (int i = val * 4; i > (val - 1) * 4; --i)
					{
						int64_t tmp = (1LL << i);
						if (bitCard & tmp)
						{
							bitCard ^= tmp;
							j = i;
							break;
						}
					}
					if (j != -1)
						vecOutput.push_back(j);
					else
						return false;
				}
			}
			return true;
		}

	};

};