#include <memory>
#include "SerializableBuffer.h"

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

SerializableBuffer &SerializableBuffer::operator<<(char chData)
{
	if (m_MaxSize - m_Rear > sizeof(char))
	{
		// TODO: resize
	}

	char *ptr = (char *)(m_Buffer + m_Rear);
	*ptr = chData;

	MoveWritePos(sizeof(char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(unsigned char byData)
{
	if (m_MaxSize - m_Rear > sizeof(unsigned char))
	{
		// TODO: resize
	}

	unsigned char *ptr = (unsigned char *)(m_Buffer + m_Rear);
	*ptr = byData;

	MoveWritePos(sizeof(unsigned char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(short shData)
{
	if (m_MaxSize - m_Rear > sizeof(short))
	{
		// TODO: resize
	}

	short *ptr = (short *)(m_Buffer + m_Rear);
	*ptr = shData;

	MoveWritePos(sizeof(short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(unsigned short wData)
{
	if (m_MaxSize - m_Rear > sizeof(unsigned short))
	{
		// TODO: resize
	}

	unsigned short *ptr = (unsigned short *)(m_Buffer + m_Rear);
	*ptr = wData;

	MoveWritePos(sizeof(unsigned short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(int iData)
{
	if (m_MaxSize - m_Rear > sizeof(int))
	{
		// TODO: resize
	}

	int *ptr = (int *)(m_Buffer + m_Rear);
	*ptr = iData;

	MoveWritePos(sizeof(int));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(long lData)
{
	if (m_MaxSize - m_Rear > sizeof(long))
	{
		// TODO: resize
	}

	long *ptr = (long *)(m_Buffer + m_Rear);
	*ptr = lData;

	MoveWritePos(sizeof(long));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(float fData)
{
	if (m_MaxSize - m_Rear > sizeof(float))
	{
		// TODO: resize
	}

	float *ptr = (float *)(m_Buffer + m_Rear);
	*ptr = fData;

	MoveWritePos(sizeof(float));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(__int64 iData)
{
	if (m_MaxSize - m_Rear > sizeof(__int64))
	{
		// TODO: resize
	}

	__int64 *ptr = (__int64 *)(m_Buffer + m_Rear);
	*ptr = iData;

	MoveWritePos(sizeof(__int64));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(double dData)
{
	if (m_MaxSize - m_Rear > sizeof(double))
	{
		// TODO: resize
	}

	*(double *)(m_Buffer + m_Rear) = dData;

	MoveWritePos(sizeof(double));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(char &chData)
{
	if (GetDataSize() < sizeof(char))
	{
		throw;
	}

	chData = *(char *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(unsigned char &byData)
{
	if (GetDataSize() < sizeof(unsigned char))
	{
		throw;
	}

	byData = *(unsigned char *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(unsigned char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(short &shData)
{
	if (GetDataSize() < sizeof(short))
	{
		throw;
	}

	shData = *(short *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(unsigned short &wData)
{
	if (GetDataSize() < sizeof(char))
	{
		throw;
	}

	wData = *(unsigned short *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(unsigned short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(int &iData)
{
	if (GetDataSize() < sizeof(int))
	{
		throw;
	}

	iData = *(int *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(int));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(long &lData)
{
	if (GetDataSize() < sizeof(long))
	{
		throw;
	}

	lData = *(long *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(long));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(float &fData)
{
	if (GetDataSize() < sizeof(float))
	{
		throw;
	}

	fData = *(float *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(float));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(__int64 &iData)
{
	if (GetDataSize() < sizeof(__int64))
	{
		throw;
	}

	iData = *(__int64 *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(__int64));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(double &dData)
{
	if (GetDataSize() < sizeof(double))
	{
		throw;
	}

	dData = *(double *)(m_Buffer + m_Front);
	MoveReadPos(sizeof(double));

	return *this;
}
