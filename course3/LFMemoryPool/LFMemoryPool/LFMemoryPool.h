#pragma once
// #define SAFE_MODE

template<typename DATA>
class LFMemoryPool
{
	typedef MPNode<DATA> *(LFMemoryPool<DATA>:: *AllocFunc)(void);
public:
	__forceinline LFMemoryPool(int initCount, bool placementNewFlag)
		: m_iSize(0), m_bPlacementNewFlag(placementNewFlag)
	{
		// 이 부분은 Single 스레드로 진행 - 전역객체
		// initCount 만큼 FreeList 할당
		for (int i = 0; i < initCount; i++)
		{
			MPNode<DATA> *node;
			if (m_bPlacementNewFlag)
			{
				node = PlacementNewAlloc();
			}
			else
			{
				node = NewAlloc();
			}

			node->next = m_pTop;
			m_pTop = node;
		}
	}

	__forceinline ~LFMemoryPool()
	{
		// FreeList에 있는 것 삭제
		while (m_pTop != nullptr)
		{
			if (m_bPlacementNewFlag)
			{
				m_pTop->data.~DATA();
			}
			MPNode<DATA> *delNode = m_pTop;
			m_pTop = m_pTop->next;
			free(delNode);
		}

	}

	__forceinline DATA *Alloc()
	{
		MPNode<DATA> *node;

		// 새로 할당해서 반환하는 영역 - 동기화 필요 없음
		if (m_pTop == nullptr)
		{
			if (m_bPlacementNewFlag)
			{
				node = PlacementNewAlloc();
			}
			else
			{
				node = NewAlloc();
			}
		}
		else // 스택 관점에서 Pop
		{
			// 이미 할당된 것을 Pop
			MPNode<DATA> *t;
			MPNode<DATA> *newTop;
			ULONG_PTR newTopWithIdent;
			MPNode<DATA> *addr;

			do 
			{
				// TOP 은 믹스된 버전
				t = m_pTop;
				// t 저장하고
				addr = (MPNode<DATA> *)GetNodePtr((ULONG_PTR)t);
				newTop = addr->next;
				if (newTop != nullptr)
					newTopWithIdent = CombinePtrAndIdnetifier(newTop->identifier, (ULONG_PTR)newTop);

			} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newTopWithIdent, (PVOID)t) != t);

			node = addr;

			if (m_bPlacementNewFlag)
			{
				node = new (node) MPNode<DATA>;
			}
		}


		// data의 주소를 반환
		return &node->data;
	}

	// 스택 관점에서 Push
	__forceinline void Free(DATA *ptr)
	{
#ifdef SAFE_MODE
		unsigned __int64 intPtr = (__int64)ptr;
		intPtr -= sizeof(void *);
		MPNode<DATA> *newTop = (MPNode<DATA> *)intPtr;

		if (newTop->poolPtr != this)
			throw;

		if (newTop->safeWord != 0xFFFF)
			throw;
#else

		// Node 대입
		MPNode<DATA> *newTop = (MPNode<DATA> *)ptr;
#endif

		if (m_bPlacementNewFlag)
		{
			newTop->data.~DATA();
		}

		MPNode<DATA> *t;
		ULONG_PTR identifier = InterlockedIncrement(&m_currentIdentifier) % MAX_IDENTIFIER;
		MPNode<DATA> *addr;
		ULONG_PTR newTopWithIdent;

		// 여기부터 동기화 영역
		do 
		{
			t = m_pTop;
			addr = (MPNode<DATA> *)GetNodePtr((ULONG_PTR)t);
			newTop->next = addr;
			newTopWithIdent = CombinePtrAndIdnetifier(identifier, (ULONG_PTR)newTop);

		} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newTopWithIdent, (PVOID)t) != t);
	}

private:

	__forceinline MPNode<DATA> *PlacementNewAlloc()
	{
		MPNode<DATA> *mallocNode = (MPNode<DATA> *)malloc(sizeof(MPNode<DATA>));
		MPNode<DATA> *newNode = new (mallocNode) MPNode<DATA>;
		newNode->identifier = InterlockedIncrement(&m_currentIdentifier) % MAX_IDENTIFIER;
#ifdef SAFE_MODE
		newNode->poolPtr = this;
#endif
		InterlockedIncrement(&m_iSize);
		return newNode;
	}

	__forceinline MPNode<DATA> *NewAlloc()
	{
		MPNode<DATA> *newNode = new MPNode<DATA>;
		newNode->identifier= InterlockedIncrement(&m_currentIdentifier) % MAX_IDENTIFIER;
#ifdef SAFE_MODE
		newNode->poolPtr = this;
#endif
		InterlockedIncrement(&m_iSize);
		return newNode;
	}

private:
	bool			m_bPlacementNewFlag;
	LONG			m_iSize = 0;
	ULONG_PTR		m_currentIdentifier = 0;
	MPNode<DATA>	*m_pTop = nullptr;
};