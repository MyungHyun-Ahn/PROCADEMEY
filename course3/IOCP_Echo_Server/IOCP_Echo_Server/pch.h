#pragma once

#pragma warning(disable: 26495)

// 라이브러리
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")

// 윈도우
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <windows.h>
#include <process.h>

// C 런타임 라이브러리
#include <stdlib.h>
#include <stdio.h>

// std 자료구조
#include <unordered_map>
#include <map>
#include <vector>

#include "Define.h"
#include "RingBuffer.h"
#include "SerializableBuffer.h"