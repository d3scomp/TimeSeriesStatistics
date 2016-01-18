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
#include "TimedValueAccumulation.h"

template <typename SeriesType, size_t WindowSize>
class TimeSeries {
public:
	TimeSeries() {
		timeWindow = 0;
		index = 0;
	}

	void addSample(SeriesType sample, size_t time){ // time in ms
		if(this->timeWindow < (time/1000)){
			// If the timeWindow belongs to the next timeWindow window
			this->timeWindow = time/1000;
			index = nextIndex();
			accumulation[index] = TimedValueAccumulation<SeriesType>(this->timeWindow);
			squares[index] = TimedValueAccumulation<SeriesType>(this->timeWindow);
		}
		accumulation[index].incrementValue(sample);
		squares[index].incrementValue(sample*sample);
	}

	StudentsDistribution<SeriesType, WindowSize> getDistribution(){
		size_t nIndex = nextIndex();
		std::cout << nIndex << std::endl;
		return StudentsDistribution<SeriesType, WindowSize>(nIndex, accumulation, squares);
	}


private:
	size_t timeWindow; // in s
	size_t index;
	TimedValueAccumulation<SeriesType> accumulation[WindowSize];
	TimedValueAccumulation<SeriesType> squares[WindowSize];

	size_t nextIndex(){
		return (index + 1) % WindowSize;
	}

};

#endif /* TIMESERIES_H_ */
