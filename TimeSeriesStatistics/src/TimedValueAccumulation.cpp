/*
 * TimedValue.cpp
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#include "TimedValueAccumulation.h"

template <typename ValueType>
class TimedValueAccumulation {

private:
	size_t time;
	ValueType value;
	size_t samplesCnt;

public:
	TimedValueAccumulation(size_t time){
		this->time = time;
		this->value = 0;
		samplesCnt = 0;
	}

	void incrementValue(ValueType value){
		this->value += value;
		++samplesCnt;
	}

	ValueType getValue(){
		return value;
	}

	size_t getSamplesCnt(){
		return samplesCnt;
	}

};

