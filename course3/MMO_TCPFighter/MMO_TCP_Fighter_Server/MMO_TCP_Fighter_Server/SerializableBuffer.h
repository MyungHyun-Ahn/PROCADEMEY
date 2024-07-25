#pragma once
class SerializableBuffer
{
public:
	enum class DEFINE
	{
		DEFAULT_SIZE = 1400
	};

	SerializableBuffer();
	SerializableBuffer(int size);

	virtual ~SerializableBuffer();


	void Clear()
	{
		m_Rear = 0;
		m_Front = 0;
	}

	bool Enqueue(char *buffer, int size);
	bool Dequeue(char *buffer, int size);

	int GetBufferSize() { return m_MaxSize; }
	int GetDataSize() { return m_Rear - m_Front; }

	// 외부에서 버퍼를 직접 조작하기 위한 용도
	char *GetBufferPtr() { return m_Buffer; }
	int MoveWritePos(int size) { m_Rear += size; return m_Rear; }
	int MoveReadPos(int size) { m_Front += size; return m_Front; }

public:
	SerializableBuffer &operator<<(char chData);
	SerializableBuffer &operator<<(unsigned char byData);

	SerializableBuffer &operator<<(short shData);
	SerializableBuffer &operator<<(unsigned short wData);

	SerializableBuffer &operator<<(int iData);
	SerializableBuffer &operator<<(long lData);
	SerializableBuffer &operator<<(float fData);

	SerializableBuffer &operator<<(__int64 iData);
	SerializableBuffer &operator<<(double dData);



	SerializableBuffer &operator>>(char &chData);
	SerializableBuffer &operator>>(unsigned char &byData);

	SerializableBuffer &operator>>(short &shData);
	SerializableBuffer &operator>>(unsigned short &wData);

	SerializableBuffer &operator>>(int &iData);
	SerializableBuffer &operator>>(long &lData);
	SerializableBuffer &operator>>(float &fData);

	SerializableBuffer &operator>>(__int64 &iData);
	SerializableBuffer &operator>>(double &dData);


private:
	char *m_Buffer;
	int m_Front = 0;
	int m_Rear = 0;
	int m_MaxSize = (int)DEFINE::DEFAULT_SIZE;
};
