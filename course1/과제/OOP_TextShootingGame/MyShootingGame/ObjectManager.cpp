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

void ObjectManager::CreateEnemyMissile()
{
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
			g_SceneMgr->LoadScene(SCENE_CODE::GAMEOVER);
			ResetObject(false);
			return;
		}
		else if (baseObj->GetObjectType() == ObjectType::Enemy)
		{
			// Scene���� Enemy Count ����
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
			if ((*it1)->GetObjectType() == ObjectType::Missile &&
				(*it1)->GetObjectType() == ObjectType::UI &&
				(*it1)->GetObjectType() == ObjectType::ETC)
				continue;
			
			Missile *missile = static_cast<Missile *>((*it1));
			missile->Collision((*it2)); // �浹 �Ǵ� ����
		}
	}
}
