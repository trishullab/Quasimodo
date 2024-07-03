#!/usr/bin/python3

import sys
import re
import os
from collections import OrderedDict

def output_avg_time(inp_folder, out_folder):
    directory = os.fsencode(inp_folder)
    benchmarks = ['ghz', 'bv', 'dj', 'simons', 'qft', 'grovers', 'shors']
    values = {'ghz': OrderedDict(), 'bv':OrderedDict(), 'dj':OrderedDict(), 'simons':OrderedDict(), 'qft':OrderedDict(), 'grovers': OrderedDict(), 'shors': OrderedDict() }
    mem_vals = {'ghz': OrderedDict(), 'bv':OrderedDict(), 'dj':OrderedDict(), 'simons':OrderedDict(), 'qft':OrderedDict(), 'grovers': OrderedDict(), 'shors': OrderedDict() }
    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        if filename.endswith(".txt"): 
            with open(inp_folder + "/" + filename, "r") as inp:
                count = 0
                time_taken = 0
                total_count = 0
                memory = 0
                for line in inp.readlines():
                    if "time" in line:
                        tokens = re.split(" |\n", line)
                        total_count = total_count + 1
                        time_taken = time_taken + float(tokens[4])
                        memory = memory + int(tokens[8])
                        count = count + 1
                short_file_name = filename.split("/")[-1].split('.')[0]
                entry = short_file_name.split('_')
                # curr_benchmark = values[entry[0]]
                if int(entry[2]) in values[entry[0]]:
                    if count == 0:
                        values[entry[0]][int(entry[2])][entry[1]] = 'Timeout (15min)/OOM'
                        mem_vals[entry[0]][int(entry[2])][entry[1]] = 'Timeout (15min)/OOM'
                    else:
                        values[entry[0]][int(entry[2])][entry[1]] = time_taken/count
                        mem_vals[entry[0]][int(entry[2])][entry[1]] = memory/count
                else:
                    val = 'Timeout (15min)/OOM'
                    mem_val = 'Timeout (15min)/OOM'
                    if count != 0:
                        val = time_taken/count
                        mem_val = memory/count
                    values[entry[0]][int(entry[2])] = {entry[1] : val}
                    mem_vals[entry[0]][int(entry[2])] = {entry[1] : mem_val}
                    
    with open(out_folder + "/" + 'table.tex', 'w') as f:
        # Creating a table with results in tex format
        pretext = ['\\begin{table}[h]\n', '\\centering\n', '\\begin{tabular}{|c|c|c|c|c|}\n', '\\hline\n', '\\multirow{2}*{Benchmarks} & \\multirow{2}*{\#Qubits} & \\multicolumn{2}{c|}{WCFLOBDD} & \\multicolumn{2}{c|}{WBDD} & \\multicolumn{2}{c|}{CFLOBDD} \\\\ \n', '\\cline{3-5}\n', ' & & Time (sec) & Time (sec) & Time (sec) \\\\ \n']
        f.writelines(pretext)
        f.write('\\hline \n')
        for benchmark in values:
            curr_values = values[benchmark]
            curr_mem_values = mem_vals[benchmark]
            for qubit in sorted(curr_values.keys()):
                v1 = 'Timeout/OOM'
                v2 = 'Timeout/OOM'
                v3 = 'Timeout/OOM'
                m1 = v1
                m2 = v2
                m3 = v3
                if 'CFLOBDD' in curr_values[qubit] and not isinstance(curr_values[qubit]['CFLOBDD'], str):
                    v1 = "%0.4f" % curr_values[qubit]['CFLOBDD']
                    m1 = "%0.4f" % curr_mem_values[qubit]['CFLOBDD'] 
                if 'WCFLOBDD' in curr_values[qubit] and not isinstance(curr_values[qubit]['WCFLOBDD'], str):
                    v2 = "%0.4f" % curr_values[qubit]['WCFLOBDD']
                    m2 = "%0.4f" % curr_mem_values[qubit]['WCFLOBDD'] 
                if 'WBDD' in curr_values[qubit] and not isinstance(curr_values[qubit]['WBDD'], str) :
                    v3 = "%0.4f" % curr_values[qubit]['WBDD']
                    m3 = "%0.4f" % curr_mem_values[qubit]['WBDD'] 
                if benchmark != "shors":
                    if not isinstance(v1, str) and not isinstance(v2, str) and not isinstance(v3, str):
                        f.write(benchmark + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                    elif isinstance(v1, str):
                        f.write(benchmark + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " \\\\ \n") 
                    elif isinstance(v2, str):
                        f.write(benchmark + " & " + str(qubit) + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                    elif isinstance(v3, str):
                        f.write(benchmark + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v1 + " & " + m1 + " \\\\ \n")
                else:
                    if qubit == 4:
                        if not isinstance(v1, str) and not isinstance(v2, str) and not isinstance(v3, str):
                            f.write(benchmark + "(15,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v1, str):
                            f.write(benchmark + "(15,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " \\\\ \n")
                        elif isinstance(v2, str):
                            f.write(benchmark + "(15,2)" + " & " + str(qubit) + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v3, str):
                            f.write(benchmark + "(15,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v1 + " & " + m1 + " \\\\ \n")
                    if qubit == 5:
                        if not isinstance(v1, str) and not isinstance(v2, str) and not isinstance(v3, str):
                            f.write(benchmark + "(21,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v1, str):
                            f.write(benchmark + "(21,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " \\\\ \n")
                        elif isinstance(v2, str):
                            f.write(benchmark + "(21,2)" + " & " + str(qubit) + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v3, str):
                            f.write(benchmark + "(21,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v1 + " & " + m1 + " \\\\ \n")
                    if qubit == 6:
                        if not isinstance(v1, str) and not isinstance(v2, str) and not isinstance(v3, str):
                            f.write(benchmark + "(39,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v1, str):
                            f.write(benchmark + "(39,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " \\\\ \n")
                        elif isinstance(v2, str):
                            f.write(benchmark + "(39,2)" + " & " + str(qubit) + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v3, str):
                            f.write(benchmark + "(39,2)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v1 + " & " + m1 + " \\\\ \n")
                    if qubit == 7:
                        if not isinstance(v1, str) and not isinstance(v2, str) and not isinstance(v3, str):
                            f.write(benchmark + "(95,8)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v1, str):
                            f.write(benchmark + "(95,8)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + v3 + " & " + m3 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " \\\\ \n")
                        elif isinstance(v2, str):
                            f.write(benchmark + "(95,8)" + " & " + str(qubit) + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v3 + " & " + m3 + " & " + v1 + " & " + m1 + " \\\\ \n")
                        elif isinstance(v3, str):
                            f.write(benchmark + "(95,8)" + " & " + str(qubit) + " & " + v2 + " & " + m2 + " & " + " \\multicolumn{2}{c|}{Timeout/OOM} " + " & " + v1 + " & " + m1 + " \\\\ \n")
                f.write('\\hline \n')
        
        posttext = ['\\end{tabular}\n', '\\end{table}\n']
        f.writelines(posttext)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print ("Invalid number of arguments. Usage# python3 compute_results.py <inp_folder> <out_folder>")
        sys.exit()
    inp_folder = sys.argv[1]
    out_folder = sys.argv[2]

    output_avg_time(inp_folder, out_folder)

