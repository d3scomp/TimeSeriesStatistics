# Time Series Statistics

This project contains C++ classes that allow keeping a history of a time series with defined length
and subsequential statistical tests over this series.

From the time series it can be taken a Student's distribution of its mean, and of its linear regression elements
such as Lra, Lrb and Lr(x). Over these distributions it is then possible to test hypotheses.

To save computation resources at runtime this implementation uses precomputed values of t-table
for different number of degrees of freedom and different confidence levels. In order to keep the flexibility
of the testing there is a script that generates the t-tables for predefiend confidence levels and
allows customization of the source code.

## Usage

Before using the classes provided in this project it is desirable to generate the set of t-tables
that suits the needs of the target project. For this purpose there is the gen_ttable.py script.
This script generates the TTable.h and TTable.cpp source files. If the script is invoked without
arguments it generates t-table for confidene level 0.025 and 0.05. Otherwise the script expects
as its arguments the list of confidence levels for which it generates the tables. Together with
these tables there is corresponding enum ALPHAS, which contains the enumeration of confidence
levels that can be then used in the source code.

To save space but keep the number of t-table population high there is used approximation of values
with high degree of freedom. The variables minorCount and boost serves for tuning this property.
minorCount denotes range with t-table entries with equal distance and boost denotes the increasing
gap between these entries after each minorCount entries. For minorCount = 100 and boost = 3 that
means that first 100 entries will have distance (distance in degrees of freedom) 1, second 100 entries
will have distance 8 (2^boost), third 100 entries will have the distance 64 and so on. The missing
entries in the table are approximated.

After the TTable.h and TTable.cpp files are generated the classes can be used in an application.
The TimeSeries class serves for storing the time series with defined history length. The history
length is specified as template parameters: WindowCnt is the count of history windows; WindowSize
is the time length of a window (in milliseconds). The WindowsCnt * WindowSize = time length of
the time series history.

Using the TimeSeries.addSample method the time series history can be populated. And subsequential
queries can be than asked. There can be obtained the mean, and parameters of linear regression
from the time series. These quantities are returned as Student's distribution over them.

Finally with the Student's distribution it is possible to test hypotheses for some threshold
and it's inequality with the queried quantity on some confidence level.

The project contains a demo application that shows how this project can be used.

## Tutorial

In this section there is step by step tutorial on how to use this project.
The easiest way to use this project is through Eclipse IDE with installed CDT and PyDev.

1. If you want to use other confidence levels than (alpha = 0.005, 0.00625, 0.001, 0.0125, 0.025, 0.05), generate TTable.h and TTable.cpp
  * invoke gen_ttable.py located in scripts folder (using Python 2.7) with the confidence levels as arguments
2. Include the files located in src directory to your project
3. Declare an instance of TimeSeries for each time series window you want to test
  * TimeSeries<10, 1000> ts;
    * Where the first parameter is the number of time windows and the second parameter is a window duration
    * The instance above keeps time series with a history of 10 seconds (10 * 1000 ms)
    * The instance of TimeSeries easily takes a couple of kilobytes of memory and may cause stack overflow. It is better to allocate it statically on heap rather (i.e. make it static or declare it outside a method).
4. Whenever you get a value from sensor, add it to the respective timeseries along with a timestamp (in ms)
  * ts.addSample(sample, time);
5. Test the predicted real value of the time series assuming a linear trend
  * StudentsDistribution lra = ts.getLra();
  * bool answer = lra.isLessThan(tresholdToTest, ALPHA_0_05); // Second parameter is confidence level from TTable.h
  * or use operators: bool anser = lra < thresholdToTest
