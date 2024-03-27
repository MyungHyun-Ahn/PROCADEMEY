#pragma once
class FileManager
{
	SINGLE(FileManager)

public:
	char *SceneLoad(const char *filePath);
	stStageInfo *StageInfoParse(const char *filePath, int &stageCount);
	char *StageLoad(const char *stageName);
	stEnemyInfo *EnemyParse(const char enemyName);

private:
	char *Load(const char *filePath);
	
private:
	std::map<char, stEnemyInfo *> m_mapEnemyInfos;
};

extern FileManager *g_FileMgr;