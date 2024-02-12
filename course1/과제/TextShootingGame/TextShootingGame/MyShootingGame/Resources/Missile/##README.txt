미사일 정의 방법
1. Missile Shape
    구분자 SHAPE - 뒤에 나오는 문자는 미사일의 모양
    미사일 몸체 정의 ex) *, ^
    
2. Missile Life Time
    구분자 LIFETIME
    미사일 생존 프레임 수
    * 0일 시 화면을 벗어날 때까지


3. Missile Pattern
    구분자 PATTERN
    미사일의 패턴 정의 - Enemy 패턴과 동일한 방식으로 작성


example
~~~
SHAPE
*

LIFETIME
0 // 맵 밖으로 벗어날 때까지

PATTERN
1
-1, 0 // Y 방향으로 진행
~~~