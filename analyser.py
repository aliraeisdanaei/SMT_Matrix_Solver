import sys

class Node(object):
	def __init__(self, val: int, num_children: int):
		self.val = val
		self.num_children = num_children

	def __repr__(self):
		return f'Val: {self.val} Num Children: {self.num_children}'

	def __lt__(self, other):
		return self.num_children > other.num_children

class Thread(object):
	def __init__(self):
		self.solved = {}
	
	def is_solved(self, col):
		return col in self.solved

	def __repr__(self):
		return f'Len: {len(self.solved)}'
	
	def add_row(self, i_row):
		self.solved[i_row] = None

def is_parentless(parent_list: list):
	return len(parent_list) == 0

def get_parentless_parents(parents: dict, children: dict) -> list:
	"""
	Args:
		parents (dict): the dict of the parents
		children (dict): the dict of the children

	Returns:
		list: of the simple parentless parents
			all parentless parents are popped from the parents dictionary
	"""
	parentless_parents = []
	for parent in parents:
		if(is_parentless(parents[parent])):
			# try:
			# 	crnt_parent_node = Node(parent, len(children[parent]))
			# except KeyError:
			# 	crnt_parent_node = Node(parent, 0)
			parentless_parents.append(parent)
	for parentless in parentless_parents:
		parents.pop(parentless)
	
	# parentless_parents.sort()
	return parentless_parents

def write_str(filename, s):
	with open(filename, 'w') as file:
		file.write(s)

def read(filename) -> tuple:
	parents = {}
	children = {}
	matrix_index = {}
	index_rows = {}
	index_cols = {}
	num_rows = 0 
	num_cols = 0
	num_non_zeros = 0
	
	with open(filename, 'r') as file:
		lines = file.readlines()
		i = 0
		first_line = True
		for line in lines:
			if(not line.startswith('%') and not line == "\n"):
				values = line.split(' ')
				if(first_line):
					num_rows = int(values[0])
					num_cols = int(values[1])
					num_non_zeros = int(values[1])
					first_line = False
				else:
					row = int(values[0]) - 1
					col = int(values[1]) - 1
					matrix_index[(row, col)] = i
					i += 1
					if(row == col):
						try:
							parents[row]
						except KeyError:
							parents[row] = []
						try:
							children[col]
						except KeyError:
							children[col] = []
					else:
						try:
							parents[row].append(col)
						except KeyError:
							parents[row] = []
							parents[row].append(col)

						try:
							children[col].append(row)
						except KeyError:
							children[col] = []
							children[col].append(row)
					
					if row not in index_rows:
						index_rows[row] = row
					if col not in index_cols:
						index_cols[col] = col

	return (parents, children, matrix_index, index_rows, index_cols)

def get_matrix_index(row: int, col: int, matrix_index: dict) -> int:
	return matrix_index[[row, col]]

def solve_children(parent, parents, children):
	try:
		children_of_parent = children[parent]

		for child in children_of_parent:
			# solve that child
			parents[child].remove(parent)
		children.pop(parent)
	except:
		pass

def solve_row(i_row, parents, children):
	# make all of my parents disown me
	my_parents = parents[i_row]
	for parent in my_parents:
		children[parent].remove(i_row)
	
	# I have no more parents, I am a parentless parent
	parents[i_row] = []

def split_into_n_threads(rows, parents, num_threads) -> tuple:
	threads = [Thread() for i in range(num_threads)]
	left_overs = Thread()

	for i_row in rows:
		fit_in_threads = [True for i in range(num_threads)]
		my_parents = parents[i_row]
		for parent in my_parents:
			for i_thread in range(num_threads):
				fit_in_threads[i_thread] = threads[i_thread].is_solved(parent) and fit_in_threads[i_thread]

		thread_preference = i_row % num_threads
		for i_thread in range(num_threads):
			i_thread_try = (i_thread + thread_preference) % num_threads
			if(fit_in_threads[i_thread_try]):
				threads[i_thread_try].add_row(i_row)
				break
			elif(i_thread == num_threads - 1):
				# last thread to try and could not fit
				left_overs.add_row(i_row)
	
	return (threads, left_overs)

def create_c_row_ordering(rows, var_name) -> str:
	c_code = ''
	if(var_name != ''):
		c_code += f'int {var_name}_len = {len(rows)};\n'
		c_code += f'int {var_name}[{len(rows)}] = ' + '{'
	else:
		c_code += '{'

	for i in range(len(rows)):
		c_code += f'{rows[i]}'
		if(i != len(rows) - 1):
			c_code += ', '

	if(var_name != ''):
		c_code += '};\n'
	else:
		c_code += '}'

	return c_code

def create_c_col_ordering(parentless_parents, children, matrix_index, var_name) -> str:
	c_code = ''
	if(var_name != ''):
		c_code += f'int {var_name}_len = {len(rows)};\n'
		c_code += f'int {var_name}[{len(rows)}][3] = ' + '{'
	else:
		c_code += '{'
	for i in range(len(parentless_parents)):
		parent = parentless_parents[i]
		children_of_parent = children[parent]
		for j in range(len(children_of_parent)):
			i_val = matrix_index[(children_of_parent[j], parent)] 
			i_sol = parent
			i_b = children_of_parent[j]
			c_code += '{' + f'{i_val}, {i_sol}, {i_b}' + '}'
			if(j != len(children_of_parent) - 1):
				c_code += ', '
		if(i != len(parentless_parents) - 1 and len(children_of_parent) != 0):
			c_code += ', '
	if(var_name != ''):
		c_code += '};\n'
	else:
		c_code += '}'
	return c_code

def create_c_thread_row_ordering(threads: list, children, var_name) -> str:
	c_code = ''
	c_code += f'int number_threads = {len(threads)};\n'
	c_code += f'int {var_name}_lens[] = ' + '{'
	threads_list = [list(x.solved.keys()) for x in threads]
	longest_thread_len = 0
	for i in range(len(threads_list)):
		c_code += f'{len(threads_list[i])}'
		if(i != len(threads_list) - 1):
			c_code += ', '
		if(longest_thread_len < len(threads_list[i])):
			longest_thread_len = len(threads_list[i])

	c_code += '};\n'
	c_code += f'int {var_name}[{len(threads)}][{longest_thread_len}] = ' + '{'
	for i in range(len(threads_list)):
		c_code += create_c_row_ordering(threads_list[i], '')
		if(i != len(threads_list) - 1):
			c_code += ', '
	c_code += '};\n'
	return c_code


if(__name__ == "__main__"):
	try:
		filename = sys.argv[1]
	except:
		filename = "Inputs/mtx_files/torso1_leftside.mtx" 

	num_rows_first_section = 8000
	num_threads = 4

	c_code = ''

	parents, children, matrix_index, index_rows, index_cols = read(filename)
	print(f"num rows: {len(parents)}")

	rows = list(parents.keys())
	rows.sort()
	for i_row in rows[:num_rows_first_section]:
		solve_row(i_row, parents, children)

	parentless_parents = get_parentless_parents(parents, children)
	first_rows_c_code = create_c_row_ordering(parentless_parents, 'first_rows_sync')
	first_cols_c_code = create_c_col_ordering(parentless_parents, children, matrix_index, 'first_cols_async')

	total = 0
	len_parentless = len(parentless_parents)
	print(f"len of parentless: {len_parentless}")
	for parentless in parentless_parents:
		total += len(children[parentless])
		solve_children(parentless, parents, children)
	print(f"total {total} avg: {float(total) / len_parentless}")

	rows = list(parents.keys())
	rows.sort()
	threads, left_overs = split_into_n_threads(rows, parents, num_threads)
	threads_c_code = create_c_thread_row_ordering(threads, children, 'threads') 
	print('threads: ', threads)
	print('left overs: ', left_overs)
	for thread in threads:
		 thread_rows = thread.solved
		 for row in thread_rows:
			 solve_row(row, parents, children)

	parentless_parents = get_parentless_parents(parents, children)
	second_cols_c_code = create_c_col_ordering(parentless_parents, children, matrix_index, 'second_cols_async')

	total = 0
	len_parentless = len(parentless_parents)
	print(f"len of parentless: {len_parentless}")
	for parentless in parentless_parents:
		total += len(children[parentless])
		solve_children(parentless, parents, children)
	print(f"total {total} avg: {float(total) / len_parentless}")
	rows = list(parents.keys())
	rows.sort()
	print(f"num rows: {len(rows)}")
	second_rows_c_code = create_c_row_ordering(rows, 'second_rows_sync')

	c_code += first_rows_c_code + '\n'
	c_code += first_cols_c_code + '\n'
	c_code += threads_c_code + '\n'
	c_code += second_cols_c_code + '\n'
	c_code += second_rows_c_code 

	write_str('optimised_orderings.c', c_code)