#!/bin/bash

set -xe

clang cdong.c deps/libraylib.a -o build/cdong  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL 