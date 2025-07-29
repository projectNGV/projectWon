################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ASW/app/main0.c \
../ASW/app/systeminit.c 

COMPILED_SRCS += \
ASW/app/main0.src \
ASW/app/systeminit.src 

C_DEPS += \
ASW/app/main0.d \
ASW/app/systeminit.d 

OBJS += \
ASW/app/main0.o \
ASW/app/systeminit.o 


# Each subdirectory must supply rules for building sources it contributes
ASW/app/main0.src: ../ASW/app/main0.c ASW/app/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/AURIX-v1.10.2-workspace/junhyeok/projectWon/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
ASW/app/main0.o: ASW/app/main0.src ASW/app/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
ASW/app/systeminit.src: ../ASW/app/systeminit.c ASW/app/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/AURIX-v1.10.2-workspace/junhyeok/projectWon/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
ASW/app/systeminit.o: ASW/app/systeminit.src ASW/app/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-ASW-2f-app

clean-ASW-2f-app:
	-$(RM) ASW/app/main0.d ASW/app/main0.o ASW/app/main0.src ASW/app/systeminit.d ASW/app/systeminit.o ASW/app/systeminit.src

.PHONY: clean-ASW-2f-app

