# Evelyn Zhao
# UID: 117901054
# Directory ID: zevelyn
	
	# reverse_prefix_sum
reverse_prefix_sum:
	# PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8

	# BODY
	lw $t0, ($a0)		# $t0 = *arr
	bne $t0, -1, rec	# if *arr != -1 (not the end of array), go to recursive
	
	li $v0, 0		# return 0
	j ret

rec:
	subu $sp, $sp, 8	# expand stack for 2 variables
	sw $t0, 4($sp)		# save *arr for later
	sw $a0, 8($sp)		# save arr on stack
	
	add $a0, $a0, 4		# go to next value in array

	jal reverse_prefix_sum	# reverse_prefix_sum(arr + 1)

	lw $t0, 4($sp)		# grab *arr
	addu $t0, $v0, $t0	# r = reverse_prefix_sum(arr + 1) + *arr

	lw $t1, 8($sp) 		# grab arr
	sw $t0, ($t1)		# *arr = r
	move $v0, $t0		# return r

ret:	
	# EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra
