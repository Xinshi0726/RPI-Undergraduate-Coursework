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

################################################################################
# Main segment
# Note: You don't need to modify anything here (and you shouldn't)
################################################################################
main:
  # Save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in an array of some length
  # Return in $v0 - the length of the array
  # Return in $v1 - the pointer to the array
  jal read_arr 
  # Move our return values to the argument registers
  #li $
  #add $a0, $v0, # length of array
  #add $a1, $v1 # pointer to array

  # Call our function to print out the array
  #jal print_arr

  # Restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# Read in our array
# Construct a loop to read in 'length' values into the array in our data segment
################################################################################
read_arr:
  # Read in the length of the array
  li    $v0, 5      # syscall value for 'read integer'
  syscall           # note: syscall will overwrite v0 with value read
  move  $t0, $v0    # store the value so we can print it out
  # Allocate sufficient space
  li    $v0, 9      # syscall for 'allocate memory' of a0 bytes
  sll   $a0, $t0, 2 # number of bytes we need to allocate = 4*length
                    # Note: sll = shift left logical = "t0 << 2" in C
                    # Can also use 'mul' pseudo-instruction
                    # Or 'mult' actual instruction (but requires another step)
  syscall           # v0 will be overwritten with pointer to allocation
  move  $t1, $v0    # set t1 as our array pointer
  # Do primary loop
  li  $t2, 0      # our loop counter
  j read_loop
read_loop:
  # Read in next value from the array and store it
  li    $v0, 5      # syscall value for 'read integer'
  syscall           # note: syscall will overwrite v0 with value read
  move  $t1, $v0    # store the value so we can print it out
  # Set the return values
  li $v0,1
  move $a0, $t1
  syscall
  # Done with loop
  li $v0,4
  la $a0,newline 
  syscall
  add $t2,$t2,1
  bne $t2,$t0,read_loop
  # Go back to main

################################################################################
# Print out the array
# Construct a loop to print out the 'length' values that were read in
################################################################################
