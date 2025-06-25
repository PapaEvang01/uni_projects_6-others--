# ================================================================
# 🔢 Fibonacci Sequence Generator – RISC-V Assembly
# ================================================================
# This program prompts the user to enter an integer `n` and prints
# the first `n` numbers of the Fibonacci sequence using RISC-V assembly.
#
# It demonstrates:
# - Integer input/output using ECALLs
# - Conditional branching and loops
# - Register-based arithmetic (addition, copy, etc.)
#
# Fibonacci starts with: 1 1 2 3 5 8 ...
# ================================================================

.data
str1: .asciz "Enter a number:"           # Prompt message
str2: .asciz "Fibonacci sequence:\n"     # Header before printing sequence
str3: .asciz " "                         # Space between numbers

.text
.globl main
main:
    # ----------------------------------------
    # Print prompt: "Enter a number:"
    # ----------------------------------------
    la a0, str1         # Load address of prompt string
    li a7, 4            # ECALL code for print_string
    ecall               # Print the prompt

    # ----------------------------------------
    # Read user input (integer)
    # ----------------------------------------
    li a7, 5            # ECALL code for read_int
    ecall               # Read integer into a0
    mv t0, a0           # Move input to t0 (n)

    # Exit early if input is 0
    beqz t0, exit       # if (n == 0) exit

    # ----------------------------------------
    # Print "Fibonacci sequence:"
    # ----------------------------------------
    la a0, str2         # Load header string
    li a7, 4            # print_string
    ecall

    # ----------------------------------------
    # Initialize Fibonacci variables:
    # s1 = a = 0
    # s2 = b = 1
    # s3 = c = a + b (later)
    # t1 = loop index = 2 (already printed first number)
    # ----------------------------------------
    li s1, 0            # a = 0
    li s2, 1            # b = 1
    li s3, 0            # c = 0
    li t1, 2            # i = 2 (start from 3rd term)

    # ----------------------------------------
    # Print the first Fibonacci number if n >= 1
    # ----------------------------------------
    bge t0, s2, print1  # if (n >= 1) go to print1

# -----------------------------------------------------
# Loop: Calculate and print Fibonacci numbers
# -----------------------------------------------------
for:
    bgt t1, t0, exit     # if i > n, exit

    # c = a + b
    add s3, s2, s1       # s3 = s2 + s1

    # Shift values for next iteration
    mv s1, s2            # a = b
    mv s2, s3            # b = c

    # Print next Fibonacci number
    mv a0, s2            # a0 = b
    j print              # jump to print routine

# -----------------------------------------------------
# Print first number (1) before loop starts
# -----------------------------------------------------
print1:
    mv a0, s2            # a0 = 1
    li a7, 1             # ECALL for print_int
    ecall

    # Print a space
    la a0, str3
    li a7, 4             # ECALL for print_string
    ecall

    j for                # jump to loop

# -----------------------------------------------------
# Generic print routine for other Fibonacci numbers
# -----------------------------------------------------
print:
    li a7, 1             # print_int
    ecall

    # Print space between numbers
    la a0, str3
    li a7, 4             # print_string
    ecall

    # Prepare for next iteration
    addi t1, t1, 1       # i++
    j for

# -----------------------------------------------------
# Exit program
# -----------------------------------------------------
exit:
    li a7, 10            # ECALL for exit
    ecall
