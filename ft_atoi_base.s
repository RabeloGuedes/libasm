global ft_atoi_base

section .text

ft_atoi_base:
  cmp rdi, 0        ; check for a null pointer
  je .failed_check
  cmp rsi, 0        ; check for a null pointer
  je .failed_check
  jmp .check_base

.finished_check:
  cmp rdx, 1
  jle .failed_check
  mov r12, rdx    ; NOW store the correct base length in r12

.convert:
  xor rdx, rdx  ; set rdx to zero to iterate over the str
  xor r14, r14  ; set r14 to zero, it will be used to store the result
  xor r15, r15  ; set r15 to zero, it will be used for sign (0 = positive, 1 = negative)

.skip_whitespace:
  mov al, byte [rdi + rdx]
  cmp al, 32      ; space
  je .inc_str
  cmp al, 9       ; tab
  je .inc_str
  cmp al, 10      ; newline
  je .inc_str
  cmp al, 11      ; vertical tab
  je .inc_str
  cmp al, 12      ; form feed
  je .inc_str
  cmp al, 13      ; carriage return
  je .inc_str
  jmp .handle_signs

.inc_str:
  inc rdx
  jmp .skip_whitespace

.handle_signs:
  mov al, byte [rdi + rdx]
  cmp al, '+'
  je .check_first_sign
  cmp al, '-'
  je .set_negative
  jmp .convert_loop

.check_first_sign:
  cmp r15, 0      ; check if we already saw a sign
  jne .done       ; if yes, stop conversion (return 0)
  mov r15, 2      ; mark that we saw a '+' (neutral sign)
  inc rdx
  jmp .convert_loop

.set_negative:
  cmp r15, 0      ; check if we already saw a sign
  jne .done       ; if yes, stop conversion (return 0)
  mov r15, 1      ; mark negative
  inc rdx
  jmp .convert_loop

.convert_loop:
  cmp byte [rdi + rdx], byte 0
  je  .done 
  jmp .get_base_index

.got_index:
  imul  r14, r12  ; multiplies the accumulated value by the length (base)  
  add   r14, r13  ; adds the new index to the accumulator
  inc   rdx       ; add one to rdx to iterate over the string (equivalent to i++)
  jmp .convert_loop
  
.get_base_index:
  xor rcx, rcx  ; set rcx to zero to store index of the sysmbol in the base

.get_base_index_loop:
  cmp byte [rsi + rcx], byte 0
  je  .done               ; character not found in base, stop conversion

  mov al, byte [rdi + rdx]
  cmp byte [rsi + rcx], al
  je  .found_char
  
  inc rcx
  jmp .get_base_index_loop

.found_char:
  mov r13, rcx    ; NOW store the correct index in r13
  jmp .got_index

.check_base:
  xor   rdx, rdx

.check_base_loop_1:
  cmp byte [rsi + rdx], byte 0 ; check for null terminator
  je .finished_check

  cmp byte [rsi + rdx], byte '+' ; check for plus sign
  je .failed_check

  cmp byte [rsi + rdx], byte '-' ; check for minus sign
  je .failed_check

  cmp byte [rsi + rdx], byte ' ' ; check for space character
  je .failed_check

  cmp byte [rsi + rdx], 9  ; check for tab character
  je .failed_check

  cmp byte [rsi + rdx], 10 ; check for newline character
  je .failed_check

  cmp byte [rsi + rdx], 11 ; check for vertical tab character
  je .failed_check

  cmp byte [rsi + rdx], 12 ; check for form feed character
  je .failed_check

  cmp byte [rsi + rdx], 13 ; check for carriage return character
  je  .failed_check

  xor rcx, rcx          ; set rcx to zero to start second loop

.check_base_loop_2:
  cmp rcx, rdx          ; check if both indexes are equal
  je .inc_rdx

  mov al, byte [rsi + rdx]
  cmp al, byte [rsi + rcx] ; check for duplicate characters
  je .failed_check

  inc rcx
  jmp .check_base_loop_2


.inc_rdx:
  inc rdx
  jmp .check_base_loop_1

.failed_check:
  mov rax, 0
  ret

.done:
  mov rax, r14
  cmp r15, 1      ; check if negative (1 = negative, 0 or 2 = positive)
  jne .return
  neg rax         ; negate the result if r15 is 1
.return:
  ret
