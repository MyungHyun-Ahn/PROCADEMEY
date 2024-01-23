#pragma once

size_t		tcslen(const TCHAR *str);

TCHAR		*tcscat(TCHAR *dest, const TCHAR *source);

TCHAR		*tcstok(TCHAR *str, const TCHAR *delimiters, TCHAR **context);

int			tcscmp(const TCHAR *str1, const TCHAR *str2);