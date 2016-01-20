/*
 * StudentsDistribution.h
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 */

#ifndef STUDENTSDISTRIBUTION_H_
#define STUDENTSDISTRIBUTION_H_

#define MAX_SAMPLES	100
#define CONFIDENCE 0.95

#include <cstdlib>
#include <iostream>
#include <math.h>
#include "TTable.h"

class StudentsDistribution {
public:
	/*
	 * df - degrees of freedom
	 */
	StudentsDistribution(size_t df, double mean, double variance);
	bool isLessThan(double sample, ALPHAS a);
	bool isLessThanOrEqual(double sample, ALPHAS a);
	bool isGreaterThan(double sample, ALPHAS a);
	bool isGreaterThanOrEqual(double sample, ALPHAS a);

private:
	size_t df; // degrees of freedom
	double mean;
	double variance;
	double getICDF(ALPHAS a);
};

#endif /* STUDENTSDISTRIBUTION_H_ */

