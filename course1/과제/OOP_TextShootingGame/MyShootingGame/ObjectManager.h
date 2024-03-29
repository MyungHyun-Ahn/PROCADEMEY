#pragma once
class ObjectManager
{
	SINGLE(ObjectManager)

public:
	void CreateEnemy(stPos pos, stEnemyInfo *info);
	void CreatePlayer();
	void CreatePlayerMissile(const stPos &curDir, const stPos &startPos);
	void CreateEnemyMissile(const stPos &startPos, const stMissileInfo &info);

	void Update();
	void Render();

	void GarbageCollector();
	// 게임이 진행 중이라면 true를 전달하여 UI와 Player가 삭제되지 않도록 한다.
	void ResetObject(bool isGameRunning = false);

private:
	void Collision();

private:
	List<BaseObject *> m_ObjList;
};

extern ObjectManager *g_ObjMgr;