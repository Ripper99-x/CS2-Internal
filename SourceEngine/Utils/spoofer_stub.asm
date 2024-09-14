PUBLIC _spoofer_stub
option casemap :none

; if the line is labelled as MODIFY you need to modify the register it reads or writes to
; valid registers include: rbx, rbp+0x0, rdi, rsi, r12, r13+0x0, r14, r15

_TEXT SEGMENT
_spoofer_stub PROC
    pop r11 ; poping without setting up stack frame, r11 is the return address (the one in our code)
    add rsp, 8 ; skipping callee reserved space
    mov rax, [rsp + 24] ; dereference shell_param
        
    mov r10, [rax] ; load shell_param.trampoline
    mov [rsp], r10 ; store address of trampoline as return address
        
    mov r10, [rax + 8] ; load shell_param.function
    mov [rax + 8], r11 ; store the original return address in shell_param.function
     
    mov [rax + 16], rdi ; preserve register in shell_param.register_ | MODIFY
    lea rdi, fixup ; load fixup address in register | MODIFY
    mov [rax], rdi ; store address of fixup label in shell_param.trampoline | MODIFY
    mov rdi, rax ; preserve address of shell_param in register | MODIFY
        
    jmp r10 ; call shell_param.function
fixup:
    sub rsp, 16
    mov rcx, rdi ; restore address of shell_param | MODIFY
    mov rdi, [rcx + 16] ; restore register from shell_param.register_ | MODIFY
    jmp QWORD PTR [rcx + 8] ; jmp to the original return address
_spoofer_stub ENDP
_TEXT ENDS
END