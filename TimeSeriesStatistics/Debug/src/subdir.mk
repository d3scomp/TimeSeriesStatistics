################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/StudentsDistribution.cpp \
../src/TDistribution.cpp \
../src/TimeSeries.cpp \
../src/TimeSeriesStatisticsDemo.cpp 

OBJS += \
./src/StudentsDistribution.o \
./src/TDistribution.o \
./src/TimeSeries.o \
./src/TimeSeriesStatisticsDemo.o 

CPP_DEPS += \
./src/StudentsDistribution.d \
./src/TDistribution.d \
./src/TimeSeries.d \
./src/TimeSeriesStatisticsDemo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


