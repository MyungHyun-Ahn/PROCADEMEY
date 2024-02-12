#include "pch.h"
#include "FileLoader.h"
#include "Console.h"
#include "SceneManager.h"
#include "StageInfo.h"
#include "Enemy.h"
#include "Missile.h"

char *FileLoad(const char *filePath)
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

    char *loadBuffer = (char *)malloc((sizeof(char)) * fileSize);
    if (loadBuffer == nullptr)
    {
        // error 상황
        return nullptr;
    }
    
    fread_s(loadBuffer, sizeof(char) * fileSize, sizeof(char), fileSize, loadFile);
    return loadBuffer;
}

void FileSceneParse(char *buffer)
{
    const char *del = "\r\n";
    char *nextTok = nullptr;
    char *token = strtok_s(buffer, del, &nextTok);

    int colNum = 0;
    while (token != nullptr)
    {
        memcpy_s(g_curScene.m_szConsoleBuffer[colNum++], dfSCREEN_WIDTH, token, dfSCREEN_WIDTH);
        token = strtok_s(nullptr, del, &nextTok);
    }
}

void FileStageInfoParse(const char *filePath)
{
    char *stageInfoBuffer = FileLoad(filePath);
    if (stageInfoBuffer == nullptr)
    {
        // 에러인 상황
        return;
    }
    const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(stageInfoBuffer, del, &nextTok);

    // Stage 개수 파싱
    g_iStageCount = atoi(token);
    g_StageInfos = (stStageInfo *)malloc(sizeof(stStageInfo) * g_iStageCount);
    if (g_StageInfos == nullptr)
    {
        // 에러인 상황
        return;
    }

    token = strtok_s(nullptr, del, &nextTok);
    int stageIndex = 0;
    for (int i = 0; i < g_iStageCount; i++)
    {
		// 파일 이름
		size_t fileNameSize = (strlen(token) + 1) * sizeof(char); // NULL 문자 자리 포함
		g_StageInfos[stageIndex].m_chFileName = (char *)malloc(fileNameSize);
		if (g_StageInfos[stageIndex].m_chFileName == nullptr)
		{
			return;
		}
		memcpy_s(g_StageInfos[stageIndex].m_chFileName, fileNameSize, token, fileNameSize);


		token = strtok_s(nullptr, del, &nextTok);
		g_StageInfos[stageIndex].m_iEnemyCount = atoi(token);

		g_StageInfos[stageIndex].m_arrEnemys = (char *)malloc(sizeof(char) * g_StageInfos[stageIndex].m_iEnemyCount + 1);

		token = strtok_s(nullptr, del, &nextTok);
		size_t enemySize = strlen(token) * sizeof(char) + 1;
		memcpy_s(g_StageInfos[stageIndex].m_arrEnemys, enemySize, token, enemySize);
		// g_StageInfos[stageIndex].m_arrEnemys[(sizeof(wchar_t) + 1) * g_StageInfos[stageIndex].m_iEnemyCount] = (wchar_t)NULL;
		token = strtok_s(nullptr, del, &nextTok);
        stageIndex++;
    }
}

void FileEnemyParse(const char enemyName)
{
    char enemyPath[MAX_FILE_NAME] = "Resources\\Enemy\\";
    enemyPath[16] = enemyName;
    enemyPath[17] = '\0';

    strcat_s(enemyPath, ".txt");

    if (g_mapEnemyInfos.find(enemyName) != g_mapEnemyInfos.end())
    {
        // 이미 로드한 것
        return;
    }

    char *enemyBuffer = FileLoad(enemyPath);
    if (enemyBuffer == nullptr)
    {
        // 에러 상황
        return;
    }

	const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(enemyBuffer, del, &nextTok);
    if (strcmp(token, "SHAPE") != 0)
    {
        // 잘못된 구분자
        return;
    }
    stEnemyInfo *newEnemy = (stEnemyInfo *)malloc(sizeof(stEnemyInfo));
    if (newEnemy == nullptr)
        return;

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_chShape = token[0]; // 복사

    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "STATS") != 0)
	{
		// 잘못된 구분자
		return;
	}

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_iHp = atoi(token);

    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MISSILE_TYPE") != 0)
	{
		// 잘못된 구분자
		return;
	}

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_iMissileCount = atoi(token);
    newEnemy->m_MissileInfos = (stMissileInfo *)malloc(sizeof(stMissileInfo) * newEnemy->m_iMissileCount);
    if (newEnemy->m_MissileInfos == nullptr)
        return;


    // 미사일 파싱
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

        // 미사일 정보 파싱
        char *missileBuffer = FileLoad(newEnemy->m_MissileInfos[i].m_szFileName);
        if (missileBuffer == nullptr)
            return;

		char *nextTok2 = nullptr;
		char *token2 = strtok_s(missileBuffer, del, &nextTok2);

		if (strcmp(token2, "SHAPE") != 0)
		{
			// 잘못된 구분자
			return;
		}

        token2 = strtok_s(NULL, del, &nextTok2);
        newEnemy->m_MissileInfos[i].m_chShape = token2[0];

        token2 = strtok_s(NULL, del, &nextTok2);
		if (strcmp(token2, "LIFETIME") != 0)
		{
			// 잘못된 구분자
			return;
		}

        token2 = strtok_s(NULL, del, &nextTok2);
        int lifetime = atoi(token2);
        if (lifetime == 0)
            lifetime = INT32_MAX;

        newEnemy->m_MissileInfos[i].m_iLifetime = lifetime;

        token2 = strtok_s(NULL, del, &nextTok2);
		if (strcmp(token2, "PATTERN") != 0)
		{
			// 잘못된 구분자
			return;
		}

        token2 = strtok_s(NULL, del, &nextTok2);

        newEnemy->m_MissileInfos[i].m_iMissileMaxIndex = atoi(token2);

        newEnemy->m_MissileInfos[i].m_iMissileMoves = (stPos *)malloc(sizeof(stPos) * newEnemy->m_MissileInfos[i].m_iMissileMaxIndex);
        if (newEnemy->m_MissileInfos[i].m_iMissileMoves == nullptr)
            return;


        for (int j = 0; j < newEnemy->m_MissileInfos[i].m_iMissileMaxIndex; j++)
        {
            token2 = strtok_s(NULL, del, &nextTok2);
            int my = atoi(token2);
            token2 = strtok_s(NULL, del, &nextTok2);
            int mx = atoi(token2);
            newEnemy->m_MissileInfos[i].m_iMissileMoves[j] = stPos{ my, mx };
        }

        if (missileBuffer != nullptr)
            free(missileBuffer);
    }

    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MOVE_PATTERN") != 0)
	{
		// 잘못된 구분자
		return;
	}

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_iMovesCount = atoi(token);

    newEnemy->m_Moves = (stPos *)malloc(sizeof(stPos) * newEnemy->m_iMovesCount);
    if (newEnemy->m_Moves == nullptr)
        return;

    for (int i = 0; i < newEnemy->m_iMovesCount; i++)
    {
        token = strtok_s(NULL, del, &nextTok);
        int my = atoi(token);
        token = strtok_s(NULL, del, &nextTok);
        int mx = atoi(token);

        newEnemy->m_Moves[i] = stPos{ my, mx };
    }

    g_mapEnemyInfos.insert(std::pair<char, stEnemyInfo*>(enemyName, newEnemy));
    if (enemyBuffer == nullptr)
        free(enemyBuffer);
}
