#pragma once
#include <stdint.h>
struct MsgHeader
{
	uint32_t m_len;
	uint32_t m_msgid;
	MsgHeader() : m_len(0), m_msgid(0) {}
	MsgHeader(uint32_t _len, uint32_t _msgid) : m_len(_len), m_msgid(_msgid) {}
};