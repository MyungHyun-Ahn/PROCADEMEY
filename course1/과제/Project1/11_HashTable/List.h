#pragma once
#define OUT
#define IN

template<typename T>
struct Node
{
	Node() : m_Data(nullptr), m_Prev(nullptr), m_Next(nullptr) { }

	Node(IN const T &data) : m_Prev(nullptr), m_Next(nullptr)
	{
		// TODO : Ŭ������ ��� ���� ������ �����ؾ� ��
		m_Data = new T;
		*m_Data = data;
	}

	~Node()
	{
		if (m_Data != nullptr)
		{
			delete m_Data;
		}
	}

	T *m_Data;
	Node *m_Prev;
	Node *m_Next;
};


template<typename T>
class Iterator
{
public:
	Iterator(IN Node<T> *pNode) : m_pCur(pNode) {}

	Node<T>* GetNode()
	{
		return m_pCur;
	}

	T& operator*()
	{
		return *this->m_pCur->m_Data;
	}

	Iterator& operator++()
	{
		m_pCur = m_pCur->m_Next;
		return *this;
	}

	Iterator operator++(int)
	{
		Iterator temp(m_pCur);
		m_pCur = m_pCur->m_Next;
		return temp;
	}

	Iterator& operator--()
	{
		m_pCur = m_pCur->m_Prev;
		return *this;
	}

	Iterator operator--(int)
	{
		Iterator temp(m_pCur);
		m_pCur = m_pCur->m_Prev;
		return temp;
	}

	bool operator==(IN const Iterator& other)
	{
		return m_pCur == other.m_pCur;
	}

	bool operator!=(IN const Iterator& other)
	{
		return m_pCur != other.m_pCur;
	}

private:
	Node<T>* m_pCur;
};

template<typename T>
class List
{
public:
	// ���� ���
	List() : m_Head(new Node<T>()), m_Tail(new Node<T>())
	{
		m_Head->m_Next = m_Tail;
		m_Tail->m_Prev = m_Head;
	}

	~List()
	{
		while (m_Head->m_Next != m_Tail)
		{
			if (m_Head->m_Next != nullptr)
				Delete(m_Head->m_Next);
		}

		if (m_Head != nullptr)
			delete m_Head;

		if (m_Tail != nullptr)
			delete m_Tail;
	}

	bool Back(OUT T *data)
	{
		if (m_Tail->m_Prev == m_Head)
			return false;

		data = *m_Tail->m_Prev->m_Data;;
		return true;
	}

	bool Front(OUT T *data)
	{
		if (m_Head->m_Next == m_Tail)
			return false;

		data = *m_Head->m_Next->m_Data;;
		return true;
	}

	bool PushFront(IN const T &data)
	{
		if (!Push(m_Head, data))
			return false;

		return true;
	}

	bool PushBack(IN const T &data)
	{
		if (!Push(m_Tail->m_Prev, data))
			return false;

		return true;
	}

	bool PopFront()
	{
		if (m_Head->m_Next == m_Tail)
			return false;

		if (!Delete(m_Head->m_Next))
			return false;

		return true;
	}

	bool PopBack()
	{
		if (m_Tail->m_Prev == m_Head)
			return false;

		if (!Delete(m_Tail->m_Prev))
			return false;

		return true;
	}

	// node ����
	bool Delete(IN Node<T>* node)
	{
		Node<T>* delNodePrev = node->m_Prev;
		Node<T>* delNodeNext = node->m_Next;

		// 2 ��带 ����
		delNodeNext->m_Prev = delNodePrev;
		delNodePrev->m_Next = delNodeNext;

		if (node != nullptr)
		{
			delete node;
		}

		return true;
	}

	using Iterator = Iterator<T>;

	Iterator begin()
	{
		return Iterator(m_Head->m_Next);
	}

	Iterator end()
	{
		return Iterator(m_Tail);
	}


private:
	// ������ node�� �ڿ� ����
	bool Push(IN Node<T> *node, IN const T &data)
	{
		Node<T> *newNode = new Node<T>(data);
		Node<T> *nextNode = node->m_Next;
		Node<T> *prevNode = node;

		newNode->m_Next = nextNode;
		newNode->m_Prev = prevNode;
		nextNode->m_Prev = newNode;
		prevNode->m_Next = newNode;

		return true;
	}


private:
	Node<T> *m_Head;
	Node<T> *m_Tail;
};
