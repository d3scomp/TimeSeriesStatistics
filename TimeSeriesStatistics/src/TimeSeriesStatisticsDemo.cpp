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
#include "TTable.h"

using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	size_t sampleCnt = 20;

	TimeSeries<10000, 1000> s;
	for (size_t i = 0; i < sampleCnt; ++i) {
		s.addSample(i % 5, i * 300);
	}
	StudentsDistribution m = s.getMean();

	cout << sampleCnt << " samples less than mean "
			<< m.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal mean "
			<< m.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than mean "
			<< m.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal mean "
			<< m.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	StudentsDistribution lra = s.getLra();

	cout << sampleCnt << " samples less than lra "
			<< lra.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal lra "
			<< lra.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than lra "
			<< lra.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal lra "
			<< lra.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	StudentsDistribution lrb = s.getLrb();

	cout << sampleCnt << " samples less than lrb "
			<< lra.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal lrb "
			<< lra.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than lrb "
			<< lra.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal lrb "
			<< lra.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	StudentsDistribution lr = s.getLr(2);

	cout << sampleCnt << " samples less than lr "
			<< lra.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal lr "
			<< lra.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than lr "
			<< lra.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal lr "
			<< lra.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	TimeSeries<6000, 2000> sj;
	for (size_t j = 1; j <= 5; ++j) {
		sj.addSample(j, j * 300);
		StudentsDistribution m = sj.getMean();

		cout << j << " samples less than mean "
				<< m.isLessThan(2, ALPHA_0_05) << endl;
		cout << j << " samples less than or equal mean "
				<< m.isLessThanOrEqual(2, ALPHA_0_05) << endl;
		cout << j << " samples greater than mean "
				<< m.isGreaterThan(2, ALPHA_0_05) << endl;
		cout << j << " samples greater than or equal mean "
				<< m.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

		StudentsDistribution lrj = sj.getLr(2);

		cout << j << " samples less than lr "
				<< lrj.isLessThan(2, ALPHA_0_05) << endl;
		cout << j << " samples less than or equal lr "
				<< lrj.isLessThanOrEqual(2, ALPHA_0_05) << endl;
		cout << j << " samples greater than lr "
				<< lrj.isGreaterThan(2, ALPHA_0_05) << endl;
		cout << j << " samples greater than or equal lr "
				<< lrj.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;
	}

	return 0;
}
