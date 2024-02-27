USER_OBJS :=
LIBS :=
O_SRCS := 
CPP_SRCS := 
C_UPPER_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
CU_SRCS := 
ASM_SRCS := 
CXX_SRCS := 
C++_SRCS := 
CC_SRCS := 
CU_DEPS := 
OBJS := 
C++_DEPS := 
C_DEPS := 
CC_DEPS := 
CPP_DEPS := 
EXECUTABLES := 
CXX_DEPS := 
C_UPPER_DEPS := 

RM := rm -rf

# Every subdirectory with source files must be described here
SUBDIRS := \
src \

CU_SRCS += \
./ProjectionErrorStraight.cu \
./mrc_file.cu 

CU_DEPS += \
./ProjectionErrorStraight.d \
./mrc_file.d 

OBJS += \
./ProjectionErrorStraight.o \
./mrc_file.o 


# Each subdirectory must supply rules for building sources it contributes
ProjectionErrorStraight: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: NVCC Linker'
	nvcc  -link -o  "ProjectionErrorStraight" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
./%.o: ./%.cu
	@echo 'Building file: $<'
	@echo 'Invoking: NVCC Compiler'
	nvcc -G -g -O0 -gencode arch=compute_20,code=sm_20 -odir "src" -M -o "$(@:%.o=%.d)" "$<"
	nvcc --compile -G -O0 -g -gencode arch=compute_20,code=compute_20 -gencode arch=compute_20,code=sm_20  -x cu -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean:
	-$(RM) $(CU_DEPS)$(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ProjectionErrorStraight	
	-@echo ' '
