make
./tiger-compiler $1
gcc -Wl,--wrap,getchar -m64 $1.s runtime.c -o test.out