"""
Process data from runs on STM32F4 DISCOVERY into time duration samples.
"""

import csv

samplesExt = '-10-1'

computationON = 0b10000000
batchON = 0b01000000
phasemask = 0b00110000
phaseMEAN = 0b00000000
phaseLRA = 0b00010000
phaseLRB = 0b00100000
phaseLRX = 0b00110000
mean = 0
lra = 1
lrb = 2
lrx = 3

starttime = None
times = [[] for i in [mean, lra, lrb, lrx]]

print 'started ...'

with open ('../data/smodes' + samplesExt + '.csv', 'rb') as datafile:
    spamreader = csv.reader(datafile, delimiter=',')
    for row in spamreader:
        code = int(row[1], 2)
        if code & batchON:
            if code & computationON and starttime is None:
                starttime = long(row[0])
                if (code & phasemask) == phaseMEAN:
                    phase = mean
                elif (code & phasemask) == phaseLRA:
                    phase = lra
                elif (code & phasemask) == phaseLRB:
                    phase = lrb
                elif (code & phasemask) == phaseLRX:
                    phase = lrx
            elif (not code & computationON) and starttime is not None:
                endtime = long(row[0])
                times[phase].append(endtime - starttime)
                starttime = None
        elif starttime is not None:
            endtime = long(row[0])
            times[phase].append(endtime - starttime)
            starttime = None
            
    
#     for p in [mean, lra, lrb, lrx]:
#         print "phase ", p
#         print times[p]

    for n in ['mean', 'lra', 'lrb', 'lrx']:       
        f = open('../data/' + n + samplesExt + '.csv', 'w')
        for row in times[eval(n)]:
            f.write(str(row) + '\n')
        f.close()
        
    print 'done.'
    
                
                
                