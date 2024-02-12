#pragma once

// 미사일 정보 구조체
struct stMissileInfo
{
	// MISSILE 정보
	char				m_szFileName[MAX_FILE_NAME]; // 파일 이름
	stPos				m_iStartPos;				// 미사일 시작 상대 좌표
	unsigned int		m_iFirstTime;				// 미사일 처음 발사 타이밍
	unsigned int		m_iCooltime;				// 미사일 쿨타임
	int					m_iDamage;					// 미사일 데미지
	char				m_chShape;
	int					m_iLifetime;
	int					m_iMissileMaxIndex;			// 미사일 종류 별 최대 인덱스
	stPos				*m_iMissileMoves;			// 미사일 움직임
};

struct stMissile
{
	// 미사일 활성화 여부
	bool m_bIsActive;

	// 적이 쐈는지
	bool m_bIsEnemy;

	// 미사일 데미지
	int m_iDamage;

	// 현재 좌표
	stPos m_stPos;

	// 미사일 모양
	wchar_t m_chShape;

	// 미사일 라이프 타임
	// 몇 프레임 존재할 수 있는가
	int m_iCurLife = 0;
	int m_iMaxLife = INT32_MAX;

	// 진행 방향 - 다양한 진행 방향을 위해
	// m_arrDir의 원본 포인터는 다른 곳에서 가지고 있다.
	// 그저 전달받을 뿐
	// Missile에서 절대 할당 해제해서는 안된다.
	int m_iDirSize;
	int m_iDirIndex = 0;
	stPos *m_arrDir = nullptr;
};

extern stMissile g_arrMissile[dfMAX_MISSILE_COUNT];