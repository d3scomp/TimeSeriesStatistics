"""
Creates box plot from data processed by process_data.py
"""

import matplotlib.pyplot as plt
from numpy import *

separatePlots = False

fileloc = '../data/'
fileext = '-10-1'
sourcefileext = fileext + '.csv'
plotfileext = fileext + '.png'

print 'started ...'

mean = loadtxt(fileloc + 'mean' + sourcefileext, unpack=True)
lra = loadtxt(fileloc + 'lra' + sourcefileext, unpack=True)
lrb = loadtxt(fileloc + 'lrb' + sourcefileext, unpack=True)
lrx = loadtxt(fileloc + 'lrx' + sourcefileext, unpack=True)

if not separatePlots:
    plt.figure()
    plt.boxplot([mean, lra, lrb, lrx])
    plt.ylabel('Nano seconds')
    plt.xticks([1,2,3,4],['mean', 'lra', 'lrb', 'lr(x)'])
    plt.xlabel('Statistical methods')
    plt.savefig(fileloc + 'boxplot' + plotfileext)
else:
    for name in ['mean', 'lra', 'lrb', 'lrx']:
        plt.figure()
        plt.boxplot(eval(name))
        plt.ylabel('Nano seconds')
        plt.xlabel(name)
        plt.savefig(fileloc + 'boxplot_' + name + plotfileext)

print 'done.'
