#pragma once

constexpr int CACHE_SIZE = 512;
constexpr int CACHE_WAY = 8;

// L1 ĳ�� �ùķ�����
// L1 ĳ�� ũ�� 32KB
// Index �⺻ 9��Ʈ, offset ĳ�ö��� �ȿ��� offset ������ tag
// 


class CacheSimulator
{
public:
	CacheSimulator() : m_iPtrSize(sizeof(void *))
	{
		
	}
	~CacheSimulator()
	{

	}

	bool IsCacheHit(void *ptr)
	{

		return true;
	}


private:
	int m_iPtrSize;
	int m_arrCacheL1[CACHE_SIZE / CACHE_WAY][CACHE_WAY];
};

