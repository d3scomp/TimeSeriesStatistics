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

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	TimeSeries<10000,1000> s;
	for(size_t i = 0; i < 20; ++i){
		s.addSample(i%5, i*300);
	}
	StudentsDistribution d = s.getMean();

	cout << d.isLessThan(2, 0.95) << endl;

	TDistribution td;
	int err;
	for(size_t i = 0; i < 100; ++i){
		double t = td.tnc(0.975, i, 0, &err);
		if(err){
			cout << "err " << err << endl;
		}
		cout << t << endl;
	}

	return 0;
}
