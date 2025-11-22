global ft_strcpy

section .text

ft_strcpy:
  xor   rdx, rdx
  mov   rax, rdi

.loop:
  mov   cl, byte [rsi + rdx]
  mov   byte [rdi + rdx], cl
  test  cl, cl
  jz    .finish
  inc   rdx
  jmp   .loop

.finish:
  ret
