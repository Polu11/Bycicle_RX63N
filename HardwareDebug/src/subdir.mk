################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/ADb10.c \
../src/Bycicle_RX63N.c \
../src/CMT.c \
../src/PID.c \
../src/pwm.c \
../src/sensore.c 

COMPILER_OBJS += \
src/ADb10.obj \
src/Bycicle_RX63N.obj \
src/CMT.obj \
src/PID.obj \
src/pwm.obj \
src/sensore.obj 

C_DEPS += \
src/ADb10.d \
src/Bycicle_RX63N.d \
src/CMT.d \
src/PID.d \
src/pwm.d \
src/sensore.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

