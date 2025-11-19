global ft_strlen

section .text

ft_strlen:
  xor rax, rax

.loop:
  movzx rcx, byte [rdi + rax]
  test rcx, rcx

  jz .finish_loop
  inc rax
  jmp .loop

.finish_loop:
  ret 
