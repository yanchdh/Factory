#pragma once
#include <vector>
#include <stdint.h>
#include "Numerical.h"
#include "classicddz.pb.h"

namespace ClassicDouDiZhu {

	std::vector<int> vec_card_type = 
	{
		e_card_1, e_card_2, e_card_3, e_card_3_1, e_card_3_2, e_card_4, e_card_4_1, e_card_4_2_1, e_card_4_2_2
	};

	std::vector<int8_t> vec_val_list =
	{
		3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 14
	};

	//////////////////////////////////////////////////////////////////////////////

	void		card_info_print(const card_info& info)
	{
		int card_type = info.card_type();
		fprintf(stdout, "card_type[%x] ", card_type);
		if (card_type == e_card_0)
		{
			fprintf(stdout, "\n");
			return;
		}
		fprintf(stdout, "cards: [ ");
		const std::string& data = info.data();
		for (int i = 2; i < (int)data.length(); ++i)
		{
			int card_id = data[i];
			int val = (card_id - 1) / 4 + 1;
			if (card_id == max_card_id)
				val++;
			fprintf(stdout, "%d ", val);
		}
		fprintf(stdout, "]\n");
	}

	bool		select_card_valid(const std::vector<int8_t>& vec_select_card)
	{
		int sz = vec_select_card.size();
		if (sz < min_select_card_num || sz > max_select_card_num)
			return false;
		int64_t bitCard = 0;
		for (int i = 0; i < sz; ++i)
		{
			int64_t card_id = vec_select_card[i];
			if (card_id < min_card_id || card_id > max_card_id)
				return false;
			card_id = (1LL << card_id);
			if ((bitCard & card_id) == card_id)
				return false;
			bitCard |= card_id;
		}
		return true;
	}

	bool		card_type_check(int card_type, int card_num, const std::vector<int8_t>& vec_val_num, int8_t& bg, int8_t& ed)
	{
		int n1 = ((card_type >> 16) & 0xF);
		int n2 = ((card_type >> 12) & 0xF);
		int p1 = ((card_type >>  8) & 0xF);
		int p2 = ((card_type >>  4) & 0xF);
		int min_shunzi_num = (card_type & 0xF);
		if (card_num % (n1 * p1 + n2 * p2) != 0)
			return false;
		int shunzi_num = card_num / (n1 * p1 + n2 * p2);
		if (shunzi_num > 1 && min_shunzi_num == 0)
			return false;
		if (shunzi_num != 1 && shunzi_num < min_shunzi_num || shunzi_num > max_card_shunzi_num)
			return false;
		for (int i = vec_val_list.size() - 1 - (shunzi_num == 1 ? 0 : 2); i >= shunzi_num - 1; --i)
		{
			bg = i - shunzi_num + 1;
			ed = i;
			bool is_valid = true;
			for (int j = bg; j <= ed; ++j)
			{
				if (vec_val_num[vec_val_list[j]] < n1)
				{
					is_valid = false;
					break;
				}
			}
			if (!is_valid)
				continue;
			if (n2 == 0)
				return true;
			for (int j = vec_val_list.size() - 1; j >= 0; --j)
			{
				int n = vec_val_num[vec_val_list[j]];
				if (j >= bg && j <= ed)
					n -= n1;
				if (n % n2 != 0)
				{
					is_valid = false;
					break;
				}
			}
			if (is_valid)
				return true;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////

	card_info*	card_info_generator(const std::vector<int8_t>& vec_select_card)
	{
		if (vec_select_card.empty())
		{
			card_info* pInfo = new card_info();
			pInfo->set_card_type(e_card_0);
			pInfo->set_data("");
			return pInfo;
		}

		if (!select_card_valid(vec_select_card))
			return NULL;

		int card_num = vec_select_card.size();
		std::vector<int8_t> vec_val_num(max_card_val + 1, 0);
		for (int i = 0; i < card_num; ++i)
		{
			int card_id = vec_select_card[i];
			int val = (card_id - 1) / 4 + 1;
			vec_val_num[val]++;
		}

		for (int i = vec_card_type.size() - 1; i >= 0; --i)
		{
			int card_type = vec_card_type[i];
			int8_t bg, ed;
			if (card_type_check(card_type, card_num, vec_val_num, bg, ed))
			{
				fprintf(stdout, "%d -> %d : %lx\n", vec_val_list[bg], vec_val_list[ed], card_type);
				card_info* pInfo = new card_info();
				pInfo->set_card_type(card_type);
				std::string* data = pInfo->mutable_data();
				data->reserve(2 + card_num);
				data->push_back(bg);
				data->push_back(ed);
				for (int i = 0; i < card_num; ++i)
					data->push_back(vec_select_card[i]);
				return pInfo;
			}
		}
		return NULL;
	}

	bool		card_info_valid(const card_info& info)
	{
		int card_type = info.card_type();
		if (card_type == e_card_0)
			return true;

		if (!e_classic_doudizhu_card_type_IsValid(card_type))
			return false;
		
		const std::string& data = info.data();

		int len = data.length();
		if (len < 2)
			return false;

		int card_num = len - 2;
		int bg = data[0];
		int ed = data[1];

		if (ed < bg)
			return false;

		int n1 = ((card_type >> 16) & 0xF);
		int n2 = ((card_type >> 12) & 0xF);
		int p1 = ((card_type >> 8) & 0xF);
		int p2 = ((card_type >> 4) & 0xF);
		int min_shunzi_num = (card_type & 0xF);

		if ((n1 * p1 + n2 * p2) * (ed - bg + 1) != card_num)
			return false;

		std::vector<int8_t> vec_val_num(max_card_val + 1, 0);
		for (int i = 2; i < len; ++i)
		{
			int card_id = data[i];
			int val = (card_id - 1) / 4 + 1;
			vec_val_num[val]++;
		}

		for (int i = bg; i <= ed; ++i)
		{
			if (vec_val_num[vec_val_list[i]] < n1)
				return false;
		}
		if (n2 == 0)
			return true;
		for (int j = vec_val_list.size() - 1; j >= 0; --j)
		{
			int n = vec_val_num[vec_val_list[j]];
			if (j >= bg && j <= ed)
				n -= n1;
			if (n % n2 != 0)
				return false;
		}
		return true;
	}

	bool			card_info_compare(const card_info& bigger, const card_info& smaller)
	{
		int bigger_card_type = bigger.card_type();
		const std::string& bigger_data = bigger.data();
		int bigger_bg = bigger_data[0];
		int bigger_ed = bigger_data[1];
		int bigger_len = bigger_ed - bigger_bg + 1;
		int smaller_card_type = smaller.card_type();
		const std::string& smaller_data = smaller.data();
		int smaller_bg = smaller_data[0];
		int smaller_ed = smaller_data[1];
		int smaller_len = smaller_ed - smaller_bg + 1;

		if (bigger_card_type == smaller_card_type)
		{
			if (bigger_len == smaller_len)
			{
				if (bigger_bg > smaller_bg)
					return true;
				else if (bigger_len == 1 && bigger_data[2] == max_card_id)
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
		{
			if (bigger_card_type == e_card_2 && bigger_bg == bigger_ed && bigger_ed == vec_val_list.back())
				return true;
			if (smaller_card_type == e_card_4 && smaller_bg == smaller_ed && smaller_ed == vec_val_list.back())
				return false;
			if (bigger_card_type == e_card_4 && bigger_ed - bigger_bg + 1 == card_color_num)
				return true;
		}
		return false;
	}

}