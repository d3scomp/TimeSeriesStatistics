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
		resetDefaultAlpha();
	}

	bool StudentsDistribution::operator<=(double threshold) {
		if (df < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double dist = (mean - threshold) / sqrt(variance);
		double icdfValue = -getICDF();

		return icdfValue >= dist;
	}

	bool StudentsDistribution::operator<=(StudentsDistribution other) {
		double combinedMean = this->mean - other.mean;
		double combinedVariance = this->variance + other.variance;

		int combinedDf = (combinedVariance*combinedVariance)
				/ ((this->variance*this->variance) / (double) this->df
						+ (other.variance*other.variance) / (double) other.df);
		if (combinedDf < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double icdfValue = -getICDF(combinedDf);

		return icdfValue >= combinedMean / sqrt(combinedVariance);
	}

	bool StudentsDistribution::operator>=(double threshold) {
		if (df < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double dist = (mean - threshold) / sqrt(variance);
		double icdfValue = getICDF();
	
		return icdfValue <= dist;
	}

	bool StudentsDistribution::operator>=(StudentsDistribution other) {
		double combinedMean = this->mean - other.mean;
		double combinedVariance = this->variance + other.variance;

		int combinedDf = (combinedVariance*combinedVariance)
				/ ((this->variance*this->variance) / (double) this->df
						+ (other.variance*other.variance) / (double) other.df);
		if (combinedDf < 1) {
			// accept hypothesis if there is not enough samples
			return true;
		}

		double icdfValue = getICDF(combinedDf);

		return icdfValue <= combinedMean / sqrt(combinedVariance);
	}

	double StudentsDistribution::getICDF(int dfValue) {
		/* Though declared as one-dimensional, icdfA (a table with critical values for a particular alpha) is essentially a two dimensional table. 
		   The rows are indexed by major_idx, the columns are indexed by minor_idx. 
		   The first row corresponds to critical values for degrees of freedom 1, 2, 3, ..., minor_count
		   The second row starts where the previous row left off and goes by minor_step, i.e.  minor_count + minor_step, minor_count + minor_step * 2, ..., minor_count + minor_step * minor_count
		   The same pattern repets for all subsequent rows. The minor_step is computed as 2^(major_idx * boost), which means that the minor_step grows dramatically with every row.
		   As the result the algorithm below needs only a couple of iterations to get to degrees of freedom in order of milions. With current settings, it needs 6 iterations to reach df=1e9
		   Once it finds the critical value, it returns it. If the particular degree of freedom is not present, it interpolates the critical value
		   between to closest higer and lower degree of freedom.
		*/

		const double *icdfA = ttable::icdf[a];
		int base = 0;
		int major_idx = -1;
		int base_incr = 0;
		int minor_step = 1;
		while (base + base_incr < dfValue) {
			base += base_incr;
			major_idx += 1;
			minor_step = (1 << major_idx * ttable::boost);
			base_incr = minor_step * ttable::minor_count;
		}
		dfValue -= base;
		if (dfValue % minor_step == 0) {
			int minor_idx = dfValue / minor_step - 1;
			int idx = major_idx * ttable::minor_count + minor_idx;
			return icdfA[idx];
		} else {
			int minor_idx = dfValue / minor_step;
			int df_high = base + (minor_idx + 1) * minor_step;
			int df_low = df_high - minor_step;
			int idx_high = major_idx * ttable::minor_count + minor_idx;
			int idx_low = idx_high - 1;
			double delta = double(dfValue) / (df_high - df_low);
			return icdfA[idx_high] * delta + icdfA[idx_low] * (1 - delta);
		}
	}



