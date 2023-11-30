# Kookmin Block Cipher (KBC) Test용
KBC is an C program for testing block cipher.

Currently, it has been tested only on Linux.  

**Environmental System Information**\
CPU: quad core 11th Gen Intel Core i7-1165G7\
Kernel: 5.15.133.1-microsoft-standard-WSL2 x86_64\
RAM: 7.6 GiB
## Status

**Padding**
- PKCS#7

**Block Ciphers**
- AES: AES-128, AES-192, AES-256 https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf
- LEA: LEA-128, LEA-192, LEA-256
- ARIA:
- SEED:
- PIPO: PIPO64/128, PIPO64/256

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
        ├── pipo_test
        ├── lea_test
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


