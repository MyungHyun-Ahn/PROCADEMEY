#pragma once
#define OUT
#define IN

template<typename T>
class CStack
{
public:
	CStack() : m_iSize(0), m_iCapacity(1) { m_arrStack = new T[m_iCapacity]; };
	CStack(int size) : m_iSize(0), m_iCapacity(size) { m_arrStack = new T[m_iCapacity]; };
	~CStack() 
	{ 
		if (m_arrStack != nullptr)
		{
			delete[] m_arrStack;
		}
	};

	bool Pop(OUT T &outData);
	bool Push(IN const T &data);
	void Reserve(int size);
	bool isEmpty() { return m_iSize == 0; }

private:
	int CalcNewStackSize();
	void Expansion(int size);

private:
	T *m_arrStack;
	int m_iSize;
	int m_iCapacity;
};

template<typename T>
inline bool CStack<T>::Pop(OUT T &outData)
{
	if (m_iSize <= 0)
		return false;

	outData = m_arrStack[m_iSize - 1];
	m_iSize--;
	return true;
}

template<typename T>
inline bool CStack<T>::Push(IN const T &data)
{
	if (m_iSize >= m_iCapacity)
	{
		int newSize = CalcNewStackSize();
		Expansion(newSize);
	}

	m_arrStack[m_iSize++] = data;
	return true;
}

template<typename T>
inline void CStack<T>::Reserve(int size)
{
	if (m_iCapacity < size)
		return;

	Expansion(size);
	m_iCapacity = size;
}

template<typename T>
inline int CStack<T>::CalcNewStackSize()
{
	int newCapacity = static_cast<int>(static_cast<double>(m_iCapacity) * 1.5);

	if (newCapacity == m_iCapacity)
		newCapacity += 1;

	return newCapacity;
}

template<typename T>
inline void CStack<T>::Expansion(int size)
{
	if (size < m_iCapacity)
		return;

	T *newArr = new T[size];
	// บนป็
	for (int i = 0; i < m_iSize - 1; i++)
	{
		newArr[i] = m_arrStack[i];
	}

	delete[] m_arrStack;

	m_arrStack = newArr;
	m_iCapacity = size;
}
