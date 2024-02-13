#pragma once

struct stMissileInfo;

struct stEnemyInfo
{
	// Shape
	char m_chShape;
	
	// hp
	int m_iHp;

	int m_iMissileCount;
	stMissileInfo *m_MissileInfos;

	int m_iMovesCount;
	stPos *m_Moves;
};

// Enemy 구조체
struct stEnemy
{
	// 생존 여부
	bool				m_bIsActive = false; // 기본값 false

	// 현재 좌표
	stPos				m_stPos;

	// SHAPE
	char				m_chShape;

	// STATS
	int					m_iCurHp;
	int					m_iMaxHp;

	// 미사일 정보 - m_MissileInfos는 할당해제하면 안된다.
	// 외부 map 에 저장한 것을 그냥 가져다 쓸 뿐
	int					m_iMissileCount = 0;
	stMissileInfo		*m_MissileInfos = nullptr;

	// 미사일 쿨타임 정보 - 이것은 Enemy 가 동적할당 해제해야 한다.
	unsigned int					*m_arrMissilePrevTime;

	// 이동 경로
	int					m_iMoveCount;
	int					m_iCurMoveIndex;
	stPos				*m_arrMovePos = nullptr; // 이것 또한 여기서 할당해제하지 않는다.
};

// Stage 로드한 정보대로 Enemy 생성
void EnemyInit(void);

// Enumy 삭제
void EnemyRelease(void);

// Enemy Logic Update
void EnemyUpdate(void);

// Enemy 이동
void EnemyMove(void);

// Enemy 미사일
void EnemyAttack(void);

// Enemy Get Damage
void EnemyGetDamage(int enemyIndex, int damage);

void EnemyClearCheck(void);

// Enemy를 버퍼에 씀
void EnemyRender(void);