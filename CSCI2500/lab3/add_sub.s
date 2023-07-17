################################################################################
## Data segment - Program data like static arrays, strings, etc.
################################################################################
.data 
  result:       .asciiz "Result is:"
  newline:      .asciiz "\n"
  tab:          .asciiz "\t"
  smiley:       .asciiz ":)"

################################################################################
## Text segment - Our program code
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label
  .globl print    # print is another function we could call     

################################################################################
# Main function - Like with high level languages like C, we start here
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # We're going to compute f = (g + h) - (i + j)
  # f = ?,  g = 1,  h = 2,  i = 3,  j = 4
  # f = s0, g = s1, h = s2, i = s3, j = s4

  # li = load immediate ('immediate' is fancy word for 'constant')
  li  $s1, 1        # set g = 1
  li  $s2, 2        # set h = 2
  li  $s3, 3        # set i = 3
  li  $s4, 4        # set j = 4

  add $t0, $s1, $s2 # temp0 = g + h
  add $t1, $s3, $s4 # temp1 = i + j
  sub $s0, $t0, $t1 # f = temp0 - temp1

  # time to output our result
  add  $a1, $s0, $0    # copy our f result into an 'argument' register
                    # Note: move is a pseudo-instruction, meaning that it
                    # isn't directly part of the ISA or it doesn't directly
                    # translate to a machine instruction. In this instance,
                    # the mips 'compiler' would turn it into:
                    # add $a0, $s0, $0 - $0 is the 'zero' register, which just
                    # holds the value of 0. Also note: li is also a 
                    # pseudo-instruction (more later)

  jal print         # Jump and link to 'function' print, this overwrites the
                    # $ra register (why we need to save it on the stack)


  # restore $ra, free stack space, and return (since we're in main - we exit)
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# Our print function
################################################################################
# We'll be making use of system calls here. We'll generally be using them for
# input and output, similar to how we've done printf() and scanf() in C. 
# The general method for system calls is:
# - Load into argument register $a0 some value to passed to the system call
# - Load into register $v0 an integer representing what system call to make
# - Make the system call with 'syscall'
# For possible system calls and $v0 values, see:
#   https://en.wikibooks.org/wiki/MIPS_Assembly/System_Instructions
# Note: As this is a leaf function, we don't need to store $ra on the stack
print:
  la    $a0, result   # load address of 'result' string into $a0
  li    $v0, 4        # set $v0 for system call of 'print string'
  syscall             # do our bizniz

  add  $a0, $a1,$0      # put our result of 'f' into $a0
  li    $v0, 1        # set $v0 for system call of 'print integer'
  syscall             # biznazzzzzz

  la    $a0, tab      # and repeat for the tab, smiley, newline
  li    $v0, 4
  syscall 
  la    $a0, smiley   # Note: $v0 is already appropriately set, only need to
  syscall             # update $a0 register
  la    $a0, newline
  syscall

  jr    $ra           # jump register - jump to whatever is in $ra
