Enemy 정의 방법
1. Enemy Shape
    구분자 SHAPE
    적의 몸체를 정의 ex) Q, W, E, R

2. Enemy Stats
    구분자 STATS
    hp, speed
    * speed = 50 - 1프레임에 1칸
    * 50을 넘을 시 충돌처리 제대로 동작하지 않을 수 있음

3. Missile Type : 미사일 개수
    구분자 MISSILE_TYPE
    미사일의 타입을 정의 - 파일 명
    * 행으로 작성
    * 미사일 파일 명, 시작 상대 좌표, 첫 발사 타이밍, 쿨타임, 데미지, 스피드
    * 미사일의 쿨타임 정의 - ms 단위 1s : 1000ms


4. Move Pattern
    Enemy의 움직임 패턴을 정의 - 모든 이동을 마쳤을 때 원점으로 돌아오는 것이 좋다.
    패턴 개수
    y, x


example
~~~
SHAPE
E

STATS
1

MISSILE_TYPE
1
UP.txt, 1, 0, 0, 1000, 1

MOVE_PATTERN
8
0, 1
0, 1
0, 1
0, 1
0, -1
0, -1
0, -1
0, -1
~~~