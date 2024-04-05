#include <iostream>
#include <cstring>
#include <cstdint>

int main() {
    // 16바이트 크기의 배열 선언
    uint8_t array[16];
    memset(array, 0xee, 16);
    // uint64_t 변수 선언 및 초기화
    uint64_t counter = 1;
    for (int k = 0; k < 5; k++) {
        // 배열에 counter 값을 복사
        memcpy(array + 8, &counter, sizeof(uint64_t));

        // 배열 출력
        std::cout << "Array contents: ";
        for (int i = 0; i < 16; ++i) {
            std::cout << std::hex << (int)array[i] << " ";
        }
        std::cout << std::endl;
        counter+= 256;
    }

    return 0;
}
