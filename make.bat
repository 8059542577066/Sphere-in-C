@echo off
rem gcc 5.1.0 tdm64-1
gcc sphere.c -Wl,--stack,83886080 -O3 -o sphere.exe
pause