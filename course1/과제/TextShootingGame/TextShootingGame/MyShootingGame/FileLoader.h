#pragma once
// FileLoader
// Scene, Enemy ���� �ҷ����� ����


// Scene Parse
void FileSceneParse(const char *filePath);
void FileStageParse(const char *stageName);

// Stage Load
void FileStageInfoParse(const char *filePath);

// Enemy Parse
void FileEnemyParse(const char enemyName);