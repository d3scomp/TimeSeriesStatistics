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

	bool isLessOrEqualTo(double threshold, ALPHAS confidence);
	bool isLessOrEqualTo(StudentsDistribution other, ALPHAS confidence);
	bool isGreaterOrEqualTo(double threshold, ALPHAS confidence);
	bool isGreaterOrEqualTo(StudentsDistribution other, ALPHAS confidence);

	inline bool isLessThan(double threshold, ALPHAS confidence) { return !isGreaterOrEqualTo(threshold, confidence); }
	inline bool isLessThan(StudentsDistribution other, ALPHAS confidence) { return !isGreaterOrEqualTo(other, confidence); }
	inline bool isGreaterThan(double threshold, ALPHAS confidence) { return !isLessOrEqualTo(threshold, confidence); }
	inline bool isGreaterThan(StudentsDistribution other, ALPHAS confidence) { return !isLessOrEqualTo(other, confidence); }

	inline bool operator<=(double threshold) { return isLessOrEqualTo(threshold, a); }
	inline bool operator<=(StudentsDistribution other) { return isLessOrEqualTo(other, a); }
	inline bool operator>=(double threshold) { return isGreaterOrEqualTo(threshold, a); }
	inline bool operator>=(StudentsDistribution other) { return isGreaterOrEqualTo(other, a); }

	inline bool operator<(double threshold) { return isLessThan(threshold, a); }
	inline bool operator<(StudentsDistribution other) { return isLessThan(other, a); }
	inline bool operator>(double threshold) { return isGreaterThan(threshold, a); }
	inline bool operator>(StudentsDistribution other) { return isGreaterThan(other, a); }

	inline double getMean() { return mean; }
	inline double getVariance() { return variance; }

	void setAlpha(ALPHAS a) { this->a = a; }
	void resetToDefaultAlpha() { a = static_cast<ALPHAS>(DEFAULT_ALPHA_INDEX); }
	static void setDefaultAlpha(int index) { DEFAULT_ALPHA_INDEX = index; }
	ALPHAS getAlpha() { return a; }

private:
	int df; // degrees of freedom
	double mean;
	double variance;
	ALPHAS a;

	double getICDF(ALPHAS confidence, int dfValue);
	inline double getICDF(ALPHAS confidence) { return getICDF(confidence, df); }

	static int DEFAULT_ALPHA_INDEX;
};

#endif /* STUDENTSDISTRIBUTION_H_ */

