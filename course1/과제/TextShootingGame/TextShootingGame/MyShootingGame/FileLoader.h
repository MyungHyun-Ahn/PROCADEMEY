#pragma once
// FileLoader
// Scene, Enemy 등을 불러오기 위함

// 파일 읽기
char *FileLoad(const char *filePath);

// Scene Parse
void FileSceneParse(char *buffer);

// Stage Load
void FileStageInfoParse(const char *filePath);

void FileEnemyParse(const char enemyName);