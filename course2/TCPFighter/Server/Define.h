#pragma once

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 5000

#define dfTICK_PER_FRAME (1000 / 50)

// 패킷 코드
#define dfPACKET_CODE 0x89;

// MAX hp
#define dfMAX_PLAYER_HP 100

// 이 좌표에 닿는 경우 움직임을 멈추어야 함
#define dfRANGE_MOVE_TOP	50
#define dfRANGE_MOVE_LEFT	10
#define dfRANGE_MOVE_RIGHT	630
#define dfRANGE_MOVE_BOTTOM	470

// 프레임 당 이동 단위
#define dfMOVE_X 3
#define dfMOVE_Y 2

// 이동 오류체크 범위
#define dfERROR_RANGE		50

// 공격 범위
#define dfATTACK1_RANGE_X		80
#define dfATTACK1_RANGE_Y		10

#define dfATTACK2_RANGE_X		90
#define dfATTACK2_RANGE_Y		10

#define dfATTACK3_RANGE_X		100
#define dfATTACK3_RANGE_Y		20

#define dfDAMAGE1 5
#define dfDAMAGE2 7
#define dfDAMAGE3 10

// 이동 방향
enum class MOVE_DIR
{
	MOVE_DIR_LL = 0,
	MOVE_DIR_LU = 1,
	MOVE_DIR_UU = 2,
	MOVE_DIR_RU = 3,
	MOVE_DIR_RR = 4,
	MOVE_DIR_RD = 5,
	MOVE_DIR_DD = 6,
	MOVE_DIR_LD = 7,
	MOVE_DIR_STOP = 8
};

// 싱글톤
#define SINGLE(type)		private:										\
								type();										\
								~type();									\
								inline static type *m_instPtr = nullptr;	\
							public:											\
								static type *GetInstance()					\
								{											\
									if (m_instPtr == nullptr)				\
									{										\
										m_instPtr = new type();				\
										atexit(Destory);					\
									}										\
																			\
									return m_instPtr;						\
								}											\
																			\
								static void Destory()						\
								{											\
									delete m_instPtr;						\
									m_instPtr = nullptr;					\
								}

struct stPos
{
	int m_iX;
	int m_iY;
};