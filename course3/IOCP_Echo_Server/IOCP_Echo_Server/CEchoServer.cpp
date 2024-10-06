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
	USHORT index = GetSessionIndex(sessionID);
	Session *p = m_pArrSession[index];
	if (p->m_SessionID != sessionID)
		__debugbreak();

	// 클라이언트 정보 생성
	// wprintf(L"[Content] Client Join : %lld\n", sessionID);
	SerializableBuffer *sBuffer = SerializableBuffer::Alloc();
	sBuffer->Clear();
	(*sBuffer) << LOGIN_PAYLOAD;
	SendPacket(sessionID, sBuffer);
}

void CEchoServer::OnClientLeave(const UINT64 sessionID)
{
	// 클라이언트 종료 처리
	// wprintf(L"[Content] Client Leave : %lld\n", sessionID);
}

void CEchoServer::OnRecv(const UINT64 sessionID, SerializableBuffer *message)
{
	// Content 헤더 확인 -> 처리
	// 지금은 그냥 출력

	__int64 num;
	*message >> num;

	UINT64 id = GetSessionIndex(sessionID);
	// printf("[Content] Client ID : %lld, Recv Msg : %lld\n", id, num);

	SerializableBuffer *sBuffer = SerializableBuffer::Alloc();
	sBuffer->Clear();
	(*sBuffer) << num;
	SendPacket(sessionID, sBuffer);
}

void CEchoServer::OnError(int errorcode, WCHAR *errMsg)
{
}
