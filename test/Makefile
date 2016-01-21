
SHARED_CPP = ../src/StudentsDistribution.cpp \
			 ../src/TTable.cpp			 
SHARED_H = ../src/StudentsDistribution.h \
		   ../src/TimeSeries.h \
		   ../src/TTable.h
INCLUDE_DIR = ../src

LR_TEST = lr_test
LR_TEST_CPP = $(LR_TEST).cpp
ifeq ($(OS),Windows_NT)
	LR_TEST_OUT = $(LR_TEST).exe
else
	LR_TEST_OUT = $(LR_TEST)
endif
MEAN_TEST = mean_test
MEAN_TEST_CPP = $(MEAN_TEST).cpp
ifeq ($(OS),Windows_NT)
	MEAN_TEST_OUT = $(MEAN_TEST).exe
else
	MEAN_TEST_OUT = $(MEAN_TEST).exe
endif


.PHONY: clean all


all: lr_test mean_test

lr_test:
	g++ -o $(LR_TEST_OUT) $(LR_TEST_CPP) $(SHARED_CPP) -I$(INCLUDE_DIR)
	
mean_test:
	g++ -o $(MEAN_TEST_OUT) $(MEAN_TEST_CPP) $(SHARED_CPP) -I$(INCLUDE_DIR)
	
clean:
	rm $(LR_TEST_OUT) $(MEAN_TEST_OUT)
	