.text

partition:
        # array in a0
        # p in a1
        # r in a2

        # ADD CODE HERE
        addi t1, a1, 0 #save p in t1
        slli t1, t1, 2 #multiply by 4
        add t1, t1, a0 #correct to array
        
        addi t2, a2, 0 #save r in t2
        slli t2, t2, 2#multiplyby 4
        add t2, t2, a0 #correct to array
        
 
        
        lw t0, 0(t1) #store pivot in t0
        addi t1, t1, -4 #I-1
	
	
loop1: #while
	blt  t1, t2, loop2 #gå til første indre lop
	jal zero, Done #done


loop2: #j things
	addi t2, t2, -4 #j-1
	lw t3, 0(t2) #værdi i j
	blt t0, t3, loop2 #loop tilbage
	beq zero, zero, loop3 #næste loop

loop3: #i things
	addi t1, t1, 4 # i+1
	lw t3, 0(t1) #værdi i i
	blt t3, t0, loop3 #loop tilbage
	blt t1, t2, swap #byt
	jal zero, Done


swap:
	lw t3, 0(t1) #t3 = array[I]
	lw t4, 0(t2) #t4 = array[J]
	sw t4, 0(t1) #array[I] = t3
	sw t3, 0(t2) #array[J] = t2
	beq zero, zero, loop1 #tilbage til store loop


Done:
	addi t0, t2, 4 #j+1, 4 da vi har words
	sub t0, t0, a0 #dekorriger for array
	srli t0, t0, 2 #dekorriger for words
	addi a0, t0, 0 #læg t0 i a0
	jalr, zero, ra, 0 #gå tilbage til ra, altså called
	
