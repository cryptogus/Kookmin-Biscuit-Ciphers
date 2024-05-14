https://csrc.nist.gov/projects/Cryptographic-Algorithm-Validation-Program/details?source=TDES&number=591

https://csrc.nist.gov/Projects/cryptographic-algorithm-validation-program/Block-Ciphers

https://www.cryptosys.net/test-bed.html

https://csrc.nist.gov/pubs/sp/800/20/upd2/final

https://pages.nist.gov/ACVP/draft-celi-acvp-symmetric.html

https://cryptography.io/en/3.4.7/development/test-vectors.html
 
https://the-x.cn/en-US/cryptography/TripleDes.aspx -> ctr mode test vector 이걸로 생성(ctr mode 만 없음... 추천 안한데)

https://mh-nexus.de/en/hxd/ -> .rsp 파일들 이걸로 읽고 fseek 적용함

vi -b TCBCMMT3.rsp (windwos에서 작성) -> 개행 뒤에 ^M 이라는게 있다.
vi -b TCTRMMT3.rsp (linux에서 작성)

그래서 test.c 파일들 보면 ctr만 fseek의 이동 값이 1개씩 작다.
^M 제거하려면 dos2unix 명령어가 필요한데 여기서는 적용하지 않았다.

`dos2unix`는 윈도우즈 스타일(CRLF) 줄 바꿈을 유닉스 스타일(LF) 줄 바꿈으로 변환하는 유틸리티입니다. 이 명령은 텍스트 파일을 리눅스 환경에서 사용할 수 있도록 줄 바꿈을 변환하는 데 사용됩니다.

Ubuntu에서 `dos2unix` 유틸리티를 설치하려면 다음 명령을 사용할 수 있습니다:

```
sudo apt-get install dos2unix
```

위 명령을 실행하면 `dos2unix` 유틸리티가 시스템에 설치됩니다. 이제 줄 바꿈을 변환하려는 파일에 대해 `dos2unix` 명령을 사용할 수 있습니다.

https://csrc.nist.gov/pubs/sp/800/20/upd2/final -> Monte Carlo Test reference

monte garlo test에서 key 값들에 1씩 차이가 나는 경우는 parity bit까지 꼼꼼하게 챙기는 nist의 덕분이다.  
실제 64비트 키여도 패리티 비트는 키의 역할을 하지 않으므로 56bit 키 강도를 가지는게 DES임.

그래서
```c
for (int j = 0; j < 8; j++) {
            tkey[j] ^= plaintext[j] ^ 0x1010101010101010;
}
```
이거는 암호문 결과에 영향을 미침.

하지만 패리티비트에만 영향을 끼치는
```c
for (int j = 0; j < 8; j++) {
            tkey[j] ^= plaintext[j] ^ 0x0101010101010101;
}
```
는 key값은 정답과 다르게 찍히는데, 실제 암호화 결과에는 영향을 안미친다.

실제로 TCBCMonte3.rsp key들의 1바이트씩 bit단위로 바꿔보면, 패리티 비트 추가 규칙을 잘 지키고 있는 것으로 확인했다.

즉, nist는 패리티 비트 규칙을 추가해서(잘 지켜서) key값에 가끔씩 1씩 차이가 나는거에요