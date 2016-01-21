/*******************************************************************************
 * Copyright 2016 Charles University in Prague
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

/*
 * StudentsDistribution.h
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 *
 *  This class provides methods to test hypothesis for given threshold on
 *  computed Student's distribution.
 */

#ifndef STUDENTSDISTRIBUTION_H_
#define STUDENTSDISTRIBUTION_H_

#include "TTable.h"

class StudentsDistribution {
public:
	StudentsDistribution(int df, double mean, double variance);

	bool isLessThanOrEqual(double threshold, ALPHAS a);
	bool isLessThanOrEqual(StudentsDistribution other, ALPHAS a);
	bool isGreaterThanOrEqual(double threshold, ALPHAS a);
	bool isGreaterThanOrEqual(StudentsDistribution other, ALPHAS a);

	inline bool isLessThan(double threshold, ALPHAS a) { return !isGreaterThanOrEqual(threshold, a); }
	inline bool isLessThan(StudentsDistribution other, ALPHAS a) { return !isGreaterThanOrEqual(other, a); }
	inline bool isGreaterThan(double threshold, ALPHAS a) { return !isLessThanOrEqual(threshold, a); }
	inline bool isGreaterThan(StudentsDistribution other, ALPHAS a) { return !isLessThanOrEqual(other, a); }

	inline double getMean() { return mean; }
	inline double getVariance() { return variance; }

private:
	int df; // degrees of freedom
	double mean;
	double variance;
	inline double getICDF(ALPHAS a, int dfValue);
	double getICDF(ALPHAS a) { return getICDF(a, df); }
};

#endif /* STUDENTSDISTRIBUTION_H_ */

