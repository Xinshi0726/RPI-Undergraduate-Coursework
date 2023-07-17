# This function should check if the input argument is 0 or not. If the argument
# is 0, the function should pass the value of 1 to the cluck_cluck function
# AND return the value of 1 to the caller. Likewise, if the argument is not
# 0, the function should pass the value of 0 to the cluck_cluck function AND 
# return the value of 0 to the caller.

bawk_bawk:
  # get our input argument
  sw  $ra, 0($sp) #store the ra in case we need it
  sw  $s0, 4($sp) #saved register must be saved on the stack
  move  $s0, $a0

  # if our input == 0
  #   argument/return value = 1
  # else
  #   argument/return value = 0
  beq $s0, $zero, zero
  li  $a0, 0
  j cluck_cluck #in the case argument is not 0, call cluck cluck with argument 0
  li $v0, 0 #vi should denote the return value, and we set it to 1 by definition 
  jr $ra #jump to the function that called bawk_bawk
zero:
  li  $a0, 1

  # call cluck_cluck function with argument of 1/0
  j   cluck_cluck
  li $v0,1 #vi should denote the return value, and we set it to 0 by definition 
  # return to caller
  jr  $ra
