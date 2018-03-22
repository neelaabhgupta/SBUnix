.text


#//Citation - https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf
#isr handler for interuppt 32
.global isr_num32
isr_num32:
	#pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rsi
	pushq %rdi
	#pushq %rsp
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	#pushq %r13
	pushq %r14
	pushq %r15
	call isr_handler_32
	popq %r15
	popq %r14
	#popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	#popq %rsp
	popq %rdi
	popq %rsi
	popq %rbx
	popq %rdx
	popq %rcx
	#popq %rax
	iretq


#isr handler for interuppt 33
.global isr_num33
isr_num33:
	#pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rsi
	pushq %rdi
	#pushq %rsp
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	#pushq %r13
	pushq %r14
	pushq %r15
	call isr_handler_33
	popq %r15
	popq %r14
	#popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	#popq %rsp
	popq %rdi
	popq %rsi
	popq %rbx
	popq %rdx
	popq %rcx
	#popq %rax
	iretq


#isr handler for interuppt 14
.global isr_num14
isr_num14:
	#pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rsi
	pushq %rdi
	#pushq %rsp
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	#pushq %r13
	pushq %r14
	pushq %r15
	call isr_handler_14
	popq %r15
	popq %r14
	#popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	#popq %rsp	
	popq %rdi
	popq %rsi
	popq %rbx
	popq %rdx
	popq %rcx
	#popq %rax
	addq $8, %rsp
	iretq


#isr handler for interuppt 8
.global isr_num8
isr_num8:
	#pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rsi
	pushq %rdi
	#pushq %rsp
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	#pushq %r13
	pushq %r14
	pushq %r15
	call isr_handler_8
	popq %r15
	popq %r14
	#popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	#popq %rsp
	popq %rdi
	popq %rsi
	popq %rbx
	popq %rdx
	popq %rcx
	#popq %rax
	iretq

#y, %rdi​,​ ​%rsi​, %rdx​, %rcx​, %r8​, and %r9
.global isr_num128
isr_num128:
	pushq %rdi
	#pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rsi
	# pushq %rdi
	#pushq %rsp
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	#pushq %r11
	pushq %r12
	#pushq %r13
	pushq %r14
	#pushq %r15
	call isr_handler_128
	#popq %r15
	popq %r14
	#popq %r13
	popq %r12
	#popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	#popq %rsp
	#popq %rdi
	popq %rsi
	popq %rbx
	popq %rdx
	popq %rcx
	#popq %rax
	popq %rdi
	iretq

