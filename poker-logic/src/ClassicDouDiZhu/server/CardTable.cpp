#include "CardTable.h"
#include "Shuffle.h"
#include "ClassicDouDiZhu/Numerical.h"
#include "ClassicDouDiZhu/Rule.h"
#include <assert.h>
#include "Global.h"
#include "ServerNet.h"

#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	namespace server {

		CardTable::CardTable(int64_t tableId, CardTableInfoPtr pCardTableInfo, const std::vector<PlayerPtr>& vecPlayer)
			: m_callSeat(pCardTableInfo->m_dzSeat)
			, m_dzSeat(-1)
			, m_disCardSeat(-1)
			, m_tableId(tableId)
			, m_pCardTableInfo(pCardTableInfo)
			, m_vecPlayer(vecPlayer)
		{
			assert(g_seat_num == m_vecPlayer.size());
			for (int i = 0; i < g_seat_num; ++i)
			{
				int64_t uid = m_vecPlayer[i]->GetUid();
				std::shared_ptr<PlayerInfo> pPlayerInfo(new PlayerInfo(uid, tableId, i));
				m_vecPlayer[i]->SetClassicDDZPlayerInfo(pPlayerInfo);
			}
		}

		void CardTable::Start()
		{
			msg_response_classicddz_start msg;
			msg.set_tableid(m_tableId);
			for (int i = 0; i < g_seat_num; ++i)
			{
				auto pPlayerInfo = m_vecPlayer[i]->GetClassicDDZPlayerInfo();
				auto info = msg.add_playerinfo();
				info->set_seat(pPlayerInfo->m_seat);
				info->set_uid(pPlayerInfo->m_uid);
			}
			auto pDzInfo = msg.mutable_dizhuinfo();
			pDzInfo->set_cardid(m_pCardTableInfo->m_dzCardId);
			pDzInfo->set_pos(m_pCardTableInfo->m_dzPos);
			pDzInfo->set_seat(m_pCardTableInfo->m_dzSeat);
			for (int i = 0; i < g_seat_num; ++i)
			{
				msg.set_data(m_pCardTableInfo->m_vecPlayerCard[i]);
				m_vecPlayer[i]->SendPBMsg(response_classicddz_start, msg);
			}
		}

		void CardTable::Stop()
		{
			msg_response_classicddz_end end;
			end.set_isdizhuwin(m_isDzWin);
			for (int i = 0; i < g_seat_num; ++i)
			{
				m_vecPlayer[i]->SendPBMsg(response_classicddz_end, end);
			}
			for (int i = 0; i < g_seat_num; ++i)
			{
				m_vecPlayer[i]->ClearClassicDDZPlayerInfo();
			}
		}

		void CardTable::ReqCall(const msg_request_classicddz_call& req)
		{
			int32_t seat = req.seat();
			if (m_callSeat != seat)
			{
				fprintf(stderr, "ReqCall m_disCardSeat[%d] != seat[%d]\n", m_disCardSeat, seat);
				return;
			}
			auto pCallPlayerInfo = m_vecPlayer[m_callSeat]->GetClassicDDZPlayerInfo();
			int32_t score = req.score();
			msg_response_classicddz_call rsp;
			rsp.set_score(score);
			rsp.set_seat(seat);
			pCallPlayerInfo->SetCallScore(score);
			if (m_dzSeat == -1)
			{
				if (score > 0)
				{
					m_dzSeat = m_callSeat;
				}
			}
			else
			{
				auto pDzPlayerInfo = m_vecPlayer[m_dzSeat]->GetClassicDDZPlayerInfo();
				if (pDzPlayerInfo->m_callScore < pCallPlayerInfo->m_callScore)
				{
					m_dzSeat = m_callSeat;
				}
				else
				{
					rsp.set_score(0);
					pCallPlayerInfo->SetCallScore(0);
				}
			}
			m_callSeat = (m_callSeat + 1) % g_seat_num;
			//////////////////////////////////////////////////////////////////////////////
			if (pCallPlayerInfo->m_callScore == 3 || m_callSeat == m_pCardTableInfo->m_dzSeat)
			{
				auto pDzPlayerInfo = m_vecPlayer[m_dzSeat]->GetClassicDDZPlayerInfo();
				if (pDzPlayerInfo->m_callScore == 0)
					pDzPlayerInfo->SetCallScore(1);

				m_pCardTableInfo->SetHideCard(m_dzSeat);
				m_disCardSeat = m_dzSeat;
				msg_response_classicddz_start_discard start_discard;
				start_discard.set_seat(m_dzSeat);
				start_discard.set_score(pDzPlayerInfo->m_callScore);
				start_discard.set_data(m_pCardTableInfo->m_dzHideCard);
				for (int i = 0; i < g_seat_num; ++i)
				{
					m_vecPlayer[i]->SendPBMsg(response_classicddz_start_discard, start_discard);
				}
			}
			else
			{
				rsp.set_callseat(m_callSeat);
				for (int i = 0; i < g_seat_num; ++i)
				{
					m_vecPlayer[i]->SendPBMsg(response_classicddz_call, rsp);
				}
			}
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

		void CardTable::ReqDisCard(const msg_request_classicddz_discard& req)
		{
			int32_t seat = req.seat();
			if (m_disCardSeat != seat)
			{
				fprintf(stderr, "ReqDisCard m_disCardSeat[%d] != seat[%d]\n", m_disCardSeat, seat);
				return;
			}
			const card_info& info = req.cardinfo();
			bool bvalid = card_info_valid(req.cardinfo());
			if (!bvalid)
			{
				fprintf(stderr, "card_info invalid\n");
				return;
			}
			if (!CanDisCard(info))
			{
				fprintf(stderr, "!CanDisCard(info)\n");
				return;
			}
			if (!m_pCardTableInfo->DisCard(seat, info))
			{
				fprintf(stderr, "!m_pCardTableInfo->DisCard(info)\n");
				return;
			}
			m_disCardSeat = (m_disCardSeat + 1) % g_seat_num;
			m_vecHistoryCardInfo.push_back(info);
			m_isEnd = (0 == m_pCardTableInfo->GetCardNum(seat));

			msg_response_classicddz_discard discard;
			discard.set_seat(seat);
			*discard.mutable_cardinfo() = info;
			for (int i = 0; i < g_seat_num; ++i)
			{
				m_vecPlayer[i]->SendPBMsg(response_classicddz_discard, discard);
			}
			if (m_isEnd)
			{
				m_isDzWin = (seat == m_dzSeat);
			}
		}

	};

};