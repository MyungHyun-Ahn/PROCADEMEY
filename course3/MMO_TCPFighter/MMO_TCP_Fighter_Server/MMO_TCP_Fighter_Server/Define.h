#pragma once

// 서버 정보
constexpr const char *SERVER_IP = "0.0.0.0";
constexpr short SERVER_PORT = 10611;


constexpr int FRAME_PER_SECOND = 25;
constexpr int TICK_PER_FRAME = (1000 / FRAME_PER_SECOND);


constexpr BYTE PACKET_IDENTIFIER = 0x89;


// 30초 이상이 되도록 아무런 메시지 수신도 없는경우 접속 끊음
constexpr int NETWORK_PACKET_RECV_TIMEOUT = 30000;


// 화면 이동 범위
constexpr int RANGE_MOVE_TOP = 0;
constexpr int RANGE_MOVE_LEFT = 0;
constexpr int RANGE_MOVE_RIGHT = 6400;
constexpr int RANGE_MOVE_BOTTOM = 6400;

// 섹터 크기
constexpr int SECTOR_SIZE = 160;

// 주변 몇칸까지 보이게할 것인지
constexpr int SECTOR_VIEW_START = 1; // 대각선 1칸 위에서 부터
constexpr int SECTOR_VIEW_COUNT = 3; // 3 x 3

constexpr int SECTOR_MAX_X = (RANGE_MOVE_RIGHT - RANGE_MOVE_LEFT) / SECTOR_SIZE;
constexpr int SECTOR_MAX_Y = (RANGE_MOVE_BOTTOM - RANGE_MOVE_TOP) / SECTOR_SIZE;

// 최대 체력
constexpr int MAX_PLAYER_HP = 100;

// 공격범위
constexpr int ATTACK1_RANGE_X = 80;
constexpr int ATTACK2_RANGE_X = 90;
constexpr int ATTACK3_RANGE_X = 100;
constexpr int ATTACK1_RANGE_Y = 10;
constexpr int ATTACK2_RANGE_Y = 10;
constexpr int ATTACK3_RANGE_Y = 20;


// 공격 데미지
constexpr int ATTACK1_DAMAGE = 1;
constexpr int ATTACK2_DAMAGE = 2;
constexpr int ATTACK3_DAMAGE = 3;


// 캐릭터 이동 속도   // 25fps 기준 이동속도
constexpr int SPEED_PLAYER_X = 3 * (50 / FRAME_PER_SECOND);	// 3   50fps;
constexpr int SPEED_PLAYER_Y = 2 * (50 / FRAME_PER_SECOND);	// 2   50fps;


// 이동 오류체크 범위
constexpr int ERROR_RANGE = 50;

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