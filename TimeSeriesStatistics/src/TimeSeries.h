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

/*
 * WindowSize - The number of subWindows keep the history of
 * SubWindowSize - The time period to accumulate the samples within together. (in milliseconds)
 */
template<size_t WindowCnt, size_t SubWindowSize>
class TimeSeries {
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

	double computeMean(size_t n) {
		// m = 1/N * sum(x_i)
		double m = sum(sampleSum);
		return n != 0
				? m / n
				: 0;
	}

	double computeMeanVariance(size_t n) {
		// Corrected sample variance
		// v^2 = ( 1/(N-1) * sum(x_i - avg(x))^2 ) / N
		// v^2 = ( 1/(N-1) * (sum(x_i^2) - 2avg(x)*sum(x_i) + n*avg(x)^2) ) / N
		// v^2 = ( 1/(N-1) * (sum(x_i^2) - 2avg(x)*sum(x_i) + x*avg(x)) ) / N

		// Check that division by zero won't occur
		if(n < 2){
			return 0;
		}

		double x2 = sum(sampleSquaresSum);
		double x = sum(sampleSum);
		double avgX = x / n;

		return ((x2 - 2*avgX*x + x*avgX) / (n - 1)) / n;
	}

	double computeLraMean(size_t n) {
		// a = avg(t) - b*avg(x)

		// Check that division by zero won't occur
		if(n == 0){
			return 0;
		}

		double avgT = sum(timeSum) / n;
		double b = computeLrbMean(n);
		double avgX = sum(sampleSum) / n;

		return avgT - b * avgX;
	}

	/*
	 * varianceB = v_b^2
	 */
	double computeLraVariance(size_t n, double varianceB) {
		// v^2 = v_b^2 * 1/n * sum(x_i^2)
		double x2 = sum(sampleSquaresSum);

		return n != 0
				? varianceB * x2 / n
				: 0;
	}

	double computeLrbMean(size_t n) {
		// b = (avg(xt) - avg(x)avg(t)) / (avg(x^2) - avg(x)^2)

		// Check that division by zero won't occur
		if(n == 0){
			return 0;
		}

		double avgXT = sum(sampleTimeSum) / n;
		double avgXavgT = (sum(sampleSum) / n)
				* (sum(timeSum) / n);
		double avgX2 = sum(sampleSquaresSum) / n;
		double avgX = sum(sampleSum) / n;
		double div = avgX2 - avgX * avgX;

		return div != 0
				? (avgXT - avgXavgT) / div
				: 0;
	}

	/*
	 * epsilon = sum(epsilon_i^2)
	 */
	double computeLrbVariance(size_t n, double epsilon) {
		// v^2 = (1/(n-2) * epsilon) / sum((x_i - avg(x))^2)
		// v^2 = (1/(n-2) * epsilon) / (sum(x_i^2) - 2avg(x)*sum(x_i) + n*avg(x)^2)
		// v^2 = (1/(n-2) * epsilon) / (sum(x_i^2) - avg(x)*sum(x_i))

		// Check that division by zero won't occur
		if(n == 0){
			return 0;
		}

		double x = sum(sampleSum);
		double x2 = sum(sampleSquaresSum);
		double avgX = x / n;
		double div = x2 - avgX * x;

		return div != 0 && n != 2
				? (epsilon / (n - 2)) / div
				: 0;
	}

	double computeLrMean(double a, double b, double x) {
		// y = a + b*x
		return a + b * x;
	}

	/*
	 * point is the point of interest
	 */
	double computeLrVariance(size_t n, double epsilon, double point) {
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / sum(x_i - avg(x))^2)
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / (sum(x_i^2) - 2*avg(x)*sum(x_i) + n*avg(x^2)))
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / (sum(x_i^2) - 2*avg(x)*sum(x_i) + x^2))
		// v^2 = 1/(n-2) * epsilon * (1/n + (point - avg(x))^2 / (2*sum(x_i^2) - 2*avg(x)*sum(x_i)))

		// Check that division by zero won't occur
		if(n == 0){
			return 0;
		}

		double x = sum(sampleSum);
		double x2 = sum(sampleSquaresSum);
		double avgX = x / n;
		double pAvgX = (point - avgX);
		double div = 2*x2 - 2 * avgX * x;

		return div != 0 && n != 2
				? epsilon / (n - 2) * (1 / (double) n + pAvgX * pAvgX / div)
				: 0;
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

	double computeEpsilonSquaredSum(double a, double b, size_t n) {
		// sum(epsilon_i^2) = sum((t_i - a - bx_i)^2)
		// sum(epsilon_i^2) = sum(t_i^2) + n*a^2 + b^2*sum(x_i^2)
		//						+ 2ab*sum(x_i) - 2a*sum(t_i) - 2b*sum(x_i*t_i)

		double t2 = sum(timeSquaresSum);
		double x2 = sum(sampleSquaresSum);
		double x = sum(sampleSum);
		double t = sum(timeSum);
		double xt = sum(sampleTimeSum);

		return t2 + n * a * a + b * b * x2 + 2 * a * b * x - 2 * a * t
				- 2 * b * xt;
	}

};

#endif /* TIMESERIES_H_ */
