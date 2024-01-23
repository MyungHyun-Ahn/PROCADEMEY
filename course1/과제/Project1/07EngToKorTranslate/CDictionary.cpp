#include "pch.h"
#include "CDictionary.h"
#include "CFileLoader.h"
#include "CMMFLoader.h"
#include "TString.h"

// 초기 단어 목록은 여기서 추가
CDictionary::CDictionary()
{
	m_WordList.clear();
	m_WordList.reserve(50); // 미리 50칸 정도 확보

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
	_tprintf(TEXT("단어 목록 할당 해제! \n"));
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

// 프로그램이 종료될 때 자동으로 소멸된다. (소멸자도 호출)
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

// 파일 형식은 영어, 한글\n
void CDictionary::Parse()
{
	if (m_pLoadData == nullptr)
	{
		_tprintf(TEXT("CDictionary::Parse() 파싱할 데이터가 없습니다.\n"));
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