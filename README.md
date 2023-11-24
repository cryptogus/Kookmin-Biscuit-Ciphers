# Kookmin Block Cipher (KBC) Test용
KBC is an C program for testing block cipher

Currently, it has been tested only on Linux
## Status

**Padding**
- PKCS#7

**Block Ciphers**
- AES: AES-128,
- LEA:
- ARIA:
- SEED:
- PIPO:

**Modes of Operation**
- ECB
- CBC

## Linux
1. Install dependencies:

    On Ubuntu:
    
    ```bash
    $ sudo apt update
    $ sudo apt upgrade
    $ sudo apt install cmake gcc valgrind
    ```

    build:
    ```bash
    $ cmake -B build -S . && cd build
    $ make -j$(nproc)
    ```


