#include "Logic.h"
#include "classicddz.pb.h"
#include "ClassicDouDiZhu/Numerical.h"

#include <vector>

namespace ClassicDouDiZhu {

	namespace server {

		Logic::Logic()
			: m_tableId(0)
		{
			m_vecMatchPlayer.reserve(g_seat_num);
		}

		void Logic::ProcessConncetionClose(PlayerPtr player)
		{
			for (int i = m_vecMatchPlayer.size() - 1; i >= 0; --i)
			{
				if (m_vecMatchPlayer[i]->GetUid() == player->GetUid())
				{
					m_vecMatchPlayer.erase(m_vecMatchPlayer.begin() + i);
					break;
				}
			}
			auto pPlayerInfo = player->GetClassicDDZPlayerInfo();
			if (pPlayerInfo)
			{
				int64_t tableId = pPlayerInfo->m_tableId;
				auto pCardTable = GetCardTable(tableId);
				if (pCardTable)
				{
					pCardTable->Stop();
					RemoveCardTable(pCardTable);
				}
			}
		}

		void Logic::ProcessReqStart(PlayerPtr player, std::shared_ptr<char> pdata, int len)
		{
			msg_request_classicddz_start req;
			if (!req.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_request_classicddz_start Failed\n");
				return;
			}
			m_vecMatchPlayer.push_back(player);

			//match ok
			if (m_vecMatchPlayer.size() == g_seat_num)
			{
				m_tableId++;
				CardTableInfoPtr pCardTableInfo = m_shuffle.GenCardTableInfo();
				CardTablePtr pCardTable(new CardTable(m_tableId, pCardTableInfo, m_vecMatchPlayer));
				AddCardTable(pCardTable);
				pCardTable->Start();
				m_vecMatchPlayer.clear();
			}
		}

		void Logic::ProcessReqCall(PlayerPtr player, std::shared_ptr<char> pdata, int len)
		{
			msg_request_classicddz_call req;
			if (!req.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_request_classicddz_call Failed\n");
				return;
			}
			int64_t tableId = req.tableid();
			auto pCardTable = GetCardTable(tableId);
			if (!pCardTable)
				return;
			pCardTable->ReqCall(req);
		}

		void Logic::ProcessReqDiscard(PlayerPtr player, std::shared_ptr<char> pdata, int len)
		{
			msg_request_classicddz_discard req;
			if (!req.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_request_classicddz_discard Failed\n");
				return;
			}
			int64_t tableId = req.tableid();
			auto pCardTable = GetCardTable(tableId);
			if (!pCardTable)
				return;
			pCardTable->ReqDisCard(req);
			if (pCardTable->IsEnd())
			{
				fprintf(stdout, "tableid[%lld] End\n", tableId);
				pCardTable->Stop();
				RemoveCardTable(pCardTable);
			}
		}

		CardTablePtr	Logic::GetCardTable(int64_t tableId)
		{
			auto it = m_id2CardTable.find(tableId);
			if (it == m_id2CardTable.end())
				return NULL;
			return it->second;
		}
		void			Logic::AddCardTable(CardTablePtr pCardTable)
		{
			m_id2CardTable[pCardTable->GetTableId()] = pCardTable;
		}
		void			Logic::RemoveCardTable(CardTablePtr pCardTable)
		{
			m_id2CardTable.erase(pCardTable->GetTableId());
		}


	};
};