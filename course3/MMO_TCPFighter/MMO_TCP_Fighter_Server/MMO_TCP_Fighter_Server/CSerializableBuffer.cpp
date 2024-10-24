#include "pch.h"
#include "CSerializableBuffer.h"

CObjectPool<CSerializableBuffer> g_SBufferPool(500, false);

CSerializableBuffer::CSerializableBuffer()
{
	m_Buffer = new char[m_MaxSize];
}

CSerializableBuffer::CSerializableBuffer(int size) : m_MaxSize(size)
{
	m_Buffer = new char[m_MaxSize];
}

CSerializableBuffer::~CSerializableBuffer()
{
	delete m_Buffer;
}

bool CSerializableBuffer::Enqueue(char *buffer, int size)
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

bool CSerializableBuffer::Dequeue(char *buffer, int size)
{
	if (GetDataSize() < size)
	{
		return false;
	}

	memcpy_s(buffer, size, m_Buffer + m_Front, size);
	MoveReadPos(size);

	return true;
}