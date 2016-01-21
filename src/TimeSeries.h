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
 * TimeSeries.h
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 *
 *  Holds samples of a time series with a predefined history length.
 *  Provides probability distribution over the mean and over
 *  the linear regression of the data.
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "StudentsDistribution.h"
#include <math.h>

/*
 * WindowCnt - The number of windows to keep the history of
 * WindowSize - The time period to accumulate the samples within together. (in milliseconds)
 */
template<int WindowCnt, int WindowSize>
class TimeSeries {
public:
	TimeSeries() {
		time = 0;
		index = 0;
		for (int i = 0; i < WindowCnt; ++i) {
			sampleCounts[i] = 0;
			sampleSum[i] = 0;
			sampleSquaresSum[i] = 0;
		}
	}

	void addSample(double sample, int sampleTime) { // time in ms
		while ((time + WindowSize) < sampleTime) { // While here to skip potential unfilled subWindows
			// If the sample_time belongs to the next time SubWindow
			time += WindowSize; // Move to next time window and initialize it

			index = nextIndex();

			totalSampleCounts -= sampleCounts[index];	
			sampleCounts[index] = 0;

			totalSampleSum -= sampleSum[index];
			sampleSum[index] = 0;

			totalSampleSquaresSum -= sampleSquaresSum[index];
			sampleSquaresSum[index] = 0;

			totalTimeSum -= timeSum[index];
			timeSum[index] = 0;

			totalTimeSquaresSum -= timeSquaresSum[index];
			timeSquaresSum[index] = 0;

			totalSampleTimeSum -= sampleTimeSum[index];
			sampleTimeSum[index] = 0;
						
		}

		totalSampleCounts += 1;
		sampleCounts[index] += 1;

		totalSampleSum += sample;
		sampleSum[index] += sample;

		double sample2 = sample * sample;
		totalSampleSquaresSum += sample2;
		sampleSquaresSum[index] += sample2;

		double sampleTimeD = sampleTime;

		totalTimeSum += sampleTimeD;
		timeSum[index] += sampleTimeD;
		
		double sampleTime2 = sampleTimeD * sampleTimeD;
		totalTimeSquaresSum += sampleTime2;
		timeSquaresSum[index] += sampleTime2;

		double sampleSampleTime = sample * sampleTime;
		totalSampleTimeSum += sampleSampleTime;
		sampleTimeSum[index] += sampleSampleTime;
	}

	StudentsDistribution getMean() {
		int sampleCnt = computeSampleCnt();
		double mean = computeMean(sampleCnt);
		double variance = computeMeanVariance(sampleCnt);

		return StudentsDistribution(sampleCnt-1, mean, variance);
	}

	StudentsDistribution getLra() {
		int sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double e2 = computeEpsilonSquaredSum(a, b, sampleCnt);
		double varianceB = computeLrbVariance(sampleCnt, e2);
		double varianceA = computeLraVariance(sampleCnt, varianceB);

		return StudentsDistribution(sampleCnt-2, a, varianceA);
	}

	StudentsDistribution getLrb() {
		int sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double e2 = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrbVariance(sampleCnt, e2);

		return StudentsDistribution(sampleCnt-2, b, variance);
	}

	StudentsDistribution getLr(double x) {
		int sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double mean = computeLrMean(a, b, x);
		double e2 = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrVariance(sampleCnt, e2, x);

		return StudentsDistribution(sampleCnt-2, mean, variance);
	}

private:
	int time; // time in ms
	int index;
	int sampleCounts[WindowCnt];
	double sampleSum[WindowCnt]; // sum(x_i)
	double sampleSquaresSum[WindowCnt]; // sum(x_i^2)
	double timeSum[WindowCnt]; // sum(t_i)
	double timeSquaresSum[WindowCnt]; // sum(t_i^2)
	double sampleTimeSum[WindowCnt]; // sum(x_i*t_i)

	int totalSampleCounts;
	double totalSampleSum;
	double totalSampleSquaresSum;
	double totalTimeSum;
	double totalTimeSquaresSum;
	double totalSampleTimeSum;


	inline int nextIndex() {
		return (index + 1) % WindowCnt;
	}

	inline int computeSampleCnt() {
		return totalSampleCounts;
	}

	double computeMean(int n) {
		if (n <= 0) {
			return NAN;
		}

		double m = totalSampleSum;
		return m / n;
	}

	double computeMeanVariance(int n) {
		if (n <= 1) {
			return NAN;
		}

		double x2 = totalSampleSquaresSum;
		double x = totalSampleSum;

		return (x2 - x*x/n) / ((n - 1) * n);
	}

	double computeLraMean(int n) {
		if (n <= 0) {
			return NAN;
		}

		double y = totalSampleSum;
		double x = totalTimeSum;
		double b = computeLrbMean(n);

		return (y - b * x) / n;
	}

	double computeLraVariance(int n, double varianceB) {
		if (n <= 0) {
			return NAN;
		}

		double x2 = totalTimeSquaresSum;

		return varianceB * x2 / n;
	}

	double computeLrbMean(int n) {
		if (n <= 0) {
			return NAN;
		}

		double x = totalTimeSum;
		double x2 = totalTimeSquaresSum;
		double y = totalSampleSum;
		double xy = totalSampleTimeSum;

		double nom = xy - x*y / n;
		double denom = x2 - x*x / n;

		return denom != 0 ? nom / denom : NAN;
	}

	/*
	 * e2 = computeEpsilonSquaredSum(...)
	 */
	double computeLrbVariance(int n, double e2) {
		if (n <= 0) {
			return NAN;
		}

		double x = totalTimeSum;
		double x2 = totalTimeSquaresSum;

		double nom = e2 / (n - 2);
		double denom = x2 - x*x/n;
		return denom != 0 ? nom / denom : NAN;
	}

	double computeLrMean(double a, double b, double x) {
		return a + b * x;
	}

	/*
	 * point is the point of interest
	 */
	double computeLrVariance(int n, double e2, double point) {
		// Check that division by zero won't occur
		if (n <= 2) {
			return NAN;
		}

		double x = totalTimeSum;
		double x2 = totalTimeSquaresSum;
		double pAvgX = point - x/n;
		double denom = x2 - x*x/n;

		return denom != 0 ? (e2 / (n-2)) * (1.0 / n + pAvgX * pAvgX / denom) : NAN;
	}

	double computeSum(double *samples) {
		double s = 0;
		for (int i = 0; i < WindowCnt; ++i) {
			s += samples[i];
		}
		return s;
	}

	int computeSum(int *samples) {
		int s = 0;
		for (int i = 0; i < WindowCnt; ++i) {
			s += samples[i];
		}
		return s;
	}

	double computeEpsilonSquaredSum(double a, double b, int n) {
		if (n <= 2) {
			return NAN;
		}

		double x = totalTimeSum;
		double x2 = totalTimeSquaresSum;
		double y = totalSampleSum;
		double y2 = totalSampleSquaresSum;
		double xy = totalSampleTimeSum;

		return y2 + n*a*a + b*b*x2 - 2*a*y - 2*b*xy + 2*a*b*x;
	}

};

#endif /* TIMESERIES_H_ */
