# -*- coding: utf-8 -*-
"""
Generates files with tabulated quantiles Student's t-distribution for alphas given on the command line.

Usage: gen_ttable <alpha 1> ... <alpha n>

If no alphas are given on the command line, it generates the table for 0.025 and 0.05

Example: gen_ttable 0.0125 0.025 0.05

"""

import numpy as np
from scipy.stats import t
import sys
import re


minorCount = 100
boost = 3


def getLimits(dfMax):
	base = 0
	for majorIdx in xrange(0, sys.maxint):
		minorStep = (1 << majorIdx * boost)
		base += minorStep * minorCount

		if base >= dfMax:
			break

	return (base, (majorIdx + 1) * minorCount)


dfMax, idfTableSize = getLimits(100000)


def getICDFTable(alpha):
	icdf = np.zeros((1, minorCount))
	base = 0
	for majorIdx in xrange(0, sys.maxint):
		minorStep = (1 << majorIdx * boost)
		for minorIdx in xrange(0, minorCount):
			df = (minorIdx + 1) * minorStep + base
			icdf[majorIdx, minorIdx] = t.ppf(alpha, df)

		base += minorStep * minorCount

		if base >= dfMax:
			break

		icdf.resize((majorIdx + 2, minorCount))
		
	return icdf.reshape((-1))


	
alphas = [float(alphaStr) for alphaStr in sys.argv[1:]]
if len(alphas) == 0:
	alphas = [0.025, 0.05]


out = open('../src/TTable.cpp', 'w')
out.write('''/*
 * TTable.cpp
 *
 * Quantiles of the Student's t-distribution
 */

#include "TTable.h"

const int minor_count = {0};
const int boost = {1};
const int df_max = {2};

const double icdf[{3}][{4}] = {{
{5}
}};
'''.format(
	minorCount, 	boost, dfMax,
	len(alphas), idfTableSize, 
	',\n\n'.join( ['\t{' + ','.join([str(val) for val in getICDFTable(alpha)]) + '}' for alpha in alphas] )
))
out.close()

dotToUnderscore = re.compile('\.')

out = open('../src/TTable.h', 'w')
out.write('''/*
 * TTable.h
 *
 * Header file for quantiles of the Student's t-distribution
 */

#ifndef TTABLE_H_
#define TTABLE_H_

enum ALPHAS {{{0}}};

extern const int minor_count;
extern const int boost;
extern const int df_max;

extern const double icdf[{1}][{2}];

#endif /* TTABLE_H_ */
'''.format(
	', '.join(['ALPHA_' + dotToUnderscore.sub('_', str(alpha)) for alpha in alphas]),
	len(alphas), idfTableSize
))
out.close()



"""

def getICDF(df, icdfTable):
	base = 0
	majorIdx = -1
	base_incr = 0
	minorStep = 1
	while base + base_incr < df:
		base += base_incr
		majorIdx += 1
		minorStep = (1 << majorIdx * boost)
		base_incr = minorStep * minorCount
		
	df -= base
	if df % minorStep == 0:
		minorIdx = df / minorStep - 1
		idx = majorIdx * minorCount + minorIdx
		# stdout.write('base: {}  majorIdx: {}  minorIdx: {}  idx: {}\n'.format(base, majorIdx, minorIdx, idx))
		return icdfTable[idx]
	else:
		minorIdx = df / minorStep
		df_high = base + (minorIdx + 1) * minorStep
		df_low = df_high - minorStep
		idx_high = majorIdx * minorCount + minorIdx
		idx_low = idx_high - 1
		delta = float(df) / (df_high - df_low)
		# stdout.write('base: {}  majorIdx: {}  minorIdx: {}  df_high: {}  df_low: {}  idx_high: {}  idx_low: {}  delta: {}\n'.format(base, majorIdx, minorIdx, df_high, df_low, idx_high, idx_low, delta))
		return icdfTable[idx_high] * delta + icdfTable[idx_low] * (1 - delta)
		

"""