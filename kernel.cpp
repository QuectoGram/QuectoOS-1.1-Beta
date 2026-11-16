#include <stdint.h>
#include <stddef.h>
// Making I/O
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}
// Making Kernel
extern "C" __attribute__((section(".text.kernel_main"))) void kernel_main(void) {
    (void)inb(0x3DA);
    outb(0x3C0, 0x10);
    uint8_t mode = inb(0x3C1);
    mode &= ~0x08;
    (void)inb(0x3DA);
    outb(0x3C0, 0x10);
    outb(0x3C0, mode);
    (void)inb(0x3DA);
    outb(0x3C0, 0x20);
    volatile uint8_t *video_memory = reinterpret_cast<volatile uint8_t *>(static_cast<uintptr_t>(0xB8000));
    for (size_t i = 0; i < 80 * 25; i++) {
        video_memory[i * 2]     = ' ';
        video_memory[i * 2 + 1] = 0x1F;
    }
    // Create Texts
    const char str[] = "Welcome to QuectoOS";
    for (size_t i = 0; str[i] != '\0'; i++) {
        video_memory[i * 2]     = static_cast<uint8_t>(str[i]);
        video_memory[i * 2 + 1] = 0x1F;
    }
    const char str2[] = "---------------------------";
    const size_t row2_offset = 80 * 2;
    for (size_t n = 0; str2[n] != '\0'; n++) {
        video_memory[row2_offset + (n * 2)]     = static_cast<uint8_t>(str2[n]);
        video_memory[row2_offset + (n * 2) + 1] = 0x1F;
    }
    const char str1[] = "Made by U123573";
    const size_t row1_offset = 80 * 2 * 2;
    for (size_t j = 0; str1[j] != '\0'; j++) {
        video_memory[row1_offset + (j * 2)]     = static_cast<uint8_t>(str1[j]);
        video_memory[row1_offset + (j * 2) + 1] = 0x1F;
    }
    // Create Cursor
    outb(0x3D4, 0x0A); outb(0x3D5, 0x20);
    outb(0x64, 0xA8); while (inb(0x64) & 2); outb(0x64, 0xD4);
    while (inb(0x64) & 2); outb(0x60, 0xF4); while (!(inb(0x64) & 1)); (void)inb(0x60);
    int x = 320, y = 200, s = 0, p = 2000;
    uint8_t b[3], c = ' ', a = 0x1F;
    video_memory[p] = '|'; video_memory[p + 1] = 0x1E;
    while (1) {
        if ((inb(0x64) & 0x21) == 0x21 && (b[s++] = inb(0x60), s == 3)) {
            s = 0; video_memory[p] = c; video_memory[p + 1] = a;
            x += static_cast<int8_t>(b[1]); y -= static_cast<int8_t>(b[2]);
            x = x < 0 ? 0 : (x > 639 ? 639 : x);
            y = y < 0 ? 0 : (y > 399 ? 399 : y);
            p = ((y / 16) * 80 + (x / 8)) * 2;
            c = video_memory[p]; a = video_memory[p + 1];
            video_memory[p] = '|'; video_memory[p + 1] = 0x1E;
        }
    }
}
