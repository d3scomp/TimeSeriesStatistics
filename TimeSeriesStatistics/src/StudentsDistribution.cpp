/*
 * StudentsDistribution.cpp
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 */

#include "StudentsDistribution.h"
#include <math.h>

/*
 * df - degrees of freedom
 */
StudentsDistribution::StudentsDistribution(size_t df, double mean, double variance)
		: df(df), mean(mean), variance(variance) {

	}

	bool StudentsDistribution::isLessThan(double sample, ALPHAS a){
		double dist = (mean - sample) / sqrt(variance);
		double icdfValue = getICDF(a);

		return -icdfValue >= dist;
	}

	bool StudentsDistribution::isLessThanOrEqual(double sample, ALPHAS a){
		double dist = (mean - sample) / sqrt(variance);
		double icdfValue = getICDF(a);

		return -icdfValue > dist;
	}

	bool StudentsDistribution::isGreaterThan(double sample, ALPHAS a){
		double dist = (mean - sample) / sqrt(variance);
		double icdfValue = getICDF(a);

		return -icdfValue <= dist;
	}

	bool StudentsDistribution::isGreaterThanOrEqual(double sample, ALPHAS a){
		double dist = (mean - sample) / sqrt(variance);
		double icdfValue = getICDF(a);

		return -icdfValue < dist;
	}

	double StudentsDistribution::getICDF(ALPHAS a){
		const double *icdfA = icdf[a];
		int base = 0;
		int major_idx = -1;
		int base_incr = 0;
		int minor_step = 1;
		while(base + base_incr < df){
			base += base_incr;
			major_idx += 1;
			minor_step = (1 << major_idx * boost);
			base_incr = minor_step * minor_count;
		}
		df -= base;
		if(df % minor_step == 0){
			int minor_idx = df / minor_step - 1;
			int idx = major_idx * minor_count + minor_idx;
			std::cout << "base: " << base << " major_idx: " << major_idx
					<< " minor_idx: " << minor_idx << " idx: " << idx << std::endl;
			return icdfA[idx];
		} else {
			size_t minor_idx = df / minor_step;
			int df_high = base + (minor_idx + 1) * minor_step;
			int df_low = df_high - minor_step;
			int idx_high = major_idx * minor_count + minor_idx;
			int idx_low = idx_high - 1;
			double delta = float(df) / (df_high - df_low);
			std::cout << "base: " << base << " major_idx: " << major_idx
					<< " minor_idx: " << minor_idx << " df_high: " << df_high
					<< "df_low: " << df_low << " idx_high: " << idx_high
					<< "idx_low: " << idx_low << " delta: " << delta << std::endl;
			return icdfA[idx_high] * delta + icdfA[idx_low] * (1 - delta);
		}
	}

/*static const double tTable_data[] = // For 0.95 level significance
{0.0, 12.70615030, 4.30265572, 3.18244929, 2.77645085, 2.57057763,
       2.44691364, 2.36462256, 2.30600562, 2.26215888, 2.22813923,
       2.20098627, 2.17881279, 2.16036824, 2.14478859, 2.13145085,
       2.11990482, 2.10981852, 2.10092366, 2.09302470, 2.08596247,
       2.07961420, 2.07387529, 2.06865479, 2.06389813, 2.05953711,
       2.05553078, 2.05182914, 2.04840944, 2.04523075, 2.04227035,
       2.03951458, 2.03693161, 2.03451691, 2.03224317, 2.03011040,
       2.02809133, 2.02619048, 2.02439423, 2.02268893, 2.02107457,
       2.01954208, 2.01808234, 2.01669081, 2.01536750, 2.01410330,
       2.01289367, 2.01173861, 2.01063358, 2.00957401, 2.00855993,
       2.00758222, 2.00664544, 2.00574504, 2.00488102, 2.00404429,
       2.00323938, 2.00246631, 2.00171598, 2.00099748, 2.00029717,
       1.99962414, 1.99896931, 1.99834175, 1.99772785, 1.99713667,
       1.99656369, 1.99600890, 1.99546775, 1.99494479, 1.99443547,
       1.99394435, 1.99346232, 1.99299847, 1.99254373, 1.99210262,
       1.99167516, 1.99125679, 1.99084752, 1.99045189, 1.99006535,
       1.98968791, 1.98931957, 1.98896032, 1.98861016, 1.98826910,
       1.98793259, 1.98760972, 1.98729139, 1.98697762, 1.98667294,
       1.98637735, 1.98608631, 1.98579982, 1.98552243, 1.98524958,
       1.98498582, 1.98472207, 1.98446741, 1.98421730, 1.98397174};

const double *StudentsDistribution::tTable = tTable_data;*/



