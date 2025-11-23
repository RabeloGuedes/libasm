global  ft_strcmp

section .text

ft_strcmp:
  xor rdx, rdx
  xor rax, rax

.loop:
  mov cl, byte [rdi + rdx]
  mov ch, byte [rsi + rdx]

  cmp cl, ch
  jne .finish

  test cl, cl
  jz .finish

  inc rdx
  jmp .loop

.finish:
  sub   cl, ch
  movsx rax, cl
  ret
