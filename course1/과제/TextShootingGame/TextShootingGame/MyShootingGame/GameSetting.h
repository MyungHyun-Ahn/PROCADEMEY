#pragma once

#define dfFPS 50
#define dfFIXED_DELTATIME 1 / (double)dfFPS
#define dfTIME_PERIOD 1

#define dfSCREEN_WIDTH	155		// �ܼ� ���� 0~153ĭ + NULL ���� 1ĭ
#define dfSCREEN_HEIGHT	42		// �ܼ� ���� 0~41ĭ

#define dfMAX_MISSILE_COUNT 10000 // �̻��� Ǯ ����


// �÷��̾� ���
#define dfPLAYER_SHAPE 'A'

// �÷��̾� �̻��� ���
#define dfPLAYER_MISSILE_SHAPE 'o'

// �÷��̾� �ɷ�ġ
#define dfPLAYER_HP 20
#define dfPLAYER_DAMAGE 10

#define dfPLAYER_START_POS_Y 35
#define dfPLAYER_START_POS_X 76

#define dfPLAYER_ATTACK_COOLTIME 200