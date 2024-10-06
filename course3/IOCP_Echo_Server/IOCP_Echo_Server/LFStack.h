#pragma once
extern DebugInfo	logging[200000];
extern LONG64		logIndex;

template<typename T>
class LFStack
{
public:
	LFStack() : m_LFMemoryPool(100, false)
	{

	}

	void Push(T data)
	{
		SNode<T> *newNode = m_LFMemoryPool.Alloc();
		newNode->data = data;
		SNode<T> *t;
		SNode<T> *addr;
		ULONG_PTR identifier = InterlockedIncrement(&m_CurrentIdentifier);
		ULONG_PTR newTopWithIdent;

		do
		{
			t = m_pTop;
			addr = (SNode<T> *)GetNodePtr((ULONG_PTR)t);
			newNode->next = addr;
			newTopWithIdent = CombinePtrAndIdnetifier(identifier, (ULONG_PTR)newNode);

		} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newTopWithIdent, (PVOID)t) != t);

		LONG64 backUp = InterlockedIncrement64(&logIndex);
		logging[backUp % 200000] = { backUp, 0, (USHORT)GetCurrentThreadId(), (USHORT)data, addr, newNode };

		// »ðÀÔ ¼º°ø
	}

	void Pop(T *data)
	{
		SNode<T> *t;
		SNode<T> *newTop;
		SNode<T> *addr;
		ULONG_PTR newTopWithIdent;

		do
		{
			t = m_pTop;
			addr = (SNode<T> *)GetNodePtr((ULONG_PTR)t);
			newTop = addr->next;
			// newTopWithIdent = CombinePtrAndIdnetifier(newTop->identifier, (ULONG_PTR)newTop);

		} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newTop, (PVOID)t) != t);

		// &logging[backUp % 200000]
		LONG64 backUp = InterlockedIncrement64(&logIndex);
		logging[backUp % 200000] = { backUp, 1, (USHORT)GetCurrentThreadId(), (USHORT)addr->data, addr, newTop };

		*data = addr->data;
		m_LFMemoryPool.Free(addr);
	}


public:
	LFMemoryPool<SNode<T>> m_LFMemoryPool;
	SNode<T> *m_pTop = nullptr;
	ULONG_PTR			m_CurrentIdentifier = 0;
};