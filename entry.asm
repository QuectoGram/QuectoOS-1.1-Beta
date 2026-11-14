[BITS 16]
[ORG 0x10000]

start:
    cli
    mov ax, 0x1000
    mov ds, ax

    in al, 0x92
    or al, 2
    out 0x92, al

    xor ax, ax
    mov es, ax
    mov di, 0x1000
    mov cx, 0x1800
    rep stosw

    mov dword [es:0x1000], 0x2003
    mov dword [es:0x2000], 0x3003
    mov dword [es:0x3000], 0x0083

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov eax, 0x1000
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    lgdt [gdt_descriptor - 0x10000]
    mov eax, cr0
    or eax, (1 << 31) | (1 << 0)
    mov cr0, eax

    o32 jmp dword 0x08:init_lm

[BITS 64]
init_lm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, 0x90000
    sub rsp, 32

    mov rax, 0x10200
    call rax

    cli
.hang:
    hlt
    jmp .hang

align 8
gdt_start:
    dq 0x0000000000000000
    dq 0x00209A0000000000
    dq 0x0000920000000000
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dq gdt_start

times 512-($-$$) db 0
