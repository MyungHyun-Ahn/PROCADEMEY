#pragma once

struct stWord
{
	const TCHAR *m_Eng;
	const TCHAR *m_Kor;
};

class CDictionary
{
private:
	CDictionary();
	~CDictionary();

public:
	static CDictionary *GetInstance();

	const TCHAR *SearchKor(TCHAR *eng_);
	const TCHAR *SearchEng(TCHAR *kor_);

private:
	void Parse();
	void AddWord(TCHAR *eng_, TCHAR *kor_);

private:
	std::vector<stWord *> m_WordList;
	TCHAR *m_pLoadData = nullptr;
};

