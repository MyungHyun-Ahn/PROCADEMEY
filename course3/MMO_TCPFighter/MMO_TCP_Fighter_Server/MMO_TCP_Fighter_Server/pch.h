#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <deque>
#include <vector>
#include <unordered_map>
#include <map>
#include <locale>
#include <process.h>
#include <memory>

#include "ObjectPool.h"
#include "RingBuffer.h"
#include "SerializableBuffer.h"

#pragma warning(disable : 26495)
#pragma warning(disable : 4244)