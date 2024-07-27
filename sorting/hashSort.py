import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

def hash_sort(array, k = 130):

    starting_position = array[0]
    ending_position = array[0]
    avg_val = abs(sum(array)//len(array))
    jump_dist = avg_val * k if avg_val != 0 else k
    average_spread_entries = {}
    interval_tracker = {}
    array_ind = 0

    for item in array:
      if item in average_spread_entries:
        average_spread_entries[item].append(item)
      else:
        average_spread_entries[item] = [item]
        interval_tracker[(item//jump_dist) * jump_dist] = 1
      starting_position = item if starting_position > item else starting_position
      ending_position = item if ending_position < item else ending_position

    ending_position += jump_dist + 1
    start_widow = (starting_position // jump_dist) * jump_dist

    end_window = start_widow + jump_dist
    while end_window <= ending_position:
        if any(value in interval_tracker for value in range(start_widow, end_window)):
            for itr in range(start_widow, end_window):
                if itr in average_spread_entries:
                    items = average_spread_entries[itr]
                    for item in items:
                        array[array_ind] = item
                        array_ind += 1
        start_widow += jump_dist
        end_window += jump_dist



if __name__ == "__main__":
        from helpers.array_utils import (generate_random_array, is_the_array_sorted)
        import time
        arr = generate_random_array(1000)
        arr_copy = arr.copy()
        start = time.time() 
        hash_sort(arr)
        print(time.time() - start)
        print(is_the_array_sorted(arr, arr_copy))