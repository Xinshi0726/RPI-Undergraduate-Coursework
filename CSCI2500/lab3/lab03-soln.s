################################################################################
# Lab 3: Assembling some arrays
# CSCI-2500 Spring 2021
################################################################################
## Data segment
################################################################################
.data
  newline:      .asciiz "\n"

################################################################################
## Text segment
################################################################################
.text
  .align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl read_arr   # allocate and read in an array of some length
  .globl print_arr  # print out the values that were read in

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in an array of some length
  # Return in $v0 - the length of the array
  # Return in $v1 - the pointer to the array
  jal read_arr

  # Move our return values to the argument registers
  move $a0, $v0 # length of array
  move $a1, $v1 # pointer to array
  jal print_arr

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# Read in our array
# Construct a loop to read in 10 values into the array in our data segment
################################################################################
read_arr:
  # Read in the length of the array
  li    $v0, 5      # syscall value for 'read integer'
  syscall           # note: syscall will overwrite v0 with value read
  move  $t0, $v0    # set the length of our array

  # Allocate sufficient space
  li    $v0, 9      # syscall for 'allocate memory' of a0 bytes
  sll   $a0, $t0, 2 # number of bytes we need to allocate = 4*length
  syscall           # v0 will be overwritten with pointer to allocation
  move  $t1, $v0    # set t1 as our array pointer

  # Set up primary loop
  li    $t2,  0     # Counter for our loop
  move  $t3,  $t1   # We'll use this to keep track of where we're writing

  # Do primary loop
read_loop:
  # Read in next value from the array and store it
  li  $v0, 5      
  syscall

  sw  $v0, 0($t3) # store our value in the array

  addi  $t3, $t3, 4         # increment our memory address by a word 
  addi  $t2, $t2, 1         # increment our loop counter 
  bne   $t0, $t2, read_loop # go back if we aren't done yet
  # Done with loop

  # Set the return values
  move  $v0, $t0  # length
  move  $v1, $t1  # array pointer

  # Go back to main
  jr  $ra

################################################################################
# Print out the array
# Construct a loop to print out the 10 values that were read in
################################################################################
print_arr:
  move  $t0, $a0    # The length of our array   
  move  $t1, $a1    # The memory address we're reading from
  li    $t2, 0      # Counter for our loop
 
  # Do primary loop
print_loop:
  # Output the integer from the array
  li  $v0, 1      # syscall value for 'print integer'
  lw  $a0, 0($t1) # load our value from the array
  syscall

  # Output a new line
  li  $v0, 4        # syscall value for 'print string'
  la  $a0, newline  # load up our newline string
  syscall

  addi  $t1, $t1, 4           # increment our memory address by a word
  addi  $t2, $t2, 1           # increment our loop counter 
  bne   $t0, $t2, print_loop  # go back if we aren't done yet
  # Done with loop

  # Go back to main
  jr  $ra
