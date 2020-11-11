################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CollectionInfo.cpp \
../MarianaPredict.cpp \
../MarianaRedu.cpp \
../Sparse.cpp \
../collReg.cpp \
../collText.cpp \
../documentCollection.cpp \
../helperRoutines.cpp \
../parseCommandLine.cpp \
../svm.cpp \
../uciReg.cpp \
../uciType.cpp 

O_SRCS += \
../Sparse.o \
../documentCollection.o \
../helperRoutines.o \
../parseCommandLine.o \
../svm.o \
../uciReg.o \
../uciType.o 

OBJS += \
./CollectionInfo.o \
./MarianaPredict.o \
./MarianaRedu.o \
./Sparse.o \
./collReg.o \
./collText.o \
./documentCollection.o \
./helperRoutines.o \
./parseCommandLine.o \
./svm.o \
./uciReg.o \
./uciType.o 

CPP_DEPS += \
./CollectionInfo.d \
./MarianaPredict.d \
./MarianaRedu.d \
./Sparse.d \
./collReg.d \
./collText.d \
./documentCollection.d \
./helperRoutines.d \
./parseCommandLine.d \
./svm.d \
./uciReg.d \
./uciType.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


