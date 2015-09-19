global main
extern printf
section .data
fmt: 
	db "%d", 10
section .text
main:
	push rbp
	mov rbp, rsp
	sub rsp, 8 
	mov dword [rbp-4], 0 
	mov dword [rbp-8], 1	
	mov rax, -2 
	call print
	pop rbp
	ret
print:
	mov rdi, fmt 
	mov rsi, [rbp+4*rax]
	xor rax, rax
	call printf	
	ret
