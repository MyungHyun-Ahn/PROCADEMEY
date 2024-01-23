#pragma once

class CDictionary;

enum class SelectNum
{
	ENG_TO_KOR = 1,
	KOR_TO_ENG,
	EXIT = 3
};

class CTranslator
{
private:
	CTranslator();
	~CTranslator();

public:
	static CTranslator		*GetInstance();
	void					Start();

private:
	DWORD					startTranslator();
	void					Interface();
	void					InputStr();
	void					OutputStr();
	void					ToLower();
	void					Translate(SelectNum selOption);
	

private:
	BOOL				m_bIsRunning = FALSE;
	TCHAR				m_InputBuffer[MAX_STRING_SIZE];
	TCHAR				m_OutputBuffer[MAX_STRING_SIZE];
	size_t				m_iCounter = 0;
	const TCHAR			*m_Sep;
};

