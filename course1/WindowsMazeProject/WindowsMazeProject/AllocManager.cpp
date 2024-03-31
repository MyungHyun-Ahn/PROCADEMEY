#include "pch.h"
#include <map>
#include "AllocManager.h"

#ifdef new
#undef new
#endif

#ifdef delete
#undef delete
#endif

CAllocManager g_AllocManager;

CAllocManager::CAllocManager()
{
	MakeLogFileName();

	FILE *pFile;
	errno_t err = fopen_s(&pFile, m_szLogFIleName, "a");
	if (err != 0 || pFile == 0)
	{
		throw err;
	}

	// ���� ���� ����
	fclose(pFile);
}

CAllocManager::~CAllocManager()
{
	try
	{
		FILE *pFile;
		errno_t err = fopen_s(&pFile, m_szLogFIleName, "a");
		if (err != 0 || pFile == 0)
		{
			throw err;
		}

		// ������� ������ LEAK
		for (const auto &it : m_AllocInfos)
		{
			fprintf_s(pFile, "LEAK [0x%p] [%08d] %s : %d\n", it.first, it.second.m_iSize, it.second.m_szFileName, it.second.m_iLine);
		}

		fclose(pFile);
	}
	catch (errno_t err)
	{
		// ����
		printf("%d", err);
	}
}

void CAllocManager::RegisterAllocInfo(void *ptr, int size, const char *fileName, int line, bool isArray)
{
	m_AllocInfos.insert(std::pair<void *, stAllocInfo>(ptr, stAllocInfo{size, fileName, line, isArray}));
}

stAllocInfo *CAllocManager::SearchAllocInfo(void *ptr)
{
	auto it = m_AllocInfos.find(ptr);

	// ���� ���
	if (it == m_AllocInfos.end())
		return nullptr;

	return &it->second;
}

bool CAllocManager::DeleteAllocInfo(void *ptr)
{
	// NOALLOC

	auto it = m_AllocInfos.find(ptr);
	if (it == m_AllocInfos.end())
	{
		return false;
	}

	// ���� ����
	m_AllocInfos.erase(ptr);
	return true;
}

void CAllocManager::MakeLogFileName()
{
	__time64_t time64 = _time64(nullptr);

	tm nowTm;

	errno_t err = localtime_s(&nowTm, &time64);
	if (err != 0)
	{
		// ����
	}

	sprintf_s(m_szLogFIleName, "ALLOC_LOG\\Alloc_%04d%02d%02d_%02d%02d%02d.log", nowTm.tm_year + 1900, nowTm.tm_mon, nowTm.tm_mday, nowTm.tm_hour, nowTm.tm_min, nowTm.tm_sec);
}

void CAllocManager::WriteNOALLOCLog(void *ptr)
{
	FILE *pFile;
	errno_t err = fopen_s(&pFile, m_szLogFIleName, "a");
	if (err != 0 || pFile == 0)
	{
		throw err;
	}

	fprintf_s(pFile, "NOALLOC [0x%p] %s : %d\n", ptr, m_szDeleteFileName, m_iDeleteLine);

	fclose(pFile);

	m_szDeleteFileName = nullptr;
	m_iDeleteLine = 0;
}

void CAllocManager::WriteARRAYLog(void *ptr)
{
	FILE *pFile;
	errno_t err = fopen_s(&pFile, m_szLogFIleName, "a");
	if (err != 0 || pFile == 0)
	{
		throw err;
	}

	fprintf_s(pFile, "ARRAY [0x%p] %s : %d\n", ptr, g_AllocManager.m_szDeleteFileName, g_AllocManager.m_iDeleteLine);

	fclose(pFile);

	m_szDeleteFileName = nullptr;
	m_iDeleteLine = 0;
}

#define GLOBAL_ALLOC

#ifdef GLOBAL_ALLOC
void *operator new (size_t size, const char *File, int Line)
{
	void *ptr = malloc(size);

	g_AllocManager.RegisterAllocInfo(ptr, size, File, Line);

	return ptr;
}

void *operator new[](size_t size, const char *File, int Line)
	{
		void *ptr = malloc(size);
		g_AllocManager.RegisterAllocInfo(ptr, size, File, Line, true);
		return ptr;
	}

	void operator delete (void *p)
	{
		if (g_AllocManager.GetDeleteLog() == nullptr)
		{
			free(p);
			return;
		}

		if (g_AllocManager.DeleteAllocInfo(p))
		{
			free(p);
			return;
		}

		// ���� ������ �ƴ� ��Ȳ
		// �迭�� �����ߴ��� p - sizeof(void*)
		try
		{
			stAllocInfo *allocInfo = g_AllocManager.SearchAllocInfo((void *)((int)p - sizeof(void *)));
			if (allocInfo == nullptr)
			{
				// NOALLOC
				g_AllocManager.WriteNOALLOCLog(p);
				return;
			}

			// ARRAY? üũ
			if (allocInfo->m_bArray)
			{
				g_AllocManager.WriteARRAYLog(p);
			}
		}
		catch (int err)
		{
			printf("delete error : %d", err);
		}

	g_AllocManager.SetDeleteLog(nullptr, 0);
}

// ������ ��� ���ʿ� �Ҹ��� ȣ����� �߸��� ����
void operator delete[](void *p)
{
	if (g_AllocManager.GetDeleteLog() == nullptr)
	{
		free(p);
		return;
	}

	if (g_AllocManager.DeleteAllocInfo(p))
	{
		free(p);
		return;
	}

	// �迭�� ���� ���ߴ��� p + sizeof(void*)
	try
	{
		stAllocInfo *allocInfo = g_AllocManager.SearchAllocInfo((void *)((int)p + sizeof(void *)));
		if (allocInfo == nullptr)
		{
			// NOALLOC
			g_AllocManager.WriteNOALLOCLog(p);
			return;
		}

		// ARRAY? üũ
		if (allocInfo->m_bArray)
		{
			g_AllocManager.WriteARRAYLog(p);
		}
	}
	catch (int err)
	{
		printf("delete[] error : %d", err);
	}

	g_AllocManager.SetDeleteLog(nullptr, 0);
}

void operator delete (void *p, const char *File, int Line)
{
	free(p);
}

void operator delete[](void *p, const char *File, int Line)
{
	free(p);
}
#endif