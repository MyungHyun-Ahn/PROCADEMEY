#pragma once

constexpr int		dfFPS = 50;
constexpr double	dfFIXED_DELTATIME = 1 / (double)dfFPS;
constexpr int		dfTIME_PERIOD = 1;

constexpr int		dfSCREEN_WIDTH = 155;		// �ܼ� ���� 0~153ĭ + NULL ���� 1ĭ
constexpr int		dfSCREEN_HEIGHT = 42;		// �ܼ� ���� 0~41ĭ
constexpr int		dfMAX_MISSILE_COUNT = 10000; // �̻��� Ǯ ����


// �÷��̾� ���
constexpr char		dfPLAYER_SHAPE = 'A';

// �÷��̾� �̻��� ���
constexpr char		dfPLAYER_MISSILE_SHAPE = 'o';
constexpr int		dfPLAYER_MISSILE_SPEED = 50;

// �÷��̾� �ɷ�ġ
constexpr int		dfPLAYER_HP = 30;
constexpr int		dfPLAYER_DAMAGE = 10;
constexpr int		dfPLAYER_SPEED = 30;

constexpr int		dfPLAYER_START_POS_Y = 35;
constexpr int		dfPLAYER_START_POS_X = 76;

constexpr int		dfPLAYER_ATTACK_COOLTIME = 100;