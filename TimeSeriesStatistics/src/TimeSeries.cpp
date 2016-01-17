/*
 * TimeSeries.cpp
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#include "TimeSeries.h"
#include "TimedValueAccumulation.h"
#include "StudentsDistribution.h"

template <typename SeriesType, size_t WindowSize>
class TimeSeries{

private:
	size_t timeWindow; // in s
	size_t index;
	TimedValueAccumulation<SeriesType> accumulation[WindowSize];
	TimedValueAccumulation<SeriesType> squares[WindowSize];

	size_t nextIndex(){
		return (index + 1) % WindowSize;
	}

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

	StudentsDistribution getDistribution(){
		return StudentsDistribution<1, SeriesType, WindowSize>(nextIndex, accumulation, squares);
	}

};
