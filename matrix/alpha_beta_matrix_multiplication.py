import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import numpy as np

from helpers.array_utils import generate_random_array

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


if __name__ == "__main__":
    mat_a, mat_b = [], []
    dimension = 50
    print(f"Generating matrix with size {dimension}..")
    for _ in range(dimension):
        mat_a.append(generate_random_array(dimension))
    for _ in range(dimension):
        mat_b.append(generate_random_array(dimension))
    print("Starting sorting....")
    import time
    s = time.time()
    a = alpha_beta_matrix_multiplication(mat_a, mat_b)
    print(time.time() - s)
    
