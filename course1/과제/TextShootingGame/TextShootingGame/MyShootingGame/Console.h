//////////////////////////////////////////////////////////////
// Windows 의 콘솔 화면에서 커서 제어.
// 
//////////////////////////////////////////////////////////////

#pragma once

//-------------------------------------------------------------
// 콘솔 제어를 위한 준비 작업.
//
//-------------------------------------------------------------
void ConsoleInitial(void);

//-------------------------------------------------------------
// 콘솔 화면의 커서를 X, Y 좌표로 이동시킨다.
//
//-------------------------------------------------------------
void ConsoleMoveCursor(int iPosX, int iPosY);

//--------------------------------------------------------------------
// 버퍼의 내용을 화면으로 찍어주는 함수.
//
// 적군,아군,총알 등을 szScreenBuffer 에 넣어주고, 
// 1 프레임이 끝나는 마지막에 본 함수를 호출하여 버퍼 -> 화면 으로 그린다.
//--------------------------------------------------------------------
void ConsoleBufferFlip(void);


//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void ConsoleBufferClear(void);

//--------------------------------------------------------------------
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void ConsoleSpriteDraw(const stPos &pos, char chSprite);