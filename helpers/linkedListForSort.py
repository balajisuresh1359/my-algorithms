from helpers.node import LinkedListNode as Node
from helpers.bst import BST

class LinkedList:
  def __init__(self, data, order = 'ASC'):
    self.tail = self.head = Node(data)
    self.sum_of_t = 0
    self.length = 1
    self.order = order
    if order == 'ASC':
        self.find_smaller = False
        self.compare = lambda x, y: x >= y
        self.iterator_function = self.__iter_from_head__
    else:
        self.find_smaller = True
        self.compare = lambda x, y: x <= y
        self.iterator_function = self.__iter_from_tail__

  def __len__(self):
    return self.length
  
  def __iter__(self):
      return self.iterator_function()

  def __iter_from_head__(self):
      current = self.head
      while current:
          yield current.data
          current = current.next
      return None

  def __iter_from_tail__(self):
      current = self.tail
      while current:
          yield current.data
          current = current.prev
      return None

  def append(self, data):
    node = Node(data)
    self.tail.next = node
    node.prev = self.tail
    self.tail = node
    self.length += 1

  def last(self):
    return self.tail.data
  
  def insert(self, data_list):
      processed_item = BST()

      for data in data_list:
        inserted_node_ref = processed_item.find_nearest(data, self.find_smaller)
        ptr = inserted_node_ref if inserted_node_ref is not None else self.tail
        inserted_node = self.insert_in_order(data, ptr)
        processed_item.insert(data, inserted_node)


  def insert_in_order(self, data, curr_item):
      while self.compare(curr_item.data, data):
          curr_item = curr_item.prev

      node = Node(data)
      node.next = curr_item.next
      node.prev = curr_item
      curr_item.next.prev = node
      curr_item.next = node
      self.length += 1
      return node
