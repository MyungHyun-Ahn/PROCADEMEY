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
	// ������ ���� ���̶�� true�� �����Ͽ� UI�� Player�� �������� �ʵ��� �Ѵ�.
	void ResetObject(bool isGameRunning = false);

private:
	void Collision();

private:
	List<BaseObject *> m_ObjList;
};

extern ObjectManager *g_ObjMgr;