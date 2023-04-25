#!/usr/bin/python3

import sys
import re

def output_avg_time(filename):
    with open(filename, "r") as inp:
        count = 0
        time_taken = 0
        total_count = 0
        for line in inp.readlines():
            if "time" in line:
                tokens = re.split(" |\n", line)
                total_count = total_count + 1
                if tokens[0] == 'Correct':
                    time_taken = time_taken + float(tokens[4])
                    count = count + 1
        short_file_name = filename.split("/")[-1]
        if count == 0:
            print(short_file_name, count, total_count, time_taken)
        else:
            print(short_file_name,  count, total_count, time_taken/count)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print ("Invalid number of arguments. Usage# python3 compute_output.py <file_name>")
        sys.exit()
    file_name = sys.argv[1]

    output_avg_time(file_name)

