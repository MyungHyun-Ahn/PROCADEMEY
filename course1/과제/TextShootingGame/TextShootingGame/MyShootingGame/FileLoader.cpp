#include "pch.h"
#include "FileLoader.h"
#include "Console.h"
#include "SceneManager.h"
#include "StageInfo.h"
#include "Enemy.h"
#include "Missile.h"

#include <conio.h>

// ���� ���� �б� - �ܺο��� ���� ȣ���ϴ� ���� ����. (������� ���� ����)
char *FileLoad(const char *filePath)
{
    FILE *loadFile;
    errno_t err = fopen_s(&loadFile, filePath, "rb");
    if (err != 0 || loadFile == nullptr)
    {
        // error ��Ȳ
        return nullptr;
    }

    fseek(loadFile, 0, SEEK_END);
    int fileSize = ftell(loadFile);
    fseek(loadFile, 0, SEEK_SET);

    int loadBufferSize = sizeof(char) * (fileSize + 1);
    char *loadBuffer = (char *)malloc(loadBufferSize);
    if (loadBuffer == nullptr)
    {
        // error ��Ȳ
        return nullptr;
    }
    
    fread_s(loadBuffer, loadBufferSize, sizeof(char), fileSize, loadFile);
    loadBuffer[loadBufferSize - 1] = '\0';

    fclose(loadFile);
    return loadBuffer;
}

void FileSceneParse(const char *filePath)
{
    char *buffer = FileLoad(filePath);
    const char *del = "\r\n";
    char *nextTok = nullptr;
    char *token = strtok_s(buffer, del, &nextTok);

    int colNum = 0;
    while (token != nullptr)
    {
        memcpy_s(g_curScene.m_szConsoleBuffer[colNum++], dfSCREEN_WIDTH, token, dfSCREEN_WIDTH);
        token = strtok_s(nullptr, del, &nextTok);
    }

    if (buffer != nullptr)
    {
		free(buffer);
        buffer = nullptr;
    }
}

void FileStageParse(const char *stageName)
{
	char stagePath[MAX_FILE_NAME] = "Resources\\Stage\\";

	strcat_s(stagePath, stageName);

    FileSceneParse(stagePath);
}

void FileStageInfoParse(const char *filePath)
{
    char *stageInfoBuffer = FileLoad(filePath);
    if (stageInfoBuffer == nullptr)
    {
        // ������ ��Ȳ
        return;
    }
    const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(stageInfoBuffer, del, &nextTok);

    // Stage ���� �Ľ�
    g_iStageCount = atoi(token);
    g_StageInfos = (stStageInfo *)malloc(sizeof(stStageInfo) * g_iStageCount);
    if (g_StageInfos == nullptr)
    {
        // ������ ��Ȳ
        return;
    }

    token = strtok_s(nullptr, del, &nextTok);
    int stageIndex = 0;
    for (int i = 0; i < g_iStageCount; i++)
    {
		// ���� �̸�
		size_t fileNameSize = (strlen(token) + 1) * sizeof(char); // NULL ���� �ڸ� ����
		g_StageInfos[stageIndex].m_chFileName = (char *)malloc(fileNameSize);
		if (g_StageInfos[stageIndex].m_chFileName == nullptr)
		{
			return;
		}
		memcpy_s(g_StageInfos[stageIndex].m_chFileName, fileNameSize, token, fileNameSize);


		token = strtok_s(nullptr, del, &nextTok);
		g_StageInfos[stageIndex].m_iEnemyCount = atoi(token);

		token = strtok_s(nullptr, del, &nextTok);
		g_StageInfos[stageIndex].m_iEnemyTypeCount = atoi(token);

		token = strtok_s(nullptr, del, &nextTok);
		size_t enemySize = strlen(token) * sizeof(char) + 1;
        g_StageInfos[stageIndex].m_arrEnemys = (char *)malloc(enemySize);
		memcpy_s(g_StageInfos[stageIndex].m_arrEnemys, enemySize, token, enemySize);
		token = strtok_s(nullptr, del, &nextTok);
        stageIndex++;
    }

    if (stageInfoBuffer != nullptr)
        free(stageInfoBuffer);
}

void FileEnemyParse(const char enemyName)
{
	if (g_mapEnemyInfos.find(enemyName) != g_mapEnemyInfos.end())
	{
		// �̹� �ε��� ��
		return;
	}

    char enemyPath[MAX_FILE_NAME] = "Resources\\Enemy\\";
    enemyPath[16] = enemyName;
    enemyPath[17] = '\0';

    strcat_s(enemyPath, ".txt");

    char *enemyBuffer = FileLoad(enemyPath);
    if (enemyBuffer == nullptr)
    {
        // ���� ��Ȳ
        return;
    }

	const char *del = ",\r\n ";
	char *nextTok = nullptr;
	char *token = strtok_s(enemyBuffer, del, &nextTok);
    if (strcmp(token, "SHAPE") != 0)
    {
		if (enemyBuffer != nullptr)
		{
			free(enemyBuffer);
			enemyBuffer = nullptr;
		}
        return;
    }
    stEnemyInfo *newEnemy = (stEnemyInfo *)malloc(sizeof(stEnemyInfo));
    if (newEnemy == nullptr)
        return;

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_chShape = token[0]; // ����

    // Stats
    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "STATS") != 0)
	{
		if (enemyBuffer != nullptr)
		{
			free(enemyBuffer);
			enemyBuffer = nullptr;
		}
		return;
	}

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_iHp = atoi(token);

	token = strtok_s(NULL, del, &nextTok);
	newEnemy->m_iSpeed = atoi(token);

    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MISSILE_TYPE") != 0)
	{
		if (enemyBuffer != nullptr)
		{
			free(enemyBuffer);
			enemyBuffer = nullptr;
		}
		return;
	}

    token = strtok_s(NULL, del, &nextTok);
    newEnemy->m_iMissileCount = atoi(token);
    newEnemy->m_MissileInfos = (stMissileInfo *)malloc(sizeof(stMissileInfo) * newEnemy->m_iMissileCount);
    if (newEnemy->m_MissileInfos == nullptr)
        return;


    // �̻��� �Ľ�
    // �̻��� ���� map�� �����ϰ� ������ key ���� �ָ��ؼ� �н�
    for (int i = 0; i < newEnemy->m_iMissileCount; i++)
    {
        // �̻��� ���� ���
        token = strtok_s(NULL, del, &nextTok);
        memcpy_s(newEnemy->m_MissileInfos[i].m_szFileName, MAX_FILE_NAME, "Resources\\Missile\\", 19);
        strcat_s(newEnemy->m_MissileInfos[i].m_szFileName, token);

        // ���� ��� ��ǥ
        token = strtok_s(NULL, del, &nextTok);
        int y = atoi(token);
		token = strtok_s(NULL, del, &nextTok);
		int x = atoi(token);
        newEnemy->m_MissileInfos[i].m_iStartPos = stPos{ y, x };

        // ù �߻� Ÿ�̹�
        token = strtok_s(NULL, del, &nextTok);
        newEnemy->m_MissileInfos[i].m_iFirstTime = atoi(token);

        // ��Ÿ��
        token = strtok_s(NULL, del, &nextTok);
        newEnemy->m_MissileInfos[i].m_iCooltime = atoi(token);

        // ������
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iDamage = atoi(token);

		// ���ǵ�
		token = strtok_s(NULL, del, &nextTok);
		newEnemy->m_MissileInfos[i].m_iSpeed = atoi(token);

        // �̻��� ���� �Ľ�
        char *missileBuffer = FileLoad(newEnemy->m_MissileInfos[i].m_szFileName);
        if (missileBuffer == nullptr)
            return;

		char *nextTok2 = nullptr;
		char *token2 = strtok_s(missileBuffer, del, &nextTok2);

		if (strcmp(token2, "SHAPE") != 0)
		{
			if (missileBuffer != nullptr)
			{
				free(missileBuffer);
                missileBuffer = nullptr;
			}
			return;
		}

        token2 = strtok_s(NULL, del, &nextTok2);
        newEnemy->m_MissileInfos[i].m_chShape = token2[0];

        token2 = strtok_s(NULL, del, &nextTok2);
		if (strcmp(token2, "LIFETIME") != 0)
		{
			if (missileBuffer != nullptr)
			{
				free(missileBuffer);
                missileBuffer = nullptr;
			}
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
			if (missileBuffer != nullptr)
			{
				free(missileBuffer);
                missileBuffer = nullptr;
			}
			return;
		}

        token2 = strtok_s(NULL, del, &nextTok2);

        newEnemy->m_MissileInfos[i].m_iMissileMaxMoveIndex = atoi(token2);

        newEnemy->m_MissileInfos[i].m_MissileMoves = (stPos *)malloc(sizeof(stPos) * newEnemy->m_MissileInfos[i].m_iMissileMaxMoveIndex);
        if (newEnemy->m_MissileInfos[i].m_MissileMoves == nullptr)
            return;


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

        if (missileBuffer != nullptr)
            free(missileBuffer);
    }

    token = strtok_s(NULL, del, &nextTok);
	if (strcmp(token, "MOVE_PATTERN") != 0)
	{
		if (enemyBuffer != nullptr)
		{
			free(enemyBuffer);
			enemyBuffer = nullptr;
		}
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
    if (enemyBuffer != nullptr)
    {
        free(enemyBuffer);
        enemyBuffer = nullptr;
    }
}
