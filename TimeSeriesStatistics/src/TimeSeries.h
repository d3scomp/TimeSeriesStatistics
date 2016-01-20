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

#include <cstdlib>
#include <iostream>
#include "StudentsDistribution.h"

/*
 * WindowSize - The number of subWindows keep the history of
 * SubWindowSize - The time period to accumulate the samples within together. (in milliseconds)
 */
template<size_t WindowCnt, size_t SubWindowSize>
class TimeSeries { // TODO: check division by zero
public:
	TimeSeries() {
		time = 0;
		index = 0;
		for (size_t i = 0; i < WindowCnt; ++i) {
			sampleCounts[i] = 0;
			sampleSum[i] = 0;
			sampleSquaresSum[i] = 0;
		}
	}

	void addSample(double sample, size_t sampleTime) { // time in ms
		while ((time + SubWindowSize) < sampleTime) { // While here to skip potential unfilled subWindows
			// If the sample_time belongs to the next time SubWindow
			time += SubWindowSize; // Move to next time window and initialize it
			index = nextIndex();
			sampleCounts[index] = 0;
			sampleSum[index] = 0;
			sampleSquaresSum[index] = 0;
			timeSum[index] = 0;
			timeSquaresSum[index] = 0;
			sampleTimeSum[index] = 0;
		}
		sampleCounts[index] += 1;
		sampleSum[index] += sample;
		sampleSquaresSum[index] += sample * sample;
		timeSum[index] += sampleTime;
		timeSquaresSum[index] = sampleTime * sampleTime;
		sampleTimeSum[index] += sample * sampleTime;
	}

	StudentsDistribution getMean() {
		size_t sampleCnt = computeSampleCnt();
		double mean = computeMean(sampleCnt);
		double variance = computeMeanVariance(sampleCnt);

		return StudentsDistribution(sampleCnt-1, mean, variance);
	}

	StudentsDistribution getLra() {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double varianceB = computeLrbVariance(sampleCnt, epsilon);
		double varianceA = computeLraVariance(sampleCnt, varianceB);

		return StudentsDistribution(sampleCnt-2, a, varianceA);
	}

	StudentsDistribution getLrb() {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrbVariance(sampleCnt, epsilon);

		return StudentsDistribution(sampleCnt-2, b, variance);
	}

	StudentsDistribution getLr(double x) {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double mean = computeLrMean(a, b, x);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrVariance(sampleCnt, epsilon, x);

		return StudentsDistribution(sampleCnt-2, mean, variance);
	}

private:
	size_t time; // time in s
	size_t index;
	size_t sampleCounts[WindowCnt];
	double sampleSum[WindowCnt]; // sum(x_i)
	double sampleSquaresSum[WindowCnt]; // sum(x_i^2)
	double timeSum[WindowCnt]; // sum(t_i)
	double timeSquaresSum[WindowCnt]; // sum(t_i^2)
	double sampleTimeSum[WindowCnt]; // sum(x_i*t_i)

	size_t nextIndex() {
		return (index + 1) % WindowCnt;
	}

	size_t computeSampleCnt() {
		return sum(sampleCounts);
	}

	double computeMean(size_t sampleCnt) {
		// m = 1/N * sum(x_i)
		double m = sum(sampleSum);
		m /= sampleCnt;
		return m;
	}

	double computeMeanVariance(size_t sampleCnt) {
		// Corrected sample variance
		// v^2 = ( 1/(N-1) * sum(x_i - avg(x))^2 ) / N
		// v^2 = ( 1/(N-1) * (sum(x_i^2) - 2avg(x)*sum(x_i) + n*avg(x)^2) ) / N
		double x2 = sum(sampleSquaresSum);
		double x = sum(sampleSum);
		double avgX = average(x, sampleCnt);

		return ((x2 - 2*avgX*x + sampleCnt*avgX*avgX) / (sampleCnt - 1)) / sampleCnt;
	}

	double computeLraMean(size_t sampleCnt) {
		// a = avg(t) - b*avg(x)
		double avgT = average(sum(timeSum), sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double avgX = average(sum(sampleSum), sampleCnt);

		return avgT - b * avgX;
	}

	/*
	 * varianceB = v_b^2
	 */
	double computeLraVariance(size_t sampleCnt, double varianceB) {
		// v^2 = v_b^2 * 1/n * sum(x_i^2)
		double x2 = sum(sampleSquaresSum);

		return varianceB * x2 / sampleCnt;
	}

	double computeLrbMean(size_t sampleCnt) {
		// b = (avg(xt) - avg(x)avg(t)) / (avg(x^2) - avg(x)^2)
		double avgXT = average(sum(sampleTimeSum), sampleCnt);
		double avgXavgT = average(sum(sampleSum), sampleCnt)
				* average(sum(timeSum), sampleCnt);
		double avgX2 = average(sum(sampleSquaresSum), sampleCnt);
		double avgX = average(sum(sampleSum), sampleCnt);

		return (avgXT - avgXavgT) / (avgX2 - avgX * avgX);
	}

	/*
	 * epsilon = sum(epsilon_i^2)
	 */
	double computeLrbVariance(size_t sampleCnt, double epsilon) {
		// v^2 = (1/(n-2) * epsilon) / sum((x_i - avg(x))^2)
		// v^2 = (1/(n-2) * epsilon) / (sum(x_i^2) - 2avg(x)*sum(x_i) + n*avg(x)^2)
		// v^2 = (1/(n-2) * epsilon) / (sum(x_i^2) - avg(x)*sum(x_i))
		double x = sum(sampleSum);
		double x2 = sum(sampleSquaresSum);
		double avgX = average(x, sampleCnt);

		return (epsilon / (sampleCnt - 2)) / (x2 - avgX * x);
	}

	double computeLrMean(double a, double b, double x) {
		// y = a + b*x
		return a + b * x;
	}

	/*
	 * point is the point of interest
	 */
	double computeLrVariance(size_t sampleCnt, double epsilon, double point) {
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / sum(x_i - avg(x))^2)
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / (sum(x_i^2) - 2*avg(x)*sum(x_i) + n*avg(x^2)))
		double x = sum(sampleSum);
		double x2 = sum(sampleSquaresSum);
		double avgX = average(x, sampleCnt);
		double avgX2 = average(x2, sampleCnt);
		double pAvgX = (point - avgX);

		return epsilon / (sampleCnt - 2)
				* (1 / (double) sampleCnt
						+ pAvgX * pAvgX
								/ (x2 - 2 * avgX * x + sampleCnt * avgX2));
	}

	double average(double sum, size_t cnt) {
		return sum / cnt;
	}

	double sum(double *samples) {
		double s = 0;
		for (size_t i = 0; i < WindowCnt; ++i) {
			s += samples[i];
		}
		return s;
	}

	size_t sum(size_t *samples) {
		size_t s = 0;
		for (size_t i = 0; i < WindowCnt; ++i) {
			s += samples[i];
		}
		return s;
	}

	double computeEpsilonSquaredSum(double a, double b, size_t sampleCnt) {
		// sum(epsilon_i^2) = sum((t_i - a - bx_i)^2)
		// sum(epsilon_i^2) = sum(t_i^2) + n*a^2 + b^2*sum(x_i^2)
		//						+ 2ab*sum(x_i) - 2a*sum(t_i) - 2b*sum(x_i*t_i)

		double t2 = sum(timeSquaresSum);
		double x2 = sum(sampleSquaresSum);
		double x = sum(sampleSum);
		double t = sum(timeSum);
		double xt = sum(sampleTimeSum);

		return t2 + sampleCnt * a * a + b * b * x2 + 2 * a * b * x - 2 * a * t
				- 2 * b * xt;
	}

};

#endif /* TIMESERIES_H_ */
