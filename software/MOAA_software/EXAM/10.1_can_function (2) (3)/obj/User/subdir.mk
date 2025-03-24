################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v20x_it.c \
../User/main.c \
../User/op_adc_byMax.c \
../User/soft_spi_byMax.c \
../User/system_ch32v20x.c 

OBJS += \
./User/ch32v20x_it.o \
./User/main.o \
./User/op_adc_byMax.o \
./User/soft_spi_byMax.o \
./User/system_ch32v20x.o 

C_DEPS += \
./User/ch32v20x_it.d \
./User/main.d \
./User/op_adc_byMax.d \
./User/soft_spi_byMax.d \
./User/system_ch32v20x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Debug" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Core" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\GPIO\10.1_can_function\User" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

