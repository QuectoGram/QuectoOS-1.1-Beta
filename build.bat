@echo off
setlocal
echo 1 Building 16 bit Bootloader
nasm -f bin boot.asm -o boot.bin
if errorlevel 1 goto error
echo 2 Building 64 bit Entry & C++ Kernel
nasm -f bin entry.asm -o entry.bin
if errorlevel 1 goto error
g++ -m64 -ffreestanding -fno-pie -fno-stack-protector -fno-rtti -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -mno-red-zone -mcmodel=small -nostdlib -c kernel.cpp -o kernel.o
if errorlevel 1 goto error
g++ -m64 -nostdlib -Wl,-nmagic -Wl,--image-base,0x10200 -Wl,--section-alignment,16 -Wl,--file-alignment,16 -T linker.ld kernel.o -o kernel.exe
if errorlevel 1 goto error
objcopy -O binary -j .text -j .rodata -j .data kernel.exe kernel.bin
if errorlevel 1 goto error
echo 3 Generating QuectoOS 1.1 Beta img (11 KB)
copy /b boot.bin + entry.bin + kernel.bin "QuectoOS-1.1-Beta.os" > nul
powershell -Command "$f = [System.IO.File]::OpenWrite('QuectoOS-1.1-Beta.os'); if ($f.Length -lt 11264) { $f.SetLength(11264) }; $f.Close()"
echo.
echo SUCCESS Generated QuectoOS 1.1 Beta
pause
exit /b 0
:error
echo.
echo Build failed
pause
exit /b 1
