#pragma once

#define SESSION_INDEX_MASK	0xffff000000000000
#define SESSION_ID_MASK		0x0000ffffffffffff


class CLanServer
{
public:
	bool Start(const CHAR *openIP, const USHORT port, USHORT createWorkerThreadCount, USHORT maxWorkerThreadCount, INT maxClientCount);
	// void Stop();
	inline int GetSessionCount() { return m_ClientCount; }

	void SendPacket(const UINT64 sessionId, SerializableBuffer *sBuffer);
	bool Disconnect(Session *pSession);
	bool ReleaseSession(Session *pSession);

	void Monitor();

	virtual bool OnConnectionRequest(const WCHAR *ip, USHORT port) = 0;
	virtual void OnAccept(const UINT64 sessionID) = 0; // Accept 후 접속 처리 완료 후 호출
	virtual void OnClientLeave(const UINT64 sessionID) = 0; // Release 후 호출
	virtual void OnRecv(const UINT64 sessionID, SerializableBuffer *message) = 0; // 패킷 수신 완료 후
	virtual void OnError(int errorcode, WCHAR *errMsg) = 0;

public:
	int WorkerThread();
	int AccepterThread();

// 세션 관련 함수
protected:
	// 세션 관련 초기화
	void InitializeSession(USHORT maxClientCount)
	{
		InitializeSRWLock(&m_DisconnectStackLock);

		// 여기에는 실제 포인터를 저장
		// IOCP 핸들에 Index와 결합된 버전의 포인터를 전달

		// Disconnect 스택에 모두 저장
		AcquireSRWLockExclusive(&m_DisconnectStackLock);
		for (int i = maxClientCount - 1; i >= 0; i--)
		{
			m_DisconnectIndexStack.emplace_back(i);
		}
		ReleaseSRWLockExclusive(&m_DisconnectStackLock);
	}

	USHORT GetSessionIndex(UINT64 SessionId)
	{
		UINT64 p64 = SessionId;
		// 상위 2바이트 꺼내기
		UINT64 maskP64 = p64 & SESSION_INDEX_MASK;
		maskP64 = maskP64 >> 48;
		return (USHORT)maskP64;
	}

	UINT64 GetSessionId(UINT64 SessionId)
	{
		UINT64 maskP64 = SessionId & SESSION_ID_MASK;
		return (UINT64)maskP64;
	}

	UINT64 CombineIndex(USHORT stackIndex, UINT64 Index)
	{
		UINT64 p64 = Index;
		UINT64 index64 = stackIndex;
		index64 = index64 << 48;
		return (p64 | index64);
	}

private:
	int m_ClientCount = 0;
	UINT64 m_CurrentID = 0;

	SOCKET m_listenSocket = INVALID_SOCKET;
	UINT32 m_maxWorkerThreadCount = 0;
	BOOL m_isWorkerRun = TRUE;
	std::vector<HANDLE> m_WorkerThreads;

	// Accepter
	BOOL m_isAccepterRun = TRUE;
	HANDLE m_AccepterThread;

	// IOCP 핸들
	HANDLE m_IOCPHandle = INVALID_HANDLE_VALUE;

	// TPS
	LONG m_AcceptTPS = 0;
	LONG m_RecvMsgTPS = 0;
	LONG m_SendMsgTPS = 0;

	// 최대 Session 수
	USHORT m_MaxSessionCount; // 2바이트 사용 최대 65535
	// Session 배열
public: Session *m_pArrSession[65535];

	// Disconnect Stack
	std::vector<USHORT> m_DisconnectIndexStack;
	// Disconnect Stack 락
	SRWLOCK m_DisconnectStackLock;

};

extern CLanServer *g_Server;