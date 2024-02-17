#pragma once

// 미사일 정보 구조체
struct stMissileInfo
{
	// MISSILE 정보
	char				m_szFileName[MAX_FILE_NAME];	// 파일 이름
	stPos				m_iStartPos;					// 미사일 시작 상대 좌표
	unsigned int		m_iFirstTime;					// 미사일 처음 발사 타이밍
	unsigned int		m_iCooltime;					// 미사일 쿨타임
	int					m_iDamage;						// 미사일 데미지
	int					m_iSpeed;						// 미사일 속도
	char				m_chShape;						// 미사일 모양
	int					m_iLifetime;					// 미사일 생명주기
	int					m_iMissileMaxMoveIndex;			// 미사일 종류 별 최대 인덱스
	stPos				*m_MissileMoves;				// 미사일 움직임
};

struct stMissile
{
	// 미사일 활성화 여부
	bool				m_bIsActive;

	// 적이 쐈는지
	bool				m_bIsEnemy;

	// 미사일 데미지
	int					m_iDamage;
	int					m_iSpeed;

	// 현재 좌표
	stPos				m_stPos;

	// 미사일 모양
	char				m_chShape;

	// 미사일 라이프 타임
	// 몇 프레임 존재할 수 있는가
	int					m_iCurLife = 0;
	int					m_iMaxLife = INT32_MAX;

	// 진행 방향 - 다양한 진행 방향을 위해
	// Missile에서 절대 할당 해제해서는 안된다.
	int					m_iDirSize;
	int					m_iDirIndex = 0;
	double				m_dDirIndex = 0; // double index : deltaTime 곱하는 용도
	stPos				*m_arrDir = nullptr;

	// 플레이어 전용 진행 방향 백업용
	stPos				m_DirBak;
};


// Enemy 미사일 생성
void MissileEnemyCreate(const stMissileInfo &info, const stPos &curPos);

// 모든 미사일 비활성화
void MissileRelease(void);

// Player Defualt 미사일 생성
// curDir는 진행 방향
void MissilePlayerDefualtCreate(const stPos &curDir);

// 미사일 Logic Update
void MissileUpdate(void);

// 미사일 움직임
void MissileMove(void);

// 미사일 충돌 처리
void MissileCollision(void);

// 미사일 Render
void MissileRender(void);