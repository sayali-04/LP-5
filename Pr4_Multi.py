!nvidia-smi
import numpy as np
from numba import cuda
import time

@cuda.jit
def matrix_mul(A, B, C):

    row, col = cuda.grid(2)

    if row < C.shape[0] and col < C.shape[1]:

        temp = 0

        for k in range(A.shape[1]):
            temp += A[row, k] * B[k, col]

        C[row, col] = temp

if __name__ == "__main__":

    print("=== CUDA Matrix Multiplication ===")

    N = int(input("Enter size of matrix: "))

    A = np.random.randint(1, 10, (N, N))
    B = np.random.randint(1, 10, (N, N))

    print("\nSample Matrix A:")
    print(A[:5, :5])

    print("\nSample Matrix B:")
    print(B[:5, :5])

    d_A = cuda.to_device(A)
    d_B = cuda.to_device(B)
    d_C = cuda.device_array((N, N))

    threads_per_block = (16, 16)

    blocks_per_grid_x = (N + threads_per_block[0] - 1) // threads_per_block[0]
    blocks_per_grid_y = (N + threads_per_block[1] - 1) // threads_per_block[1]

    start = time.time()

    matrix_mul[(blocks_per_grid_x, blocks_per_grid_y), threads_per_block](d_A, d_B, d_C)

    cuda.synchronize()

    end = time.time()

    C = d_C.copy_to_host()

    start_cpu = time.time()

    C_cpu = np.dot(A, B)

    end_cpu = time.time()

    print("\nSample Result Matrix:")
    print(C[:5, :5])

    print("\nGPU Time:", end - start, "seconds")

    print("CPU Time:", end_cpu - start_cpu, "seconds")

    print("Result Correct:", np.allclose(C, C_cpu))