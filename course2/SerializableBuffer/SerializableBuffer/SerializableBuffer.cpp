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

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &chData, sizeof(char));
	MoveWritePos(sizeof(char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(unsigned char byData)
{
	if (m_MaxSize - m_Rear > sizeof(unsigned char))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &byData, sizeof(unsigned char));
	MoveWritePos(sizeof(unsigned char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(short shData)
{
	if (m_MaxSize - m_Rear > sizeof(short))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &shData, sizeof(short));
	MoveWritePos(sizeof(short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(unsigned short wData)
{
	if (m_MaxSize - m_Rear > sizeof(unsigned short))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &wData, sizeof(unsigned short));
	MoveWritePos(sizeof(unsigned short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(int iData)
{
	if (m_MaxSize - m_Rear > sizeof(int))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &iData, sizeof(int));
	MoveWritePos(sizeof(int));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(long lData)
{
	if (m_MaxSize - m_Rear > sizeof(long))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &lData, sizeof(long));
	MoveWritePos(sizeof(long));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(float fData)
{
	if (m_MaxSize - m_Rear > sizeof(float))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &fData, sizeof(float));
	MoveWritePos(sizeof(float));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(__int64 iData)
{
	if (m_MaxSize - m_Rear > sizeof(__int64))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &iData, sizeof(__int64));
	MoveWritePos(sizeof(__int64));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator<<(double dData)
{
	if (m_MaxSize - m_Rear > sizeof(double))
	{
		// TODO: resize
	}

	memcpy_s(m_Buffer + m_Rear, m_MaxSize - m_Rear, &dData, sizeof(double));
	MoveWritePos(sizeof(double));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(char &chData)
{
	if (GetDataSize() < sizeof(char))
	{
		throw;
	}

	memcpy_s(&chData, sizeof(char), m_Buffer + m_Front, sizeof(char));
	MoveReadPos(sizeof(char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(unsigned char &byData)
{
	if (GetDataSize() < sizeof(unsigned char))
	{
		throw;
	}

	memcpy_s(&byData, sizeof(unsigned char), m_Buffer + m_Front, sizeof(unsigned char));
	MoveReadPos(sizeof(unsigned char));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(short &shData)
{
	if (GetDataSize() < sizeof(short))
	{
		throw;
	}

	memcpy_s(&shData, sizeof(short), m_Buffer + m_Front, sizeof(short));
	MoveReadPos(sizeof(short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(unsigned short &wData)
{
	if (GetDataSize() < sizeof(char))
	{
		throw;
	}

	memcpy_s(&wData, sizeof(unsigned short), m_Buffer + m_Front, sizeof(unsigned short));
	MoveReadPos(sizeof(unsigned short));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(int &iData)
{
	if (GetDataSize() < sizeof(int))
	{
		throw;
	}

	memcpy_s(&iData, sizeof(int), m_Buffer + m_Front, sizeof(int));
	MoveReadPos(sizeof(int));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(long &lData)
{
	if (GetDataSize() < sizeof(long))
	{
		throw;
	}

	memcpy_s(&lData, sizeof(long), m_Buffer + m_Front, sizeof(long));
	MoveReadPos(sizeof(long));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(float &fData)
{
	if (GetDataSize() < sizeof(float))
	{
		throw;
	}

	memcpy_s(&fData, sizeof(float), m_Buffer + m_Front, sizeof(float));
	MoveReadPos(sizeof(float));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(__int64 &iData)
{
	if (GetDataSize() < sizeof(__int64))
	{
		throw;
	}

	memcpy_s(&iData, sizeof(__int64), m_Buffer + m_Front, sizeof(__int64));
	MoveReadPos(sizeof(__int64));

	return *this;
}

SerializableBuffer &SerializableBuffer::operator>>(double &dData)
{
	if (GetDataSize() < sizeof(double))
	{
		throw;
	}

	memcpy_s(&dData, sizeof(double), m_Buffer + m_Front, sizeof(double));
	MoveReadPos(sizeof(double));

	return *this;
}
