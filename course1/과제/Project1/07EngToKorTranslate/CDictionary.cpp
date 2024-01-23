#include "pch.h"
#include "CDictionary.h"
#include "CFileLoader.h"
#include "CMMFLoader.h"
#include "TString.h"

// �ʱ� �ܾ� ����� ���⼭ �߰�
CDictionary::CDictionary()
{
	m_WordList.clear();
	m_WordList.reserve(50); // �̸� 50ĭ ���� Ȯ��

#ifdef MODE_MMF
	CMMFLoader fileLoader(TEXT("word.txt"), MAX_DICT_SIZE);
#else
	CFileLoader fileLoader(TEXT("word.txt"), MAX_DICT_SIZE);
#endif
	m_pLoadData = fileLoader.Load();
	Parse();
}

CDictionary::~CDictionary()
{
	_tprintf(TEXT("�ܾ� ��� �Ҵ� ����! \n"));
	for (stWord *word : m_WordList)
	{
		if (word != nullptr)
			delete word;
	}

#ifdef MODE_MMF
	if (m_pLoadData != nullptr)
	{
		UnmapViewOfFile(m_pLoadData);
		m_pLoadData = nullptr;
	}
#else
	if (m_pLoadData != nullptr)
	{
		delete m_pLoadData;
		m_pLoadData = nullptr;
	}
#endif
}

// ���α׷��� ����� �� �ڵ����� �Ҹ�ȴ�. (�Ҹ��ڵ� ȣ��)
CDictionary *CDictionary::GetInstance()
{
	static CDictionary instance;
	return &instance;
}

const TCHAR *CDictionary::SearchKor(TCHAR *eng_)
{
	const TCHAR *result = nullptr;
	for (stWord *word : m_WordList)
	{
		if (tcscmp(word->m_Eng, eng_) == 0)
		{
			result = word->m_Kor;
			break;
		}
	}
	
	return result;
}

const TCHAR *CDictionary::SearchEng(TCHAR *kor_)
{
	const TCHAR *result = nullptr;
	for (stWord *word : m_WordList)
	{
		if (tcscmp(word->m_Kor, kor_) == 0)
		{
			result = word->m_Eng;
			break;
		}
	}

	return result;
}

// ���� ������ ����, �ѱ�\n
void CDictionary::Parse()
{
	if (m_pLoadData == nullptr)
	{
		_tprintf(TEXT("CDictionary::Parse() �Ľ��� �����Ͱ� �����ϴ�.\n"));
		return;
	}

	TCHAR *token = nullptr;
	const TCHAR *seps = TEXT(" ,\r\n");
	TCHAR *word1 = tcstok(m_pLoadData + 1, seps, &token);
	TCHAR *word2 = tcstok(NULL, seps, &token);
	while (word1 != nullptr && word2 != nullptr)
	{
		_tprintf(TEXT("Parse Data : %s, %s\n"), word1, word2);
		AddWord(word1, word2);
		word1 = tcstok(NULL, seps, &token);
		word2 = tcstok(NULL, seps, &token);
	}
}

void CDictionary::AddWord(TCHAR *eng_, TCHAR *kor_)
{
	stWord *newWord = new stWord;
	newWord->m_Eng = eng_;
	newWord->m_Kor = kor_;
	m_WordList.push_back(newWord);
}