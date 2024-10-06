#include "pch.h"
#include "LFMemoryPool.h"
#include "Session.h"
#include "CLanServer.h"

void Session::RecvCompleted(int size)
{
	m_RecvBuffer.MoveRear(size);

	DWORD currentUseSize = m_RecvBuffer.GetUseSize();
	while (currentUseSize > 0)
	{
		USHORT packetHeader;
		int retVal = m_RecvBuffer.Peek((char *)&packetHeader, PACKET_HEADER_SIZE);

		if (m_RecvBuffer.GetUseSize() < PACKET_HEADER_SIZE + packetHeader)
			break;


		m_RecvBuffer.MoveFront(retVal);

		SerializableBuffer *buffer = SerializableBuffer::Alloc();
		buffer->Clear();
		retVal = m_RecvBuffer.Dequeue(buffer->GetContentBufferPtr(), packetHeader);
		if (retVal != packetHeader)
			__debugbreak();

		if (packetHeader == 0)
			__debugbreak();

		buffer->MoveWritePos(retVal);

		g_Server->OnRecv(m_SessionID, buffer);
		SerializableBuffer::Free(buffer);
		currentUseSize = m_RecvBuffer.GetUseSize();
	}
}

bool Session::SendPacket(SerializableBuffer *buffer)
{
	m_SendCountBack++;
	int size = m_SendBuffer.Enqueue((char *)&buffer, sizeof(SerializableBuffer *));
	PostSend();
	return true;
}

void Session::SendCompleted(int size)
{
	// m_SendCount를 믿고 삭제 진행
	for (int i = 0; i < m_SendCount; i++)
	{
		SerializableBuffer *sendSBuffer;
		m_SendBuffer.Dequeue((char *)&sendSBuffer, sizeof(SerializableBuffer *));
		SerializableBuffer::Free(sendSBuffer);
	}

	// m_SendBuffer.MoveFront(size);
	InterlockedExchange(&m_SendFlag, 0);
}

bool Session::PostRecv()
{
	if (m_IsValid == FALSE)
		return true;

	int errVal;
	int retVal;

	WSABUF wsaBuf[2];
	wsaBuf[0].buf = m_RecvBuffer.GetRearPtr();
	wsaBuf[0].len = m_RecvBuffer.DirectEnqueueSize();
	wsaBuf[1].buf = m_RecvBuffer.GetPQueuePtr();
	wsaBuf[1].len = m_RecvBuffer.GetFreeSize() - wsaBuf[0].len;

	memset(&m_RecvOverlapped, 0, sizeof(OVERLAPPED));

	InterlockedIncrement(&m_ioCount);

	DWORD flag = 0;
	retVal = WSARecv(m_ClientSocket, wsaBuf, 2, nullptr, &flag, (LPWSAOVERLAPPED)&m_RecvOverlapped, NULL);

	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		if (errVal != WSA_IO_PENDING)
		{
			if (errVal != 10054 && errVal != 10038)
				wprintf(L"[WSARecv] Error Code : %d \n", errVal);

			if (InterlockedDecrement(&m_ioCount) == 0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Session::PostSend()
{
	if (m_IsValid == FALSE)
		return true;

	// 보낼게 없으면 반환
	if (m_SendBuffer.GetUseSize() <= 0)
		return true;

	if (InterlockedExchange(&m_SendFlag, TRUE) == TRUE)
		return true;

	int errVal;
	int retVal;

	DWORD sendBufferUseSize = m_SendBuffer.GetUseSize();

	// 보낼 개수
	int sendCount = sendBufferUseSize / sizeof(SerializableBuffer *);
	m_SendCount = sendCount;// min(MAX_SEND_WSABUF_COUNT, sendCount); // 일단 최대개수로

	for (int i = 0; i < m_SendCount; i++)
	{
		SerializableBuffer *sendSBuffer;
		// 이상한 값이 1개 씩 sendBuffer에 들어있다? - 넣을 때 추적
		// Login Packet이 Accept 스레드에서 수행되어 발생한 문제
		m_SendBuffer.Peek((char *)&sendSBuffer, sizeof(SerializableBuffer *), i * sizeof(SerializableBuffer *));
		m_SendWSABuf[i].buf = sendSBuffer->GetBufferPtr();
		m_SendWSABuf[i].len = sendSBuffer->GetDataSize() + sendSBuffer->GetHeaderSize();
	}

	m_SendCountBack = 0;

	memset(&m_SendOverlapped, 0, sizeof(OVERLAPPED));

	InterlockedIncrement(&m_ioCount);

	DWORD sendBytes = 0;
	retVal = WSASend(m_ClientSocket, m_SendWSABuf, m_SendCount, &sendBytes, 0, (LPOVERLAPPED)&m_SendOverlapped, NULL);

	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		if (errVal != WSA_IO_PENDING)
		{
			if (errVal != 10054 && errVal != 10038)
				wprintf(L"[WSASend] Error Code : %d \n", errVal);

			if (InterlockedDecrement(&m_ioCount) == 0)
			{
				InterlockedExchange(&m_SendFlag, FALSE);
				return false;
			}
		}
	}

	return true;
}
