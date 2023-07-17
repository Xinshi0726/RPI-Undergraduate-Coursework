################################################################################
## Data segment - Program data like static arrays, strings, etc.
################################################################################
.data 
  array:        .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
  newline:      .asciiz "\n"

################################################################################
## Text segment - Our program code
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label
  .globl print    # print is another function we could call     

################################################################################
# Our main boi/grl/non-binary person
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # We're going to read from an index in our array, modify its values, and then
  # read from that location again

  # read it in
  la  $s0, array    # load memory address of array into s0
  lw  $t0, 32($s0)  # load (address of $s0) + 32 bytes
                    # this is equivalent to array[8] or array + 8 in C, where 
                    # array is an int* pointer. Note: sizeof(int) = 4 bytes

  # print it out along with a newline
  move  $a0, $t0
  li    $v0, 1
  syscall 
  la    $a0, newline
  li    $v0, 4
  syscall 

  # modify it
  li  $t0, 1234     
  sw  $t0, 32($s0)  # store t0=1234 into array[8]

  # load it again then print it out
  lw  $a0, 32($s0)  # loading it directly into the argument register
  li  $v0, 1
  syscall 

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra
