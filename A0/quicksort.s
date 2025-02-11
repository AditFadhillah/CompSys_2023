.include "partition.s"

        .text
quicksort:
        # a0 stores array
        # a1 stores start
        # a2 stores end

       	add s0, zero, a0	# s0 = a0 stores array
       	add s1, zero, a1	# s1 = a1 stores start
       	add s2, zero, a2	# s2 = a2 stores end
       	
       	sub s4, s2, s1		# (end - start)
       	addi s5, zero, 2	# bit shift for integer/word
       	blt s4, s5, return_QS	# if (end - start < 2) branch to return_QS 
      	 
       	jal ra, partition	# run partition	
       	add s3, zero, a4	# q = partition(a, start, end)
      	 
       	addi a0, s0, 0	
       	addi a1, s1, 0
       	addi a2, s3, 0		# prepare to run quicksort(a, start, q)
       
       	jal ra quicksort	# run quicksort(a, start, q) (caller)
       
       	addi a0, s0, 0
     	addi a1, s3, 0
	addi a2, s2, 0		# prepare to run quicksort(a, q, end)
       
      	jal ra, quicksort	# run quicksort(a, q, end) (caller)
       
return_QS:
	jalr zero, 0(ra)	#

       
