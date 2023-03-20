# Evelyn Zhao
# UID: 117901054
# Directory ID: zevelyn
	
	# fibonacci

fibonacci:
	# PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp 4($sp)
	addu $fp, $sp, 8

	# BODY
	bgt $a0, 1, rec 	# if not base case (0 or 1), go to rec
	
	move $v0, $a0		# base case (0 or 1)
	j ret

rec:
	subu $sp, $sp, 8	# make room on stack for 2 variables
	sw $a0, 4($sp)		# save the current n
	
	sub $a0, $a0, 1         # n - 1
	jal fibonacci		# call fib(n - 1) & stores the value in $v0
	lw $a0, 4($sp)		# restore n
	
	sw $v0, 8($sp)		# save return value in stack

	sub $a0, $a0, 2		# n - 2
	jal fibonacci		# fib(n - 2)
	
	lw $t0, 8($sp)		# get return from fib(n-1)
	addu $v0, $t0, $v0 	# fib(n - 1) + fib(n - 2)
	
ret:	
	# EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra
