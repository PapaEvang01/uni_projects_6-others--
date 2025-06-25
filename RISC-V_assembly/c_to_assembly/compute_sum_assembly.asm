# ================================================================
# 🧮 Row Sum of a 2D Array in RISC-V Assembly
# ================================================================
# This program implements in RISC-V Assembly a classic C function
# that calculates the sum of each row in a two-dimensional array.
# It was developed as part of a university lab exercise.
#
# 🔁 Translated C equivalent:
# const int N = 2;
# const int M = 3;
# int a[N][M] = {342, 654, 263, 463, 975, 378};
# int rows_sum[N];
#
# void compute_row_sum(int a[N][M], int rows_sum[N]) {
#     for (int i = 0; i < N; i++) {
#         rows_sum[i] = 0;
#         for (int j = 0; j < M; j++) {
#             rows_sum[i] += a[i][j];
#         }
#     }
# }
#
# int main() {
#     compute_row_sum(a, rows_sum);
# }
#
# 🛠️ The implementation demonstrates key low-level concepts in RISC-V:
# memory addressing, nested loops, load/store operations, and register
# arithmetic. It processes the 2D array row by row and stores the result
# in an output array.
#
# ================================================================

.data
rows:       .word 2
cols:       .word 3

array2d:
    .word 342, 654, 263
    .word 463, 975, 378

rows_sum:
    .word 0, 0

.text
.globl main
main:
    # Load arguments for function call
    la   a0, array2d     # a0 = base address of 2D array
    lw   a1, cols        # a1 = number of columns (M)
    lw   a2, rows        # a2 = number of rows (N)
    la   a3, rows_sum    # a3 = base address for output array

    jal  ra, compute_row_sum

    j end_program


# void compute_row_sum(int a[N][M], int M, int N, int result[])
# a0 = &a[0][0], a1 = M, a2 = N, a3 = result[]
compute_row_sum:
    slli s4, a1, 2       # s4 = column size in bytes (M * 4)
    li   s1, 0           # i = 0
    mv   s5, a0          # s5 = current row start address

row_loop:
    beq  s1, a2, end_rs  # if i == N, done
    li   t0, 0           # sum = 0
    li   s2, 0           # j = 0
    mv   s6, s5          # s6 = current column address

col_loop:
    beq  s2, a1, end_sum
    lw   t1, 0(s6)       # t1 = a[i][j]
    add  t0, t0, t1      # sum += a[i][j]
    addi s6, s6, 4       # move to next column
    addi s2, s2, 1       # j++
    j col_loop

end_sum:
    sw   t0, 0(a3)       # result[i] = sum
    addi a3, a3, 4       # move to next result index
    add  s5, s5, s4      # move to next row base address
    addi s1, s1, 1       # i++
    j row_loop

end_rs:
    jr ra

end_program:
    nop
