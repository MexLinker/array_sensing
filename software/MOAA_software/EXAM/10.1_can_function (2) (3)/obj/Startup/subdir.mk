################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
C:/Users/mexlinkInShanghai/Desktop/workInDec/HIT_proj/work_in_Dec/ch32V208/CH32V20xEVT/EVT/EXAM/SRC/Startup/startup_ch32v20x_D8W.S 

OBJS += \
./Startup/startup_ch32v20x_D8W.o 

S_UPPER_DEPS += \
./Startup/startup_ch32v20x_D8W.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_ch32v20x_D8W.o: C:/Users/mexlinkInShanghai/Desktop/workInDec/HIT_proj/work_in_Dec/ch32V208/CH32V20xEVT/EVT/EXAM/SRC/Startup/startup_ch32v20x_D8W.S
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"C:\Users\mexlinkInShanghai\Desktop\workInDec\HIT_proj\work_in_Dec\ch32V208\CH32V20xEVT\EVT\EXAM\SRC\Startup" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

