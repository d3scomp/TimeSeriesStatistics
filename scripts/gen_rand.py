#!/usr/bin/python 

"""
Generates random x,y samples, where x is drawn from uniform distribution and y is drawn from normal distribution

Usage: get_rand.py <outfile> <number of samples> <beta>
"""

import numpy as np
import sys


len = int(sys.argv[2])
beta = float(sys.argv[3])

x = np.sort(np.random.uniform(0, 60000, len))
y = np.random.normal(2, 3, len) + x*beta

out = open(sys.argv[1], "w")
out.write("""#define randLen {0}
int randX[randLen] = {{{1}}};
double randY[randLen] = {{{2}}};
""".format(
	len, 
	",".join([str(int(v)) for v in x]),
	",".join([str(v) for v in y])
))

out.close()
