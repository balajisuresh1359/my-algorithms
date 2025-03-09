import numpy as np
from random import randint
import time

def generate_random_int():
    min_int = -10 ** 6
    max_int = 10 ** 6
    return randint(int(min_int), int(max_int))

def generate_random_array(size):
    return [generate_random_int() for _ in range(size)]

def alpha_beta_matrix_multiplication(A, B):
    nA = np.array(A)
    nB = np.array(B)
    la = nA.shape[0]
    lb, l0b = nB.shape
    sumArows = np.sum(nA, axis=1)
    sumBcols = np.sum(nB, axis=0)
    firstItemOfRowBarSumArows = nA[:, 0] / (sumArows)
    inverseOfEachSumArows = np.ones(la) / (sumArows)
    inverseOfEachSumBcols = np.ones(lb) / (sumBcols)

    a_idx = 0
    result = np.zeros((la, l0b))

    for a in nA:
      sa_a_idx = sumArows[a_idx]
      fiorbsa_a_idx = firstItemOfRowBarSumArows[a_idx]
      first_item_sub_all = a[1:] - a[0]
      ioesa_a_idx = inverseOfEachSumArows[a_idx]
      b_idx = 0
      for col_itr in range(lb):
          beta = np.dot(nB[1:, col_itr], first_item_sub_all)
          alpha = fiorbsa_a_idx + (beta * ioesa_a_idx * inverseOfEachSumBcols[b_idx])
          result[a_idx][b_idx] = round(sa_a_idx * sumBcols[b_idx] * alpha)
          b_idx += 1
      a_idx += 1

    return result

def dot_matrix_multiplication(A, B):
    return np.dot(A, B)

def normal_matrix_multiplication(A, B):
    nA = np.array(A)
    nB = np.array(B)
    n = len(nA)
    result = np.zeros((n, n), dtype=int)
    
    for i in range(n):
        for j in range(n):
            for k in range(n):
                result[i, j] += nA[i, k] * nB[k, j]
    
    return result

def measure_time(func, A, B, trials=2):
    times = []
    for _ in range(trials):
        start = time.perf_counter()
        func(A, B)
        end = time.perf_counter()
        times.append(end - start)
    
    return sum(times) / len(times)


if __name__ == "__main__":
    mat_a, mat_b = [], []
    dimension = 750
    print(f"Generating matrix with size {dimension}..")
    for _ in range(dimension):
        mat_a.append(generate_random_array(dimension))
    for _ in range(dimension):
        mat_b.append(generate_random_array(dimension))
    print("Starting multiplication....")
    print("Running normal_matrix_multiplication")
    normal_algo_time = measure_time(normal_matrix_multiplication, mat_a, mat_b)
    print("Running ab_algo_time")
    ab_algo_time = measure_time(alpha_beta_matrix_multiplication, mat_a, mat_b)
    print("Running dot_matrix_multiplication")
    dot_prd_algo_time = measure_time(dot_matrix_multiplication, mat_a, mat_b)

    print(f"\nExecution Time:")
    print(f"Normal Algorithm: {normal_algo_time:.6f} sec")
    print(f"Ab Algorithm: {ab_algo_time:.6f} sec")
    print(f"NumPy dot() Method: {dot_prd_algo_time:.6f} sec")



# Generating matrix with size 750..
# Starting multiplication....
# Running normal_matrix_multiplication
# Running ab_algo_time
# Running dot_matrix_multiplication

# Execution Time:
# Normal Algorithm: 312.451826 sec
# Ab Algorithm: 5.155557 sec
# NumPy dot() Method: 0.906895 sec
