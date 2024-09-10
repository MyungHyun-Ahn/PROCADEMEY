#pragma once
class CLanServer
{
public:
	bool Start(const CHAR *openIP, const USHORT port, USHORT createWorkerThreadCount, USHORT maxWorkerThreadCount, INT maxClientCount);
	// void Stop();
	inline int GetSessionCount() { return m_ClientCount; }

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

private:
	int m_ClientCount = 0;
	int m_CurrentID = 0;

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

};

extern CLanServer *g_Server;
extern std::unordered_map<UINT64, Session *>g_SessionMap;
extern CRITICAL_SECTION g_SessionMapLock;