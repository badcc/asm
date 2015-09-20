// Anomalies
// Memory restricted to 128 bytes.
// Manual optimizations:
// Decrement at the end of zeroed while loops (possible?)
#include <stdio.h>
#define MAX_BUFFER 128
int main(int argc, char** argv) {
	int MemoryIndex = 128; // TODO(alex): Calculate memory needed.
	FILE* File = fopen(argv[1], "r");	
	char Buffer[MAX_BUFFER];
	int BytesRead = fread(Buffer, 1, MAX_BUFFER, File);
	fclose(File);
	printf("Buffered %d Bytes.\n", BytesRead);
	printf("Memory Size: %d Bytes.\n", MemoryIndex);
	File = fopen("alx_compiled.asm", "w");
	fprintf(File, "global main\nextern printf\nsection .data\nd: db \"%%d\", 0\nc: db \"%%c\", 0\nsection .text\nmain:\n");
	fprintf(File, "push rbp\nmov rbp, rsp\n"); // Stack Frame
	// NOTE(alex): The program's memory is constrainted to stack size at the moment.
	fprintf(File, "mov rax, -%d\nInit:\nmov byte [rbp+rax], 0\ninc rax\njnz Init\n", MemoryIndex);
	//fprintf(File, "sub rsp, %d\n", 4 * 4); // TODO(alex): Dynamic stack allocation.
	char PrintOffset = 0;
	for (int Index = 0; Index < BytesRead; Index++) {
		char c = Buffer[Index];
		switch(c) {
		case '+':
		case '-':
		{
			int Value = 0;
			for (;;) {
				if (c == '+') Value++;
				else if (c == '-') Value--;
				else { Index--; break; }
				c = Buffer[++Index];
			}
			// NOTE(alex): Optimize multiple inc/dec (+/-)
			if (Value == 1) fprintf(File, "inc byte [rbp-%d]\n", MemoryIndex);
			else if (Value == -1) fprintf(File, "dec byte [rbp-%d]\n", MemoryIndex);
			else if (Value > 1) fprintf(File, "add byte [rbp-%d], %d\n", MemoryIndex, Value);
			else if (Value < -1) fprintf(File, "sub byte [rbp-%d], %d\n", MemoryIndex, Value);
			break;
		}
		case ',':
		case '.':
		{
			fprintf(File, "xor rax, rax\nmov rdi, %c\nmovzx rsi, byte [rbp-%d]\n", c == ',' ? 'd' : 'c', MemoryIndex);
			if (PrintOffset)
				fprintf(File, "add rsi, %d\n", PrintOffset);
			fprintf(File, "call printf\n");
			PrintOffset = 0;
			break;
		}
		case '>': MemoryIndex--; break;
		case '<': MemoryIndex++; break;
		case '[':
		{
			fprintf(File, "L%d:\n", Index);
			int Stack = 1;
			int NewIndex = 0;
			for (NewIndex = Index+1; NewIndex < BytesRead; NewIndex++) {
				c = Buffer[NewIndex];
				if (c == ']') Stack--;
				else if (c == '[') Stack++;
				if (Stack == 0) break;	
			}	
			fprintf(File, "cmp byte [rbp-%d], 0\nje L%d\n", MemoryIndex, NewIndex);
			break;
		}
		case ']':
		{
			fprintf(File, "L%d:\n", Index);
			int Stack = 1;
			int NewIndex = 0;
			for (NewIndex = Index-1; NewIndex > 0; NewIndex--) {
				c = Buffer[NewIndex];
				if (c == ']') Stack++;
				else if (c == '[') Stack--;
				if (Stack == 0) break;
			}
			// NOTE(alex): Optimize out cmp if dec at end of loop.
			if (Buffer[Index-1] != '-')
				fprintf(File, "cmp byte [rbp-%d], 0\n", MemoryIndex);
			fprintf(File, "jne L%d\n", NewIndex);	
			break;
		}
		case '/': fprintf(File, "xor rax, rax\nmov rdi, c\nmov rsi, 10\ncall printf\n"); break;
		default: PrintOffset = c;
		}
		if (MemoryIndex <= 0) {
			printf("FATAL ERROR (%d,op:%c): Memory out of bounds.\n", Index, c);
			break;
		}
	}	
	fprintf(File, "pop rbp\n"); // Stack Frame
	//fprintf(File, "add rsp, %d\n", 4 * 4); // TODO(alex): Dynamic stack allocation.
	fprintf(File, "ret\n");
	fclose(File);
	return 0;
}
