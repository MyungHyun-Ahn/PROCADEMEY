#pragma once

constexpr int		dfFPS = 50;
constexpr double	dfFIXED_DELTATIME = 1 / (double)dfFPS;
constexpr int		dfTIME_PERIOD = 1;

constexpr int		dfSCREEN_WIDTH = 155;		// 콘솔 가로 0~153칸 + NULL 문자 1칸
constexpr int		dfSCREEN_HEIGHT = 42;		// 콘솔 세로 0~41칸
constexpr int		dfMAX_MISSILE_COUNT = 10000; // 미사일 풀 개수


// 플레이어 모양
constexpr char		dfPLAYER_SHAPE = 'A';

// 플레이어 미사일 모양
constexpr char		dfPLAYER_MISSILE_SHAPE = 'o';
constexpr int		dfPLAYER_MISSILE_SPEED = 50;

// 플레이어 능력치
constexpr int		dfPLAYER_HP = 30;
constexpr int		dfPLAYER_DAMAGE = 10;
constexpr int		dfPLAYER_SPEED = 30;

constexpr int		dfPLAYER_START_POS_Y = 35;
constexpr int		dfPLAYER_START_POS_X = 76;

constexpr int		dfPLAYER_ATTACK_COOLTIME = 100;