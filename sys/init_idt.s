

.text



.global init_lidt
init_lidt:
	
	lidt (%rdi)
	retq
