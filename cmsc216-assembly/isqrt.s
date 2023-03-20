# Evelyn Zhao
# UID: 117901054
# Directory ID: zevelyn

	# isqrt
isqrt:
	# PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8

	# BODY
	bge $a0, 2, rec		# if n >= 2, go to recursive
	move $v0, $a0		# n < 2, return n
	j ret

rec:
	subu $sp, $sp, 12	# expand stack for 3 variables
	sw $a0, 4($sp)		# load n onto stack
	srl $a0, $a0, 2		# prepare to call isqrt on  n >> 2

	jal isqrt		# call isqrt on n >> 2

	sll $t0, $v0, 1		# small = isqrt(n >> 2) << 1
	sw $t0, 8($sp)		# store small on stack
	addu $t0, $t0, 1	# large = small + 1
	sw $t0, 12($sp)		# store large on stack

	mul $t0, $t0, $t0	# large * large
	lw $t1, 4($sp)		# load n
	
	bgt $t0, $t1, else	# go to else if large * large > n
	lw $v0, 12($sp)		# return large

	j end

else:
	lw $v0, 8($sp)		# return small

end:
	add $sp, $sp, 12	# shrink stack

ret:	
	# EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra
