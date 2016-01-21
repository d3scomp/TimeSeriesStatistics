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
 * StudentsDistribution.cpp
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 *
 *  This class provides methods to test hypothesis for given threshold on
 *  computed Student's distribution.
 */

#include "StudentsDistribution.h"
#include <math.h>

/*
 * df - degrees of freedom
 */
StudentsDistribution::StudentsDistribution(int df, double mean, double variance)
		: df(df), mean(mean), variance(variance) {

	}

	bool StudentsDistribution::isLessThanOrEqual(double threshold, ALPHAS a) {
		if (df < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double dist = (mean - threshold) / sqrt(variance);
		double icdfValue = -getICDF(a);

		return icdfValue >= dist;
	}

	bool StudentsDistribution::isGreaterThanOrEqual(double threshold, ALPHAS a) {
		if (df < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double dist = (mean - threshold) / sqrt(variance);
		double icdfValue = getICDF(a);
	
		return icdfValue <= dist;
	}

	double StudentsDistribution::getICDF(ALPHAS a) {
		const double *icdfA = icdf[a];
		int base = 0;
		int major_idx = -1;
		int base_incr = 0;
		int minor_step = 1;
		while (base + base_incr < df) {
			base += base_incr;
			major_idx += 1;
			minor_step = (1 << major_idx * boost);
			base_incr = minor_step * minor_count;
		}
		df -= base;
		if (df % minor_step == 0) {
			int minor_idx = df / minor_step - 1;
			int idx = major_idx * minor_count + minor_idx;
			return icdfA[idx];
		} else {
			int minor_idx = df / minor_step;
			int df_high = base + (minor_idx + 1) * minor_step;
			int df_low = df_high - minor_step;
			int idx_high = major_idx * minor_count + minor_idx;
			int idx_low = idx_high - 1;
			double delta = double(df) / (df_high - df_low);
			return icdfA[idx_high] * delta + icdfA[idx_low] * (1 - delta);
		}
	}



