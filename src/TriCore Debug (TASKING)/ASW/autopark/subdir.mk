################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASW/autopark/autopark.c \
../ASW/autopark/level.c 

COMPILED_SRCS += \
ASW/autopark/autopark.src \
ASW/autopark/level.src 

C_DEPS += \
ASW/autopark/autopark.d \
ASW/autopark/level.d 

OBJS += \
ASW/autopark/autopark.o \
ASW/autopark/level.o 


# Each subdirectory must supply rules for building sources it contributes
ASW/autopark/autopark.src: ../ASW/autopark/autopark.c ASW/autopark/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/PROJECT/projectWon/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
ASW/autopark/autopark.o: ASW/autopark/autopark.src ASW/autopark/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
ASW/autopark/level.src: ../ASW/autopark/level.c ASW/autopark/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/PROJECT/projectWon/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
ASW/autopark/level.o: ASW/autopark/level.src ASW/autopark/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-ASW-2f-autopark

clean-ASW-2f-autopark:
	-$(RM) ASW/autopark/autopark.d ASW/autopark/autopark.o ASW/autopark/autopark.src ASW/autopark/level.d ASW/autopark/level.o ASW/autopark/level.src

.PHONY: clean-ASW-2f-autopark

