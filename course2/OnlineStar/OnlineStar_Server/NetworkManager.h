#pragma once
class NetworkManager
{
	SINGLE(NetworkManager)
public:
	bool Start();
	bool Select();

private:
	bool ProcessPacket();

private:
	SOCKET listenSocket;
};