/*
 * TimedValue.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#ifndef TIMEDVALUEACCUMULATION_H_
#define TIMEDVALUEACCUMULATION_H_

template <typename ValueType>
class TimedValueAccumulation {
public:
	TimedValueAccumulation(size_t time);
	void incrementValue(ValueType value);
	ValueType getValue();
	size_t getSamplesCnt();
};

#endif /* TIMEDVALUEACCUMULATION_H_ */
