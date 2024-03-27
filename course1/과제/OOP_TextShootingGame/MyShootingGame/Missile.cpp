#include "pch.h"
#include "GameSetting.h"
#include "BaseObject.h"
#include "Missile.h"
#include "Enemy.h"
#include "Player.h"
#include "ConsoleManager.h"

Missile::Missile(bool isEnemy, const stPos &startPos, const stMissileInfo &info)
	: BaseObject(ObjectType::Missile, { startPos.m_iY + info.m_iStartPos.m_iY , startPos.m_iX + info.m_iStartPos.m_iX })
	, m_bIsEnemy(isEnemy)
	, m_chShape(info.m_chShape)
	, m_iDamage(info.m_iDamage)
	, m_iSpeed(info.m_iSpeed)
	, m_iCurLife(0)
	, m_iMaxLife(info.m_iLifetime)
	, m_iDirIndex(0)
	, m_dDirIndex(0.)
	, m_iDirSize(info.m_iMissileMaxMoveIndex)
	, m_arrDir(info.m_MissileMoves)
{
}

Missile::Missile(bool isEnemy, const stPos &curDir, const stPos &startPos)
	: BaseObject(ObjectType::Missile, { startPos.m_iY + curDir.m_iY , startPos.m_iX + curDir.m_iX })
	, m_bIsEnemy(isEnemy)
	, m_chShape(dfPLAYER_MISSILE_SHAPE)
	, m_iDamage(dfPLAYER_DAMAGE)
	, m_iSpeed(dfPLAYER_MISSILE_SPEED)
	, m_iCurLife(0)
	, m_iMaxLife(INT32_MAX)
	, m_iDirIndex(0)
	, m_dDirIndex(0.)
	, m_iDirSize(1)
	, m_DirBak(curDir)
	, m_arrDir(&m_DirBak)
{

}

Missile::~Missile()
{
}

void Missile::Update()
{
	Move();
}

void Missile::Render()
{
	g_ConsoleMgr->SpriteDraw(m_Pos, m_chShape);
}

void Missile::Move()
{
	m_dDirIndex += m_iSpeed * dfFIXED_DELTATIME;
	// m_dDirIndex를 int 로 캐스팅한게 m_iDirIndex 보다 커지면
	// 다음 칸으로 이동 가능한 것
	if ((int)m_dDirIndex < m_iDirIndex)
		return;

	// index 가 1부터 시작하게 됨
	m_iDirIndex++;

	int index = (m_iDirIndex - 1) % m_iDirSize;

	stPos &nextDir = m_arrDir[index];
	m_Pos.m_iY += nextDir.m_iY;
	m_Pos.m_iX += nextDir.m_iX;

	m_iCurLife++;

	// 생명 프레임 초과
	if (m_iCurLife > m_iMaxLife)
	{
		m_bIsActive = false;
	}

	// 화면 밖으로 벗어난 경우 비활성화
	if (m_Pos.m_iY > dfSCREEN_HEIGHT || m_Pos.m_iX > dfSCREEN_WIDTH - 1)
	{
		m_bIsActive = false;
	}

	if (m_Pos.m_iY < 0 || m_Pos.m_iX < 0)
	{
		m_bIsActive = false;
	}
}

void Missile::Collision(BaseObject *obj)
{
	if (!m_bIsActive)
		return;

	if (!CheckCollision(obj->GetPos()))
		return;

	// 여기까지 코드가 진행되면 충돌한 경우
	switch (obj->GetObjectType())
	{
	case ObjectType::Enemy:
	{
		Enemy *enemy = static_cast<Enemy *>(obj);
		enemy->GetDamage(m_iDamage);
		m_bIsActive = false;
	}
		break;

	case ObjectType::Player:
	{
		Player *player = static_cast<Player *>(obj);
		player->GetDamage(m_iDamage);
		m_bIsActive = false;
	}
		break;
	}
}

bool Missile::CheckCollision(stPos &targetPos)
{
	if ((m_Pos.m_iY == targetPos.m_iY && m_Pos.m_iX == targetPos.m_iX))
		return true;

	return false;
}