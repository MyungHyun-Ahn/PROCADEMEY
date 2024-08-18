#include "pch.h"
#include "SerializableBuffer.h"

ObjectPool<SerializableBuffer> g_SBufferPool(500, false);

SerializableBuffer::SerializableBuffer()
{
	m_Buffer = new char[m_MaxSize];
}

SerializableBuffer::SerializableBuffer(int size) : m_MaxSize(size)
{
	m_Buffer = new char[m_MaxSize];
}

SerializableBuffer::~SerializableBuffer()
{
	delete m_Buffer;
}

bool SerializableBuffer::Enqueue(char *buffer, int size)
{
	if (m_MaxSize - m_Rear > size)
	{
		// TODO: resize

		return false;
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, buffer, size);
	MoveWritePos(size);

	return true;
}

bool SerializableBuffer::Dequeue(char *buffer, int size)
{
	if (GetDataSize() < size)
	{
		return false;
	}

	memcpy_s(buffer, size, m_Buffer + m_Front, size);
	MoveReadPos(size);

	return true;
}