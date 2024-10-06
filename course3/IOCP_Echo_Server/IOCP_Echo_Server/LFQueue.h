#pragma once
template<typename T>
class LFQueue
{
public:
	LFQueue() : m_LFMemoryPool(100, false)
	{
		m_Size = 0;
		QNode<T> *dummy = m_LFMemoryPool.Alloc();
		dummy->next = NULL;

		m_Head = (QNode<T> *)CombinePtrAndIdnetifier(InterlockedIncrement(&m_CurrentIdentifierHead), (ULONG_PTR)dummy);
		m_Tail = (QNode<T> *)CombinePtrAndIdnetifier(InterlockedIncrement(&m_CurrentIdentifierTail), (ULONG_PTR)dummy);
	}

	void Enqueue(T t)
	{
		QNode<T> *node = m_LFMemoryPool.Alloc();
		node->data = t;
		node->next = NULL;
		QNode<T> *newTail = (QNode<T> *)CombinePtrAndIdnetifier(InterlockedIncrement(&m_CurrentIdentifierTail), (ULONG_PTR)node);

		while (true)
		{
			QNode<T> *tail = m_Tail;
			QNode<T> *tailAddr = (QNode<T> *)GetNodePtr((ULONG_PTR)tail);
			QNode<T> *next = tailAddr->next;

			if (next == NULL)
			{
				if (InterlockedCompareExchangePointer((PVOID *)&tailAddr->next, node, next) == next)
				{
					InterlockedCompareExchangePointer((PVOID *)&m_Tail, newTail, tail);
					break;
				}
			}
			else // next가 있는 상황 - next로 tail을 밀어줌
			{
				QNode<T> *newTail = (QNode<T> *)CombinePtrAndIdnetifier(InterlockedIncrement(&m_CurrentIdentifierTail), (ULONG_PTR)next);
				InterlockedCompareExchangePointer((PVOID *)&m_Tail, newTail, tail);
			}
		}

		InterlockedIncrement(&m_Size);
	}

	int Dequeue(T *t)
	{
		if (InterlockedDecrement(&m_Size) < 0)
			return InterlockedIncrement(&m_Size);

		while (true)
		{
			QNode<T> *tail = m_Tail;
			QNode<T> *tailAddr = (QNode<T> *)GetNodePtr((ULONG_PTR)tail);

			QNode<T> *head = m_Head;
			QNode<T> *headAddr = (QNode<T> *)GetNodePtr((ULONG_PTR)head);
			QNode<T> *next = headAddr->next;

			QNode<T> *newHead = (QNode<T> *)CombinePtrAndIdnetifier(InterlockedIncrement(&m_CurrentIdentifierHead), (ULONG_PTR)next);

			if (next == NULL)
			{
				return -1;
			}
			else
			{
				if (InterlockedCompareExchangePointer((PVOID *)&m_Head, newHead, head) == head)
				{
					*t = next->data;
					m_LFMemoryPool.Free(headAddr);
				}
			}
		}

		return 0;
	}

private:
	LONG m_Size = 0;
	LONG m_CurrentIdentifierHead = 0;
	LONG m_CurrentIdentifierTail = 0;
	QNode<T> *m_Head;
	QNode<T> *m_Tail;

	LFMemoryPool<QNode<T>> m_LFMemoryPool;
};