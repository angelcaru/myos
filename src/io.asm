bits 64

%macro INFN 2
global %1
%1:
    xor rax, rax
    push rdx
    mov rdx, rdi
    in %2, dx
    pop rdx
    ret
%endmacro

%macro OUTFN 2
global %1
%1:
    push rdx
    mov rdx, rdi
    mov rax, rsi
    out dx, %2
    pop rdx
    ret
%endmacro

INFN in8, al
INFN in16, ax
INFN in32, eax

OUTFN out8, al
OUTFN out16, ax
OUTFN out32, eax
