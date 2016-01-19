/*
 * TimeSeries.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
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
template <size_t WindowSize, size_t SubWindowSize>
class TimeSeries {
public:
	TimeSeries() {
		time = 0;
		index = 0;
		for(size_t i = 0; i < WindowSize; ++i){
			sampleCnt[i] = 0;
			sampleSum[i] = 0;
			sampleSquares[i] = 0;
		}
	}

	void addSample(double sample, size_t sample_time){ // time in ms
		while((time + SubWindowSize) < sample_time){ // While here to skip potential unfilled subWindows
			// If the sample_time belongs to the next time SubWindow
			time += SubWindowSize; // Move to next time window and initialize it
			index = nextIndex();
			sampleCnt[index] = 0;
			sampleSum[index] = 0;
			sampleSquares[index] = 0;
		}
		sampleCnt[index] += 1;
		sampleSum[index] += sample;
		sampleSquares[index] += sample * sample;
	}

	StudentsDistribution getMean(){
		// TODO: compute the values here
		return StudentsDistribution(sampleCnt, sampleSum, sampleSquares, WindowSize);
	}


private:
	size_t time; // time in s
	size_t index;
	size_t sampleCnt[WindowSize];
	double sampleSum[WindowSize];
	double sampleSquares[WindowSize];

	size_t nextIndex(){
		return (index + 1) % WindowSize;
	}

};

#endif /* TIMESERIES_H_ */
