# TDES Multi-block Message Test & Monte Carlo Test

ubuntu 22.04 에서 TEST 하였음.
- TDES의 ctr mode는 nist에서 제공하지 않음 (왜냐하면 ECB로 테스트하는 거랑 별로 다를게 없긴해서?)
 
In addition, the tests for ECB Mode shall be performed on the encryption engine used for the counter mode. (Counter mode is tested by selecting the ECB mode) 
### usage
```bash
$ sudo apt install unzip cmake gcc build-essential
$ cmake -B build -S .
$ cd build && make

$ ./ecb_mmt_test
$ ./cbc_mmt_test
$ ./ctr_mmt_test

$ ./ecb_mct_test
$ ./cbc_mct_test
```
Monte Carlo Test에서, KEY 값들이 1씩 차이가 나는 이유는, parity bit 추가 여부 때문이다.

*.rsp → 정답파일
*.nif → nif TDES에 의한 암복호화 결과 값