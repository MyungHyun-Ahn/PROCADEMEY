#pragma once

// �̱���
#define SINGLE(type)		private:										\
								type();										\
								~type();									\
								inline static type *m_instPtr = nullptr;	\
							public:											\
								static type *GetInstance()					\
								{											\
									if (m_instPtr == nullptr)				\
									{										\
										m_instPtr = new type();				\
										atexit(Destory);					\
									}										\
																			\
									return m_instPtr;						\
								}											\
																			\
								static void Destory()						\
								{											\
									delete m_instPtr;						\
									m_instPtr = nullptr;					\
								}

enum class LOG_LEVEL
{
	DEBUG,
	SYSTEM,
	ERR
};

class Logger
{
	SINGLE(Logger)
public:
	void WriteLog(const WCHAR* type, LOG_LEVEL logLevel, const WCHAR* fmt, ...);
	void WriteLogHex(const WCHAR* type, LOG_LEVEL logLevel, const WCHAR* log, BYTE* pByte, int byteLen);
	void WriteLogConsole(LOG_LEVEL logLevel, const WCHAR* fmt, ...);

	inline void SetDirectory(const WCHAR* directoryName) { m_directoryName = directoryName; }
	inline void SetLogLevel(LOG_LEVEL logLevel) { m_LogLevel = logLevel; }

private:
	CRITICAL_SECTION m_cs;

	__int64 m_LogCount = 0;
	LOG_LEVEL m_LogLevel = LOG_LEVEL::DEBUG;
	const WCHAR* m_directoryName = nullptr;
	FILE* m_pFile = nullptr;

};

extern Logger* g_Logger;