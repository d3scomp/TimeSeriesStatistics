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
	StudentsDistribution(size_t sampleCnt, double mean, double variance)
		: sampleCnt(sampleCnt), mean(mean), variance(variance) {

	}

	bool isLessThan(double sample, double confidence){
		TDistribution td;
		int err;
		double confidenceInterval = td.tnc(confidence, sampleCnt-1, 0, &err);
		if(err){
			std::cerr << "tnc exited with error " << err << std::endl;
 		}

		return sample < mean + confidenceInterval;
	}

	bool isLessThanOrEqual(double sample, double confidence){
		TDistribution td;
		int err;
		double confidenceInterval = td.tnc(confidence, sampleCnt-1, 0, &err);
		if(err){
			std::cerr << "tnc exited with error " << err << std::endl;
 		}

		return sample <= mean + confidenceInterval;
	}

	bool isGreaterThan(double sample, double confidence){
		TDistribution td;
		int err;
		double confidenceInterval = td.tnc(confidence, sampleCnt-1, 0, &err);
		if(err){
			std::cerr << "tnc exited with error " << err << std::endl;
 		}

		return sample > mean - confidenceInterval;
	}

	bool isGreaterThanOrEqual(double sample, double confidence){
		TDistribution td;
		int err;
		double confidenceInterval = td.tnc(confidence, sampleCnt-1, 0, &err);
		if(err){
			std::cerr << "tnc exited with error " << err << std::endl;
 		}

		return sample >= mean - confidenceInterval;
	}

private:
	size_t sampleCnt;
	double mean;
	double variance;

};

#endif /* STUDENTSDISTRIBUTION_H_ */

