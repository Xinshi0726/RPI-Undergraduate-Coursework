################################################################################
## Data segment
################################################################################
.data 
  array:        .asciiz "Our array is at: "
  newline:      .asciiz "\n"

################################################################################
## Text segment
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label

################################################################################
# Our main kitty
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # We're going to allocate an array that will hold 10 integers

  li    $t0, 10     # length of our array
  li    $v0, 9      # syscall for 'allocate memory' of a0 bytes
  sll   $a0, $t0, 2 # number of bytes we need to allocate = 4*length
                    # Note: sll = shift left logical = "t0 << 2" in C
                    # Can also use 'mul' pseudo-instruction
                    # Or 'mult' actual instruction (but requires another step)
  syscall           # v0 will be overwritten with pointer to allocation
  move  $t1, $v0    # set t1 as our array pointer

  # print out our allocated memory address
  li    $v0, 4      # syscall for 'print string'
  la    $a0, array  # set argument for print string
  syscall           # business

  li    $v0, 1      # syscall for 'print integer'
  move  $a0, $t1    # set argument as memory address
  syscall           # business part deux

  li    $v0, 4        # syscall for 'print string'
  la    $a0, newline  # set argument
  syscall             # bn part trois

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

