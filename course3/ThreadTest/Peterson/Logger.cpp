#include <windows.h>
#include <share.h>
#include <time.h>
#include <strsafe.h>
#include "Logger.h"

Logger* g_Logger;

Logger::Logger()
{
	InitializeCriticalSection(&m_cs);
}

Logger::~Logger()
{

}

void Logger::WriteLog(const WCHAR* type, LOG_LEVEL logLevel, const WCHAR* fmt, ...)
{
	if (m_LogLevel > logLevel)
		return;

	tm tmTime;
	time_t nowTime = time(nullptr);
	localtime_s(&tmTime, &nowTime);

	CreateDirectory(m_directoryName, NULL);

	WCHAR fileName[256];

	StringCchPrintf(fileName, 256, L".\\%s\\%d%02d_%s.txt", m_directoryName, tmTime.tm_year + 1900, tmTime.tm_mon + 1, type);

	long long logCount = InterlockedIncrement64(&m_LogCount);

	WCHAR logLevelStr[10] = { 0, };

	switch (logLevel)
	{
	case LOG_LEVEL::DEBUG:StringCchPrintf(logLevelStr, 10, L"DEBUG");
		break;
	case LOG_LEVEL::SYSTEM:
		StringCchPrintf(logLevelStr, 10, L"SYSTEM");
		break;
	case LOG_LEVEL::ERR:
		StringCchPrintf(logLevelStr, 10, L"ERROR");
		break;
	default:
		break;
	}

	WCHAR messageBuf[256];

	va_list va;
	va_start(va, fmt);
	HRESULT hResult = StringCchVPrintf(messageBuf, 256, fmt, va);
	if (hResult != S_OK)
	{
		WriteLog(L"ERROR", LOG_LEVEL::ERR, L"�α� ���� ũ�� ����");
	}
	va_end(va);

	WCHAR totalBuf[1024];
	//��Ż ���ۿ� �ְ� ���� ����
	hResult = StringCchPrintf(totalBuf, 1024, L"[%s] [%d-%02d-%02d %02d:%02d:%02d / %s / %09lld] %s\n",
		type, tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
		tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec, logLevelStr, logCount, messageBuf);

	if (hResult != S_OK)
	{
		WriteLog(L"ERROR", LOG_LEVEL::ERR, L"�α� ���� ũ�� ����");
	}

	EnterCriticalSection(&m_cs);

	m_pFile = _wfsopen(fileName, L"a, ccs = UTF-16LE", _SH_DENYWR);
	if (m_pFile == nullptr)
	{
		wprintf(L"file open fail, errorCode = %d\n", GetLastError());
		LeaveCriticalSection(&m_cs);
		return;
	}
	fwrite(totalBuf, 1, wcslen(totalBuf) * sizeof(WCHAR), m_pFile);

	fclose(m_pFile);
	m_pFile = nullptr;

	LeaveCriticalSection(&m_cs);
}

WCHAR g_MessageBuf[10000] = { 0, };

void Logger::WriteLogHex(const WCHAR* type, LOG_LEVEL logLevel, const WCHAR* log, BYTE* pByte, int byteLen)
{
	int offset = 0;

	offset += swprintf(g_MessageBuf + offset, 10000 - offset, L"%s: ", log);

	for (int i = 0; i < byteLen; i++)
	{
		offset += swprintf(g_MessageBuf + offset, 10000 - offset, L"%02X ", pByte[i]);
	}

	WriteLog(type, logLevel, g_MessageBuf);
}

void Logger::WriteLogConsole(LOG_LEVEL logLevel, const WCHAR* fmt, ...)
{
	if (m_LogLevel > logLevel)
	{
		return;
	}

	WCHAR logBuffer[1024] = { 0, };

	va_list va;

	va_start(va, fmt);
	HRESULT hResult = StringCchVPrintf(logBuffer, 1024, fmt, va);
	if (hResult != S_OK)
	{
		WriteLogConsole(LOG_LEVEL::ERR, L"�α� ���� ũ�� ����");
	}
	va_end(va);

	wprintf(L"%s\n", logBuffer);
}