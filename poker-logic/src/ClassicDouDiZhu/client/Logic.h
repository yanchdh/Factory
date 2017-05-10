#pragma once
#include <stdint.h>
#include "CardTable.h"

namespace ClassicDouDiZhu {

	namespace client {

		class Logic
		{
		public:
			Logic(const Logic&) = delete;
			Logic& operator=(const Logic&) = delete;
			explicit Logic();

			void ProcessReqClassicDouDiZhuStart();
			void ProcessReqClassicDouDiZhuCall();
			void ProcessReqClassicDouDiZhuDiscard();
			void ProcessRspClassicDouDiZhuStart(int fd, std::shared_ptr<char> pdata, int len);
			void ProcessRspClassicDouDiZhuCall(int fd, std::shared_ptr<char> pdata, int len);
			void ProcessRspClassicDouDiZhuStartDiscard(int fd, std::shared_ptr<char> pdata, int len);
			void ProcessRspClassicDouDiZhuDiscard(int fd, std::shared_ptr<char> pdata, int len);
			void ProcessRspClassicDouDiZhuEnd(int fd, std::shared_ptr<char> pdata, int len);

		private:
			CardTablePtr								m_pCardTable;
		};

	};

};