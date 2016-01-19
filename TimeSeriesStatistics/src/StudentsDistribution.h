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
#include "TDistribution.h"

class StudentsDistribution {
public:
	StudentsDistribution(size_t sampleCnt, double mean, double variance);
	bool isLessThan(double sample, double confidence);
	bool isLessThanOrEqual(double sample, double confidence);
	bool isGreaterThan(double sample, double confidence);
	bool isGreaterThanOrEqual(double sample, double confidence);

private:
	size_t sampleCnt;
	double mean;
	double variance;

};

#endif /* STUDENTSDISTRIBUTION_H_ */

