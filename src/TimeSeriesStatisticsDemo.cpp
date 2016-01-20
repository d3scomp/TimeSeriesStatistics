/*******************************************************************************
 * Copyright 2016 Charles University in Prague
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

//============================================================================
// Name        : TimeSeriesStatisticsDemo.cpp
// Author      : Dominik Skoda <skoda@d3s.mff.cuni.cz>
// Version     :
// Copyright   : Your copyright notice
// Description : Demonstrate the usage of TimeSeries and StudentsDistribution.
//============================================================================

#include <iostream>
#include <cstdlib>
#include "TimeSeries.h"
#include "StudentsDistribution.h"
#include "TTable.h"

using namespace std;

int main() {
	size_t sampleCnt = 20;

	TimeSeries<10, 1000> s;
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
			<< lrb.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal lrb "
			<< lrb.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than lrb "
			<< lrb.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal lrb "
			<< lrb.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	StudentsDistribution lr = s.getLr(2);

	cout << sampleCnt << " samples less than lr "
			<< lr.isLessThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples less than or equal lr "
			<< lr.isLessThanOrEqual(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than lr "
			<< lr.isGreaterThan(2, ALPHA_0_05) << endl;
	cout << sampleCnt << " samples greater than or equal lr "
			<< lr.isGreaterThanOrEqual(2, ALPHA_0_05) << endl;

	TimeSeries<3, 2000> sj;
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
