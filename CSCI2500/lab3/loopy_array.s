################################################################################
## Data segment
################################################################################
.data 
  array:        .word   0, 0, 0, 0, 0

################################################################################
## Text segment
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label

################################################################################
# Our main frame
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # We're going to initialize all values in an array of length 5 to 1
  # To do this, we'll assemble what is essentially a do-while loop

  li  $t0, 5      # length of our array
  li  $t1, 1      # the value we're going to be writing
  li  $t2, 0      # our loop counter
  la  $t3, array  # the base address of our array

loopy_business:
  sll $t4, $t2, 2   # get the offset from our base address using our counter
                    # offset = 4*i, where i is our counter and int is 4 bytes
  add $t4, $t4, $t3 # get the address of the current index we're writing to
                    # address of index i = (base address) + 4*i
  sw  $t1, 0($t4)   # store the value of '1' to address in $t4

  # branch back up to 'loopy_business' if our loop counter is not equal to the
  # length of our array
  # i.e. - while (i != length)
  addi  $t2, $t2, 1               # increment our loop counter by 1
  bne   $t2, $t0, loopy_business  # branch if (i != 5)

  # Note: the below is the same, but as a while loop instead
  # We just now need two labels instead of only one
  li  $t2, 0            # reset our loop counter
  la  $t3, array        # and our base memory address

loopy_business2:
  beq $t2, $t0, exit    # exit loop if (i == 5)
  
  sll $t4, $t2, 2       # same stuff as before
  add $t4, $t4, $t3
  sw  $t1, 0($t4)

  addi  $t2, $t2, 1     # increment our loop counter
  j   loopy_business2   # jump back to the start of our loop
exit:

  # print out all values in the array
  li  $v0, 1        # print integer
  lw  $a0, 0($t3)   # array[0]
  syscall 
  lw  $a0, 4($t3)   # array[1]
  syscall 
  lw  $a0, 8($t3)   # array[2]
  syscall 
  lw  $a0, 12($t3)  # array[3]
  syscall 
  lw  $a0, 16($t3)  # array[4]
  syscall 

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

