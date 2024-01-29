// ANTI_DISASSEMBLER_H

#ifndef ANTI_DISASSEMBLER_H
#define ANTI_DISASSEMBLER_H

#ifdef __x86_64__
#define X86_64_MORPH \
    __asm__ volatile(\
        "xorq     %rax, %rax     \n"\
        "jz       0f             \n"\
        "call     1f             \n"\
        "0:                      \n"\
        "pop      %rax           \n"\
        "1:                      \n"\
        "nop                      \n"\
        "nop                      \n"\
        "nop                      \n"\
        "nop                      \n"\
    );
#else
#define X86_MORPH \
    __asm__ volatile(\
        "xorl     %eax, %eax     \n"\
        "jz       0f             \n"\
        "call     1f             \n"\
        "0:                      \n"\
        "pop      %eax           \n"\
        "1:                      \n"\
        "nop                      \n"\
        "nop                      \n"\
        "nop                      \n"\
        "nop                      \n"\
    );
#endif

#endif  // ANTI_DISASSEMBLER_H
