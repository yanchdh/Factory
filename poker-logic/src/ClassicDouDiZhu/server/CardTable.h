#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include "io_event.h"
#include "Player.h"
#include "CardTableInfo.h"
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	namespace server {

		class Shuffle;
		class CardTable
		{
		public:
			CardTable(const CardTable&) = delete;
			CardTable& operator=(const CardTable&) = delete;
			explicit CardTable(int64_t tableId, CardTableInfoPtr pCardTableInfo, const std::vector<PlayerPtr>& vecPlayer);
			inline int64_t GetTableId(void) { return m_tableId; }
			inline bool IsEnd(void) { return m_isEnd; }
			inline bool IsDzWin(void) { return m_isEnd ? m_isDzWin : false; }
			inline bool CanDisCard(const card_info& info);
			void Start();
			void Stop();
			void ReqCall(const msg_request_classicddz_call& req);
			void ReqDisCard(const msg_request_classicddz_discard& req);

		private:
			bool									m_isEnd;
			bool									m_isDzWin;
			int32_t									m_dzSeat;
			int32_t									m_callSeat;
			int32_t									m_disCardSeat;
			int64_t									m_tableId;

			std::vector<PlayerPtr>					m_vecPlayer;
			CardTableInfoPtr						m_pCardTableInfo;

			std::vector<card_info>					m_vecHistoryCardInfo;
		};

		typedef std::shared_ptr<CardTable> CardTablePtr;

	};

};