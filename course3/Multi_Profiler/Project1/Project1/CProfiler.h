#pragma once


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

#define __WFUNC__ _T(__FUNCTION__)

#define PROFILE_BEGIN(funcName, num)											\
				CProfile profile_##num(funcName, L#num)


#define MICRO_SECOND 1000000

struct stPROFILE
{
	DWORD				m_dwThreadId = GetCurrentThreadId();
	__int64				m_iTotalTime = 0;
	__int64				m_iMinTime[2] = { INT64_MAX, INT64_MAX };
	__int64				m_iMaxTime[2] = { 0, 0 };
	__int64				m_iCall = 0;
};

class CProfile
{
public:
	CProfile(const std::wstring &tagName);
	CProfile(const std::wstring &tagName, const std::wstring &index);
	~CProfile();

	const std::wstring m_wsTagName;
	LARGE_INTEGER m_iStartTime;
};


class CProfileManager
{
	SINGLE(CProfileManager)

public:
	void RegisterProfile(const std::wstring &tagName);
	void UpdateProfile(const std::wstring &tagName, __int64 &runtime);

	void ResetProfile();

public:
	void DataOutToFile();

private:
	LARGE_INTEGER m_lFreq;
	DWORD m_dwProfileTlsIndex;
	LONG m_lResetFlag = -1;
	// map 삭제, 조회 용도
	std::vector<std::unordered_map<std::wstring, stPROFILE> *> m_arrProfileMaps;
};