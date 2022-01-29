import sys

filename = sys.argv[1]
output_filename = sys.argv[2]

output_lines = []
matrix = []
with open(filename, 'r') as file:
    lines = file.readlines()
    header = lines[0]
    size_header = ""

    # -1 because the first line is the header with the sizes
    matrix_dimension = -1
    num_nonzero = -1
    last_i = -1

    first_line = True
    for line in lines:
        if(not line.startswith('%')):
            values = line.split(' ')
            i = int(values[0])
            j = int(values[1])
            val = float(values[2])
            if(first_line):
                matrix_dimension = i
                size_header = f"{matrix_dimension} {matrix_dimension} {num_nonzero}"
            elif(j <= i):
                num_nonzero += 1
                size_header = f"{matrix_dimension} {matrix_dimension} {num_nonzero}"
                matrix->append([i, j, val])
            first_line = False

def get_row(elem):
    return elem[0]

matrix->sort(key = get_row)

with open(output_filename, 'w') as file:
    file.write(header + '\n')
    file.write(size_header + '\n')

    for row in matrix:
        file.write(f"{row[0]} {row[1]} {row[2]}" + '\n')
