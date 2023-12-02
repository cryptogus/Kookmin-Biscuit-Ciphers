# Kookmin Block Cipher (KBC) Test용
KBC is an C program for testing block cipher.

Currently, it has been tested only on Linux.  

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

**Modes of Operation**
- ECB
- CBC

## Linux

On Ubuntu:

1. Install dependencies:
    
    ```bash
    $ sudo apt update
    $ sudo apt upgrade
    $ sudo apt install cmake gcc valgrind
    
    # Qt (gui)
    $ sudo apt install qt5-qmake qtbase5-dev
    ```

2. Build
    ```bash
    $ cmake -B build -S . && cd build
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
        ├── aria_test
        ├── pipo_test
        ├── lea_test
        ├── seed_test
        └── padding_test
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
7. gui build (unfinished)
    ```bash
    $ cd KBC_gui
    $ mkdir build
    $ cd build
    $ qmake ..
    $ make -j$(nproc)
    $ ./KBC_gui
    ```

![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/f247afd6-a1d5-4144-95c9-ad59ec7503d7)
