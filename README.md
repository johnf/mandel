# XMand

A Mandelbrot set viewer for X11, written in C against plain Xlib.

This is one of the first C projects I wrote while I was at university, back in
1996. It is kept here mostly for nostalgia. The code has had a light tidy-up
since (indentation, compiler warnings, 24-bit colour) but is otherwise as it
was.

## Building

You need a C compiler, the Xlib development headers and `make`. On
Debian/Ubuntu:

```sh
sudo apt install build-essential libx11-dev libxext-dev
make
```

The committed `Makefile` is generated from the `Imakefile`. To regenerate it
for your system, install `xutils-dev` and run `xmkmf`.

## Running

```sh
./XMand
```

Options:

| Option           | Description                                  |
| ---------------- | -------------------------------------------- |
| `-h`             | Print help                                   |
| `-display <d>`   | X display to connect to                      |
| `-width <w>`     | Window width in pixels (default 1280)        |
| `-height <h>`    | Window height in pixels (default 1280)       |
| `-root`          | Draw on the root window                      |

Once the set has been drawn, the colours cycle until you press a key. After
that you can zoom: click once to set one corner of a rectangle, move the
mouse, and click again to set the opposite corner. The selected region is then
redrawn.
