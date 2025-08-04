################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASW/fsm.c 

COMPILED_SRCS += \
ASW/fsm.src 

C_DEPS += \
ASW/fsm.d 

OBJS += \
ASW/fsm.o 


# Each subdirectory must supply rules for building sources it contributes
ASW/fsm.src: ../ASW/fsm.c ASW/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/AURIX-v1.10.2-workspace/junhyeok/projectWon/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
ASW/fsm.o: ASW/fsm.src ASW/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-ASW

clean-ASW:
	-$(RM) ASW/fsm.d ASW/fsm.o ASW/fsm.src

.PHONY: clean-ASW

