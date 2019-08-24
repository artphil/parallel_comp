import sys
import os

pmax = 10
nmax = 8
testes = 10

# file = open(sys.argv[1], 'w')
# if not file: quit()

log = [ 10**k for k in range(nmax) ]
for n in log:
    for p in range(1,pmax+1):
        # file.write('./parallel_primes {} t {}\n >> t1'.format(n,p) )
        os.system('echo ./parallel_primes {} t {} >> t1'.format(n,p) )
        for x in range(testes):
            os.system('./parallel_primes {} t {} >> t1'.format(n,p) )
            # file.write(str(p)+' threads;'+str(os.system('./parallel_primes {} t {} '.format(n,p) ) )+'; ')
        # file.write('\n')

# file.close()