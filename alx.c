#include <stdio.h>
#define MAX_BUFFER 128
int main(int argc, char** argv) {
	int MemoryIndex = 1; // dword, 4 bytes.
	FILE* File = fopen("test.alx", "r");	
	char Buffer[MAX_BUFFER];
	int BytesRead = fread(Buffer, 1, MAX_BUFFER, File);
	fclose(File);
	printf("Buffered %d Bytes.\n", BytesRead);
	//int MemoryCells = 2;
	int LabelCounter = 0;
	int LabelIndex = 0;
	// TODO(alex): Memory size calculation.
	// TODO(alex): Init local variables to 0.
	//for (int Index = 0; Index < BytesRead; Index++) {
		//if (Buffer[Index] == '>') MemoryCells++;	
		//if (Buffer[Index] == '<') MemoryCells--;
	//}
	// TODO(alex): Disassemble to see if we need 4 bytes per char (4 byte pointer?)
	//printf("Predicted memory allocation size: %d Bytes.\n", MemoryCells * 4); 
	File = fopen("alx_compiled.asm", "w");
	fprintf(File, "global main\nextern printf\nsection .data\nfmt: db \"%%d\", 10\nsection .text\nmain:\n");
	fprintf(File, "push rbp\nmov rbp, rsp\n"); // Stack Frame
	fprintf(File, "sub rsp, %d\n", 4 * 4); // TODO(alex): Dynamic stack allocation.
	for (int Index = 0; Index < BytesRead; Index++) {
		char c = Buffer[Index];
		// TODO(alex): Optimize multiple arithmetic with single instruction.
		switch(c) {
		case '+':
			fprintf(File, "inc dword [rbp-%d]\n", MemoryIndex * 4);
			break;
		case '-':
			fprintf(File, "dec dword [rbp-%d]\n", MemoryIndex * 4);
			break;
		case '.':
			fprintf(File, "mov rdi, fmt\nmov rsi, [rbp-%d]\nxor rax, rax\ncall printf\n", MemoryIndex * 4);
			break;
		case '>': MemoryIndex++; break;
		case '<': MemoryIndex--; break;
		case '[':
			fprintf(File, "L%d:\n", LabelCounter++);
			LabelIndex++;
			break;
		case ']':
			fprintf(File, "cmp dword [rbp-%d], 0\n", MemoryIndex * 4);
			// TODO(alex): Proper jump label calculation.
			fprintf(File, "jne L%d\n", LabelCounter-1);	
			break;
		}
	}	
	fprintf(File, "pop rbp\n"); // Stack Frame
	fprintf(File, "add rsp, %d\n", 4 * 4); // TODO(alex): Dynamic stack allocation.
	fprintf(File, "ret\n");
	fclose(File);
	return 0;
}
