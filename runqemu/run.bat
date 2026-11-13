@echo off
qemu-system-i386 -machine pc -drive file=QuectoOS-1.1-Beta.os,format=raw,if=floppy
pause
