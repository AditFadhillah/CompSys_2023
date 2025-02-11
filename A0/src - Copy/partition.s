        .text
partition:
        # array in a0
        # p in a1
        # r in a2

        # ADD CODE HERE
	addi sp, sp, -4
	sw ra, 0(sp)
	slli t0, a2, 2   # end * sizeof(int)
	add t0, t0, a0  
	lw t0, 0(t0)     # pivot = arr[end]
	addi t1, a1, -1  # i = (start - 1)

	addi t2, a1, 0   # j = start
LOOP:
	beq t2, a2, LOOP_DONE   # while (j < end)

	slli t3, t2, 2   # j * sizeof(int)
	add a6, t3, a0   # (arr + j)
	lw t3, 0(a6)     # arr[j]

	addi t0, t0, 1   # pivot + 1
	blt t0, t3, CURR_ELEMENT_GTE_PIVOT  # if (pivot <= arr[j])
	addi t1, t1, 1   # i++

	slli t5, t1, 2   # i * sizeof(int)
	add a7, t5, a0   # (arr + i)
	lw t5, 0(a7)     # arr[i]

	sw t5, 0(a6)
	sw t3, 0(a7)     # swap(&arr[i], &arr[j])

CURR_ELEMENT_GTE_PIVOT:
	addi t2, t2, 1   # j++
	beq x0, x0, LOOP
LOOP_DONE:

	addi t5, t1, 1   # i + 1
	addi a5, t5, 0   # Save for return value.
	slli t5, t5, 2   # (i + 1) * sizeof(int)
	add a7, t5, a0   # (arr + (i + 1))
	lw t5, 0(a7)     # arr[i + 1]

	slli t3, a2, 2   # end * sizeof(int)
	add a6, t3, a0   # (arr + end)
	lw t3, 0(a6)     # arr[end]

	sw t5, 0(a6)
	sw t3, 0(a7)     # swap(&arr[i + 1], &arr[end])

	addi a0, a5, 0   # return i + 1

	lw ra, 0(sp)
	addi sp, sp, 4
	jalr x0, ra, 0
