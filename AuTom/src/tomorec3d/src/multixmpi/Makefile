RM := rm -rf


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


CPP_SRCS += \
mpi_done.cpp \
recon.cpp \
filter_prj.cpp \
mrcfile_atom.cpp \
opt.cpp \


OBJS += \
mpi_done.o \
recon.o \
filter_prj.o \
mrcfile_atom.o \
opt.o \



# Tool invocations
Muiltmpi: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MPIC++ Linker'		
	mpic++  $(OBJS)  -lm -lopencv_core -lopencv_highgui -lopencv_imgproc   -lrt -lpthread  -ldl  -Wl -o Muiltmpi
	@echo 'Finished building target: $@'
	@echo ' '

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MPIC++ Compiler'
	mpic++ -I/home/jiabin/zzy/epik/lib/opencv_2.3.1 -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -lm  -lopencv_core -lopencv_highgui -lopencv_imgproc -o $@ $<    
	@echo 'Finished building: $<'
	@echo ' '




# Other Targets
clean:
	-$(RM) $(CU_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Muiltmpi
	-@echo ' '


