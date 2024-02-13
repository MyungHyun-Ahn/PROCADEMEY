#pragma once
// FileLoader
// Scene, Enemy 등을 불러오기 위함


// Scene Parse
void FileSceneParse(const char *filePath);
void FileStageParse(const char *stageName);

// Stage Load
void FileStageInfoParse(const char *filePath);

// Enemy Parse
void FileEnemyParse(const char enemyName);