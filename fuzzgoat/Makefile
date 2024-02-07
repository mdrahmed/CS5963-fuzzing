#CC=afl-gcc
#CC=afl-cc 
CC=clang-14 # Use this to run in qemu-mode, as this will instrument dynamically
#CC=afl-c++
#CC=afl-clang-fast # use this to run in llvm or unicorn or frida or nyx mode
DEPS=main.c fuzzgoat.c
ASAN=-fsanitize=address
CFLAGS=-I.
LIBS=-lm

all: $(DEPS)
	$(CC) -o fuzzgoat $(CFLAGS) $^ $(LIBS)
	#$(CC) $(ASAN) -o fuzzgoat_ASAN $(CFLAGS) $^ $(LIBS)

afl: fuzzgoat
	afl-fuzz -i in -o out ./fuzzgoat @@

.PHONY: clean

clean:
	rm ./fuzzgoat ./fuzzgoat_ASAN
