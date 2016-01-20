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
length is specified as template parameters: WindowCnt is the count of history windows; SubWindowSize
is the time length of a window (in milliseconds). The WindowsCnt * SubWindowSize = time length of
the time series history.

Using the TimeSeries.addSample method the time series history can be populated. And subsequential
queries can be than asked. There can be obtained the mean, and parameters of linear regression
from the time series. These quantities are returned as Student's distribution over them.

Finally with the Student's distribution it is possible to test hypotheses for some threshold
and it's inequality with the queried quantity on some confidence level.

The project contains a demo application that shows how this project can be used.

