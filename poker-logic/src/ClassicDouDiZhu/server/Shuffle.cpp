#include "Shuffle.h"
#include <stdlib.h>
#include "CardTableInfo.h"
#include "ClassicDouDiZhu/Numerical.h"
#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif
namespace ClassicDouDiZhu {

	namespace server {

		Shuffle::Shuffle()
		{
			srand((unsigned int)time(NULL));
			m_vecCardId.resize(g_card_num, 0);
			for (int i = 0; i < g_card_num; ++i)
				m_vecCardId[i] = i + 1;
		}

		void Shuffle::RandomShuffle()
		{
			int swap_round = 5;
			while (swap_round--)
			{
				for (int i = 0; i < g_card_num; ++i)
				{
					int j = i + (rand() % (g_card_num - i));
					std::swap(m_vecCardId[i], m_vecCardId[j]);
				}
			}
		}

		CardTableInfoPtr Shuffle::GenCardTableInfo()
		{
			RandomShuffle();
			CardTableInfoPtr pInfo(new CardTableInfo());
			pInfo->m_dzPos = (g_card_num >> 1) - (rand() % g_seat_num);
			pInfo->m_dzSeat = pInfo->m_dzPos % g_seat_num;
			pInfo->m_dzCardId = m_vecCardId[pInfo->m_dzPos];
			for (int i = 0; i < g_card_num - g_hide_card_num; i += g_seat_num)
			{
				for (int j = 0; j < g_seat_num; ++j)
				{
					pInfo->m_vecPlayerCard[j][i / g_seat_num] = m_vecCardId[i + j];
					pInfo->m_vecPlayerBitCard[j] |= (1LL << m_vecCardId[i + j]);
				}
			}
			for (int i = g_card_num - g_hide_card_num, j = 0; j < g_hide_card_num && i < g_card_num; ++i, ++j)
			{
				pInfo->m_dzHideCard[j] = m_vecCardId[i];
			}
			return pInfo;
		}

	};

};