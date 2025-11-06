APP ?= j722s-main-r5f0_0-fw
APP_SOURCES ?= test.c uart.c

CROSS_COMPILE ?= arm-none-eabi-

.PHONY: $(APP)

CROSS_CC ?= $(CROSS_COMPILE)gcc
CROSS_SIZE ?= $(CROSS_COMPILE)size

all: $(APP)

clean:
	rm -f $(APP)

$(APP): $(APP_SOURCES) linker_script.ld
	$(CROSS_CC) $(CFLAGS) -mcpu=cortex-r5 -Og --specs=nosys.specs --specs=nano.specs -T linker_script.ld -o $(APP) $(APP_SOURCES)
	$(CROSS_SIZE) $(APP)
