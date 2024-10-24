#pragma once

class CRingBuffer
{
public:
	CRingBuffer();
	CRingBuffer(int size);
	~CRingBuffer();

	// ���� �ִ� ũ��
	inline int GetCapacity() const { return m_iCapacity - 1; }

	// ���� ������� �뷮 ���
	inline int GetUseSize() const { return (m_iRear - m_iFront + m_iCapacity) % m_iCapacity; }

	// ���� ���ۿ� ���� �뷮 ���
	inline int GetFreeSize() const { return m_iCapacity - GetUseSize() - 1; }

	// ��� ������ ����
	inline void Clear() { m_iFront = 0; m_iRear = 0; }

	// ������ ����
	int				Enqueue(char *data, int size);

	// ������ ���� - buffer�� ����
	int				Dequeue(char *buffer, int size);

	// ������ �����ϰ� �������� ����
	int				Peek(char *buffer, int size);


	// ���� �����ͷ� �ܺο��� �����ϴ� �Լ���
	// �ܺο��� �ѹ��� ������ �� �ִ� ���� ��ȯ
	inline int DirectEnqueueSize() const
	{
		if (m_iRear >= m_iFront)
		{
			return m_iCapacity - m_iRear - (m_iFront == 0 ? 1 : 0);
		}
		else
		{
			return m_iFront - m_iRear - 1;
		}
	}
	inline int DirectDequeueSize() const
	{
		if (m_iFront <= m_iRear)
		{
			return m_iRear - m_iFront;
		}
		else
		{
			return m_iCapacity - m_iFront;
		}
	}

	// Front, Rear Ŀ�� �̵�
	inline int				MoveRear(int size) { m_iRear = (m_iRear + size) % m_iCapacity; return m_iRear; }
	inline int				MoveFront(int size) { m_iFront = (m_iFront + size) % m_iCapacity; return m_iFront; }

	inline char				*GetPQueuePtr() { return m_PQueue; }

	// Front ������ ���
	inline char				*GetFrontPtr() { return m_PQueue + m_iFront; }

	// Rear ������ ���
	inline char				*GetRearPtr() { return m_PQueue + m_iRear; }

private:
	int				m_iCapacity = 15000 + 1; // ����Ʈ 1�� ����Ʈ ũ��, + 1

	int				m_iFront = 0;
	int				m_iRear = 0;

	char			*m_PQueue = nullptr;
};
