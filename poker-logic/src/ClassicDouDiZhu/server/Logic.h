#pragma once
#include <stdint.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CardTable.h"
#include "Player.h"
#include "Shuffle.h"

namespace ClassicDouDiZhu {

	namespace server {

		class Logic
		{
		public:
			Logic(const Logic&) = delete;
			Logic& operator=(const Logic&) = delete;
			explicit Logic();

			void ProcessConncetionClose(PlayerPtr player);
			void ProcessReqStart(PlayerPtr player, std::shared_ptr<char> pdata, int len);
			void ProcessReqCall(PlayerPtr player, std::shared_ptr<char> pdata, int len);
			void ProcessReqDiscard(PlayerPtr player, std::shared_ptr<char> pdata, int len);

			CardTablePtr	GetCardTable(int64_t tableId);
			void			AddCardTable(CardTablePtr pCardTable);
			void			RemoveCardTable(CardTablePtr pCardTable);

		private:
			int64_t 									m_tableId;
			std::unordered_map<int64_t, CardTablePtr>	m_id2CardTable;

			std::vector<PlayerPtr>						m_vecMatchPlayer;

			Shuffle										m_shuffle;
		};

	};

};