.section .rodata
L5:
.int 2
.string " O"
.section .rodata
L6:
.int 2
.string " ."
.section .rodata
L12:
.int 1
.string "\n"
.section .rodata
L15:
.int 1
.string "\n"
.text
.globl L1
.type L1, @function
L1:
L1framesize = 40
subq $40, %rsp
movq %rbx, 0(%rsp)
movq %rbp, 8(%rsp)
movq %r12, 24(%rsp)
movq %r14, 16(%rsp)
movq %rsp, %rax
addq $L1framesize, %rax
movq %rdi, -8(%rax)
L35:
movq $0, %r14
movq $-16, %rbx
movq $-8, %rax
movq %rsp, %rcx
addq $L1framesize, %rcx
addq %rcx, %rax
movq (%rax), %rax
addq %rax, %rbx
movq (%rbx), %r12
movq $1, %rax
subq %rax, %r12
L13:
cmp %r12, %r14
jle L14
L3:
movq $L15, %rdi
call print
jmp L34
L14:
movq $0, %rbp
movq $-16, %rbx
movq $-8, %rax
movq %rsp, %rcx
addq $L1framesize, %rcx
addq %rcx, %rax
movq (%rax), %rax
addq %rax, %rbx
movq (%rbx), %rbx
movq $1, %rax
subq %rax, %rbx
L10:
cmp %rbx, %rbp
jle L11
L4:
movq $L12, %rdi
call print
movq $1, %rax
addq %rax, %r14
jmp L13
L11:
movq $-32, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L1framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $8, %rdx
movq %r14, %rax
imulq %rdx, %rax
addq %rax, %rcx
movq (%rcx), %rax
cmp %rbp, %rax
je L7
L8:
movq $L6, %rdi
L9:
call print
movq $1, %rax
addq %rax, %rbp
jmp L10
L7:
movq $L5, %rdi
jmp L9
L34:
movq 0(%rsp), %rbx
movq 8(%rsp), %rbp
movq 24(%rsp), %r12
movq 16(%rsp), %r14

addq $40, %rsp
ret
.text
.globl L2
.type L2, @function
L2:
L2framesize = 40
subq $40, %rsp
movq %rbx, 8(%rsp)
movq %rbp, 24(%rsp)
movq %r12, 0(%rsp)
movq %r14, 16(%rsp)

movq %rsp, %rax
addq $L2framesize, %rax
movq %rdi, -8(%rax)
movq %rsi, %rbx
L37:
movq $-16, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rax
cmp %rax, %rbx
je L31
L32:
movq $0, %rbp
movq $-16, %rcx

movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %r12
movq $1, %rax
subq %rax, %r12
L29:

cmp %r12, %rbp
jle L30
L16:
L33:
movq $0, %rax
jmp L36
L31:
movq $-8, %rax
movq %rsp, %rbx
addq $L2framesize, %rbx
addq %rbx, %rax
movq (%rax), %rdi
call L1
jmp L33
L30:
movq $0, %rsi
movq $-24, %rcx

movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax

movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx

movq $8, %rdx
movq %rbp, %rax
imulq %rdx, %rax
addq %rax, %rcx
movq (%rcx), %rax
cmp %rsi, %rax
je L17
L18:
movq $0, %rax
L19:
movq $0, %rcx
cmp %rcx, %rax
jne L22
L23:
movq $0, %rax
L24:
movq $0, %rcx
cmp %rcx, %rax
jne L27
L28:
movq $1, %rax
addq %rax, %rbp
jmp L29
L17:
movq $1, %rax
movq $0, %rsi

movq $-40, %rdx

movq $-8, %rcx
movq %rsp, %rdi
addq $L2framesize, %rdi
addq %rdi, %rcx
movq (%rcx), %rcx

addq %rcx, %rdx
movq (%rdx), %rdx

movq %rbp, %rcx
addq %rbx, %rcx
movq $8, %rdi
imulq %rdi, %rcx
addq %rcx, %rdx
movq (%rdx), %rcx
cmp %rsi, %rcx
je L20
L21:
movq $0, %rax
L20:
jmp L19
L22:
movq $1, %rax
movq $0, %rsi
movq $-48, %rdx
movq $-8, %rcx
movq %rsp, %rdi
addq $L2framesize, %rdi
addq %rdi, %rcx
movq (%rcx), %rcx
addq %rcx, %rdx
movq (%rdx), %rdx
movq $7, %rdi
movq %rbp, %rcx
addq %rdi, %rcx
subq %rbx, %rcx
movq $8, %rdi
imulq %rdi, %rcx
addq %rcx, %rdx
movq (%rdx), %rcx
cmp %rsi, %rcx
je L25
L26:
movq $0, %rax
L25:
jmp L24
L27:
movq $-24, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $8, %rdx
movq %rbp, %rax
imulq %rdx, %rax
addq %rax, %rcx
movq $1, %rax
movq %rax, (%rcx)
movq $-40, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq %rbp, %rax
addq %rbx, %rax
movq $8, %rdx
imulq %rdx, %rax
addq %rax, %rcx
movq $1, %rax
movq %rax, (%rcx)
movq $-48, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $7, %rdx
movq %rbp, %rax
addq %rdx, %rax
subq %rbx, %rax
movq $8, %rdx
imulq %rdx, %rax
addq %rax, %rcx
movq $1, %rax
movq %rax, (%rcx)
movq $-32, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $8, %rdx
movq %rbx, %rax
imulq %rdx, %rax
addq %rax, %rcx
movq %rbp, (%rcx)
movq $-8, %rax
movq %rsp, %rcx
addq $L2framesize, %rcx
addq %rcx, %rax
movq (%rax), %rdi
movq $1, %rax
movq %rbx, %rsi
addq %rax, %rsi
call L2
movq $-24, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $8, %rdx
movq %rbp, %rax
imulq %rdx, %rax
addq %rax, %rcx
movq $0, %rax
movq %rax, (%rcx)
movq $-40, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq %rbp, %rax
addq %rbx, %rax
movq $8, %rdx
imulq %rdx, %rax
addq %rax, %rcx
movq $0, %rax
movq %rax, (%rcx)
movq $-48, %rcx
movq $-8, %rax
movq %rsp, %rdx
addq $L2framesize, %rdx
addq %rdx, %rax
movq (%rax), %rax
addq %rax, %rcx
movq (%rcx), %rcx
movq $7, %rdx
movq %rbp, %rax
addq %rdx, %rax
subq %rbx, %rax
movq $8, %rdx
imulq %rdx, %rax
addq %rax, %rcx
movq $0, %rax
movq %rax, (%rcx)
jmp L28
L36:
movq 8(%rsp), %rbx
movq 24(%rsp), %rbp
movq 0(%rsp), %r12
movq 16(%rsp), %r14

addq $40, %rsp
ret
.text
.globl tigermain
.type tigermain, @function
tigermain:
tigermainframesize = 56
subq $56, %rsp
movq %rbx, 0(%rsp)
movq %rsp, %rax
addq $tigermainframesize, %rax
movq %rdi, -8(%rax)
L39:
movq $-16, %rax
movq %rsp, %rbx
addq $tigermainframesize, %rbx
addq %rbx, %rax
movq $8, %rbx
movq %rbx, (%rax)

movq $-24, %rbx
movq %rsp, %rax
addq $tigermainframesize, %rax
addq %rax, %rbx

movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rdi
movq $0, %rsi
call initArray
movq %rax, (%rbx)

movq $-32, %rbx
movq %rsp, %rax
addq $tigermainframesize, %rax
addq %rax, %rbx
movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rdi
movq $0, %rsi
call initArray
movq %rax, (%rbx)

movq $-40, %rbx
movq %rsp, %rax
addq $tigermainframesize, %rax
addq %rax, %rbx

movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rdi

movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rax

addq %rax, %rdi
movq $1, %rax
subq %rax, %rdi

movq $0, %rsi
call initArray
movq %rax, (%rbx)
movq $-48, %rbx
movq %rsp, %rax
addq $tigermainframesize, %rax
addq %rax, %rbx
movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rdi
movq $-16, %rax
movq %rsp, %rcx
addq $tigermainframesize, %rcx
addq %rcx, %rax
movq (%rax), %rax
addq %rax, %rdi
movq $1, %rax
subq %rax, %rdi
movq $0, %rsi
call initArray
movq %rax, (%rbx)
movq %rsp, %rdi
addq $tigermainframesize, %rdi
movq $0, %rsi
call L2
jmp L38
L38:
movq 0(%rsp), %rbx

addq $56, %rsp
ret
