#pragma once

constexpr int CACHE_SIZE = 512;
constexpr int CACHE_WAY = 8;
constexpr int CACHE_WAY_BIT = 3;

// L1 ĳ�� �ùķ�����
// L1 ĳ�� ũ�� 32KB
// Index 9 - WAY_BIT, offset 6 : tag ������

__forceinline DWORD GetCacheIndex(UINT_PTR addr)
{
	return ((addr >> 6) & 0x3F);
}

__forceinline UINT_PTR GetCacheTag(UINT_PTR addr)
{
	UINT_PTR TAG_MASK = 0xFFF;
	return (addr & ~TAG_MASK);
}

struct CacheLine
{
public:
	CacheLine() = default;

	CacheLine(void *ptr)
	{
#ifdef _WIN64
		offset = (unsigned __int64)ptr & 0x3F;
		index = ((unsigned __int64)ptr >> 6) & 0x3F;
		tag = (unsigned __int64)ptr >> 12;
#else
		offset = (unsigned int)ptr & 0x3F;
		index = ((unsigned int)ptr >> 6) & 0x3F;
		tag = (unsigned int)ptr >> 12;
#endif
	}


#ifdef _WIN64
	unsigned __int64 offset;
	unsigned __int64 index;
	unsigned __int64 tag;
#else
	unsigned int offset;
	unsigned int index;
	unsigned int tag;
#endif
};

class CacheSimulator
{
public:
	CacheSimulator()
	{

	}

	bool IsCacheHit(void *ptr)
	{
		CacheLine cl(ptr);

		// ���� ĳ�ÿ� �ִ��� ã�´�.
		int accessIndex = -1;
		for (size_t i = 0; i < 8; i++)
		{
			if (m_arrCacheL1[cl.index * CACHE_WAY + i].tag == cl.tag)
			{
				accessIndex = i;
			}
		}

		// ĳ�� ��Ʈ
		if (accessIndex != -1)
		{
			CacheAccess(accessIndex, cl.index);
			return true;
		}
		// ĳ�� �̽�
		// ��ü ��å ����
		
		accessIndex = CacheExchange(cl.index);
		m_arrCacheL1[accessIndex - CACHE_WAY + cl.index * CACHE_WAY] = cl;

		return false;
	}

	__forceinline void CacheAccess(int accessIndex, int index)
	{
		unsigned char route = m_arrCacheRoute[index];
		unsigned char indexBit = 0b10000000; // �̰����� �ε��� �Ǵ�

		// accessIndex�� ���� ���� ��� ���
		accessIndex += CACHE_WAY;

		// accessIndex % 2 == 0 ����, 1 ������
		for (int i = 0; i < CACHE_WAY_BIT; i++)
		{
			bool isRight = true;
			if (accessIndex % 2 == 0)
			{
				isRight = false;
			}

			accessIndex = accessIndex << 1;

			unsigned char curIndex = indexBit >> accessIndex;
			if (isRight)
				route |= curIndex;
			else
				route &= ~curIndex;
		}

		m_arrCacheRoute[index] = route;
	}

	__forceinline int CacheExchange(int index)
	{
		unsigned char route = m_arrCacheRoute[index];
		unsigned char indexBit = 0b10000000; // �̰����� �ε��� �Ǵ�

		int accessIndex = 1;
		for (int i = 0; i < CACHE_WAY_BIT; i++)
		{
			unsigned char curIndex = indexBit >> accessIndex;

			// ����
			if ((route & curIndex) == curIndex)
			{
				route &= ~curIndex; // ���� ���
				accessIndex = accessIndex << 1;
			}
			else // ������
			{
				route |= curIndex; // ���� ���
				accessIndex = (accessIndex << 1) + 1;
			}
		}

		m_arrCacheRoute[index] = route;
		return accessIndex;
	}

private:
	CacheLine m_arrCacheL1[CACHE_SIZE];

	// ��� ��Ͽ�
	unsigned char m_arrCacheRoute[CACHE_SIZE / CACHE_WAY] = { 0, };
};