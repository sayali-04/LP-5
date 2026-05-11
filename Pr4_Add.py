!nvidia-smi
import numpy as np
from numba import cuda
import time

@cuda.jit
def vector_add(A, B, C):

    i = cuda.grid(1)

    if i < len(A):
        C[i] = A[i] + B[i]

if __name__ == "__main__":

    print("=== CUDA Vector Addition ===")

    N = int(input("Enter size of vectors: "))

    A = np.random.randint(0, 100, N)
    B = np.random.randint(0, 100, N)

    C = np.zeros(N, dtype=np.int32)

    print("\nSample of Vector A:", A[:10])
    print("Sample of Vector B:", B[:10])

    d_A = cuda.to_device(A)
    d_B = cuda.to_device(B)
    d_C = cuda.device_array(N)

    threads_per_block = 256

    blocks_per_grid = (N + threads_per_block - 1) // threads_per_block

    start = time.time()

    vector_add[blocks_per_grid, threads_per_block](d_A, d_B, d_C)

    cuda.synchronize()

    end = time.time()

    C = d_C.copy_to_host()

    C_cpu = A + B

    print("\nSample Result:", C[:10])

    print("\nGPU Time:", end - start, "seconds")

    print("Result Correct:", np.allclose(C, C_cpu))