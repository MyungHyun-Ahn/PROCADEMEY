#pragma once

#define dfFPS 50
#define dfFIXED_DELTATIME 1 / (double)dfFPS
#define dfTIME_PERIOD 1

#define dfSCREEN_WIDTH	155		// 콘솔 가로 0~153칸 + NULL 문자 1칸
#define dfSCREEN_HEIGHT	42		// 콘솔 세로 0~41칸

#define dfMAX_MISSILE_COUNT 10000 // 미사일 풀 개수


// 플레이어 모양
#define dfPLAYER_SHAPE 'A'

// 플레이어 미사일 모양
#define dfPLAYER_MISSILE_SHAPE 'o'

#define dfPLAYER_MISSILE_SPEED 50

// 플레이어 능력치
#define dfPLAYER_HP 30
#define dfPLAYER_DAMAGE 10
#define dfPLAYER_SPEED 30

#define dfPLAYER_START_POS_Y 35
#define dfPLAYER_START_POS_X 76

#define dfPLAYER_ATTACK_COOLTIME 100