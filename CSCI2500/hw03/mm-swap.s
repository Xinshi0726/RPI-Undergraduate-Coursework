#############################################################################	
#############################################################################
## Assignment 3: Your Name Goes Here!
#############################################################################
#############################################################################

#############################################################################
#############################################################################
## Data segment
#############################################################################
#############################################################################	
		.data
matrix_a:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
matrix_b:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
swap:	        .word 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0	
result:	        .word 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0
	
newline:        .asciiz "\n"
tab:	        .asciiz "\t"
		

#############################################################################	
#############################################################################
## Text segment
#############################################################################
#############################################################################
	
		.text		       # this is program code
		.align 2	       # instructions must be on word boundaries
		.globl main	       # main is a global label
	        .globl matrix_swap
	        .globl matrix_multiply
	        .globl matrix_print

#############################################################################	
main:
#############################################################################
		# alloc stack and store $ra
		sub $sp, $sp, 4
		sw $ra, 0($sp)

	    # load matrix A, swap and N into arg regs
	    la $a0, matrix_a
        la $a1, swap
        li $a2, 4
		jal matrix_swap
	
		# load A, B and C and N into arg regs
		la $a0, swap
		la $a1, matrix_b
		la $a2, result
	    li $a3, 4
		jal matrix_multiply

		la $a0, swap
		jal matrix_print

		# restore $ra, free stack and return
		lw $ra, 0($sp)
		add $sp, $sp, 4
		jr $ra

##############################################################################
matrix_swap: 
##############################################################################
# does matrix swap for a specific 4x4 matrix like assignment 1.
# This is a leaf function and can be done without any stack allocation.
# It is ok if you use the stack.
		sub $sp, $sp, 20
	    sw $ra, 0($sp)
	    sw $s0, 4($sp)
        sw $s1, 8($sp)
		sw $a0, 12($sp)
		sw $a1, 16($sp)

		add $t0, $zero, $a0 # reg of matrix_a
		add $t1, $zero, $a1 # reg of swap

		add $s0, $zero, $zero # i=0
copy1:		
		# if (i< size)
		slti $t2, $s0, 4 # i < 4
		beq $t2, $zero, swap1 # end
		add $s1, $zero, $zero #j=0

copy2:  		
		slti $t3, $s1, $a3   #j<4
		beq $t3, $zero, copy3 

		mul $t5, $s0, 4 # t5 = i*4
		add $t5, $t5, $s1 #t5+=j
		mul $t5, $t5, 4

		add $t6, $t5, $t0 # matrix_a
		add $t7, $t6, $t1 # swap
		lb $t8, 0($t6) # a[i+j]
		lb $t9, 0($t7) # swap[i+j]
		sb $t8, 0($t9) # swap[i+j] = a[i+j]
		addi $s1, $s1, 1 # j++
		j copy2 


copy3:	
		addi $s0, $s0, 1 #i++
		j copy1

swap1:
		j L4
L4:
	    lw $ra, 0($sp)
	    lw $s0, 4($sp)
        lw $s1, 8($sp)
		lw $a0, 12($sp)
		lw $a1, 16($sp)
		add $sp, $sp, 20
		jr $ra
	
##############################################################################
matrix_multiply: 
##############################################################################
# mult matrices swap and B together of square size N and store in result.
	

		# alloc stack and store regs.
		sub $sp, $sp, 24
	        sw $ra, 0($sp)
	        sw $a0, 4($sp)
	        sw $a1, 8($sp)
	        sw $s0, 12($sp)
	        sw $s1, 16($sp)
	        sw $s2, 20($sp)

	        #setup for i loop

	        #setup for j loop

	        #setup for k loop

                # compute swap[i][k] address and load into $t3

                # compute B[k][j] address and load into $t4

		# invoke mul instruction

	        # increment k and jump back or exit

	        #increment j and jump back or exit

	        #increment i and jump back or exit

		# retore saved regs from stack
		lw $s2, 20($sp)
	        lw $s1, 16($sp)
		lw $s0, 12($sp)
		lw $a1, 8($sp)
		lw $a0, 4($sp)
	        lw $ra, 0($sp)
	
		# free stack and return
		add $sp, $sp, 24
		jr $ra


##############################################################################
matrix_print:
##############################################################################
		sub $sp, $sp, 16
	        sw $ra, 0($sp)
	        sw $s0, 4($sp)
	        sw $s1, 8($sp)
		sw $a0, 12($sp)

	        # INSERT two for loops for matrix output
		
		add $t3, $zero, $a0
		add $s0, $zero, $zero # i=0


L1:		
		# if (i< size)
		add $t7, $zero, $zero
		slti $t7, $s0, 4
		beq $t7, $zero, L4 # end

		add $s1, $zero, $zero #j=0

L2:     
		add $t6, $zero, $zero
		slti $t6, $s1, 4
		beq $t6, $zero, L3 # print newline

		add	$t5, $s0, $s0 #t5 = 4*i
		add $t5, $t5, $t5

		add	$t5, $t5, $s1 # t5 += j
		mul $t4, $t5, 4
		add	$t5, $t4, $t3
		li	$v0, 1
		lw	$a0, 0($t5)
		syscall

		li $v0, 4
		la $a0, tab
		syscall

		addi $s1, $s1, 1 # j++
		j L2

L3:		
		li $v0, 4
		la $a0, newline
		syscall

		addi $s0, $s0, 1 # i++
		j    L1

L4:
	    lw $ra, 0($sp)
	    lw $s0, 4($sp)
        lw $s1, 8($sp)
		lw $a0, 12($sp)
		add $sp, $sp, 16
		jr $ra
	
