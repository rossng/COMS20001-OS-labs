#include <stdio.h>
#include <unistd.h>
#include <string.h>
#ifdef __linux__
    #include <sys/syscall.h>
#elif _WIN32_WINNT
    #include <windows.h>
#else
    #error Platform unsupported.
#endif

void printf_asm(const char * _Format, ...) {
#ifdef __GNUC__
    asm(".section .data             \n"
        "msg: .asciz \"hi\"         \n"
        ".section .text             \n"
        "movq $1, %%rax             \n"
        "movq $1, %%rdi             \n"
        "movq %0, %%rsi             \n" // TODO: figure out why this is broken
        "movq $12, %%rdx            \n"
        "syscall                    \n"
        : /* No output registers */
        : "r" (_Format)
        );
#endif
}

void printf_syscall(const char * _Format, ...) {
#ifdef __linux
    ssize_t s;
    s = syscall(__NR_write, 1, _Format, strlen(_Format));
#elif _WIN32_WINNT

#endif
}

void printf_write(const char * _Format, ...) {
    write(1, _Format, strlen(_Format));
}

int main(int argc, char* argv[]) {
    printf("Hello world\n");
    printf_write("Hello world\n");
    printf_syscall("Hello world\n");
    printf_asm("Hello world\n");
}