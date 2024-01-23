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
	TCHAR *m_ReadBuffer; // �Ҵ� ���� ���۴� ����ڰ� �����ؾ� ��
	HANDLE m_FileHandle;
};

