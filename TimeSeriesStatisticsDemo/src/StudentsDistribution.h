/*
 * StudentsDistribution.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
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
	StudentsDistribution(size_t *sampleCounts,
						 double *sampleSum,
						 double *squareSum,
						 size_t windowSize){
		sampleCnt = computeSampleCnt(sampleCounts, windowSize);
		mean = computeMean(sampleCnt, sampleSum, windowSize);
		variance = computeVariance(sampleCnt, mean, squareSum, windowSize);
	}

	bool isLessThan(double sample, double confidence){
		/*double combinedVariance = (variance/sampleCnt); // given sample variance is 0
			double combinedVRoot = sqrt(combinedVariance);
			double t = abs(mean - sample)/combinedVRoot;
			double tabulatedT = tTable[sampleCnt-1];
			return t <= tabulatedT;*/

		TDistribution td;
		int err;
		double confidenceInterval = td.tnc(confidence, sampleCnt-1, 0, &err);
		if(err){
			std::cerr << "tnc exited with error " << err << std::endl;
 		}

		return sample < mean + confidenceInterval;
	}

private:
	size_t sampleCnt;
	double mean;
	double variance;

	size_t computeSampleCnt(size_t *sampleCounts, size_t windowSize){
		size_t c = 0;
		for(size_t i = 0; i < windowSize; ++i){
			c += sampleCounts[i];
		}
		return c;
	}

	double computeMean(size_t sampleCnt, double *sampleSum, size_t windowSize){
		// m = 1/N * sum(x_i)
		double m = 0;
		for(size_t i = 0; i < windowSize; ++i){
			m += sampleSum[i];
		}
		m /= sampleCnt;
		return m;
	}

	double computeVariance(size_t sampleCnt, double mean, double *squareSum, size_t windowSize){
		// v^2 = (1/N * sum(x_i^2)) - m^2
		double v = 0;
		for(size_t i = 0; i < windowSize; ++i){
			v += squareSum[i];
		}
		v /= sampleCnt;
		v -= (mean*mean);
		return v;
	}
};

#endif /* STUDENTSDISTRIBUTION_H_ */

