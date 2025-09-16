section .data
SRef1 :
db "hello there!"
SRef1_L : equ $-SRef1


section .text 
global _start
_start:

mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L
syscall



mov rax , 60
mov rdi , 0
syscall