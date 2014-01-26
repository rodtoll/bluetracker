################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BlueTracker.cpp \
../src/BluetoothAdapter.cpp \
../src/BluetoothDevice.cpp \
../src/BluetoothDeviceFactory.cpp \
../src/FitbitDevice.cpp \
../src/GattTool.cpp \
../src/ISYDevice.cpp \
../src/StickNFindDevice.cpp \
../src/TISensorTagDevice.cpp 

OBJS += \
./src/BlueTracker.o \
./src/BluetoothAdapter.o \
./src/BluetoothDevice.o \
./src/BluetoothDeviceFactory.o \
./src/FitbitDevice.o \
./src/GattTool.o \
./src/ISYDevice.o \
./src/StickNFindDevice.o \
./src/TISensorTagDevice.o 

CPP_DEPS += \
./src/BlueTracker.d \
./src/BluetoothAdapter.d \
./src/BluetoothDevice.d \
./src/BluetoothDeviceFactory.d \
./src/FitbitDevice.d \
./src/GattTool.d \
./src/ISYDevice.d \
./src/StickNFindDevice.d \
./src/TISensorTagDevice.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


