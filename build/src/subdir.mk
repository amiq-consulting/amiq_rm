################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/amiq_rm_address_map.cpp \
../src/amiq_rm_field.cpp \
../src/amiq_rm_reg.cpp \
../src/amiq_rm_types.cpp 

OBJS += \
./src/amiq_rm_address_map.o \
./src/amiq_rm_field.o \
./src/amiq_rm_reg.o \
./src/amiq_rm_types.o 

CPP_DEPS += \
./src/amiq_rm_address_map.d \
./src/amiq_rm_field.d \
./src/amiq_rm_reg.d \
./src/amiq_rm_types.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"../src" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


