#!/bin/sh -xe

cmake -B build -S .
cmake --build build
build/test_main
