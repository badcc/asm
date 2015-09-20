gcc -std=c99 -o alx alx.c
./alx $1 $2.asm
nasm -felf64 $2.asm
gcc $2.o -o $2
rm $2.o $2.asm
./$2
