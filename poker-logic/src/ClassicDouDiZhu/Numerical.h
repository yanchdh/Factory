#pragma once
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	const int g_card_num = 54;
	const int g_hide_card_num = 3;
	const int g_seat_num = 3;
	const int g_seat_card_num = (g_card_num - g_hide_card_num) / g_seat_num;

	const int min_select_card_num = 1;
	const int max_select_card_num = 20;
	const int min_card_id = 1;
	const int max_card_id = 54;
	const int min_card_val = 1;
	const int max_card_val = 14;
	const int card_color_num = 4;
	const int max_card_shunzi_num = 12;

	const std::vector<int> vec_card_type =
	{
		e_card_1, e_card_2, e_card_3, e_card_3_1, e_card_3_2, e_card_4, e_card_4_1, e_card_4_2_1, e_card_4_2_2
	};

	const std::vector<int8_t> vec_val_list =
	{
		3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 14
	};
};