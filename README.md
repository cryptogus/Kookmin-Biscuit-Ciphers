# Kookmin Block Cipher (KBC) for students
KBC is an C program for testing block cipher.

Currently, it has been tested only on Linux in 2023.11

This content follows **GNU GENERAL PUBLIC LICENSE Version 3**.

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
   I used **x64 Native Tools Command Prompt for VS 2022** in the "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2022\Visual Studio Tools\VC" location.
   ```shell
   PS> cmake -B build -S . && cd build
   PS> nmake
   ```
## Linux

On Ubuntu:

1. Install dependencies:
    
    ```bash
    $ sudo apt update
    $ sudo apt upgrade
    $ sudo apt install git cmake gcc valgrind
    
    # Qt (for gui)
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

    <!-- ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/f247afd6-a1d5-4144-95c9-ad59ec7503d7) -->
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/270e46c1-a1f0-4883-8276-0abf07235dd7)

    If you build using qtcreator, you should pay attention to the location of the KBC library and api.h file in the **.pro** (qmake file) because the location of the build directory is $PWD.

    setting build directory
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/846e10b7-abfa-43b6-887e-cef07fe74369)
    qmake
    ![image](https://github.com/cryptogus/Kookmin-Block-Cipher/assets/60291830/eaf4f252-0129-4e84-9b9a-876f4e8c2115)
