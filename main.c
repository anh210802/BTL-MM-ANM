#include "aes.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    uint8_t key[16] = {
        "thisisakey123456"
    };

    char data_input[160] = {0};
    char data_encrypt[320] = {0};
    char data_decrypt[160] = {0};
    while (1) {
        printf("Data input: ");
        fgets(data_input, sizeof(data_input), stdin);
        data_input[strcspn(data_input, "\n")] = 0; 

        clock_t start_encrypt = clock();
        handleDataEncrypt(data_input, data_encrypt, key);
        clock_t end_encrypt = clock();
        float encrypt_time_ns = (float)(end_encrypt - start_encrypt) * 1e9 / CLOCKS_PER_SEC;
        printf("Data encrypt: %s\n", data_encrypt);
        printf("Process time encrypt: %.0f ns\n", encrypt_time_ns);

        clock_t start_decrypt = clock();
        handleDataDecrypt(data_encrypt, data_decrypt, key);
        clock_t end_decrypt = clock();
        float decrypt_time_ns = (float)(end_decrypt - start_decrypt) * 1e9 / CLOCKS_PER_SEC;
        printf("data decrypt: %s\n", data_decrypt);
        printf("Process time decrypt: %.0f ns\n", decrypt_time_ns);

        memset(data_input, 0, sizeof(data_input));
        memset(data_encrypt, 0, sizeof(data_encrypt));
        memset(data_decrypt, 0, sizeof(data_decrypt));
    }
    return 0;
}
