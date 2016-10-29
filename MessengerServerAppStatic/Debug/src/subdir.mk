################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Dispatcher.cpp \
../src/LoginManger.cpp \
../src/MessengerServerApp.cpp \
../src/Model.cpp \
../src/Room.cpp \
../src/User.cpp 

OBJS += \
./src/Dispatcher.o \
./src/LoginManger.o \
./src/MessengerServerApp.o \
./src/Model.o \
./src/Room.o \
./src/User.o 

CPP_DEPS += \
./src/Dispatcher.d \
./src/LoginManger.d \
./src/MessengerServerApp.d \
./src/Model.d \
./src/Room.d \
./src/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/Asi/Asi-Project/SocketUtils/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


