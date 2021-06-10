nasm -fwin64 ../test/generated/hello.asm -o ../build/hello.o
gcc ../build/hello.o -o ../build/hello.exe
cd ../build
.\hello.exe
cd ../toolchain