/*
 * StudentsDistribution.h
 *
 *  Created on: 17. 1. 2016
 *      Author: iridi
 */

#ifndef STUDENTSDISTRIBUTION_H_
#define STUDENTSDISTRIBUTION_H_

class StudentsDistribution {
public:
	template <typename SeriesType, size_t WindowSize>
	StudentsDistribution(size_t begining,
			TimedValueAccumulation<SeriesType> *accumulation,
			TimedValueAccumulation<SeriesType> *squares);
};

#endif /* STUDENTSDISTRIBUTION_H_ */
