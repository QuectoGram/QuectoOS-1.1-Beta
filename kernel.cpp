#include <stdint.h>
#include <stddef.h>
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
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
    while (1) {
        __asm__ volatile ("hlt");
    }
}
