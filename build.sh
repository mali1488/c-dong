#!/bin/bash

set -xe

FRAMEWORK_FLAGS="-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL"
CFLAGS="-Wall -Wextra -pedantic -std=c11 -ggdb"
clang src/common.c src/cdong.c deps/libraylib.a -o build/cdong $CFLAGS $FRAMEWORK_FLAGS -Ideps -Isrc