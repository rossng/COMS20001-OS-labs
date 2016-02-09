.global _start
.text

_start:
    mov $1, %rax    # syscall 1 = write
    mov $1, %rdi    # stdout
    mov $msg, %rsi
    mov $13, %rdx
    syscall

    mov $60, %rax   # syscall 60 = exit
    xor %rdi, %rdi  # get a 0
    syscall

msg:
    .ascii "Hello, world\n"
