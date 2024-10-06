#pragma once
class Session
{
public:
	friend class CLanServer;

	Session() 
		: m_RecvOverlapped(IOOperation::RECV)
		, m_SendOverlapped(IOOperation::SEND)
		, m_IsValid(FALSE)
	{}

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

	void Init(SOCKET socket, UINT64 sessionId)
	{
		m_ClientSocket = socket;
		m_SessionID = sessionId;
		m_SendFlag = FALSE;
		InterlockedExchange(&m_IsValid, TRUE);
		m_ioCount = 0;
		m_SendCount = 0;
		m_SendBuffer.Clear();
		m_RecvBuffer.Clear();
	}

	void RecvCompleted(int size);

	bool SendPacket(SerializableBuffer *buffer);
	void SendCompleted(int size);

	bool PostRecv();
	bool PostSend();

	void Clear()
	{
		InterlockedExchange(&m_IsValid, FALSE);
		m_ioCount = 0;
		m_SendFlag = 0;
		m_ClientSocket = INVALID_SOCKET;
		m_SessionID = -1;

		m_RecvBuffer.Clear();
		m_SendBuffer.Clear();
	}

public:
	static Session *Alloc()
	{
		return m_SessionLFMemoryPool.Alloc();
	}

	static void Free(Session *ptr)
	{
		m_SessionLFMemoryPool.Free(ptr);
	}

public:
	LONG				m_IsValid;

	SOCKET				m_ClientSocket;
	UINT64				m_SessionID;
	RingBuffer			m_RecvBuffer;
	RingBuffer			m_SendBuffer;

	WSABUF				m_SendWSABuf[MAX_SEND_WSABUF_COUNT];
	LONG				m_SendCount;
	LONG				m_SendCountBack;

	OverlappedEx		m_RecvOverlapped;
	OverlappedEx		m_SendOverlapped;

	LONG				m_ioCount = 0;
	LONG				m_SendFlag = 0;

	inline static LFMemoryPool<Session> m_SessionLFMemoryPool = LFMemoryPool<Session>(100, false);
};