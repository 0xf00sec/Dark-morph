// INCLUDES_H

#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/ptrace.h>
#include <string.h>
#include <sys/sendfile.h>
#include <time.h>
#include <pthread.h>
#include <limits.h>

// OpenSSL 
#include <openssl/aes.h>
#include <openssl/rand.h>

// ELF headers
#include <elf.h>

// Dynamic Linking headers
#include <dlfcn.h>

#include "infection.h"

// CONSTANTS

// Maximum size of code
#define CODESIZE 2048

// Number of XOR rounds
#define XOR_ROUNDS 5

// Uncomment the following lines if needed
// #define SYS_CUSTOM_WRITE 
// #define SIGNATURE_SIZE

#endif  // INCLUDES_H
