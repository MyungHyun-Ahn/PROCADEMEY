#include "pch.h"
#include "CProfiler.h"

CProfileManager *g_ProfileMgr;

CProfileManager::CProfileManager()
{
	QueryPerformanceFrequency(&m_lFreq);
	m_dwProfileTlsIndex = TlsAlloc();
	if (m_dwProfileTlsIndex == TLS_OUT_OF_INDEXES)
	{
		// GetLastError로 로깅
		throw TLS_OUT_OF_INDEXES;
	}
}

CProfileManager::~CProfileManager()
{
	DataOutToFile();

	for (auto pProfileMap : m_arrProfileMaps)
	{
		delete pProfileMap;
	}

	m_arrProfileMaps.clear();

	TlsFree(m_dwProfileTlsIndex);
	m_dwProfileTlsIndex = 0;
}

void CProfileManager::RegisterProfile(const std::wstring &tagName)
{
	auto pMapProfiles = (std::unordered_map<std::wstring, stPROFILE> *)TlsGetValue(m_dwProfileTlsIndex);
	// TLS 이므로 1개만 접근
	if (pMapProfiles == nullptr)
	{
		pMapProfiles = new std::unordered_map<std::wstring, stPROFILE>;
		m_arrProfileMaps.emplace_back(pMapProfiles);
	}

	auto it = pMapProfiles->find(tagName);
	if (it != pMapProfiles->end())
		return; // 이미 등록된 것

	pMapProfiles->insert(std::pair<std::wstring, stPROFILE>(tagName, stPROFILE()));
}

void CProfileManager::UpdateProfile(const std::wstring &tagName, __int64 &runtime)
{
	// Register를 무조건 진행하고 진입
	// pMapProfiles 가 없는 건은 대비하지 않음

	auto pMapProfiles = (std::unordered_map<std::wstring, stPROFILE> *)TlsGetValue(m_dwProfileTlsIndex);

	auto it = pMapProfiles->find(tagName);
	if (it == pMapProfiles->end())
		throw; // 예외 상황

	stPROFILE &profile = it->second;

	profile.m_iCall++;
	profile.m_iTotalTime += runtime;

	// MAX 갱신
	if (profile.m_iMaxTime[1] < runtime)
	{
		profile.m_iMaxTime[1] = runtime;
		if (profile.m_iMaxTime[0] < profile.m_iMaxTime[1])
		{
			__int64 temp = profile.m_iMaxTime[0];
			profile.m_iMaxTime[0] = profile.m_iMaxTime[1];
			profile.m_iMaxTime[1] = temp;

			// std::swap(profile.m_iMaxTime[0], profile.m_iMaxTime[1]);
		}
	}

	// MIN 갱신
	if (profile.m_iMinTime[1] > runtime)
	{
		profile.m_iMinTime[1] = runtime;
		if (profile.m_iMinTime[0] > profile.m_iMinTime[1])
		{
			__int64 temp = profile.m_iMinTime[0];
			profile.m_iMinTime[0] = profile.m_iMinTime[1];
			profile.m_iMinTime[1] = temp;

			// std::swap(profile.m_iMaxTime[0], profile.m_iMaxTime[1]);
		}
	}
}

void CProfileManager::DataOutToFile()
{
	WCHAR fileName[MAX_PATH];

	__time64_t time64 = _time64(nullptr);
	tm nowTm;
	errno_t err1 = localtime_s(&nowTm, &time64);
	if (err1 != 0)
	{
		throw;
	}

	swprintf_s(fileName, L"PROFILE\\Profile_%04d%02d%02d_%02d%02d%02d.log"
		, nowTm.tm_year + 1900
		, nowTm.tm_mon
		, nowTm.tm_mday
		, nowTm.tm_hour
		, nowTm.tm_min
		, nowTm.tm_sec);


	FILE *fp;
	errno_t err2 = _wfopen_s(&fp, fileName, L"w");
	if (err2 != 0 || fp == nullptr)
		throw err2;

	fwprintf_s(fp, L"-------------------------------------------------------------------------------------------------------------------------\n");
	fwprintf_s(fp, L" \t\tName\t\t | \tAverage\t\t | \t Min\t\t | \t Max\t\t | \t Call\t\t |\n");
	fwprintf_s(fp, L"-------------------------------------------------------------------------------------------------------------------------\n");

	for (auto &it : m_mapProfiles)
	{
		stPROFILE &profile = it.second;

		// 4번보다 더 적게 호출된 함수면 pass
		if (profile.m_iCall < 4)
			continue;

		__int64 totalTime = profile.m_iTotalTime;
		totalTime -= profile.m_iMaxTime[0];
		totalTime -= profile.m_iMaxTime[1];
		totalTime -= profile.m_iMinTime[0];
		totalTime -= profile.m_iMinTime[1];

		double average = (double)totalTime / (profile.m_iCall - 4);
		fwprintf_s(fp, L"\t %s \t\t| \t%.04lfus\t\t| \t%.04lfus\t\t| \t%.04lfus\t\t | \t%lld\t\t\n"
			, it.first.c_str()
			, average / m_lFreq.QuadPart * MICRO_SECOND
			, (double)profile.m_iMinTime[0] / m_lFreq.QuadPart * MICRO_SECOND
			, (double)profile.m_iMaxTime[0] / m_lFreq.QuadPart * MICRO_SECOND
			, profile.m_iCall - 4);
	}
	fwprintf_s(fp, L"-------------------------------------------------------------------------------------------------------------------------\n");
	fclose(fp);
}

void CProfileManager::ResetProfile()
{
	// -1일때만 ResetProfile 호출 가능
	if (m_lResetFlag != -1)
	{
		return;
	}

	// 0으로 만들면 Reset 준비 완료
	InterlockedIncrement(&m_lResetFlag);
}

CProfile::CProfile(const std::wstring &tagName) : m_wsTagName(tagName)
{
	CProfileManager::GetInstance()->RegisterProfile(m_wsTagName);
	QueryPerformanceCounter(&m_iStartTime);
}

CProfile::CProfile(const std::wstring &tagName, const std::wstring &index) : m_wsTagName(tagName + L"_" + index)
{
	CProfileManager::GetInstance()->RegisterProfile(m_wsTagName);
	QueryPerformanceCounter(&m_iStartTime);
}

CProfile::~CProfile()
{
	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);

	__int64 runtime = endTime.QuadPart - m_iStartTime.QuadPart;

	// Profile 갱신
	CProfileManager::GetInstance()->UpdateProfile(m_wsTagName, runtime);
}