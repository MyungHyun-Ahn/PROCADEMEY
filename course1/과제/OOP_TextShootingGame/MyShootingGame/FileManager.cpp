#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "BaseObject.h"
#include "Enemy.h"
#include "Missile.h"
#include "Scene.h"
#include "GameScene.h"
#include "FileManager.h"

FileManager *g_FileMgr;

FileManager::FileManager() {}
FileManager::~FileManager() {}

char *FileManager::SceneLoad(const char *filePath)
{
	return Load(filePath);
}

stStageInfo *FileManager::StageInfoParse(const char *filePath, int &stageCount)
{
	char *stageInfoBuffer = Load(filePath);
	if (stageInfoBuffer == nullptr)
	{
		// 에러인 상황
		return nullptr;
	}
	const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(stageInfoBuffer, del, &nextTok);

	// Stage 개수 파싱
	stageCount = atoi(token);
	stStageInfo *stageInfo = new stStageInfo[stageCount];

	token = strtok_s(nullptr, del, &nextTok);
	int stageIndex = 0;
	for (int i = 0; i < stageCount; i++)
	{
		// 파일 이름
		size_t fileNameSize = (strlen(token) + 1) * sizeof(char); // NULL 문자 자리 포함
		stageInfo[stageIndex].m_szFileName = new char[fileNameSize];
		memcpy_s(stageInfo[stageIndex].m_szFileName, fileNameSize, token, fileNameSize);


		token = strtok_s(nullptr, del, &nextTok);
		stageInfo[stageIndex].m_iEnemyCount = atoi(token);

		token = strtok_s(nullptr, del, &nextTok);
		stageInfo[stageIndex].m_iEnemyTypeCount = atoi(token);

		token = strtok_s(nullptr, del, &nextTok);
		size_t enemySize = strlen(token) * sizeof(char) + 1;
		stageInfo[stageIndex].m_arrEnemys = new char[enemySize];
		memcpy_s(stageInfo[stageIndex].m_arrEnemys, enemySize, token, enemySize);
		token = strtok_s(nullptr, del, &nextTok);
		stageIndex++;
	}

	delete stageInfoBuffer;

	return stageInfo;
}

char *FileManager::StageLoad(const char *stageName)
{
	char stagePath[MAX_FILE_NAME] = "Resources\\Stage\\";
	strcat_s(stagePath, stageName);
	return SceneLoad(stagePath);
}

stEnemyInfo *FileManager::EnemyParse(const char enemyName)
{
	auto enemyInfo = m_mapEnemyInfos.find(enemyName);

	if (enemyInfo != m_mapEnemyInfos.end())
	{
		// 이미 로드한 것
		return enemyInfo->second;
	}

	char enemyPath[MAX_FILE_NAME] = "Resources\\Enemy\\";
	enemyPath[16] = enemyName;
	enemyPath[17] = '\0';

	strcat_s(enemyPath, ".txt");

	char *enemyBuffer = Load(enemyPath);
	if (enemyBuffer == nullptr)
	{
		// 에러 상황
		return nullptr;
	}

	const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(enemyBuffer, del, &nextTok);
	if (strcmp(token, "SHAPE") != 0)
	{
		delete enemyBuffer;
		return nullptr;
	}
	stEnemyInfo *newEnemy = new stEnemyInfo;
	if (newEnemy == nullptr)
		return nullptr;

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_chShape = token[0]; // 복사

	// Stats
	token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "STATS") != 0)
	{
		delete enemyBuffer;
		return nullptr;
	}

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_iHp = atoi(token);

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_iSpeed = atoi(token);

	token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MISSILE_TYPE") != 0)
	{
		delete enemyBuffer;
		return nullptr;
	}

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_iMissileCount = atoi(token);
	newEnemy->m_MissileInfos = new stMissileInfo[newEnemy->m_iMissileCount];


	// 미사일 파싱
	// 미사일 또한 map에 저장하고 싶지만 key 값이 애매해서 패스
	for (int i = 0; i < newEnemy->m_iMissileCount; i++)
	{
		// 미사일 파일 경로
		token = strtok_s(NULL, del, &nextTok);
		memcpy_s(newEnemy->m_MissileInfos[i].m_szFileName, MAX_FILE_NAME, "Resources\\Missile\\", 19);
		strcat_s(newEnemy->m_MissileInfos[i].m_szFileName, token);

		// 시작 상대 좌표
		token = strtok_s(NULL, del, &nextTok);
		int y = atoi(token);
		token = strtok_s(NULL, del, &nextTok);
		int x = atoi(token);
		newEnemy->m_MissileInfos[i].m_iStartPos = stPos{ y, x };

		// 첫 발사 타이밍
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iFirstTime = atoi(token);

		// 쿨타임
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iCooltime = atoi(token);

		// 데미지
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iDamage = atoi(token);

		// 스피드
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iSpeed = atoi(token);

		// 미사일 정보 파싱
		char *missileBuffer = Load(newEnemy->m_MissileInfos[i].m_szFileName);
		if (missileBuffer == nullptr)
			return nullptr;

		char *nextTok2 = nullptr;
		char *token2 = strtok_s(missileBuffer, del, &nextTok2);

		if (strcmp(token2, "SHAPE") != 0)
		{
			delete missileBuffer;
			return nullptr;
		}

		token2 = strtok_s(NULL, del, &nextTok2);
		newEnemy->m_MissileInfos[i].m_chShape = token2[0];

		token2 = strtok_s(NULL, del, &nextTok2);
		if (strcmp(token2, "LIFETIME") != 0)
		{
			delete missileBuffer;
			return nullptr;
		}

		token2 = strtok_s(NULL, del, &nextTok2);
		int lifetime = atoi(token2);
		if (lifetime == 0)
			lifetime = INT32_MAX;

		newEnemy->m_MissileInfos[i].m_iLifetime = lifetime;

		token2 = strtok_s(NULL, del, &nextTok2);
		if (strcmp(token2, "PATTERN") != 0)
		{
			delete missileBuffer;
			return nullptr;
		}

		token2 = strtok_s(NULL, del, &nextTok2);

		newEnemy->m_MissileInfos[i].m_iMissileMaxMoveIndex = atoi(token2);

		newEnemy->m_MissileInfos[i].m_MissileMoves = new stPos[newEnemy->m_MissileInfos[i].m_iMissileMaxMoveIndex];

		for (int j = 0; j < newEnemy->m_MissileInfos[i].m_iMissileMaxMoveIndex; j++)
		{
			token2 = strtok_s(NULL, del, &nextTok2);
			int my = atoi(token2);
			token2 = strtok_s(NULL, del, &nextTok2);
			int mx = atoi(token2);
			newEnemy->m_MissileInfos[i].m_MissileMoves[j] = stPos{ my, mx };

			// printf("%d %d \n", mx, my);
			// _getch();
		}

		delete missileBuffer;
	}

	token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MOVE_PATTERN") != 0)
	{
		delete enemyBuffer;
		return nullptr;
	}

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_iMovesCount = atoi(token);

	newEnemy->m_Moves = new stPos[newEnemy->m_iMovesCount];

	for (int i = 0; i < newEnemy->m_iMovesCount; i++)
	{
		token = strtok_s(NULL, del, &nextTok);
		int my = atoi(token);
		token = strtok_s(NULL, del, &nextTok);
		int mx = atoi(token);

		newEnemy->m_Moves[i] = stPos{ my, mx };
	}

	m_mapEnemyInfos.insert(std::pair<char, stEnemyInfo *>(enemyName, newEnemy));
	delete enemyBuffer;
	return newEnemy;
}

char *FileManager::Load(const char *filePath)
{
	FILE *loadFile;
	errno_t err = fopen_s(&loadFile, filePath, "rb");
	if (err != 0 || loadFile == nullptr)
	{
		// error 상황
		return nullptr;
	}

	fseek(loadFile, 0, SEEK_END);
	int fileSize = ftell(loadFile);
	fseek(loadFile, 0, SEEK_SET);

	int loadBufferSize = sizeof(char) * (fileSize + 1);
	char *loadBuffer = new char[loadBufferSize];

	fread_s(loadBuffer, loadBufferSize, sizeof(char), fileSize, loadFile);
	loadBuffer[loadBufferSize - 1] = '\0';

	fclose(loadFile);
	return loadBuffer;
}

void FileManager::ResetEnemyInfo()
{
	for (auto it = m_mapEnemyInfos.begin(); it != m_mapEnemyInfos.end(); ++it)
	{
		stEnemyInfo *enemyInfo = it->second;

		if (enemyInfo != nullptr)
		{
			if (enemyInfo->m_MissileInfos != nullptr)
			{
				for (int i = 0; i < enemyInfo->m_iMissileCount; i++)
				{
					if (enemyInfo->m_MissileInfos[i].m_MissileMoves != nullptr)
					{
						free(enemyInfo->m_MissileInfos[i].m_MissileMoves);
						enemyInfo->m_MissileInfos[i].m_MissileMoves = nullptr;
					}
				}

				free(enemyInfo->m_MissileInfos);
				enemyInfo->m_MissileInfos = nullptr;
			}

			if (enemyInfo->m_Moves != nullptr)
			{
				free(enemyInfo->m_Moves);
				enemyInfo->m_Moves = nullptr;
			}

			free(enemyInfo);
			enemyInfo = nullptr;
		}
	}

	m_mapEnemyInfos.clear();
}