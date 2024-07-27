from random import randint
import numpy as np

def generate_random_int():
    min_int = -10 ** 6
    max_int = 10 ** 6
    return randint(int(min_int), int(max_int))


def generate_random_array(size):
    return [generate_random_int() for _ in range(size)]

def is_the_array_sorted(sored_array, original_array):
    return sored_array == sorted(original_array)

def check_matrix_multiplication(fun_matrix_multiple, n):
    A = np.random.randint(10, size=(n, n))
    B = np.random.randint(10, size=(n, n))
    
    result = fun_matrix_multiple(A, B)
    
    result_numpy = np.dot(A, B)
    
    if np.array_equal(result, result_numpy):
        print(f"Matrix multiplication of {fun_matrix_multiple.__name__} is correct.")
    else:
        print(f"Matrix multiplication of {fun_matrix_multiple.__name__} is incorrect.")
