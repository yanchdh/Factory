#pragma once
#include <stdint.h>
#include <memory>
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	namespace server {

		struct PlayerInfo
		{
			int32_t		m_seat;
			int32_t		m_callScore;
			int64_t		m_uid;
			int64_t		m_tableId;
			explicit PlayerInfo(int64_t uid, int64_t tableId, int32_t seat)
				: m_seat(seat)
				, m_callScore(0)
				, m_uid(uid)
				, m_tableId(tableId)
			{}

			inline void SetCallScore(int32_t score)
			{
				m_callScore = score;
			}

		};

	}

};