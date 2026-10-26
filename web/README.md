# Twenty-Four (web)

A browser version of the 24 game that runs your **actual** terminal solver: the
build compiles `../terminal/main.c` to WebAssembly and the page calls its
`find()`. main.c is not edited or copied.

## Files

| File | What it is |
|------|------------|
| `index.html` | The UI — deals the cards, runs the interactive play, and calls the engine for hints, "show solution", and the solvable-deal check. |
| `wrap.c` | `#include`s `../terminal/main.c` unchanged, and exports `solve(a,b,c,d)` / `answer()` so the page can call main.c's `find()`. |
| `shim.c` | The few C-library functions the browser lacks — `fabs`, `snprintf`, `memcpy`/`memset`, plus harmless stubs for the console/dealing parts of main.c the web build never calls. |
| `include/` | Minimal headers (`stdio.h`, `stdlib.h`, …) so `main.c` compiles for wasm without a full libc. |
| `solve.wasm` | The compiled result (checked in). |
| `build.sh` | Rebuilds `solve.wasm` from `main.c`. |

## It really is your main.c

The solver running here is `terminal/main.c`'s own `find()` — the same search,
compiled to WebAssembly. Nothing about finding the 24 is reimplemented: `wrap.c`
only adds an entry point, and `shim.c` fills in library functions the browser
doesn't provide (WebAssembly has no `printf`/`snprintf`/`fabs`). The only reason
those shims exist is that a browser has no C standard library — the logic is all
yours.

## Build

Needs clang with the wasm32 target and wasm-ld (LLVM 16+):

    ./build.sh

## Run

Serve the folder (browsers won't `fetch()` the `.wasm` from a `file://` path):

    python3 -m http.server 8000
    # open http://localhost:8000

Or host it on GitHub Pages — it's a static site. If a browser can't run
WebAssembly, the page falls back to a JavaScript solver so it still plays.
