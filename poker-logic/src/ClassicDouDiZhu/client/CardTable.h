#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include "io_event.h"
#include "CardTableInfo.h"
#include "Player.h"
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	namespace client {

		class CardTable
		{
		public:
			CardTable(const CardTable&) = delete;
			CardTable& operator=(const CardTable&) = delete;
			explicit CardTable();
			bool IsDiZhou();
			bool IsDisCard();
			inline int32_t GetClientSeat(void) { return m_clientSeat; }
			inline int64_t GetTableId(void) { return m_tableId; }
			inline CardTableInfoPtr GetCardTableInfo(void) { return m_pCardTableInfo; }
			void Init(const msg_response_classicddz_start& msg);
			void Init(const msg_response_classicddz_start_discard& msg);
			void DisPlay(void);
			bool TransformInput(const std::vector<int8_t>& vecInput, std::vector<int8_t>& vecOutput);
			bool CanDisCard(const card_info& info);
			void RspDisCard(const msg_response_classicddz_discard& msg);
			
		private:
			int32_t									m_clientSeat;
			int32_t									m_score;
			int32_t									m_dzSeat;
			int32_t									m_firstSeat;
			int32_t									m_disCardSeat;
			int64_t									m_tableId;
			std::vector<PlayerPtr>					m_vecPlayer;
			CardTableInfoPtr						m_pCardTableInfo;

			std::vector<card_info>					m_vecHistoryCardInfo;
		};

		typedef std::shared_ptr<CardTable> CardTablePtr;

	};

};