#pragma once
#include <vector>
#include <string>
#include <stdint.h>
#include "classicddz.pb.h"
#include "CardTableInfo.h"

namespace ClassicDouDiZhu {

	namespace server {

		class Shuffle
		{
		public:
			friend class CardTable;
			Shuffle(const Shuffle&) = delete;
			Shuffle& operator=(const Shuffle&) = delete;
			explicit Shuffle();
			CardTableInfoPtr GenCardTableInfo();
		private:
			void RandomShuffle();

			std::vector<int8_t>			m_vecCardId;
		};

	};

};