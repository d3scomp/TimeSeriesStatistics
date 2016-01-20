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
	StudentsDistribution(int df, double mean, double variance);
	bool isLessThan(double sample, ALPHAS a);
	bool isLessThanOrEqual(double sample, ALPHAS a);
	bool isGreaterThan(double sample, ALPHAS a);
	bool isGreaterThanOrEqual(double sample, ALPHAS a);

private:
	int df; // degrees of freedom
	double mean;
	double variance;
	double getICDF(ALPHAS a);
};

#endif /* STUDENTSDISTRIBUTION_H_ */

