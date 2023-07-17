################################################################################
## Data segment
################################################################################
.data
  newline:      .asciiz "\n"

################################################################################
## Text segment
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label

################################################################################
# Our main train - CHOO CHOO!! 🚂🚂🚂🚂🚂🚂
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # We're going to pass to our function 6 values
  # We're going to have returned from our function 3 values
  # We'll do this via the stack

  # first 4 arguments in argument register
  li  $a0, 1
  li  $a1, 2
  li  $a2, 3
  li  $a3, 4

  # next two arguments on the stack
  li  $t0, 5        # our two argument values
  li  $t1, 6 
  sub $sp, $sp, 12  # allocate space for 3 words, 2 arguments and 1 return
  sw  $t0, 4($sp)   # store arguments on the stack
  sw  $t1, 8($sp)   # Note: return will be stored at 0($sp)

  # Go to our function
  jal lotta_args

  # Get our return values and print them
  move  $t0, $v0      # get our first two values from the return registers
  move  $t1, $v1
  lw    $t2, 0($sp)   # pop the third value from the stack
  add   $sp, $sp, 12  # restore the stack pointer

  # print the returns
  li    $v0, 1
  move  $a0, $t0
  syscall
  move  $a0, $t1
  syscall
  move  $a0, $t2
  syscall

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra


################################################################################
# Lotta_args function
# We'll just store sums of arguments into return values
################################################################################
lotta_args:
  move  $t0, $a0    # load up our 6 arguments
  move  $t1, $a1
  move  $t2, $a2
  move  $t3, $a3
  lw    $t4, 4($sp)
  lw    $t5, 8($sp)

  # Note: doing this after to keep $sp offsets consistent with caller
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)  
  # Note: we don't *need* to store $ra on the stack, as this a leaf function
  # I'm just including the above instructions as an example

  # calculate our sums
  add $v0, $t0, $t1
  add $v1, $t2, $t3
  add $t6, $t4, $t5

  # restore $ra
  lw  $ra, 0($sp)
  add $sp, $sp, 4

  # save our third return value
  # doing this after the above to keep $sp consistent with caller
  sw  $t6, 0($sp)

  # and return
  jr  $ra
