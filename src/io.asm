bits 64

%macro INFN 2
global %1
%1:
    xor rax, rax
    push rdi
    push rdx
    in %2, dx
    pop rdx
    pop rdi
    ret
%endmacro

%macro OUTFN 2
global %1
%1:
    push rdi
    push rdx
    out dx, %2
    pop rdx
    pop rdi
    ret
%endmacro

INFN in8, al
INFN in16, ax
INFN in32, eax

OUTFN out8, al
OUTFN out16, ax
OUTFN out32, eax
