#include "pch.h"
#include "Define.h"
#include "GameSetting.h"
#include "BaseObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Missile.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "PrintScene.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "UiManager.h"

ObjectManager *g_ObjMgr;

ObjectManager::ObjectManager() {}
ObjectManager::~ObjectManager() {}

void ObjectManager::CreateEnemy(stPos pos, stEnemyInfo *info)
{
	BaseObject *enemy = new Enemy(pos, info);
	m_ObjList.push_back(enemy);
}

void ObjectManager::CreatePlayer()
{
	BaseObject *player = new Player();
	m_ObjList.push_back(player);
}

void ObjectManager::CreatePlayerMissile(const stPos &curDir, const stPos &startPos)
{
	BaseObject *missile = new Missile(false, curDir, startPos);
	m_ObjList.push_back(missile);
}

void ObjectManager::CreateEnemyMissile(const stPos &startPos, const stMissileInfo &info)
{
	BaseObject *missile = new Missile(true, startPos, info);
	m_ObjList.push_back(missile);
}

void ObjectManager::Update()
{
	for (auto obj : m_ObjList)
	{
		obj->Update();
	}

	Collision();
	GarbageCollector();
}

void ObjectManager::Render()
{
	for (auto obj : m_ObjList)
	{
		obj->Render();
	}
}



void ObjectManager::GarbageCollector()
{
	for (auto it = m_ObjList.begin(); it != m_ObjList.end();)
	{
		BaseObject *baseObj = *it;
		if (baseObj->IsActive())
		{
			++it;
			continue;
		}

		// ��Ȱ�� ��ü ����
		if (baseObj->GetObjectType() == ObjectType::Player)
		{
			// ���� ���� ó��
			ResetObject(false);
			g_SceneMgr->LoadScene(SCENE_CODE::GAMEOVER);
			return;
		}
		else if (baseObj->GetObjectType() == ObjectType::Enemy)
		{
			// Scene���� Enemy Count ����
			int curEnemyCount = g_SceneMgr->DecreaseEnemy();
			g_UiMgr->SendCurEnemyCountData(curEnemyCount);
			if (curEnemyCount <= 0)
			{
				ResetObject(true);
				g_SceneMgr->LoadScene(SCENE_CODE::GAME);
				return;
			}
		}

		delete baseObj;
		it = m_ObjList.erase(it);
	}
}

void ObjectManager::ResetObject(bool isGameRunning)
{
	if (!isGameRunning)
	{
		for (BaseObject *baseObj : m_ObjList)
		{
			delete baseObj;
		}

		m_ObjList.clear();
	}
	else
	{
		for (auto it = m_ObjList.begin(); it != m_ObjList.end();)
		{
			if ((*it)->GetObjectType() == ObjectType::Player)
			{
				Player *player = static_cast<Player *>(*it);
				player->CooltimeReset();
				++it;
				continue;
			}

			if ((*it)->GetObjectType() == ObjectType::UI)
			{
				++it;
				continue;
			}

			delete (*it);
			it = m_ObjList.erase(it);
		}
	}
}

void ObjectManager::Collision()
{
	for (auto it1 = m_ObjList.begin(); it1 != m_ObjList.end(); ++it1)
	{
		// �̻����� �ƴ϶�� �������� ����
		if ((*it1)->GetObjectType() != ObjectType::Missile)
			continue;

		for (auto it2 = m_ObjList.begin(); it2 != m_ObjList.end(); ++it2)
		{
			// Player�� Enemy�� �ƴ϶�� PASS
			if ((*it2)->GetObjectType() == ObjectType::Missile &&
				(*it2)->GetObjectType() == ObjectType::UI &&
				(*it2)->GetObjectType() == ObjectType::ETC)
				continue;
			
			Missile *missile = static_cast<Missile *>((*it1));

			// ���� ����, ��� ������Ʈ�� Player���
			if (missile->isEnemyMissile() && (*it2)->GetObjectType() == ObjectType::Player)
			{
				missile->Collision((*it2)); // �浹 �Ǵ� ����
				continue;
			}

			if (!missile->isEnemyMissile() && (*it2)->GetObjectType() == ObjectType::Enemy)
			{
				missile->Collision((*it2));
				continue;
			}
		}
	}
}
