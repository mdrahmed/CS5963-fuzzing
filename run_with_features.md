## Features - different modes
1. llvm-mode: just compile it with `afl-clang` and run it.
```
afl-clang -o fuzzgoat_afl fuzzgoat.c
afl-fuzz -i in -o out ./fuzzgoat_afl
```
2. Run in Qemu-mode and enable bit flips with `-Q` and `-D`
```
afl-fuzz -i in -D -o out ./fuzzgoat -Q
```
3. Run in unicorn-mode with `-U`
```
afl-clang-fast -o fuzzgoat_aflunicorn fuzzgoat.c
afl-fuzz -U -i in -o out ./fuzzgoat_aflunicorn
```
To check the support for unicorn-mode with `./fuzzgoat_aflunicorn -h` and check the output for Unicorn-related options to confirm that Unicorn mode is supported.

4. Frida-mode: install Frida first
```
afl-clang-fast -o fuzz_target_frida afl_fuzz_target.c
afl-fuzz -U -i in -o out -- ./fuzz_target_frida
```

5. nyx-mode: install nyx first
```
afl-clang-fast -o fuzz_target_nyx afl_fuzz_target.c
afl-fuzz -Y -i in -o out -- ./fuzz_target_nyx
```
