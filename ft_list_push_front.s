global ft_list_push_front
extern malloc

section .text

ft_list_push_front:
  cmp  rdi, 0    ; check if the pointer to the head pointer is holding NULL as value.
  je   .end
  
  push rdi
  push rsi

  mov  rdi, 16 ; assign struct_size to rdi to call malloc.
  call malloc wrt ..plt

  pop  rsi
  pop  rdi

  cmp  rax, 0              ; check if malloc returned NULL.
  je   .mem_failure

  mov rdx, [rdi]      ; assign the head pointer to rdx.
  mov [rax], rsi      ; assign struct data to data argument.
  mov [rax + 8], rdx  ; assign to the new node the old head pointer.
  mov [rdi], rax      ; assign the pointer to head pointer to the new node address.
  ret

.mem_failure:
  pop rsi
  pop rdi
  ret

.end:
  ret
