//============================================================================
// Name        : TimeSeriesStatisticsDemo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include "../../TimeSeriesStatistics/src/TimedValueAccumulation.h"
#include "../../TimeSeriesStatistics/src/TimeSeries.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	TimeSeries<double, 10> s;
	cout << "a" << endl;
	for(size_t i = 0; i < 20; ++i){
		cout << "b" << endl;
		s.addSample(i, i*100);
	}
	cout << "c" << endl;
	cout << s.getDistribution().getAverage() << endl;

	return 0;
}
