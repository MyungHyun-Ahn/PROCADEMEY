#include "pch.h"
#include "RingBuffer.h"

// 디폴트 생성자 10000 바이트 크기 생성
RingBuffer::RingBuffer()
{
	m_PQueue = new char[m_iCapacity];
}

RingBuffer::RingBuffer(int size) : m_iCapacity(size + 1)
{
	m_PQueue = new char[m_iCapacity];
}

RingBuffer::~RingBuffer()
{
	delete m_PQueue;
}

int RingBuffer::Enqueue(char *data, int size)
{
	// 공간 부족
	int freeSize = GetFreeSize();
	if (freeSize < size)
		return -1;

	int firstPartSize = min(size, DirectEnqueueSize());
	int secondPartSize = size - firstPartSize;

	memcpy_s(m_PQueue + m_iRear, freeSize, data, firstPartSize);
	memcpy_s(m_PQueue, secondPartSize, data + firstPartSize, secondPartSize);
	MoveRear(size);

	return size;
}

int RingBuffer::Dequeue(char *buffer, int size)
{
	int ret = Peek(buffer, size);
	if (ret == -1)
		return -1;

	MoveFront(ret);
	return ret;
}

int RingBuffer::Peek(char *buffer, int size)
{
	if (GetUseSize() < size) {
		return -1;
	}

	int firstPartSize = min(size, DirectDequeueSize());
	int secondPartSize = size - firstPartSize;

	memcpy_s(buffer, size, m_PQueue + m_iFront, firstPartSize);
	memcpy_s(buffer + firstPartSize, size - firstPartSize, m_PQueue, secondPartSize);

	return size;
}
