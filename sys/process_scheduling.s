.text
#http://www.jamesmolloy.co.uk/tutorial_html/10.-User%20Mode.html

.global switch_to
switch_to:
	movq %rsp, 0(%rdi)
	movq 0(%rsi), %rsp
	ret



.global ring0_ring3 
.extern test_user_function
ring0_ring3:
	 #sti
     movq $0x23, %rax
     movq %rax, %ds
     movq %rax, %es 
     movq %rax, %fs 
     movq %rax, %gs
     #movq %rsp, %rax

     movq %rsp, %rax
     pushq $0x23  
     pushq %rax 
     pushfq
     popq %rax
     orq $0x0000000000000200, %rax 
     pushq %rax 
     pushq $0x2B 
     #leaq test_user_function, %rax
     #push %rax
     # pushq 4096(%rdi) 
     #pushq 4136(%rdi)
     pushq 48(%rdi)
     iretq

# .global _jump_usermode 
# .extern test_user_function
# _jump_usermode:
# 	 #sti
#      movq $0x23, %rax
#      movq %rax, %ds
#      movq %rax, %es 
#      movq %rax, %fs 
#      movq %rax, %gs
#      #movq %rsp, %rax

#      movq %rsp, %rax
#      pushq $0x23  
#      pushq %rax 
#      pushfq
#      popq %rax
#      orq $0x0000000000000200, %rax 
#      pushq %rax 
#      pushq $0x2B 
#      #leaq test_user_function, %rax
#      #push %rax
#      pushq %rdi  
#      iretq