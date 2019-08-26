import sys
import os

pmax = 5
nmax = 3
testes = 10
filename = "teste.csv"
output = "output.txt"

log = [ 10**k for k in range(nmax) ]
for n in log:
    for p in range(1,pmax+1):
        os.system('echo ./parallel_primes {} t {} >> {}'.format(n,p,output) )
        for x in range(testes):
            os.system('./parallel_primes {} t {} >> {}'.format(n,p,output) )
            # file.write(str(p)+' threads;'+str(os.system('./parallel_primes {} t {} '.format(n,p) ) )+'; ')
        # file.write('\n')

file_in =  open(output, 'r')
file_out =  open(filename, 'w')

for line in file_in.readlines():
    if line[0] == '.':
        file_out.write("\n"+";".join(line[:-1].split(" ")))
    else:
        file_out.write(";"+line[:-1])


