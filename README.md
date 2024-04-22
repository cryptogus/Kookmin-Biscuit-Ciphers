- [KBC](#kookmin-biscuit-cipher-kbc-for-students)
    - [Status](#status)
    - [Windows build](#windows)
    - [Linux build](#linux)
    - [Cross compilation](#cross-compilation)

# Kookmin Biscuit Cipher (KBC) for students
KBC is an C program for testing block cipher.

Currently, it has been tested only on Linux in 2023.11

This content follows **GNU GENERAL PUBLIC LICENSE Version 3**.

**Cryptographic Calculation Tool**  
http://cryptomanager.com/

### Environmental System Information
**CPU: quad core 11th Gen Intel Core i7-1165G7**\
**Kernel: 5.15.133.1-microsoft-standard-WSL2 x86_64**\
**RAM: 7.6 GiB**

## Status

**Padding**
- PKCS#7 (Currently in stationary use)

**Block Ciphers**
- AES:  AES-128, AES-192, AES-256
- PIPO: PIPO64/128, PIPO64/256
- LEA:  LEA-128, LEA-192, LEA-256
- SEED: SEED128
- ARIA: ARIA-128, ARIA-192, ARIA-256

**Stream Cipher**
- [ChaCha20](src/chacha20/chacha20.md)(Not yet reflected in the GUI)

**Public-key cryptography(asymmetric cryptography)**
- RSA: RSA2048

**Modes of Operation**
- ECB
- CBC
- CTR (I developed the code so that it is not available for blocks that are not 16 bytes in size.)
## Windows
1. git
   https://git-scm.com/download/win

2. msvc compiler & nmake
   https://visualstudio.microsoft.com/ko/downloads/

3. cmake downloads
    https://cmake.org/download/

    <!--![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/32ba3154-f3ea-4250-aef7-a955ad36e43c)-->
   I chose Windows x64 Installer. Make sure to choose one that fits your environment.

4. build
   first: I ran "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

   second: I used **x64 Native Tools Command Prompt for VS 2022** in the "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2022\Visual Studio Tools\VC" location.
   ```powershell
   PS> cmake -B build -S . && cd build
   PS> nmake
   ```
## Linux

On Ubuntu:

1. Install dependencies:

    ```bash
    $ sudo apt update
    $ sudo apt upgrade
    $ sudo apt install git cmake gcc build-essential libssl-dev valgrind ninja

    # Qt (for gui)
    $ sudo apt install qt5-qmake qtbase5-dev
    ```

2. Build
    OpenSSL can be disabled when the user forwards it from the command line, such as -DUSE_OPENSSL=OFF.
    OpenSSL is used only on rsa2048.
    ```bash
    $ cmake -B build -S .&& cd build
    # $ cmake -B build -S . -DUSE_OPENSSL=OFF && cd build
    $ make -j$(nproc)
    ```
    ```bash
    계층 구조

    build/
    ├── openKBC
    ├── src/
    │   └── libKBC.so
    └── test/
        ├── CBC_test
        ├── ECB_test
        ├── aes_test
        ├── AES-KEY-EXPANSION-TEST
        ├── aria_test
        ├── pipo_test
        ├── lea_test
        ├── seed_test
        ├── chacha20_test
        ├── rsa2048_test
        ├── chacha_block_test
        ├── chacha_quarter_round_test
        ├── chacha_encrypt_test
        └── padding_test
    KBC_gui/
    └──libKBC.so
    ```

3. Test
    ```bash
    $ cd test
    ```
    You can test each function in this test directory.

    example:
    ```bash
    $ ./aes-test
    ```

4. After completing course 2, run `openKBC` from the build directory.

5. The `KBC` dynamic library is created at the `build/src` location. (Using with api.h)

6. How to run applications after building in the main directory
    ```bash
    $ build/openKBC
    ```
7. gui build (unfinished) - 현재 Decrypt 기능에서 평문 출력이 안 되는 등의 기타 버그 존재, widget.cpp을 수정해야함, Qt에 대한 지식 필요
    ```bash
    $ cd KBC_gui
    $ mkdir build
    $ cd build
    $ qmake ..
    $ make -j$(nproc)
    $ ./KBC_gui
    ```

    <!-- ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/f247afd6-a1d5-4144-95c9-ad59ec7503d7) -->
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/270e46c1-a1f0-4883-8276-0abf07235dd7)

    If you build using qtcreator, you should pay attention to the location of the KBC library and qt_api.h file in the **.pro** (qmake file) because the location of the build directory is $PWD.

    ### setting build directory
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/846e10b7-abfa-43b6-887e-cef07fe74369)
    ### setting .pro (qmake file)
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/eaf4f252-0129-4e84-9b9a-876f4e8c2115)

---

## GUI TROUBLESHOOTING

### 라이브러리 링크 문제:

g++로 컴파일할 때는 C++ 런타임 라이브러리가 필요할 수 있습니다. 필요한 경우 -lstdc++ 플래그를 사용하여 링크해보세요.

**C++ 코드에서 C 라이브러리를 사용하는 경우, extern "C" 블록으로 감싸진 헤더 파일을 사용하는 것이 중요합니다. 이것은 C++ 이름 맹글링과 관련된 문제를 해결할 수 있습니다. qt_api.h에 적용 중, 왜냐하면 libKBC.so 가 gcc 로 빌드, 즉 c언어로 구성된 라이브러리임.**

`KBC_gui/qt_api.h` 와 `apps/api.h`를 비교해보셈
```cpp
// 예: mylibrary.h
#ifdef __cplusplus
extern "C" {
#endif

// 라이브러리 관련 코드...

#ifdef __cplusplus
}
#endif
```


### 실행 파일의 rpath 설정 확인:

실행 파일이 ../build/src 디렉터리의 libKBC.so를 찾을 수 있도록 빌드 시에 rpath를 설정했는지 확인해야함.

`libKBC.so`을 가지고 새로운 실행 파일을 만들고 싶을 수 있으니 아래는 그 예시다. openKBC.c 대신 다른 커스텀 하고 싶은 소스코드를 넣어도 된다.
```bash
$ gcc openKBC.c -o your_executable -L../build/src -Wl,-rpath=../build/src -lKBC -lcrypto
```
여기서 -Wl,-rpath=../build/src 옵션은 실행 파일이 ../build/src 디렉토리를 동적 라이브러리 검색 경로로 추가하도록 한다.

아래 명령어로 라이브러리 경로를 잘 찾는지 확인한다.
```bash
$ ldd your_executable

linux-vdso.so.1 (0x00007ffdcdd5b000)
libKBC.so => ../build/src/libKBC.so (0x00007ff2612de000)
libcrypto.so.3 => /lib/x86_64-linux-gnu/libcrypto.so.3 (0x00007ff260e93000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ff260c6b000)
/lib64/ld-linux-x86-64.so.2 (0x00007ff2612fd000)
```
`libKBC.so => ../build/src/libKBC.so` 로 아주 잘 찾는 것을 확인할 수 있다.
`KBC_gui/KBC_gui.pro` (qmake 파일)에 옵션 적용 중.

<!-- ### 나의 Qt 스승
https://gitlab.com/gilgil/gr -->

## cross-compilation
### Cross-compiling on Linux for ARM
Binaries for ARM can be cross-compiled on Linux. In order to do so, you need to supply CMake with an appropriate [toolchain](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) file.

**For example**
```bash
$ sudo apt install gcc-aarch64-linux-gnu
$ cmake -GNinja -DCMAKE_TOOLCHAIN_FILE=.CMake/toolchain_arm64.cmake -DUSE_OPENSSL=OFF -B build -S .
$ cd build && ninja -v
```

**Test with qemu**
다른 아키텍쳐를 사용하여 컴파일한 바이너리들을 실행해볼 때 \
`qemu-[아키텍쳐명]-static [실행파일]` 로 바이너리 실행하고 \
`gdb-multiarch [실행파일]` 로 디버깅 하면 된다.
```bash
$ sudo apt install qemu-user-static
$ qemu-aarch64-static -L /usr/aarch64-linux-gnu/ openKBC
```
