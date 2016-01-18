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

class StudentsDistribution {
public:
	StudentsDistribution(size_t *sampleCounts,
						 double *sampleSum,
						 double *squareSum,
						 size_t windowSize){
		sampleCnt = computeSampleCnt(sampleCounts, windowSize);
		mean = computeMean(sampleCnt, sampleSum, windowSize);
		variance = computeVariance(sampleCnt, mean, squareSum, windowSize);
		confidenceInterval = computeConfidenceInterval(sampleCnt, variance);

		tTable[0] = 0.0;
		tTable[1] = 12.70615030;
		tTable[2] = 4.30265572;
		tTable[3] = 3.18244929;
		tTable[4] = 2.77645085;
		tTable[5] = 2.57057763;
		tTable[6] = 2.44691364;
		tTable[7] = 2.36462256;
		tTable[8] = 2.30600562;
		tTable[9] = 2.26215888;
		tTable[10] = 2.22813923;
		tTable[11] = 2.20098627;
		tTable[12] = 2.17881279;
		tTable[13] = 2.16036824;
		tTable[14] = 2.14478859;
		tTable[15] = 2.13145085;
		tTable[16] = 2.11990482;
		tTable[17] = 2.10981852;
		tTable[18] = 2.10092366;
		tTable[19] = 2.09302470;
		tTable[20] = 2.08596247;
		tTable[21] = 2.07961420;
		tTable[22] = 2.07387529;
		tTable[23] = 2.06865479;
		tTable[24] = 2.06389813;
		tTable[25] = 2.05953711;
		tTable[26] = 2.05553078;
		tTable[27] = 2.05182914;
		tTable[28] = 2.04840944;
		tTable[29] = 2.04523075;
		tTable[30] = 2.04227035;
		tTable[31] = 2.03951458;
		tTable[32] = 2.03693161;
		tTable[33] = 2.03451691;
		tTable[34] = 2.03224317;
		tTable[35] = 2.03011040;
		tTable[36] = 2.02809133;
		tTable[37] = 2.02619048;
		tTable[38] = 2.02439423;
		tTable[39] = 2.02268893;
		tTable[40] = 2.02107457;
		tTable[41] = 2.01954208;
		tTable[42] = 2.01808234;
		tTable[43] = 2.01669081;
		tTable[44] = 2.01536750;
		tTable[45] = 2.01410330;
		tTable[46] = 2.01289367;
		tTable[47] = 2.01173861;
		tTable[48] = 2.01063358;
		tTable[49] = 2.00957401;
		tTable[50] = 2.00855993;
		tTable[51] = 2.00758222;
		tTable[52] = 2.00664544;
		tTable[53] = 2.00574504;
		tTable[54] = 2.00488102;
		tTable[55] = 2.00404429;
		tTable[56] = 2.00323938;
		tTable[57] = 2.00246631;
		tTable[58] = 2.00171598;
		tTable[59] = 2.00099748;
		tTable[60] = 2.00029717;
		tTable[61] = 1.99962414;
		tTable[62] = 1.99896931;
		tTable[63] = 1.99834175;
		tTable[64] = 1.99772785;
		tTable[65] = 1.99713667;
		tTable[66] = 1.99656369;
		tTable[67] = 1.99600890;
		tTable[68] = 1.99546775;
		tTable[69] = 1.99494479;
		tTable[70] = 1.99443547;
		tTable[71] = 1.99394435;
		tTable[72] = 1.99346232;
		tTable[73] = 1.99299847;
		tTable[74] = 1.99254373;
		tTable[75] = 1.99210262;
		tTable[76] = 1.99167516;
		tTable[77] = 1.99125679;
		tTable[78] = 1.99084752;
		tTable[79] = 1.99045189;
		tTable[80] = 1.99006535;
		tTable[81] = 1.98968791;
		tTable[82] = 1.98931957;
		tTable[83] = 1.98896032;
		tTable[84] = 1.98861016;
		tTable[85] = 1.98826910;
		tTable[86] = 1.98793259;
		tTable[87] = 1.98760972;
		tTable[88] = 1.98729139;
		tTable[89] = 1.98697762;
		tTable[90] = 1.98667294;
		tTable[91] = 1.98637735;
		tTable[92] = 1.98608631;
		tTable[93] = 1.98579982;
		tTable[94] = 1.98552243;
		tTable[95] = 1.98524958;
		tTable[96] = 1.98498582;
		tTable[97] = 1.98472207;
		tTable[98] = 1.98446741;
		tTable[99] = 1.98421730;
		tTable[100] = 1.98397174;
	}

	bool isEqual(double sample){
		/*double combinedVariance = (variance/sampleCnt); // given sample variance is 0
			double combinedVRoot = sqrt(combinedVariance);
			double t = abs(mean - sample)/combinedVRoot;
			double tabulatedT = tTable[sampleCnt-1];
			return t <= tabulatedT;*/

			return sample >= mean - confidenceInterval
					&& sample <= mean + confidenceInterval;
	}

private:
	size_t sampleCnt;
	double mean;
	double variance;
	double confidenceInterval;
	double tTable[MAX_SAMPLES+1];

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

	double computeConfidenceInterval(size_t sampleCnt, double variance){
		double tScore = tTable[sampleCnt-1];
		double ci = tScore * sqrt(variance / sampleCnt);
		return ci;
	}
};

#endif /* STUDENTSDISTRIBUTION_H_ */

