#include "Logic.h"
#include "classicddz.pb.h"
#include "ClassicDouDiZhu/Numerical.h"
#include "Global.h"
#include "ClientNet.h"
#include "ClientLogic.h"
#include <iostream>
#include "ClassicDouDiZhu/Rule.h"

namespace ClassicDouDiZhu {

	namespace client {

		Logic::Logic()
			: m_pCardTable(CardTablePtr(new CardTable()))
		{
		}

		/*******************************************************************************************/
		/*****************************   ProcessReq     ********************************************/
		/*******************************************************************************************/

		void Logic::ProcessReqClassicDouDiZhuStart()
		{
			msg_request_classicddz_start req;
			req.set_uid(GLOBAL.m_uid);
			GLOBAL.m_pLogic->GetPlayer()->SendPBMsg(request_classicddz_start, req);
		}

		void Logic::ProcessReqClassicDouDiZhuCall()
		{
			int score = 0;
			while (true)
			{
				fprintf(stdout, "Call DiZhu, Please Input(0, 1, 2, 3): ");
				std::cin >> score;
				if (score >= 0 && score <= 3)
					break;
			}

			msg_request_classicddz_call req;
			req.set_tableid(m_pCardTable->GetTableId());
			req.set_seat(m_pCardTable->GetClientSeat());
			req.set_score(score);
			GLOBAL.m_pLogic->GetPlayer()->SendPBMsg(request_classicddz_call, req);
		}

		void Logic::ProcessReqClassicDouDiZhuDiscard()
		{
			std::vector<int8_t> input;
			std::vector<int8_t> output;
			int32_t val;
			while (true)
			{
				bool ok = true;
				m_pCardTable->DisPlay();
				fprintf(stdout, "Please Input(By the end of 0): ");
				while (true)
				{
					std::cin >> val;
					if (0 == val)
						break;
					input.push_back(val);
					if (input.size() > max_select_card_num)
					{
						ok = false;
						break;
					}
				}
				if (ok)
				{
					output.reserve(input.size());
					ok = m_pCardTable->TransformInput(input, output);
					if (ok)
					{
						card_info* pInfo = card_info_generator(output);
						if (!pInfo)
						{
							ok = false;
						}
						else
						{
							ok = m_pCardTable->CanDisCard(*pInfo);
							if (ok)
							{
								msg_request_classicddz_discard req;
								req.set_tableid(m_pCardTable->GetTableId());
								req.set_seat(m_pCardTable->GetClientSeat());
								req.set_allocated_cardinfo(pInfo);
								GLOBAL.m_pLogic->GetPlayer()->SendPBMsg(request_classicddz_discard, req);
								break;
							}
						}
					}
				}
				if (!ok)
				{
					input.clear();
					output.clear();
					fprintf(stdout, "Input Error, ");
				}
			}
		}

		/*******************************************************************************************/
		/*****************************   ProcessRsp     ********************************************/
		/*******************************************************************************************/

		void Logic::ProcessRspClassicDouDiZhuStart(int fd, std::shared_ptr<char> pdata, int len)
		{
			msg_response_classicddz_start rsp;
			if (!rsp.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_response_classicddz_start Failed\n");
				return;
			}
			fprintf(stdout, "response_classicddz_start\n");
			for (int i = 0; i < rsp.playerinfo_size(); ++i)
			{
				const auto& pPlayerInfo = rsp.playerinfo(i);
				fprintf(stdout, "uid: %lld; seat: %d\n", pPlayerInfo.uid(), pPlayerInfo.seat());
			}
			const std::string& data = rsp.data();
			for (int j = 0; j < (int)data.length(); ++j)
			{
				fprintf(stdout, "%d ", data[j]);
			}
			fprintf(stdout, "\n");
			m_pCardTable->Init(rsp);

			if (m_pCardTable->IsDiZhou())
			{
				ProcessReqClassicDouDiZhuCall();
			}
		}

		void Logic::ProcessRspClassicDouDiZhuEnd(int fd, std::shared_ptr<char> pdata, int len)
		{
			msg_response_classicddz_end rsp;
			if (!rsp.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_response_classicddz_end Failed\n");
				return;
			}
			bool bWin = m_pCardTable->IsDiZhou() ? rsp.isdizhuwin() : !rsp.isdizhuwin();
			if (bWin)
				fprintf(stdout, "Win\n");
			else
				fprintf(stdout, "Lose\n");
		}

		void Logic::ProcessRspClassicDouDiZhuCall(int fd, std::shared_ptr<char> pdata, int len)
		{
			msg_response_classicddz_call rsp;
			if (!rsp.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_response_classicddz_call Failed\n");
				return;
			}
			fprintf(stdout, "response_classicddz_call: seat[%d] score[%d]\n", rsp.seat(), rsp.score());
			if (rsp.has_callseat() && rsp.callseat() == m_pCardTable->GetClientSeat())
			{
				ProcessReqClassicDouDiZhuCall();
			}
		}
		void Logic::ProcessRspClassicDouDiZhuStartDiscard(int fd, std::shared_ptr<char> pdata, int len)
		{
			msg_response_classicddz_start_discard rsp;
			if (!rsp.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_response_classicddz_start_discard Failed\n");
				return;
			}
			fprintf(stdout, "seat[%d], scord[%d], hidecard[%d %d %d]\n", 
				rsp.seat(), rsp.score(), rsp.data()[0], rsp.data()[1], rsp.data()[2]);
			m_pCardTable->Init(rsp);
			if (m_pCardTable->IsDisCard())
			{
				ProcessReqClassicDouDiZhuDiscard();
			}
		}

		void Logic::ProcessRspClassicDouDiZhuDiscard(int fd, std::shared_ptr<char> pdata, int len)
		{
			msg_response_classicddz_discard rsp;
			if (!rsp.ParseFromArray(pdata.get(), len))
			{
				fprintf(stderr, "Parse msg_response_classicddz_discard Failed\n");
				return;
			}
			fprintf(stdout, "seat[%d] ", rsp.seat());
			card_info_print(rsp.cardinfo());
			m_pCardTable->RspDisCard(rsp);
			if (m_pCardTable->IsDisCard())
			{
				ProcessReqClassicDouDiZhuDiscard();
			}
		}

	};

};