#include "pch.h"
#include "Session.h"
#include "CLanServer.h"
#include "CEchoServer.h"

int main()
{
	g_Server = new CEchoServer;

	g_Server->Start(SERVER_IP, SERVER_PORT, 2, 2, 65535);

	while (true)
	{
		YieldProcessor();
	}

	return 0;
}