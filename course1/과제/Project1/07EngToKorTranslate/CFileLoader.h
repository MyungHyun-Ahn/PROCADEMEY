#pragma once
class CFileLoader
{
public:
	CFileLoader() = delete;
	CFileLoader(const TCHAR *filePath, size_t readBufferSize);
	~CFileLoader();

	TCHAR *&&Load();

private:
	const TCHAR *m_FilePath;
	size_t m_iReadBufferSize;
	TCHAR *m_ReadBuffer; // 할당 받은 버퍼는 사용자가 관리해야 함
	HANDLE m_FileHandle;
};

