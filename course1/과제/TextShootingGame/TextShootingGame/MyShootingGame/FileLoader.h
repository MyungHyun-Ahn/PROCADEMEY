#pragma once
// FileLoader
// Scene, Enemy ���� �ҷ����� ����

// ���� �б�
char *FileLoad(const char *filePath);

// Scene Parse
void FileSceneParse(char *buffer);

// Stage Load
void FileStageInfoParse(const char *filePath);

void FileEnemyParse(const char enemyName);