/*
 * TimeSeries.h
 *
 *  Created on: 17. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <cstdlib>
#include <iostream>
#include "StudentsDistribution.h"

/*
 * WindowSize - The time period to keep the history of. (in milliseconds)
 * SubWindowSize - The time period to accumulate the samples within together. (in milliseconds)
 */
template<size_t WindowSize, size_t SubWindowSize>
class TimeSeries { // TODO: check division by zero
public:
	TimeSeries() {
		time = 0;
		index = 0;
		for (size_t i = 0; i < WindowSize; ++i) {
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
		double variance = computeMeanVariance(sampleCnt, mean);

		return StudentsDistribution(sampleCnt, mean, variance);
	}

	StudentsDistribution getLra() {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double varianceB = computeLrbVariance(sampleCnt, epsilon);
		double varianceA = computeLraVariance(sampleCnt, varianceB);

		return StudentsDistribution(sampleCnt, a, varianceA);
	}

	StudentsDistribution getLrb() {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrbVariance(sampleCnt, epsilon);

		return StudentsDistribution(sampleCnt, b, variance);
	}

	StudentsDistribution getLr(double x) {
		size_t sampleCnt = computeSampleCnt();
		double a = computeLraMean(sampleCnt);
		double b = computeLrbMean(sampleCnt);
		double mean = computeLrMean(a, b, x);
		double epsilon = computeEpsilonSquaredSum(a, b, sampleCnt);
		double variance = computeLrVariance(sampleCnt, epsilon, x);

		return StudentsDistribution(sampleCnt, mean, variance);
	}

private:
	size_t time; // time in s
	size_t index;
	size_t sampleCounts[WindowSize];
	double sampleSum[WindowSize]; // sum(x_i)
	double sampleSquaresSum[WindowSize]; // sum(x_i^2)
	double timeSum[WindowSize]; // sum(t_i)
	double timeSquaresSum[WindowSize]; // sum(t_i^2)
	double sampleTimeSum[WindowSize]; // sum(x_i*t_i)

	size_t nextIndex() {
		return (index + 1) % WindowSize;
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

	double computeMeanVariance(size_t sampleCnt, double mean) {
		// v^2 = (1/N * sum(x_i^2)) - m^2
		double v = sum(sampleSquaresSum);
		v /= sampleCnt;
		v -= (mean * mean);
		return v;
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
		// v^2 = (1/(n-2) * epsilon) / (sum(x_i^2) - 2avg(x)*sum(x_i) + n*avg(x)^2
		double x = sum(sampleSum);
		double x2 = sum(sampleSquaresSum);
		double avgX = average(x, sampleCnt);

		return (epsilon / (sampleCnt - 2))
				/ (x2 - 2 * avgX * x + sampleCnt * avgX * avgX);
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
		for (size_t i = 0; i < WindowSize; ++i) {
			s += samples[i];
		}
		return s;
	}

	size_t sum(size_t *samples) {
		size_t s = 0;
		for (size_t i = 0; i < WindowSize; ++i) {
			s += samples[i];
		}
		return s;
	}

	double computeEpsilonSquaredSum(double a, double b, size_t sampleCnt) { // TODO: verify corectness
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
