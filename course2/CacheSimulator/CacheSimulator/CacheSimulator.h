#pragma once

constexpr int CACHE_SIZE = 512;
constexpr int CACHE_WAY = 8;

// L1 캐시 시뮬레이터
// L1 캐시 크기 32KB
// Index 기본 9비트, offset 캐시라인 안에서 offset 나머지 tag
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

