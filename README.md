# Twenty-Four (24 Points)

A simple implementation of the classic 24 game, playable in both the terminal and a web browser. Both versions use the **same solver**, implemented in `terminal/main.c`.

## Versions

- **Terminal (`terminal/`)**  
  A command-line version that deals four cards and can show you a solution. The solver is implemented in `find()` in `main.c`. To build and run it:

  ```bash
  gcc main.c -o 24points -lm
  ```

- **Web (`web/`)**  
  A browser-based version with a graphical interface. It compiles `terminal/main.c` to WebAssembly, so it uses the same `find()` function as the terminal version rather than a separate JavaScript solver. `web/wrap.c` includes `main.c` directly, and `web/shim.c` provides the small amount of C library support needed to run it in the browser. The interface lives in `web/index.html`.

The goal is the same in both versions: use all four cards exactly once, along with `+`, `-`, `*`, and `/`, to make 24.

For instructions on building and running the web version, see [`web/README.md`](web/README.md).