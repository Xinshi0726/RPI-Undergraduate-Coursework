################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Spring 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix 
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices
  .globl end_mult1    # multiple two non-square matrices
################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
#   Note: t0=N, t1=M, $t2 = loop counter, $t3 = length
#   $s0 = address of return matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sw  $ra, 0($sp)
  sub $sp, $sp,4
  sw $s0, 0($sp)
  # get N
  li    $v0, 5      # syscall value for 'read integer'
  syscall           # note: syscall will overwrite v0 with value read
  move  $t0, $v0    # set the length of our array
  # get M
  li    $v0, 5      # syscall value for 'read integer'
  syscall           # note: syscall will overwrite v0 with value read
  move  $t1, $v0    # set the length of our array
  # Setup up arguments and call mm_alloc - v0 is returned as base address
  mul $a0, $t0, $t1
  move $t3, $a0
  jal mm_alloc
  #store the base address, save in the stack after restore ra
  move $s0, $v0
  move $t4, $s0
  # do nested loops to read in values
  li $t2,0
  jal read_loop
  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
  move $v0, $t0
  move $v1, $t1
  # restore stack, ra, and any saved registers, if necessary
  add $sp, $sp, 4
  lw $ra, 0($sp)
  sub $sp, $sp, 4
  lw $t0, 0($sp)
  add $sp, $sp, 4
  sw $s0, 0($sp)

  move $s0, $t0
  # return to main
  jr  $ra
################################################################################
# read_loop: a helper function to read values 
#Note: I copy paste directly from lab03-sol.s
################################################################################
read_loop:
  # Read in next value from the array and store it
  li  $v0, 5      
  syscall

  sw  $v0, 0($t4) # store our value in the array

  addi  $t4, $t4, 4         # increment our memory address by a word 
  addi  $t2, $t2, 1         # increment our loop counter 
  bne   $t3, $t2, read_loop # go back if we aren't done yet
  # Done with loopn

  # Go back to main
  jr  $ra
################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary
  # Allocate sufficient space
  li    $v0, 9      # syscall for 'allocate memory' of a0 bytes
  sll   $a0, $a0, 2 # number of bytes we need to allocate = 4*length
  syscall           # v0 will be overwritten with pointer to allocation
  move  $t2, $v0    # set t1 as our array pointer

  # set return value
  move $v0, $t2
  # restore stack, ra, and any saved registers, if necessary

  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4
  sw  $ra, 0($sp)
  # do nested loops to print out values
 
   #set loop stuff
  li $t2, 0
  move $t0, $a0
  move $t1, $a1
  move $t5, $a2
  jal print_loop1
  # restore stack, ra, and any saved registers, if necessary

  # return to main
  lw $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# Note: t2: t2: out loop counter t3: inner loop counter 
# t0:outer loop end condition $t1: inner loop end condition
#t5: arr
################################################################################
print_loop1:
  # set the outer loop:
  beq $t2, $t0, end_loop1
  li $t3,0
  # do the inner loop
print_loop2:
  #set terminal condition
  beq $t3, $t1, end_loop2
  # do the print
  #print the number
  li $v0, 1
  lw $a0, 0($t5)
  syscall
  #print the tab
  li $v0, 4
  la $a0, tab
  syscall
  #increment
  add $t3, $t3, 1
  add $t5, $t5, 4
  j print_loop2
end_loop2:
  #increase the outer loop
  add $t2, $t2, 1
  #print new line
  li $v0, 4
  la $a0, newline
  syscall
  #call outer loop
  j print_loop1

end_loop1:
  # return to mm_print
  lw $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra


################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = t0 = N for matrix 1
#   a1 = t1 = M for matrix 1 <-> N for matrix 2
#   a2 = t2 = base address for matrix 1
#   a3 = t3 = M for matrix 2
#   0($sp) = t4 = base address for matrix 2
#   s0 = stored result matrix
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 4
  sw  $ra, 0($sp)
  sub $sp, $sp, 4
  sw $s0, 0($sp)
  # set up and call mm_alloc for result matrix
  move $t0, $a0
  move $t1, $a1
  move $t3, $a3
  add $sp, $sp, 8
  lw  $t4, 0($sp)
  sub $sp, $sp, 8
  mul $a0, $a0, $a3
  jal mm_alloc
  #t2 is assigned here since mmalloc reused $t2
  move $t2, $a2
  move $s0, $v0
  # three nested loops for matrix-matrix multiplication
  li $t5, 0
  li $t7,0
  jal mult1
  # set return value
  move $v0, $s0
  # restore stack, ra, and any saved registers, if necessary
  lw $s0, 0($sp)
  add $sp, $sp, 4
  lw $ra, 0($sp)
  add $sp, $sp, 4
  # return to main
  jr  $ra

################################################################################
# mult1: 1st loop of matrix mult
# Note: t5 = 1st loop counter
#       t6 = 2nd loop counter
#       t7 = 3rd loop counter
#       a3 = sum
################################################################################
mult1:
  # set the outer loop:
  beq $t5,$t0, end_mult1
  li $t6, 0
  # do the inner loop

mult2:
  #set terminal condition
  beq $t6, $t3, end_mult2
  li $t7,0
  li $a3, 0

mult3:
  #set terminal condition
  beq $t7, $t1, end_mult3

  #save the (r,i) value of the first matrix
  mul $a1, $t5, $t1
  add $a1, $a1, $t7
  sll $a1, $a1, 2
  add $t8, $t2, $a1

  #save the (i,c) value of the second matrix
  mul $a2, $t3, $t7
  add $a2, $a2, $t6
  sll $a2, $a2, 2
  add $t9, $t4, $a2

  #add em up
  lw $t8, 0($t8)
  lw $t9, 0($t9)

  mul $t8, $t8, $t9
  add $a3, $a3, $t8
  #increment
  add $t7, $t7, 1

  #loop
  j mult3

end_mult3:

  #save the (r,c) position of the result matrix, stored in s0
  mul $a1, $t3, $t5
  add $a1, $a1, $t6
  sll $a1, $a1, 2
  add $s2, $s0, $a1
  sw  $a3, 0($s2)
  #increase the outer loop
  add $t6, $t6, 1
  #call outer loop
  j mult2
end_mult2:
  #increase the outer loop
  add $t5, $t5, 1
  #call outer loop
  j mult1

end_mult1:
  jr $ra
