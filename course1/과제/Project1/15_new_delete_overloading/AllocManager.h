#pragma once

#pragma warning(disable : 26495)

struct stAllocInfo
{
	stAllocInfo() = delete;
	stAllocInfo(int size, const char *fileName, int line, bool isArray) : m_bArray(isArray), m_iSize(size), m_szFileName(fileName), m_iLine(line) {}

	int m_iSize;
	const char *m_szFileName;
	int m_iLine;
	bool m_bArray;
};

class CAllocManager
{
public:
	CAllocManager();
	~CAllocManager();

	friend void *operator new (size_t size, const char *File, int Line);
	friend void *operator new[](size_t size, const char *File, int Line);
	friend void operator delete (void *p);
	friend void operator delete[](void *p);

private:
	// AllocInfo 관련
	void				RegisterAllocInfo(void *ptr, int size, const char *fileName, int line, bool isArray = false);
	stAllocInfo			*SearchAllocInfo(void *ptr);
	// ptr이 없는 경우 false
	bool				DeleteAllocInfo(void *ptr);

	// Log 관련
	// 켠 시간 기반으로 FileName 만듬
	void				MakeLogFileName();
	void				WriteNOALLOCLog(void *ptr);
	void				WriteARRAYLog(void *ptr);

	// delete 할 때 호출될 기록 전용 함수
public:
	inline void			SetDeleteLog(const char *fileName, int line)
	{
		m_szDeleteFileName = fileName;
		m_iDeleteLine = line;
	}

	inline void			*GetDeleteLog()
	{
		return (void *)m_szDeleteFileName;
	}

private:
	std::map<void *, stAllocInfo> m_AllocInfos;
	char m_szLogFIleName[MAX_PATH];

	const char		*m_szDeleteFileName = nullptr;
	int				m_iDeleteLine = 0;
};

extern CAllocManager g_AllocManager;

void *operator new (size_t size, const char *File, int Line);
void *operator new[](size_t size, const char *File, int Line);
void operator delete (void *p);
// 실패한 경우 애초에 소멸자 호출부터 잘못될 것임
void operator delete[](void *p);
void operator delete (void *p, const char *File, int Line);
void operator delete[](void *p, const char *File, int Line);

#define new new(__FILE__, __LINE__)
#define delete g_AllocManager.SetDeleteLog(__FILE__, __LINE__); delete