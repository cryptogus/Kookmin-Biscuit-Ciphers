## Copyright and License

© 2024, cryptogus

This work is licensed under a [Creative Commons Attribution 4.0 International License](https://creativecommons.org/licenses/by/4.0/).

# ChaCha20

ChaCha20은 4x4 행렬을 사용하는 블록 암호 알고리즘이다.(정확히는 strean cipher) 각 행렬 요소는 32비트의 정수다.(512-bit blocks이고, 세세하게는 32-bit constant 4개, 256-bit key, 32-bit block counter, 96-bit nonce로 구성 되어있다.) 행렬은 16진수로 표현하였다.

참고로 20라운드라서 ChaCha20이다. 

```
    +-----+-----+-----+-----+
    |  0  |  1  |  2  |  3  |
    +-----+-----+-----+-----+
    |  4  |  5  |  6  |  7  |
    +-----+-----+-----+-----+
    |  8  |  9  |  a  |  b  |
    +-----+-----+-----+-----+
    |  c  |  d  |  e  |  f  |
    +-----+-----+-----+-----+

https://loup-vaillant.fr/tutorials/chacha20-design
https://datatracker.ietf.org/doc/html/rfc7539#section-1.1
에서는 아래와 같이 표기했다.
    
    0   1   2   3
    4   5   6   7
    8   9  10  11
   12  13  14  15

16진법이나 10진법이나 표기만 다른거고 젤 위에는 이쁘게 그리려고 16진법 씀.
```

각 요소는 32비트 정수로 표현된다. 이 행렬은 ChaCha20의 상태를 나타냅니다. 초기 상태에서는 이 행렬에 특정한 상수 값과 키(key)에 기반한 값을 초기화합니다.

## ChaCha20의 행렬 요소 묶기
ChaCha20에서 열 벡터 (columns vecotr)는 다음과 같이 정의할 수 있다. 수학과나 AES 정도 공부해본 보안학과 사람들은 쉽게 이해 가능할 거라 믿는다.

- `0`, `4`,  `8`, `12`: 열 1
- `1`, `5`,  `9`, `13`: 열 2
- `2`, `6`, `10`, `14`: 열 3
- `3`, `7`, `11`, `15`: 열 4

하지만 믿지 못해서 그림을 가져왔다. 직접 그리면 좋은데 현재 상황도 안돼고 사실 귀찮다.
![columns](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/abd51754-8320-4640-b853-9bdd6a6be2f4)
출처 - https://en.wikipedia.org/wiki/Row_and_column_spaces (역시 위키짱)  
숫자는 신경쓰지말고 세로로 묶었는데 저걸 열이라고 한다.
```
 0   1   2   3
 4   5   6   7
 8   9  10  11
12  13  14  15
```
위 행렬을 그림 처럼 세로로 묶으면 열 1,2,3,4로 나타난다는 거다.

자 그럼 대각선(Diagonal)은 어떻게 되는지 보자. 이건 조금 이해시키기 어려울지도? 참고로 diagonal matrix를 말하는게 아니라 행렬 요소들을 묶는 방식이 대각선이라는 거다.
- `0`, `5`, `10`, `15`: 대각선 1
- `1`, `6`, `11`, `12`: 대각선 2
- `2`, `7`,  `8`, `13`: 대각선 3
- `3`, `4`,  `9`, `14`: 대각선 4

애는 내가 직접 표현해야겠다. 그림이 뭔가 오해하기 좋을 것 같다.
### 대각선 1
해당하는 행렬 요소 앞에 '*'로 체크했다. 왜 대각선인지 이해가 될까??
```
 *0   1   2   3
  4  *5   6   7
  8   9 *10  11
 12  13  14 *15
```
### 대각선 2
해당하는 행렬 요소 앞에 '*'로 체크했다.
```
  0  *1   2   3
  4   5  *6   7
  8   9  10 *11
*12  13  14  15
```
### 대각선 3
해당하는 행렬 요소 앞에 '*'로 체크했다.
```
  0   1  *2   3
  4   5   6  *7
 *8   9  10  11
 12 *13  14  15
```
### 대각선 4
해당하는 행렬 요소 앞에 '*'로 체크했다.
```
  0   1   2  *3
 *4   5   6   7
  8  *9  10  11
 12  13 *14  15
```
사실 별건 아니고 ChaCha20에서 수행하는 Quarter Round연산이 열(columns)과 대각선(diagonal) 순으로 일어나기 때문에 미리 정의해둔 거다.

## Quarter Round연산 (column rounds)
ChaCha20의 Quarter Round를 수행하는 과정에서는 특정한 행렬의 요소에 대해 32-bit modulo덧셈(`+`), XOR(`^`), 그리고 Left Rotation of bits (<<<) 연산들이 적용된다.

C언어 느낌으로 pseudo code를 나타내면 아래와 같다. (나도 그냥 reference꺼 가져와서 썻다.)
```c
a += b; d ^= a; d <<<= 16;
c += d; b ^= c; b <<<= 12;
a += b; d ^= a; d <<<= 8;
c += d; b ^= c; b <<<= 7;
```
위에서 정리해둔 자료다.
- `0`, `4`,  `8`, `12`: 열 1
- `1`, `5`,  `9`, `13`: 열 2
- `2`, `6`, `10`, `14`: 열 3
- `3`, `7`, `11`, `15`: 열 4

각 열들의 원소들이 a, b, c, d가 되는거다.
예를 들어, 열 1에서 0은 a, 4는 b, 8이 c, 12가 d가 되어 Quarter Round연산을 진행한다.
마찬가지로 열 2에서 1이 a, 5가 b, 9가 c, 13은 d가 되어 Quarter Round연산을 진행한다.

이렇게 위 pseudo code 연산을 열 1 ~ 4 까지 총 4번 진행하게 된다.
## Quarter Round연산 (diagonal rounds)

column rounds와 연산 방법은 같다. 다만 다른 점은 바로 대각선으로 행렬 요소들을 묶어 연산한다는 점이다.
우리는 이미 사전에 대각선으로 행렬 요소들을 묶는 작업을 진행했었다.
- `0`, `5`, `10`, `15`: 대각선 1
- `1`, `6`, `11`, `12`: 대각선 2
- `2`, `7`,  `8`, `13`: 대각선 3
- `3`, `4`,  `9`, `14`: 대각선 4

예를 들어, 대각선 1에서 0은 a, 5는 b, 10이 c, 15가 d가 되어 Quarter Round연산을 진행한다.
마찬가지로 대각선 2에서 1이 a, 6이 b, 11이 c, 12가 d가 되어 Quarter Round연산을 진행한다.

이렇게 아까 나온 pseudo code 연산을 대각선 1 ~ 4 까지 총 4번 진행하게 된다.

이러고 보면 rfc7539 1-1에 있던 문구가 이해가 된다.
```
The matrix notation is more visually convenient and gives a better notion as to
why some rounds are called "column rounds" while others are called "diagonal rounds".
```

아래는 rfc7539 2-3 내용이다.
```r
   ChaCha20 runs 20 rounds, alternating between "column rounds" and
   "diagonal rounds".  Each round consists of four quarter-rounds, and
   they are run as follows.  Quarter rounds 1-4 are part of a "column"
   round, while 5-8 are part of a "diagonal" round:

   1.  QUARTERROUND ( 0, 4, 8,12)
   2.  QUARTERROUND ( 1, 5, 9,13)
   3.  QUARTERROUND ( 2, 6,10,14)
   4.  QUARTERROUND ( 3, 7,11,15)
   5.  QUARTERROUND ( 0, 5,10,15)
   6.  QUARTERROUND ( 1, 6,11,12)
   7.  QUARTERROUND ( 2, 7, 8,13)
   8.  QUARTERROUND ( 3, 4, 9,14)
```
**Reference**

https://loup-vaillant.fr/tutorials/chacha20-design \
https://cr.yp.to/chacha/chacha-20080128.pdf \
https://datatracker.ietf.org/doc/html/rfc7539#section-2.3 \
https://android.googlesource.com/kernel/common/+/1855aaccd74c/lib/chacha20.c
 