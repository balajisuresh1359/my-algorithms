import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from helpers.linkedListForSort import LinkedList

def merge_insertion_sort(array, pending_item_percentage = 0.55):
      input_arr_len = len(array)
      pending_item_size = max(int(input_arr_len * pending_item_percentage), 6)
      
      min_value = max_value = array[0]
      min_value_ind = max_value_ind = 0

      for i, x in enumerate(array[1:], start=1):
          if x < min_value:
              min_value = x
              min_value_ind = i
          elif x > max_value:
              max_value = x
              max_value_ind = i

      asc_array = LinkedList(min_value)
      dsc_array = LinkedList(max_value, 'DESC')
      pending_items = []

      for curr_ind in range(input_arr_len):
        if curr_ind == min_value_ind or curr_ind == max_value_ind:
            continue
        
        curr_item = array[curr_ind]
        if asc_array.last() <= curr_item <= dsc_array.last():
              (asc_array if len(asc_array) < len(dsc_array) else dsc_array).append(curr_item)
        elif asc_array.last() <= curr_item:
              asc_array.append(curr_item)
        elif dsc_array.last() >= curr_item:
              dsc_array.append(curr_item)
        else:
              pending_items.append(curr_item)
              if len(pending_items) >= pending_item_size:
                    (asc_array if len(asc_array) < len(dsc_array) else dsc_array).insert(pending_items)
                    pending_items = []
      
      if len(pending_items) != 0:
            (asc_array if len(asc_array) < len(dsc_array) else dsc_array).insert(pending_items)

      asc_array_item = iter(asc_array)
      dsc_array_item = iter(dsc_array)
      array_index = 0

      asc_value = next(asc_array_item, None)
      dsc_value = next(dsc_array_item, None)

      while asc_value is not None and dsc_value is not None:
          if asc_value < dsc_value:
              array[array_index] = asc_value
              asc_value = next(asc_array_item, None)
          else:
              array[array_index] = dsc_value
              dsc_value = next(dsc_array_item, None)
          array_index += 1

      while asc_value is not None:
          array[array_index] = asc_value
          array_index += 1
          asc_value = next(asc_array_item, None)

      while dsc_value is not None:
          array[array_index] = dsc_value
          array_index += 1
          dsc_value = next(dsc_array_item, None)


if __name__ == "__main__":
    from helpers.array_utils import (generate_random_array, is_the_array_sorted)
    arr = generate_random_array(10000)
    arr_copy = arr.copy()
    import time
    start = time.time() 
    merge_insertion_sort(arr)
    print(time.time() - start)
    print(is_the_array_sorted(arr, arr_copy))

