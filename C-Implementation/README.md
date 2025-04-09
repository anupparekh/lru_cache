# lru_chache
 
# Steps to create shared library:
gcc -c -Wall -Werror -fpic cache.c -I.
gcc -shared -o libcache.so cache.o

# Steps to build executable
gcc -o lru main.cpp -L. -lcache

