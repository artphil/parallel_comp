import sys
import os
import json

filename = "t_threads"
output = "output"

file_in =  open(output, 'r')
file_out =  open(filename, 'w')

d_times = {}
for line in file_in.readlines():
    data = line.split(" ")
    print (data)
    thread = data[4]
    time = float(data[6])
    number = data[10]

    if thread in d_times:
        d_times[thread]['time'] += time
        d_times[thread]['number'] += number

    else:
        d_times[thread] = {}
        d_times[thread]['time'] = time
        d_times[thread]['number'] = number


file_out.write(json.dumps(d_times, indent=True))
