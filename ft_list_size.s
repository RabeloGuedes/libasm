global ft_list_size

section .text

ft_list_size:
  xor rcx, rcx  ; set rcx to 0.

.loop:
  cmp rdi, 0    ; check if the pointer is NULL.
  je .end 
  
  inc rcx
  mov rdi, [rdi + 8]  ; set rdi to the next pointer.
  jmp .loop

.end:
  mov rax, rcx
  ret
