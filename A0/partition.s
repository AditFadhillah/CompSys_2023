        .text
partition:
        # a0 = int array[p]
        # a1 = int p
        # a2 = int r
	
	slli t0, a1, 2		# bit shift for integer/word (p)
	add t0, t0, a0		# update memory address
	lw t0, 0(t0)		# define p in a word array (pivot = array[p])
	
	addi t1, a1, -1 	# i = p - 1
	
	addi t2, a2, 0		# j = r

if:
	blt t1, t2, loop0	# if i < j branch to swap
	jal zero, return_P	# else jump to return (callee???)

loop0:
	addi t2, t2, -1		# j--
	
	slli t3, t2, 2		# bit shift for integer/word
	add a6, t3, a0		# update memory address
	lw t6, 0(a6)		# define array[j] dereference address to get integer
	
	blt t0, t6, loop0	# if pivot < array[j] back to loop0
	
loop1:	
	
	addi t1, t1, 1		# i++
	
	slli t4, t1, 2		# bit shift for integer/word
	add a7, t4, a0		# update memory address
	lw t5, 0(a7)		# define array[i] dereference address to get integer

	blt t5, t0, loop1	# if array[i] < pivot back to loop1
	
	blt t1, t2, swap 	# swap
	
	jal zero, return_P

	
swap:
	
	sw t5, 0(a6)		# array[i] = array[j]
	sw t6, 0(a7)		# array[j] = array[i]
	
	

	jal zero, loop0		# jump back to loop0 (callee???)

return_P:
	addi a5, t2, 1
	addi a4, a5, 0		# (j + 1) insert j in array[p] 
	#jalr zero, 0(ra)	# (callee???)


