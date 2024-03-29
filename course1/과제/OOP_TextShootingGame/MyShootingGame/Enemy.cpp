#include "pch.h"
#include "GameSetting.h"
#include "BaseObject.h"
#include "Missile.h"
#include "TimeManager.h"
#include "ConsoleManager.h"
#include "Enemy.h"
#include "Player.h"
#include "ObjectManager.h"

Enemy::Enemy(stPos pos, stEnemyInfo *info) : BaseObject(ObjectType::Enemy, pos)
{
	Init(info);
}

Enemy::~Enemy()
{
	delete m_arrMissilePrevTime;
}

void Enemy::Init(stEnemyInfo *info)
{
	m_bIsActive = true;							// 활성화
	m_chShape = info->m_chShape;				// Shape
	m_iCurHp = info->m_iHp;						// Hp
	m_iMaxHp = info->m_iHp;						// Hp
	m_iSpeed = info->m_iSpeed;					// Speed

	m_iMissileCount = info->m_iMissileCount;	// 미사일 종류
	m_MissileInfos = info->m_MissileInfos;		// 가져다 쓸 것 - 얕은 복사

	m_arrMissilePrevTime = new unsigned int[info->m_iMissileCount];
	
	// 첫 발사 타이밍을 기록
	for (int i = 0; i < info->m_iMissileCount; i++)
	{
		m_arrMissilePrevTime[i] = info->m_MissileInfos[i].m_iFirstTime - info->m_MissileInfos[i].m_iCooltime;
	}

	// 이동 정보 세팅
	m_iMoveCount = info->m_iMovesCount;
	m_iCurMoveIndex = 0;
	m_dCurMoveIndex = 0.;
	m_arrMovePos = info->m_Moves;
}

void Enemy::Update()
{
	Move();
	Attack();
}

void Enemy::Move()
{
	m_dCurMoveIndex += m_iSpeed * dfFIXED_DELTATIME;

	// m_dDirIndex를 int 로 캐스팅한게 m_iDirIndex 보다 커지면
	// 다음 칸으로 이동 가능한 것
	if ((int)m_dCurMoveIndex < m_iCurMoveIndex)
		return;

	m_iCurMoveIndex++;

	int index = (m_iCurMoveIndex - 1) % m_iMoveCount;

	stPos &nextPos = m_arrMovePos[index];

	m_Pos.m_iY += nextPos.m_iY;
	m_Pos.m_iX += nextPos.m_iX;

	m_Pos.m_iY = max(m_Pos.m_iY, 0);
	m_Pos.m_iY = min(m_Pos.m_iY, dfSCREEN_HEIGHT - 1);
	m_Pos.m_iX = max(m_Pos.m_iX, 0);
	m_Pos.m_iX = min(m_Pos.m_iX, dfSCREEN_WIDTH - 2);

	m_iCurMoveIndex++;
}

void Enemy::Attack()
{
	for (int j = 0; j < m_iMissileCount; j++)
	{
		// 쿨타임이 지나지 않았으면 continue
		if (!g_TimeMgr->CalCoolTime(m_arrMissilePrevTime[j], m_MissileInfos[j].m_iCooltime))
			continue;

		// 미사일 생성
		stMissileInfo &info = m_MissileInfos[j];

		// 미사일 생성
		// MissileEnemyCreate(info, m_Pos);
		g_ObjMgr->CreateEnemyMissile(m_Pos, info);
	}
}

void Enemy::GetDamage(int damage)
{
	m_iCurHp -= damage;
	if (m_iCurHp <= 0)
	{
		// enemy 비활성화
		m_bIsActive = false;
	}
}

void Enemy::Render()
{
	g_ConsoleMgr->SpriteDraw(m_Pos, m_chShape);
}
