#!/bin/sh
# Compile your actual terminal/main.c to WebAssembly.
# wrap.c #includes ../terminal/main.c; shim.c supplies the few libc functions
# the browser lacks; include/ holds minimal headers so main.c compiles unchanged.
set -e
clang --target=wasm32 -O2 -ffreestanding -nostdlib -Iinclude \
      -Wl,--no-entry -Wl,--export=solve -Wl,--export=answer \
      -o solve.wasm wrap.c shim.c
echo "built solve.wasm ($(wc -c < solve.wasm) bytes)"
