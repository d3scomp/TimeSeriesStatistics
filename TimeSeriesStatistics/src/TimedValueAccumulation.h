/*
 * TimedValue.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#ifndef TIMEDVALUEACCUMULATION_H_
#define TIMEDVALUEACCUMULATION_H_

#include <cstdlib>

template <typename ValueType>
class TimedValueAccumulation {
public:
	TimedValueAccumulation(size_t time){
			this->time = time;
			this->value = 0;
			samplesCnt = 0;
	}
	TimedValueAccumulation(){
			this->time = 0;
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


private:
	size_t time;
	ValueType value;
	size_t samplesCnt;

};

#endif /* TIMEDVALUEACCUMULATION_H_ */
