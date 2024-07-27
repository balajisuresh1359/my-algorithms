from helpers.node import TreeNode as Node

class BST:
    def __init__(self):
        self.root = None
    
    def insert(self, value, reference = None):
        if not self.root:
            self.root = Node(value, reference)
        else:
            self.addNode(self.root, value, reference)
    
    def insert(self, value, reference=None):
        new_node = Node(value, reference)
        if not self.root:
            self.root = new_node
            return

        current_node = self.root
        while True:
            if value < current_node.value:
                if current_node.left:
                    current_node = current_node.left
                else:
                    current_node.left = new_node
                    break
            else:
                if current_node.right:
                    current_node = current_node.right
                else:
                    current_node.right = new_node
                    break
    
    def find_nearest(self, value, find_smaller=True):
        if find_smaller:
            return self.find_nearest_smaller(value)
        else:
            return self.find_nearest_greater(value)
    
    def find_nearest_smaller(self, value):
        nearest = None
        node = self.root
        while node:
            if node.value < value:
                nearest = node
                node = node.right
            else:
                node = node.left
        return nearest.reference if nearest else None
    
    def find_nearest_greater(self, value):
        nearest = None
        node = self.root
        while node:
            if node.value > value:
                nearest = node
                node = node.left
            else:
                node = node.right
        return nearest.reference if nearest else None
