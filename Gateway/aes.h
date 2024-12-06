#ifndef INC_AES_HPP_
#define INC_AES_HPP_

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


#define AES128 1
#define AES_BLOCKLEN 16
#define AES_KEYLEN 16
#define AES_keyExpSize 176

struct AES {
	uint8_t RoundKey[AES_keyExpSize];
};

void handleDataEncrypt(const char *input_str, char *data_sent, uint8_t *key);
void handleDataDecrypt(const char* data_encrypt, char* data_decrypt, uint8_t* key);

#endif /* INC_AES_HPP_ */
