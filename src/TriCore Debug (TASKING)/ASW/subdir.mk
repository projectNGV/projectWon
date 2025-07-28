################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../ASW/autopark.c" \
"../ASW/control.c" 

COMPILED_SRCS += \
"ASW/autopark.src" \
"ASW/control.src" 

C_DEPS += \
"./ASW/autopark.d" \
"./ASW/control.d" 

OBJS += \
"ASW/autopark.o" \
"ASW/control.o" 


# Each subdirectory must supply rules for building sources it contributes
"ASW/autopark.src":"../ASW/autopark.c" "ASW/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Documents/repos/tc375/project-won/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"ASW/autopark.o":"ASW/autopark.src" "ASW/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"ASW/control.src":"../ASW/control.c" "ASW/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fC:/Users/USER/Documents/repos/tc375/project-won/src/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"ASW/control.o":"ASW/control.src" "ASW/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-ASW

clean-ASW:
	-$(RM) ./ASW/autopark.d ./ASW/autopark.o ./ASW/autopark.src ./ASW/control.d ./ASW/control.o ./ASW/control.src

.PHONY: clean-ASW

