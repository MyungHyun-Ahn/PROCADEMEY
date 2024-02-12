#pragma once
// FileLoader
// Scene, Enemy 등을 불러오기 위함

// 파일 읽기
wchar_t *FileLoad(const wchar_t *filePath);

// Scene Parse
void FileSceneParse(wchar_t *buffer);