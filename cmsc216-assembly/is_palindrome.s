# Evelyn Zhao
# UID: 117901054
# Directory ID: zevelyn

# is_palindrome
is_palindrome:
	# PROLOGUE
	subu $sp, $sp, 8
	sw $ra, 8($sp)
	sw $fp, 4($sp)
	addu $fp, $sp, 8

	# BODY
	subu $sp, $sp, 8	# expand the stack to hold 2 variables
	
	jal strlen 		# $a0 now has strlen

	li $t1, 0		# i = 0
	sw $t1, 4($sp)          # move i to stack

	move $t0, $v0		# grab strlen from $v0
	sw $t0, 8($sp)		# move strlen to stack
			
	la $t1, ($a0) 		# $t1 = pointer to beginning of string
	add $t2, $t1, $t0	# $t2 = point to end of string ('\0')
	sub $t2, $t2, 1		# change $t2 to pointer to end of string (strlen - 1)
	div $t0, $t0, 2		# save strlen / 2 in $t0
			
	j for_loop

for_loop:
	lw $t3, 4($sp)		# grab i ($t3 = i)
	
	bge $t3, $t0, success	# if i >= len / 2, jump to success
	
        lb $t4, ($t1)		# get current char
	lb $t5, ($t2)		# get opposite char
	
	bne $t4, $t5, failure	# if string[i] != string[len - i - 1], return 0
	
	add $t3, $t3, 1		# increment i
	sw $t3, 4($sp)		# update i on stack
	add $t1, $t1, 1		# move left pointer by one
	sub $t2, $t2, 1		# move right pointer by one
	
	j for_loop
	
failure:
	li $v0, 0		# return failure
	j end

success:
	li $v0, 1		# return success

end:	
	# EPILOGUE
	move $sp, $fp
	lw $ra, ($fp)
	lw $fp, -4($sp)
	jr $ra

# strlen(str)

strlen:
        # PROLOGUE
        subu $sp, $sp, 8
        sw $ra, 8($sp)
        sw $fp, 4($sp)
        addu $fp, $sp, 8

        # BODY
        li $t0, 0		# i = 0

        la $t1, 0($a0)		# put the address of the string into $t1

        j loop

loop:
        lb $t2, 0($t1) 		# grab the current char

        beqz $t2, endloop 	# if current char is null byte, go to endloop

        # did not jump to endloop
        add $t0, $t0, 1		# increment i
        add $t1, $t1, 1		# increment pointer
        j loop

endloop:
        move $v0, $t0		# return i (strlen)

        # EPILOGUE
        move $sp, $fp
        lw $ra, 0($fp)
        lw $fp, -4($sp)
        jr $ra
