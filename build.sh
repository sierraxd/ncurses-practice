#!/bin/bash

gcc -o simpleMenu $(pkg-config --libs --cflags ncurses) simpleMenu.c
