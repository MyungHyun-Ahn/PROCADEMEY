#pragma once

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 5000

#define dfTICK_PER_FRAME 50

// MAX hp
#define dfMAX_PLAYER_HP 100

// �� ��ǥ�� ��� ��� �������� ���߾�� ��
#define dfRANGE_MOVE_TOP	50
#define dfRANGE_MOVE_LEFT	10
#define dfRANGE_MOVE_RIGHT	630
#define dfRANGE_MOVE_BOTTOM	470

// ������ �� �̵� ����
#define dfMOVE_X 3
#define dfMOVE_Y 2

// �̵� ����üũ ����
#define dfERROR_RANGE		50

// ���� ����
#define dfATTACK1_RANGE_X		80
#define dfATTACK2_RANGE_X		90
#define dfATTACK3_RANGE_X		100
#define dfATTACK1_RANGE_Y		10
#define dfATTACK2_RANGE_Y		10
#define dfATTACK3_RANGE_Y		20

// �̱���
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