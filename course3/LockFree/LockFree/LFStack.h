#pragma once

struct DebugInfo
{
	// Push 0
	// Pop 1
	LONG64 num;
	DWORD pushOrPop;
	USHORT threadId;
	USHORT data;
	void *tPtr;
	void *newTPtr;
};

template<typename Data>
struct Node
{
	Data data;
	Node *next;
};

extern DebugInfo logging[200000];
extern LONG64 logIndex;

template<typename T>
class LFStack
{
public:

	void Push(T data)
	{
		Node<T> *newNode = new Node<T>;
		newNode->data = data;
		Node<T> *t;
		do 
		{
			t = m_pTop;
			newNode->next = t;
		} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newNode, (PVOID)t) != t);
		
		LONG64 backUp = InterlockedIncrement64(&logIndex);
		logging[backUp % 200000] = { backUp, 0, (USHORT)GetCurrentThreadId(), (USHORT)data, t, newNode };
		
		// ���� ����
	}

	void Pop(T *data)
	{
		Node<T> *t;
		Node<T> *newTop;

		do 
		{
			t = m_pTop;
			newTop = t->next;
		} while (InterlockedCompareExchangePointer((volatile PVOID *)&m_pTop, (PVOID)newTop, (PVOID)t) != t);

		// &logging[backUp % 200000]
		LONG64 backUp = InterlockedIncrement64(&logIndex);
		logging[backUp % 200000] = { backUp, 1, (USHORT)GetCurrentThreadId(), (USHORT)t->data, t, newTop };

		*data = t->data;
		delete t;
	}


public:
	Node<T> *m_pTop = nullptr;

};

/*
0x00007FF711643D60  72 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 16 00 / // OldTop 60 e6 b3 57 96 01 00 00 // NewTop c0 e6 b3 57 96 01 00 00  r*5.........La..`??W?...???W?...
0x00007FF711643D80  73 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 0d 00 / // OldTop c0 e6 b3 57 96 01 00 00 // NewTop 80 e6 b3 57 96 01 00 00  s*5.........?E..???W?....??W?...
0x00007FF711643DA0  74 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 00 00 / // OldTop 80 e6 b3 57 96 01 00 00 // NewTop 50 2a b5 57 96 01 00 00  t*5.........La...??W?...P*?W?...
0x00007FF711643DC0  75 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 0b 00 / // OldTop 50 2a b5 57 96 01 00 00 // NewTop 00 e6 b3 57 96 01 00 00  u*5.........?E..P*?W?....??W?...
0x00007FF711643DE0  76 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 0f 00 / // OldTop 00 e6 b3 57 96 01 00 00 // NewTop d0 29 b5 57 96 01 00 00  v*5.........La...??W?...?)?W?...
0x00007FF711643E00  77 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 0a 00 / // OldTop d0 29 b5 57 96 01 00 00 // NewTop c0 e5 b3 57 96 01 00 00  w*5.........?E..?)?W?...???W?...
0x00007FF711643E20  78 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 0e 00 / // OldTop c0 e5 b3 57 96 01 00 00 // NewTop e0 e5 b3 57 96 01 00 00  x*5.........?E..???W?...???W?...
0x00007FF711643E40  79 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 0d 00 / // OldTop e0 e5 b3 57 96 01 00 00 // NewTop 90 2a b5 57 96 01 00 00  y*5.........La..???W?...?*?W?...

// d8 45 �������� ������ Pop OldTop 90 2a b5 57 96 01 00 00 // NewTop 80 e7 b3 57 96 01 00 00
0x00007FF711643E60  7a 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 09 00 / // OldTop 90 2a b5 57 96 01 00 00 // NewTop 80 e7 b3 57 96 01 00 00  z*5.........?E..?*?W?....??W?...
// Check
0x00007FF711643E80  7b 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 0c 00 / // OldTop 80 e7 b3 57 96 01 00 00 // NewTop d0 2a b5 57 96 01 00 00  {*5.........?E...??W?...?*?W?...
0x00007FF711643EA0  7c 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId d8 45 // Data 08 00 / // OldTop d0 2a b5 57 96 01 00 00 // NewTop 00 e7 b3 57 96 01 00 00  |*5.........?E..?*?W?....??W?...

// ������� d8 45 �����尡 Push�� ����
// 0 -> 50 2a b5 57 96 01 00 00
// ..?
0x00007FF711643EC0  7d 2a 35 1f 00 00 00 00 // 00 00 00 00 // ThId d8 45 // Data 00 00 / // OldTop 00 e7 b3 57 96 01 00 00 // NewTop 50 2a b5 57 96 01 00 00  }*5.........?E...??W?...P*?W?...

// 1 -> d0 2a b5 57 96 01 00 00
// 0 -> 50 2a b5 57 96 01 00 00
// ..?
0x00007FF711643EE0  7e 2a 35 1f 00 00 00 00 // 00 00 00 00 // ThId d8 45 // Data 01 00 / // OldTop 50 2a b5 57 96 01 00 00 // NewTop d0 2a b5 57 96 01 00 00  ~*5.........?E..P*?W?...?*?W?...

// 2 -> 90 2a b5 57 96 01 00 00 -> �쿬�� New Top���� �ش� �ּҰ� ����
// 1 -> d0 2a b5 57 96 01 00 00
// 0 -> 50 2a b5 57 96 01 00 00
// ..?
0x00007FF711643F00  7f 2a 35 1f 00 00 00 00 // 00 00 00 00 // ThId d8 45 // Data 02 00 / // OldTop d0 2a b5 57 96 01 00 00 // NewTop 90 2a b5 57 96 01 00 00  .*5.........?E..?*?W?...?*?W?...

// ��ݾ��� Pop �ߴµ� Top�� 80 e7 b3 57 96 01 00 00�� ��
// 4c 61 �����尡 Block �Ǿ��ٰ� Ǯ������ Pop�� �ߴµ�
// 90 2a b5 57 96 01 00 00 �ּҿ��� 02 00 �����Ͱ� ����ְ�
// next�� 80 e7 b3 57 96 01 00 00 �̾���
0x00007FF711643F20  80 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 02 00 / // OldTop 90 2a b5 57 96 01 00 00 // NewTop 80 e7 b3 57 96 01 00 00  .*5.........La..?*?W?....??W?...

// Push Push Pop Pop ���� �����ϰ�
0x00007FF711643F40  81 2a 35 1f 00 00 00 00 // 00 00 00 00 // ThId d8 45 // Data 03 00 / // OldTop 80 e7 b3 57 96 01 00 00 // NewTop 10 2b b5 57 96 01 00 00  ?*5.........?E...??W?....+?W?...
0x00007FF711643F60  82 2a 35 1f 00 00 00 00 // 00 00 00 00 // ThId d8 45 // Data 04 00 / // OldTop 10 2b b5 57 96 01 00 00 // NewTop d0 29 b5 57 96 01 00 00  ?*5.........?E...+?W?...?)?W?...
0x00007FF711643F80  83 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 04 00 / // OldTop d0 29 b5 57 96 01 00 00 // NewTop 10 2b b5 57 96 01 00 00  ?*5.........La..?)?W?....+?W?...
0x00007FF711643FA0  84 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 03 00 / // OldTop 10 2b b5 57 96 01 00 00 // NewTop 80 e7 b3 57 96 01 00 00  ?*5.........La...+?W?....??W?...
// Check
// ���⼭ Double Free�� ���� Crash
0x00007FF711643FC0  85 2a 35 1f 00 00 00 00 // 01 00 00 00 // ThId 4c 61 // Data 0c 00 / // OldTop 80 e7 b3 57 96 01 00 00 // NewTop d0 2a b5 57 96 01 00 00  ?*5.........La...??W?...?*?W?...
*/