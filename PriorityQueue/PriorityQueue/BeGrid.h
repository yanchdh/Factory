//------------------------------------------------------
//| ��Ϸ�߼�����
//| colourstar,2015/7/30
//-------------------------------------------------------
#pragma once

#include "../Share/SeShare.h"
#include "BeCommon.h"
#include "queue.h"

// ������Ѱ·��Ҫ�����ݽṹ����

//class BeGrid;
//struct OpenListNode
//{
//	Int32	m_h;
//	TPos	m_g;
//	Int32	m_t;
//	BeGrid* m_pGrid;
//	OpenListNode(BeGrid* pGrid)
//	{
//		m_t = 0;
//		m_h = 0;
//		m_g = 0x3FFFFFFF;
//		m_pGrid = pGrid;
//	}
//	bool operator < (const OpenListNode& node) const
//	{
//		return m_g + m_h == node.m_g + node.m_h ? m_t > node.m_t : m_g + m_h > node.m_g + node.m_h;
//	}
//};

class BeGrid;
struct OpenListNode
{
	bool	m_free;
	Int32	m_h;
	TPos	m_g;
	BeGrid* m_pGrid;
	TAILQ_ENTRY(OpenListNode) m_field;
	OpenListNode(BeGrid* pGrid)
		: m_free(true)
		, m_h(0)
		, m_g(0x3FFFFFFF)
		, m_pGrid(pGrid)
	{
	}
	void reset(void)
	{
		m_free = true;
		m_g = 0x3FFFFFFF;
	}
};

class BeUnit;
class BeGrid
{
public:
	BeGrid(int x, int y);
	~BeGrid();

	// ����Ѱ·�ĸ��ֽӿ�
	void ResetState() // ����Ѱ·״̬
	{
		
	}
	// ������;�Ľӿڣ�����������ʼ���ǿ���ͨ�����Եģ�������Ϊ����ĳЩ�赲����
	void SetCanStay(bool bCan, UInt64 color)
	{
		m_bCanStay = bCan;
		if (m_color != color)
		{
			m_color = color;
			m_node.reset();
		}
	}
	bool IsObs(Int32 iOb) const;
	bool GetCanStay() const
	{
		return m_bCanStay;
	}
	void SetObs(Int32 iObs)
	{
		m_iObsBits |= iObs;
	}
	void ClrObs(Int32 iObs)
	{
		m_iObsBits &= ~iObs;
	}
	void SetParent(BeGrid* pParentGrid)
	{
		m_pParentGrid = pParentGrid;
	}
	BeGrid* GetParent()
	{
		return m_pParentGrid;
	}
	void SetUnit(BeUnit* pUnit)
	{
		m_pUnit = pUnit;
	}
	BeUnit* GetUnit() const
	{
		return m_pUnit;
	}
	void SetRegion(int iRegion)
	{
		m_iRegion = iRegion;
	}
	Int32 GetRegion() const
	{
		return m_iRegion;
	}
public:
	OpenListNode	m_node;
	Int32			m_x;
	Int32			m_y;
	UInt64			m_color;
	bool			m_bCanStay;
	Int32			m_iObsBits;
private:
	// ����Ѱ·�Ĺ̶���Ϣ
	Int32			m_iRegion;
	BeGrid*			m_pParentGrid; // ��ǰ���ӵĸ�����
	BeUnit*			m_pUnit;
};

