global main
extern printf
section .data
fmt: 
	db "%d", 10
section .text
main:
	mov rax, [rsi+8]
	cmp byte [rax], byte '+'
	sete al
	mov rdi, fmt
	movzx rsi, al
	xor rax, rax
	call printf

	ret
