#include "pch.h"
#include "CRingBuffer.h"

// ����Ʈ ������ 10000 ����Ʈ ũ�� ����
CRingBuffer::CRingBuffer()
{
	m_PQueue = new char[m_iCapacity];
}

CRingBuffer::CRingBuffer(int size) : m_iCapacity(size + 1)
{
	m_PQueue = new char[m_iCapacity];
}

CRingBuffer::~CRingBuffer()
{
	delete m_PQueue;
}

int CRingBuffer::Enqueue(char *data, int size)
{
	// ���� ����
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

int CRingBuffer::Dequeue(char *buffer, int size)
{
	int ret = Peek(buffer, size);
	if (ret == -1)
		return -1;

	MoveFront(ret);
	return ret;
}

int CRingBuffer::Peek(char *buffer, int size)
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
