/*
 * StudentsDistribution.cpp
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#include "StudentsDistribution.h"

template <size_t freedomDegrees, typename SeriesType, size_t WindowSize>
class StudentsDistribution {
private:
	size_t beginning;
	TimedValueAccumulation<SeriesType> *accumulation;
	TimedValueAccumulation<SeriesType> *squares;

	void nextIndex(size_t &index){
		index = (index + 1) % WindowSize;
	}

public:
	StudentsDistribution(size_t begining,
			TimedValueAccumulation<SeriesType> *accumulation,
			TimedValueAccumulation<SeriesType> *squares) {
		this->beginning = beginning;
		this->squares = squares;
		this->accumulation = accumulation;
	}

	SeriesType getAverage(){
		size_t index = beginning;
		size_t samplesCnt = 0;
		SeriesType sum = 0;

		do{
			sum += accumulation[index].getValue();
			samplesCnt += accumulation[index].getSamplesCnt();
			nextIndex(index);
		} while(index != beginning);

		return sum/samplesCnt;
	}

	/*double getVariance(){
		size_t index = beginning;

	}


	double getTValue(){

	}*/

};

