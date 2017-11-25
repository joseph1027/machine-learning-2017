import numpy as np
import csv
import math 
import sys

global counter

class node:
	def __init__(self):
		self.left_child = None
		self.right_child = None
		self.value = None
		self.divide = None
		self.searched = None

def read_file(filename):#Read csv into list and remove the first row. Then change data from string to float.
	with open(filename,'r') as f:
		reader = csv.reader(f)
		all_data = list(reader)
		#print('ALL_DATA_PRE:',len(all_data))
		all_data = all_data[1:len(all_data)]
		#print('ALL_DATA_LAT:',len(all_data))
		for m in range (0,len(all_data)):
			for n in range (2,11):
				all_data[m][n] = float(all_data[m][n])

	return all_data

def build_tree(root,train_data,divide_column):
	data_len = len(train_data)
	root = node()
	train_data = sorted(train_data,key=lambda x: x[divide_column])
	median_index = int(data_len/2)
	root.value = train_data[median_index]
	root.divide = divide_column

	#print('divide_column:',divide_column)
	if(divide_column==10):
		divide_column = 2
	else:
		divide_column = divide_column + 1
	#print('value:',root.value)
	#print('data_len:',data_len)
	#print('left:')
	if data_len<=1 :
		return root
	"""lcnt=0
	rcnt=0
	for i in range(0,int(data_len/2)):
		lcnt = lcnt+1
		print(train_data[i])
		print(lcnt)
	print('right:')
	for j in range(int(data_len/2)+1,data_len):
		print(train_data[j])
		rcnt = rcnt+1
		print(rcnt)
	print('\n')"""
	root.left_child = build_tree(root.left_child,train_data[:median_index],divide_column)
	if data_len > 2:
		root.right_child = build_tree(root.right_child,train_data[median_index+1:],divide_column)
	return root

def reset_node(current_node):
	current_node.searched = 0
	if current_node.left_child is not None:
		reset_node(current_node.left_child)
	if current_node.right_child is not None:
		reset_node(current_node.right_child)

def test(root,test_data):
	text_file = open("output.txt","w")
	#print(test_data)
	origin_class = None
	predict_class = None
	dis = None
	result = [[] for v in range(3)]
	class_name = ['cp','im','pp','imU','om','omL','imL','imS']
	for number in [1,5,10,100]:# k
		correct = 0
		result = [[] for v in range(0,3)]	
		for data_index in range(0,len(test_data)):#0~35
			query_data = test_data[data_index]
			origin_class = query_data[11]
			reset_node(root)
			result_table = {'cp':0,'im':0,'pp':0,'imU':0,'om':0,'omL':0,'imL':0,'imS':0}
			for k in range(0,number):
				nearest_neighbor,predict_class = knn(root,query_data)
				result_table[predict_class] = result_table[predict_class] + 1
				if(data_index >= 0 and data_index <= 2):
					result[data_index].append(nearest_neighbor)

			max_vote = 0
			for i in range(0,len(class_name)):
				name = class_name[i]
				if result_table[name] > max_vote:
					max_vote = result_table[name]
					final_predict_class = class_name[i]

			#print('Origin:',origin_class,' Predict:',final_predict_class)
			if origin_class == final_predict_class:
				correct = correct + 1
		text_file.write('KNN accuracy: ' + str(float(correct/36.0)) + '\n')
		for cnt in range(0,3):
			for out_index in range(len(result[cnt])):
				text_file.write(str(result[cnt][out_index]) + ' ')
			text_file.write('\n')
		text_file.write('\n')
	text_file.close()		
					
def knn(root,query_data):
	cur_node = root
	nearest = None
	min_distance = 1000000.0
	record = []
	while cur_node is not None:
		record.append(cur_node)
		cur_distance = distance(query_data,cur_node.value)
		cur_divide = cur_node.divide
		if cur_distance < min_distance and cur_node.searched == 0:
			nearest = cur_node
			min_distance = cur_distance
		if(query_data[cur_divide] < cur_node.value[cur_divide]):
			cur_node = cur_node.left_child
		else:
			cur_node = cur_node.right_child

	while record:
		parent_node = record.pop()
		cur_divide = parent_node.divide
		if abs(float(query_data[cur_divide])-float(parent_node.value[cur_divide])) < min_distance:
			if query_data[cur_divide] < parent_node.value[cur_divide]:
				cur_node = parent_node.right_child
			else:
				cur_node = parent_node.left_child
			if cur_node is not None:
				while cur_node is not None:
					record.append(cur_node)
					cur_distance = distance(query_data,cur_node.value)
					cur_divide = cur_node.divide
					if cur_distance < min_distance and cur_node.searched == 0:
						nearest = cur_node
						min_distance = cur_distance
					if query_data[cur_divide] < cur_node.value[cur_divide]:
						cur_node = cur_node.left_child
					else:
						cur_node = cur_node.right_child
	nearest.searched = 1
	nearest_index = nearest.value[0]
	nearest_class = nearest.value[11]
	return nearest_index,nearest_class
						


def distance(query_data,cur_node_data):
	a = query_data
	b = cur_node_data
	value = 0
	for i in range (2,11):#2~10
		value = value + math.pow(a[i] - b[i],2)
	value = math.sqrt(value)
	return value

def tree_go(root):
	global counter
	counter = counter + 1
	print(root.value)
	print(counter)
	if(root.left_child is not None):
		tree_go(root.left_child)

	if(root.right_child is not None):
		tree_go(root.right_child)

	if(root.left_child is None and root.right_child is None):
		return

if __name__ == '__main__':
	train_name = sys.argv[1]
	test_name = sys.argv[2]
	root = None
	training_data = read_file(train_name)
	testing_data = read_file(test_name)
	#print(float(training_data[0][2]))
	#print(type(training_data[0][12]))
	root = build_tree(root,training_data,2)
	global counter
	counter = 0

	#print('TREE GO!')
	#tree_go(root)
	#print (counter)
	test(root,testing_data)
	#print('T 0:',training_data[0])
	#print('RL :',root.left_child.value)

	#print(distance(training_data[0],root.left_child))
