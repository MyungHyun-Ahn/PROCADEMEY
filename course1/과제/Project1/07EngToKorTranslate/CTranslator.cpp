#include "pch.h"
#include "CTranslator.h"
#include "CDictionary.h"
#include "TString.h"

CTranslator::CTranslator()
{
	m_InputBuffer[0] = '\0';
	m_OutputBuffer[0] = '\0';
	m_Sep = TEXT(" ,\t\n");
	_tprintf(TEXT("번역기 시작 \n"));
}

CTranslator::~CTranslator()
{
	_tprintf(TEXT("번역기 종료 \n"));
}

CTranslator *CTranslator::GetInstance()
{
	static CTranslator instance;
	return &instance;
}

void CTranslator::Start()
{
	// 여기서 번역기 시작
	startTranslator();
}

DWORD CTranslator::startTranslator()
{
	m_bIsRunning = TRUE;
	while (m_bIsRunning)
	{
		Interface();
	}
	return m_bIsRunning;
}

// CUI Interface - 내부에서 입력받고 입력에 맞는 역할 수행
void CTranslator::Interface()
{
	SelectNum selectNum;
	_tprintf(TEXT("1. 영한 번역 2. 한영 번역 3. 종료\n"));
	_tprintf(TEXT("입력 >> "));
	_tscanf_s(TEXT("%d"), &selectNum);
	while(_gettchar() != '\n'); // 입력 버퍼 비우기

	switch (selectNum)
	{
	case SelectNum::ENG_TO_KOR:
		InputStr();
		ToLower(); // 소문자로 변환
		Translate(SelectNum::ENG_TO_KOR);
		OutputStr();
		break;
	case SelectNum::KOR_TO_ENG:
		InputStr();
		Translate(SelectNum::KOR_TO_ENG);
		OutputStr();
		break;
	case SelectNum::EXIT:
		m_bIsRunning = FALSE;
		break;
	}
}

void CTranslator::InputStr()
{
	_tprintf(TEXT("문장 입력 >> "));
	_getts_s(m_InputBuffer);
}

// 결과를 출력하는 함수
void CTranslator::OutputStr()
{
	_tprintf(TEXT("\n---- 번역 결과 ----\n\n"));
	_tprintf(TEXT("%s\n"), m_OutputBuffer);
	_tprintf(TEXT("%d개의 단어를 번역 성공\n"), m_iCounter);
	_tprintf(TEXT("\n-------------------\n\n"));

	// Output 버퍼 비우기
	m_OutputBuffer[0] = '\0';
}

void CTranslator::ToLower()
{
	for (size_t i = 0; m_InputBuffer[i] != '\0'; i++)
	{
		m_InputBuffer[i] |= TO_UPPER_MASK;
	}
}

// 번역 함수
void CTranslator::Translate(SelectNum selOption)
{
	TCHAR *token = nullptr;
	TCHAR *word = tcstok(m_InputBuffer, m_Sep, &token);
	size_t output_cursor = 0;
	m_iCounter = 0; // 번역 성공 문자 개수 저장

	while (word != nullptr)
	{
		if (selOption == SelectNum::ENG_TO_KOR)
		{
			const TCHAR *kor = CDictionary::GetInstance()->SearchKor(word);
			if (kor != nullptr)
			{
				m_iCounter++;
				tcscat(m_OutputBuffer, kor);
				output_cursor = tcslen(m_OutputBuffer);
				m_OutputBuffer[output_cursor] = ' ';
				m_OutputBuffer[output_cursor + 1] = '\0';
				word = tcstok(NULL, m_Sep, &token);
			}
			else // 번역 실패 시 원본 단어 붙이기
			{
				tcscat(m_OutputBuffer, word);
				output_cursor = tcslen(m_OutputBuffer);
				m_OutputBuffer[output_cursor] = ' ';
				m_OutputBuffer[output_cursor + 1] = '\0';
				word = tcstok(NULL, m_Sep, &token);
			}
			continue;
		}

		if (selOption == SelectNum::KOR_TO_ENG)
		{
			const TCHAR *eng = CDictionary::GetInstance()->SearchEng(word);
			if (eng != nullptr)
			{
				m_iCounter++;
				tcscat(m_OutputBuffer, eng);
				output_cursor = tcslen(m_OutputBuffer);
				m_OutputBuffer[output_cursor] = ' ';
				m_OutputBuffer[output_cursor + 1] = '\0';
				word = tcstok(NULL, m_Sep, &token);
			}
			else
			{
				tcscat(m_OutputBuffer, word);
				output_cursor = tcslen(m_OutputBuffer);
				m_OutputBuffer[output_cursor] = ' ';
				m_OutputBuffer[output_cursor + 1] = '\0';
				word = tcstok(NULL, m_Sep, &token);
			}
			continue;
		}
	}
}
