#include "pch.h"
#include "CTranslator.h"
#include "CDictionary.h"
#include "TString.h"

CTranslator::CTranslator()
{
	m_InputBuffer[0] = '\0';
	m_OutputBuffer[0] = '\0';
	m_Sep = TEXT(" ,\t\n");
	_tprintf(TEXT("������ ���� \n"));
}

CTranslator::~CTranslator()
{
	_tprintf(TEXT("������ ���� \n"));
}

CTranslator *CTranslator::GetInstance()
{
	static CTranslator instance;
	return &instance;
}

void CTranslator::Start()
{
	// ���⼭ ������ ����
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

// CUI Interface - ���ο��� �Է¹ް� �Է¿� �´� ���� ����
void CTranslator::Interface()
{
	SelectNum selectNum;
	_tprintf(TEXT("1. ���� ���� 2. �ѿ� ���� 3. ����\n"));
	_tprintf(TEXT("�Է� >> "));
	_tscanf_s(TEXT("%d"), &selectNum);
	while(_gettchar() != '\n'); // �Է� ���� ����

	switch (selectNum)
	{
	case SelectNum::ENG_TO_KOR:
		InputStr();
		ToLower(); // �ҹ��ڷ� ��ȯ
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
	_tprintf(TEXT("���� �Է� >> "));
	_getts_s(m_InputBuffer);
}

// ����� ����ϴ� �Լ�
void CTranslator::OutputStr()
{
	_tprintf(TEXT("\n---- ���� ��� ----\n\n"));
	_tprintf(TEXT("%s\n"), m_OutputBuffer);
	_tprintf(TEXT("%d���� �ܾ ���� ����\n"), m_iCounter);
	_tprintf(TEXT("\n-------------------\n\n"));

	// Output ���� ����
	m_OutputBuffer[0] = '\0';
}

void CTranslator::ToLower()
{
	for (size_t i = 0; m_InputBuffer[i] != '\0'; i++)
	{
		m_InputBuffer[i] |= TO_UPPER_MASK;
	}
}

// ���� �Լ�
void CTranslator::Translate(SelectNum selOption)
{
	TCHAR *token = nullptr;
	TCHAR *word = tcstok(m_InputBuffer, m_Sep, &token);
	size_t output_cursor = 0;
	m_iCounter = 0; // ���� ���� ���� ���� ����

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
			else // ���� ���� �� ���� �ܾ� ���̱�
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
