import sys
import re

regex = re.compile("0b([01]*), (\/\* .* \*\/)")

# 7 Segment: DP-G-F-E-D-C-B-A -> F-A-B-G-C-D-E-DP

for line in sys.stdin:
    elements = regex.search(line)
    if elements != None:
        if len(elements[1]) == 8:
            output = ""

            output += elements[1][2]
            output += elements[1][7]
            output += elements[1][6]
            output += elements[1][1]

            output += elements[1][5]
            output += elements[1][4]
            output += elements[1][3]
            output += elements[1][0]

            print(f'\t0b{output}, {elements[2]} /* From: {elements[1]} */')
        elif len(elements[1]) == 15:
            print(f'\t0b{elements[1]}, {elements[2]} /* Not converted.. */')
        elif len(elements[1]) == 17:
            print(f'\t0b{elements[1]}, {elements[2]} /* Not converted.. */')
    else:
        print(line, end='')

