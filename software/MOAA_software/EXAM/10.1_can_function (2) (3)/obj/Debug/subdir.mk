################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/mexlinkInShanghai/Desktop/workInDec/HIT_proj/work_in_Dec/ch32V208/CH32V20xEVT/EVT/EXAM/SRC/Debug/debug.c 

OBJS += \
./Debug/debug.o 

C_DEPS += \
./Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Debug/debug.o: C:/Users/mexlinkInShanghai/Desktop/workInDec/HIT_proj/work_in_Dec/ch32V208/CH32V20xEVT/EVT/EXAM/SRC/Debug/debug.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Debug" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Core" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\GPIO\10.1_can_function\User" -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

