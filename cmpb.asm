global main
extern printf
section .data
fmt: 
	db "%d", 10
section .text
main:
	push rbp
	mov rbp, rsp

	mov rax, [rsi+8]
	cmp byte [rax], byte '+'
	sete al
	mov rdi, fmt
	movzx rsi, al
	xor rax, rax
	call printf

	pop rbp
	ret

; Calling convention
; RDI, RSI, RDX, RCX
; R0  R1  R2  R3  R4  R5  R6  R7  R8  R9  R10  R11  R12  R13  R14  R15
; RAX RCX RDX RBX RSP RBP RSI RDI
; printf is varargs, set rax to 0 before call.
; Return in eax
; sub rsp by number of rbp that you use.