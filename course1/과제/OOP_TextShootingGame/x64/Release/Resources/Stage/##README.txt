스테이지 파일 작성 방법

STAGE_INFO 파일 - STAGE 정보 작성, 행으로 작성
1. STAGE 개수와 STAGE 파일 이름
2. 총 출현 Enemy 수
3. 각 STAGE에 출현하는 Enemy 정보
    각 STAGE에 출현하는 Enemy 종류의 수와 종류를 작성한다.

    ex)
    3
    STAGE01.txt, 15, 1, E
    STAGE02.txt, 20, 2, Q, W
    STAGE03.txt, 30, Q, W, E, R

    * 3개의 STAGE를 클리어하면 GAME CLEAR로 간주한다.
    * 위에 작성한 Enemy만 로드하므로 각 STAGE에 출현하는 Enemy의 종류는 반드시 위에서 작성한 Enemy만 Stage 파일에 존재해야 한다.


STAGE 파일 작성
1. GameSetting.h 파일에 정의된 dfSCREEN_WIDTH * dfSCREEN_HEIGHT SIZE의 크기로 STAGE를 구성한다.