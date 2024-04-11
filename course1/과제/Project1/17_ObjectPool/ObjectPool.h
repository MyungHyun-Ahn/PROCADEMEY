#pragma once

template<typename DATA>
struct Node
{
	void *poolPtr;
	DATA data;
	Node *link;
};

template<typename DATA>
class ObjectPool
{
	using Node = Node<DATA>;

public:
	inline ObjectPool(int initCount, bool placementNewFlag) 
		: m_iSize(0), m_bPlacementNewFlag(placementNewFlag)
	{
		// initCount 만큼 FreeList 할당
		for (int i = 0; i < initCount; i++)
		{
			Node *node;
			if (placementNewFlag)
			{
				// malloc 후 Placement New
				node = PlacementNewAlloc();
				node->link = top;
				top = node;
			}
			else
			{
				node = MallocAlloc();
				node->link = top;
				top = node;
			}
		}
	}
	inline ~ObjectPool()
	{
		// FreeList에 있는 것 삭제
		while (top != nullptr)
		{
			if (m_bPlacementNewFlag)
			{
				top->data.~DATA();
			}
			Node *delNode = top;
			top = top->link;
			free(delNode);
		}

	}

	inline DATA *Alloc()
	{
		Node *node;

		if (top == nullptr)
		{
			if (m_bPlacementNewFlag)
			{
				node = PlacementNewAlloc();
			}
			else
			{
				node = MallocAlloc();
			}
		}
		else
		{
			node = top;
			top = top->link;

			if (m_bPlacementNewFlag)
			{
				node = new (node) Node;
			}
		}

		// data의 주소를 반환
		return &node->data;
	}
	inline void Free(DATA *ptr)
	{
		unsigned __int64 intPtr = (__int64)ptr;
		intPtr -= sizeof(void*);
		Node *nodePtr = (Node *)intPtr;

		if (nodePtr->poolPtr != this)
			throw;

		if (m_bPlacementNewFlag)
		{
			nodePtr->data.~DATA();
			nodePtr->link = top;
			top = nodePtr;
		}
		else
		{
			// 그냥 FreeList에 등록
			nodePtr->link = top;
			top = nodePtr;
		}
	}

private:
	inline Node *PlacementNewAlloc()
	{
		Node *mallocNode = (Node *)malloc(sizeof(Node));
		Node *newNode = new (mallocNode) Node;
		newNode->poolPtr = this;
		m_iSize++;
		return newNode;
	}

	inline Node *MallocAlloc()
	{
		Node *newNode = (Node *)malloc(sizeof(Node));
		newNode->poolPtr = this;
		newNode->link = top;
		top = newNode;
		m_iSize++;
		return newNode;
	}

private:
	// FreeList
	bool m_bPlacementNewFlag;
	int m_iSize = 0;
	Node *top = nullptr;
};

