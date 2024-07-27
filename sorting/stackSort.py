import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import numpy as np

def stack_sort(array):
	top, array_index, visited_mark, array_size = 0, 0, np.inf, len(array) - 1

	stack = np.empty((array_size + 1), dtype=object)
	stack[top] = [array[top], array_index]
	array[array_index] = visited_mark

	while top < array_size:
		array_index	+= 1

		if array_index > array_size:
				array_index = 0

		if array[array_index] != visited_mark:
			while top >= 0 and stack[top][0] > array[array_index]:
					array[stack[top][1]] = stack[top][0]
					top -= 1
			top += 1
			stack[top] = [array[array_index], array_index]
			array[array_index] = visited_mark

	return [x[0] for x in stack]


if __name__ == "__main__":			
				from helpers.array_utils import (generate_random_array, is_the_array_sorted)
				arr = generate_random_array(10)
				arr_copy = arr.copy()
				import time
				start = time.time() 
				res = stack_sort(arr)
				print(time.time() - start)
				print(is_the_array_sorted(res, arr_copy))
