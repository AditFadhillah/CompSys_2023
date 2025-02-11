.include "partition.s"

        .text
quicksort:
        # a0 stores array
        # a1 stores start
        # a2 stores end

        # ADD CODE HERE
	addi sp, sp, -20
	sw ra, 16(sp)
	sw s3, 12(sp)
	sw s2, 8(sp)
	sw s1, 4(sp)
	sw s0, 0(sp)

	addi s0, a0, 0
	addi s1, a1, 0
	addi s2, a2, 0
	blt a2, a1, START_GT_END

	jal ra, partition

	addi s3, a0, 0   # pi

	addi a0, s0, 0
	addi a1, s1, 0
	addi a2, s3, -1
	jal ra, quicksort  # quicksort(arr, start, pi - 1);

	addi a0, s0, 0
	addi a1, s3, 1
	addi a2, s2, 0
	jal ra, quicksort  # quicksort(arr, pi + 1, end);

	START_GT_END:

	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw ra, 16(sp)
	addi sp, sp, 20
	jalr x0, ra, 0
