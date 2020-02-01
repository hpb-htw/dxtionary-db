# single line usage:
# python3 gen-big-raw-data.py <num-of-row> | gzip -f - > data.txt.gz
# for example:
# python3 gen-big-raw-data.py 10245679 | gzip -f - > data.txt.gz

import sys

colums = ["column_1", "column_2", "column_3", "column_4"]

# print all column
print ( ";".join(colums) )

num_of_row = int(sys.argv[1])
row_factor = int(sys.argv[2])
for i in range(num_of_row):
    row = []
    for j in range(len(colums)):
        row.append( "data {}-{}/".format(i,j)*row_factor )
        pass
    print( ";".join(row) )
    pass