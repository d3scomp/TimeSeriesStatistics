//============================================================================
// Name        : TimeSeriesStatisticsDemo.cpp
// Author      : Dominik Skoda <skoda@d3s.mff.cuni.cz>
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>
#include "TimeSeries.h"
#include "StudentsDistribution.h"
#include "TDistribution.h"
#include "TTable.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	TimeSeries<10000,1000> s;
	for(size_t i = 0; i < 20; ++i){
		s.addSample(i%5, i*300);
	}
	StudentsDistribution d = s.getMean();

	cout << d.isLessThan(2, ALPHA_0_05) << endl;

	return 0;
}
