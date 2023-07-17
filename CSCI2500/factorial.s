################################################################################
## Data segment
################################################################################
.data 

################################################################################
## Text segment
################################################################################
.text         
  .align 2        # instructions must be on word boundaries
  .globl main     # main is a global label

################################################################################
# Our main pup
################################################################################
main:
  sub $sp, $sp, 4   # allocate stack space to store $ra (return address pointer)
  sw  $ra, 0($sp)   # store $ra onto the stack ($sp is stack pointer)

  # read in an integer value
  li    $v0, 5
  syscall
  move  $a0, $v0

  # call factorial
  jal fib

  # output result
  move  $a0, $v0  # v0 holds result from function call
  li    $v0, 1
  syscall

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# factorial function
################################################################################
fib:    
  sub $sp,$sp,12          # save registers on stack
  sw $s0, 0($sp)          # 
  sw $a0, 4($sp)          # 
  sw $ra, 8($sp)          #
  bgt $a0,$zero, recurse      # if n>1 then goto generic case then do recursion
  move $v0,$a0            # if n = 0 or n = 1, then base case

  lw  $s0, 0($sp)         # restore registers from stack
  lw  $a0, 4($sp)         #
  lw  $ra, 8($sp)         #
  add $sp, $sp, 12      
  jr $ra
              
recurse:    
  sub $a0,$a0,1           # decrement argument by 1
  jal fib                 # compute fib(n-1)
  move $s0,$v0            # save fib(n-1)
  sub $a0,$a0,1           # decrement argument by 2
  jal fib                 # do recursion
  add $v0, $v0, $s0       # $v0 = fib(n-2)+fib(n-1)

  lw  $s0, 0($sp)         # restore registers from stack
  lw  $a0, 4($sp)         #
  lw  $ra, 8($sp)         #
  add $sp, $sp, 12      
  jr $ra

