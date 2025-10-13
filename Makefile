all : j722s-main-r5f0_0-fw


# Define common flags
ARCH_FLAGS = -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mfloat-abi=hard
C_FLAGS = -Wall -g -O0 $(ARCH_FLAGS) -c
LDFLAGS = -nostdlib -nostartfiles $(ARCH_FLAGS) -T linker_script.ld

OBJECTS = main.o uart.o startup.o startup1.o resource_table.o

all : j722s-main-r5f0_0-fw

j722s-main-r5f0_0-fw : $(OBJECTS)
	arm-none-eabi-gcc $(LDFLAGS) $(OBJECTS) -o j722s-main-r5f0_0-fw

startup1.o : startup1.s
	arm-none-eabi-gcc $(ARCH_FLAGS) -c startup1.s -o startup1.o

main.o : main.c 
	arm-none-eabi-gcc $(C_FLAGS) main.c -o  main.o

uart.o : uart.c
	arm-none-eabi-gcc $(C_FLAGS) uart.c -o uart.o

startup.o : startup.c
	arm-none-eabi-gcc $(C_FLAGS) startup.c -o startup.o

resource_table.o : resource_table.c
	arm-none-eabi-gcc $(C_FLAGS) resource_table.c -o resource_table.o

clean : 
	rm -f *.o j722s-main-r5f0_0-fw
