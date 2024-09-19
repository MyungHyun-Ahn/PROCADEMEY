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
		// �� �κ��� Single ������� ���� - ������ü
		// initCount ��ŭ FreeList �Ҵ�
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
		// FreeList�� �ִ� �� ����
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

		// ���� �Ҵ��ؼ� ��ȯ�ϴ� ���� - ����ȭ �ʿ� ����
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
		else // ���� �������� Pop
		{
			// �̹� �Ҵ�� ���� Pop
			MPNode<DATA> *t;
			MPNode<DATA> *newTop;
			ULONG_PTR newTopWithIdent;
			MPNode<DATA> *addr;

			do 
			{
				// TOP �� �ͽ��� ����
				t = m_pTop;
				// t �����ϰ�
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


		// data�� �ּҸ� ��ȯ
		return &node->data;
	}

	// ���� �������� Push
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

		// Node ����
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

		// ������� ����ȭ ����
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