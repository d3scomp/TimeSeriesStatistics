//============================================================================
// Name        : TimeSeriesStatisticsDemo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include "TimeSeries.h"
#include "StudentsDistribution.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	TimeSeries<10> s;
	for(size_t i = 0; i < 20; ++i){
		s.addSample(i%5, i*300);
	}
	StudentsDistribution d = s.getMean();

	cout << d.isLessThan(2, 0.95) << endl;

	return 0;
}
