gcc -std=c99 -o alx alx.c
./alx $1 
nasm -felf64 alx_compiled.asm
gcc alx_compiled.o -o alx_compiled
./alx_compiled
