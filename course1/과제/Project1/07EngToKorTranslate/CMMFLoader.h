#pragma once
class CMMFLoader
{
public:
	CMMFLoader() = delete;
	CMMFLoader(const TCHAR *filePath, size_t readBufferSize);
	~CMMFLoader();

	TCHAR *&&Load();

private:
	const TCHAR			*m_FilePath;
	size_t				m_iReadBufferSize;
	TCHAR				*m_ReadBuffer = nullptr; // �Ҵ� ���� ���۴� ����ڰ� �����ؾ� ��
	HANDLE				m_FileHandle = 0;
	HANDLE				m_MMFHandle = 0;
};

