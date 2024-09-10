#include "pch.h"
#include "Session.h"
#include "CLanServer.h"
#include "CEchoServer.h"

bool CEchoServer::OnConnectionRequest(const WCHAR *ip, USHORT port)
{
	return true;
}

void CEchoServer::OnAccept(const UINT64 sessionID)
{
	// Ŭ���̾�Ʈ ���� ����
	// wprintf(L"[Content] Client Join : %lld\n", sessionID);
}

void CEchoServer::OnClientLeave(const UINT64 sessionID)
{
	// Ŭ���̾�Ʈ ���� ó��
	// wprintf(L"[Content] Client Leave : %lld\n", sessionID);
}

void CEchoServer::OnRecv(const UINT64 sessionID, SerializableBuffer *message)
{
	// Content ��� Ȯ�� -> ó��
	// ������ �׳� ���

	__int64 num;
	*message >> num;
	// printf("[Content] Client ID : %lld, Recv Msg : %lld\n", sessionID, num);

	SerializableBuffer sBuffer;
	sBuffer << (USHORT)sizeof(__int64) << num;

	EnterCriticalSection(&g_SessionMapLock);
	Session * session = g_SessionMap[sessionID];
	
	EnterCriticalSection(&session->m_lock);
	LeaveCriticalSection(&g_SessionMapLock);

	session->SendPacket(&sBuffer);
	if (!session->PostSend())
	{
		ReleaseSession(session);
	}

	LeaveCriticalSection(&session->m_lock);
}

void CEchoServer::OnError(int errorcode, WCHAR *errMsg)
{
}
