#include "pch.h"
#include "Session.h"
#include "CLanServer.h"

void Session::RecvCompleted(int size)
{
	m_RecvBuffer.MoveRear(size);

	DWORD currentUseSize = m_RecvBuffer.GetUseSize();

	// m_SendBuffer.Enqueue(m_RecvBuffer.GetFrontPtr(), m_RecvBuffer.DirectDequeueSize());
	// m_SendBuffer.Enqueue(m_RecvBuffer.GetPQueuePtr(), currentUseSize - m_RecvBuffer.DirectDequeueSize());

	while (currentUseSize > 0)
	{
		USHORT packetHeader;
		int retVal = m_RecvBuffer.Peek((char *)&packetHeader, PACKET_HEADER_SIZE);

		if (m_RecvBuffer.GetUseSize() < PACKET_HEADER_SIZE + packetHeader)
			break;


		m_RecvBuffer.MoveFront(retVal);

		SerializableBuffer buffer;
		retVal = m_RecvBuffer.Dequeue(buffer.GetBufferPtr(), packetHeader);
		if (retVal != packetHeader)
			__debugbreak();

		if (packetHeader == 0)
			__debugbreak();

		buffer.MoveWritePos(retVal);

		g_Server->OnRecv(m_SessionID, &buffer);


		currentUseSize = m_RecvBuffer.GetUseSize();
	}
}

bool Session::SendPacket(SerializableBuffer *buffer)
{
	USHORT size = buffer->GetDataSize();
	m_SendBuffer.Enqueue((char *)&size, sizeof(USHORT));
	m_SendBuffer.Enqueue(buffer->GetBufferPtr(), buffer->GetDataSize());
	return true;
}

void Session::SendCompleted(int size)
{
	m_SendBuffer.MoveFront(size);
	InterlockedExchange(&m_SendFlag, 0);
}

bool Session::PostRecv()
{
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
		if (errVal != WSA_IO_PENDING && errVal != 10054)
		{
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
	int errVal;
	int retVal;

	// 보낼게 없으면 반환
	if (m_SendBuffer.GetUseSize() <= 0)
		return true;

	if (InterlockedExchange(&m_SendFlag, 1) == 1)
		return true;

	WSABUF wsaBuf[2];
	int wsaBufCount = 1;
	int useSize = m_SendBuffer.GetUseSize();
	int directDequeueSize = m_SendBuffer.DirectDequeueSize();

	if (useSize <= directDequeueSize)
	{
		wsaBuf[0].buf = m_SendBuffer.GetFrontPtr();
		wsaBuf[0].len = useSize;
		wsaBufCount = 1;
	}
	else
	{
		wsaBuf[0].buf = m_SendBuffer.GetFrontPtr();
		wsaBuf[0].len = directDequeueSize;
		wsaBuf[1].buf = m_SendBuffer.GetPQueuePtr();
		wsaBuf[1].len = useSize - wsaBuf[0].len;
		wsaBufCount = 2;
	}


	memset(&m_SendOverlapped, 0, sizeof(OVERLAPPED));

	InterlockedIncrement(&m_ioCount);

	DWORD sendBytes = 0;
	retVal = WSASend(m_ClientSocket, wsaBuf, wsaBufCount, &sendBytes, 0, (LPOVERLAPPED)&m_SendOverlapped, NULL);

	if (retVal == SOCKET_ERROR)
	{
		errVal = WSAGetLastError();
		if (errVal != WSA_IO_PENDING && errVal != 10054)
		{
			wprintf(L"[WSASend] Error Code : %d \n", errVal);

			if (InterlockedDecrement(&m_ioCount) == 0)
			{
				InterlockedExchange(&m_SendFlag, 0);
				return false;
			}
		}
	}

	return true;
}
