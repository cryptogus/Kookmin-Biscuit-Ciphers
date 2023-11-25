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

On Ubuntu:

1. Install dependencies:
    
    ```bash
    $ sudo apt update
    $ sudo apt upgrade
    $ sudo apt install cmake gcc valgrind
    ```

2. Build
    ```bash
    $ cmake -B build -S . && cd build
    $ make -j$(nproc)
    ```

3. Test
    ```bash
    ```


