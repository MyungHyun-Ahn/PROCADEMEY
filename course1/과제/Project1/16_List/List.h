#pragma once

template<typename T>
class List;

template<typename T>
struct Node
{
	Node() {}
	Node(T data) : m_Data(data), m_Next(nullptr), m_Prev(nullptr) {}

	T m_Data;
	Node *m_Prev;
	Node *m_Next;
};

template<typename T>
class iterator
{
public:
	iterator(Node<T> *node = nullptr) : m_Node(node) {}

	iterator operator++(int)
	{
		Node<T> *tempNode = m_Node;
		m_Node = m_Node->m_Next;
		return iterator(tempNode);
	}

	iterator &operator++()
	{
		m_Node = m_Node->m_Next;
		return *this;
	}

	iterator operator--(int)
	{
		Node<T> *tempNode = m_Node;
		m_Node = m_Node->m_Prev;
		return iterator(tempNode);
	}

	iterator &operator--()
	{
		m_Node = m_Node->m_Prev;
		return *this;
	}

	T &operator*()
	{
		return m_Node->m_Data;
	}

	bool operator==(const iterator &other)
	{
		return m_Node == other.m_Node;
	}

	bool operator!=(const iterator &other)
	{
		return !operator==(other);
	}
	
	friend iterator List<T>::erase(iterator);

private:
	operator Node<T> *()
	{
		return m_Node;
	}

	Node<T> *m_Node;
};

template<typename T>
class List
{
public:
	List()
	{
		m_Head.m_Next = &m_Tail;
		m_Head.m_Prev = nullptr;
			  
		m_Tail.m_Next = nullptr;
		m_Tail.m_Prev = &m_Head;
	}

	~List()	{ clear(); }

	T				back() { return m_Tail.m_Prev->m_Data; }
	T				front() { return m_Head.m_Next->m_Data; }

	void			push_back(T data);
	void			push_front(T data);
	void			pop_back();
	void			pop_front();
	void			clear();
	bool			empty() { return m_Size == 0; }

// iterator 관련
	using iterator = iterator<T>;

	iterator		begin() { return iterator(m_Head.m_Next); }
	iterator		end() { return iterator(&m_Tail); }
	int				size() { return m_Size; }

	iterator		erase(iterator it)
	{
		iterator delIt = it++;
		delete_node(delIt);
		return it;
	}

	void remove(T data)
	{
		for (List<T>::iterator it = begin(); it != end();)
		{
			if (*it == data)
			{
				it = erase(it);
			}
			else
			{
				++it;
			}
		}
	}

// 내부에서 사용할 범용 함수
private:
	// 항상 target의 next에만 삽입
	void append_node(Node<T> *target, T data);

	// delNode를 삭제
	void delete_node(Node<T> *delNode);

private:
	int			m_Size = 0;
	Node<T>		m_Head;
	Node<T>		m_Tail;
};

template<typename T>
inline void List<T>::push_back(T data)
{
	Node<T> *target = m_Tail.m_Prev;
	append_node(target, data);
}

template<typename T>
inline void List<T>::push_front(T data)
{
	Node<T> *target = &m_Head;
	append_node(target, data);
}

template<typename T>
inline void List<T>::pop_back()
{
	Node<T> *delNode = m_Tail.m_Prev;
	delete_node(delNode);
}

template<typename T>
inline void List<T>::pop_front()
{
	Node<T> *delNode = m_Head.m_Next;
	delete_node(delNode);
}

template<typename T>
inline void List<T>::clear()
{
	Node<T> *delNode = m_Head.m_Next;
	while (delNode != &m_Tail)
	{
		Node<T> *nextDelNode = delNode->m_Next;
		delete delNode;
		delNode = nextDelNode;
	}

	m_Size = 0;
	m_Head.m_Next = &m_Tail;
	m_Tail.m_Prev = &m_Head;
}

template<typename T>
inline void List<T>::append_node(Node<T> *target, T data)
{
	Node<T> *newNode = new Node<T>(data);
	Node<T> *nextNode = target->m_Next;
	Node<T> *prevNode = target;
	newNode->m_Next = nextNode;
	newNode->m_Prev = prevNode;

	nextNode->m_Prev = newNode;
	prevNode->m_Next = newNode;

	m_Size++;
}

template<typename T>
inline void List<T>::delete_node(Node<T> *delNode)
{
	Node<T> *delNodeNext = delNode->m_Next;
	Node<T> *delNodePrev = delNode->m_Prev;

	delNodeNext->m_Prev = delNodePrev;
	delNodePrev->m_Next = delNodeNext;

	delete delNode;
	m_Size--;
}