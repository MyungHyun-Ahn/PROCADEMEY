#pragma once
class Session
{
public:
	friend class CLanServer;

	Session(SOCKET socket, UINT64 sessionID) 
		: m_ClientSocket(socket)
		, m_SessionID(sessionID)
		, m_RecvOverlapped(IOOperation::RECV)
		, m_SendOverlapped(IOOperation::SEND)
		, m_IsValid(TRUE)
	{
	}

	~Session()
	{
	}

	void RecvCompleted(int size);

	bool SendPacket(SerializableBuffer *buffer);
	void SendCompleted(int size);

	bool PostRecv();
	bool PostSend();

public:
	BOOL m_IsValid;

	SOCKET m_ClientSocket;
	UINT64 m_SessionID;
	RingBuffer m_RecvBuffer;
	RingBuffer m_SendBuffer;

	OverlappedEx m_RecvOverlapped;
	OverlappedEx m_SendOverlapped;

	LONG m_ioCount = 0;
	LONG m_SendFlag = 0;
};

