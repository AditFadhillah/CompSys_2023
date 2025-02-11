.include "partition.s"

        .text
         
           
quicksort:
        # a0 stores array
        # a1 stores start
        # a2 stores end

        # ADD CODE HERE
        addi sp, sp, -16 #make 4 spaces in stack INQUICK
        sw ra, 12(sp)#store ra INQUICK
        sw a0, 8(sp) #store array adress in stack INQUICK
        sw a1, 4(sp) #store start in stack INQUICK
        sw a2, 0(sp) #store end in stack INQUICK
        
        sub s2, a2, a1 #end-start INQUICK
        li, s3, 2 #make two INQUICK
        blt s2, s3, return #return if start-end < 2 INQUICK
        
        
        jal ra, partition #Call partition on start and end INQUICK
        
        add s4, a0, zero #save partition result INQUICK
        lw a0, 8(sp) #restore a0 adress INQUICK
        
        
        #quicksort (array, start, q) INQUICK
    	add a2, s4, zero # end = q INQUICK
        jal ra, quicksort #quicksort end = q INQUICK
        
        
        #quicksort (array, q, end) INQUICK
	lw a2, 0(sp)#restore end
        add a1, s4, zero #start = q INQUICK
        jal ra, quicksort #quicksort start = q INQUICK
        
        

return:
	
	lw ra, 12(sp)#restore ra INQUICK
	lw a0, 8(sp)#restore array adress INQUICK
        lw a1, 4(sp)#restore start or q INQUICK
        lw a2, 0(sp)#restore end or q INQUICK
        
        addi sp, sp, 20#delete stack INQUICK
        jalr zero, ra, 0#go back to ra INQUICK

quickdone:
