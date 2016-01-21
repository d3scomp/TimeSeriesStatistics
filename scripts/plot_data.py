"""
Creates box plot from data processed by process_data.py
"""

import matplotlib.pyplot as plt
from numpy import *

separatePlots = False

fileloc = '../data/'
fileext = '-60-3'
sourcefileext = fileext + '.csv'
plotfileext = fileext + '.png'

print 'started ...'

mean = loadtxt(fileloc + 'mean' + sourcefileext, unpack=True)
lra = loadtxt(fileloc + 'lra' + sourcefileext, unpack=True)
lrb = loadtxt(fileloc + 'lrb' + sourcefileext, unpack=True)
lrx = loadtxt(fileloc + 'lrx' + sourcefileext, unpack=True)

# Scale from nano seconds to micro seconds
mean[:] = [x / 1000 for x in mean]
lra[:] = [x / 1000 for x in lra]
lrb[:] = [x / 1000 for x in lrb]
lrx[:] = [x / 1000 for x in lrx]

if not separatePlots:
    plt.figure()
    plt.boxplot([mean, lra, lrb, lrx])
    plt.ylabel('Micro seconds')
    plt.xticks([1,2,3,4],['mean', 'lra', 'lrb', 'lr(x)'])
    plt.xlabel('Statistical methods')
    plt.savefig(fileloc + 'boxplot' + plotfileext)
else:
    for name in ['mean', 'lra', 'lrb', 'lrx']:
        plt.figure()
        plt.boxplot(eval(name))
        plt.ylabel('Micro seconds')
        plt.xlabel(name)
        plt.savefig(fileloc + 'boxplot_' + name + plotfileext)

print 'done.'
