/*
 * StudentsDistribution.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#ifndef STUDENTSDISTRIBUTION_H_
#define STUDENTSDISTRIBUTION_H_

#include <cstdlib>
#include <iostream>
#include "TimedValueAccumulation.h"

template <typename SeriesType, size_t WindowSize>
class StudentsDistribution {
public:
	StudentsDistribution(size_t beginning,
			TimedValueAccumulation<SeriesType> *accumulation,
			TimedValueAccumulation<SeriesType> *squares)
				: beginning(beginning) {
		this->squares = squares;
		this->accumulation = accumulation;
	}

	SeriesType getAverage(){
		size_t index = beginning;
		size_t samplesCnt = 0;
		SeriesType sum = 0;
		std::cout << "d" << std::endl;
		do{
			std::cout << "e" << std::endl;
			std::cout << index << std::endl;
			sum += accumulation[index].getValue();
			samplesCnt += accumulation[index].getSamplesCnt();
			nextIndex(index);
		} while(index != beginning);

		std::cout << "f" << std::endl;
		return sum/samplesCnt;
	}

	/*double getVariance(){
		size_t index = beginning;

	}


	double getTValue(){

	}*/

private:
	size_t beginning;
	TimedValueAccumulation<SeriesType> *accumulation;
	TimedValueAccumulation<SeriesType> *squares;

	void nextIndex(size_t &index){
		index = (index + 1) % WindowSize;
	}
};

#endif /* STUDENTSDISTRIBUTION_H_ */

