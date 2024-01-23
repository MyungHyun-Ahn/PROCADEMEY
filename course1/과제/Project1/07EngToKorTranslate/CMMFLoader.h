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
	TCHAR				*m_ReadBuffer; // �Ҵ� ���� ���۴� ����ڰ� �����ؾ� ��
	HANDLE				m_FileHandle;
	HANDLE				m_MMFHandle;
};

