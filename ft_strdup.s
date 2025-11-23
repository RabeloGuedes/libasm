global  ft_strdup
extern malloc, ft_strlen, ft_strcpy

section .text

ft_strdup:
  push  rdi
  call  ft_strlen
  mov   rdi, rax
  inc   rdi
  call  malloc wrt ..plt

  test  rax, rax
  jz    .malloc_failed

  mov   rdi, rax
  pop   rsi
  call  ft_strcpy
  ret

.malloc_failed:
  pop   rdi
  ret
