PRG            = anod_fw-3.0
OBJ            = ./src/main.o\
				 ./src/loop.o\
				 ./src/test.o\
				 ./src/init.o\
				 ./src/dac.o\
				 ./src/adc.o\
				 ./src/spi.o\
				 ./src/usart.o\
				 ./drivers/clksys_driver.o\
				 ./drivers/TC_driver.o\
				 ./drivers/spi_driver.o\
				 ./drivers/usart_driver.o\
				 ./drivers/adc_driver.o\
				 ./drivers/event_system_driver.o
PROGRAMMER     = dragon_jtag
PORT           = usb
MCU_TARGET     = atxmega64a3
AVRDUDE_TARGET = ATXMEGA64A3
OPTIMIZE       = -Os
#OPTIMIZE       = -O1
DEFS           =
LIBS           =
INCLUDES	   = -I ./h  -I ./drivers
 
HZ             = 32000000
 
# You should not have to change anything below here.
 
CC             = avr-gcc
CXX            = avrg++
 
# Override is only needed by avr-lib build system.
 
override CFLAGS        = -g -DF_CPU=$(HZ) -std=gnu99 -Wall $(OPTIMIZE)  -funsigned-char -mmcu=$(MCU_TARGET) $(INCLUDES) $(DEFS)
override CXXFLAGS        = -g -DF_CPU=$(HZ) -Wall $(OPTIMIZE)  -funsigned-char -mmcu=$(MCU_TARGET) $(INCLUDES) $(DEFS)
override LDFLAGS       = -Wl,-Map,$(PRG).map,--section-start=.BOOT=0x020000
 
OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
 
all: $(PRG).elf lst text #eeprom
 
$(PRG).elf: $(OBJ) $(COMMONOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

./drivers/sp_driver.o:
	$(CC) -mmcu=$(MCU_TARGET) -c -o ./drivers/sp_driver.o ./drivers/sp_driver.s

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec ./src/*.o 
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

sweep:
	rm -rf *~ ./src/*~ ./h/*~ ./drivers/*~
	rm -rf ../*~
 
lst:  $(PRG).lst
 
%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
 
# Rules for building the .text rom images
 
text: hex bin srec
 
hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec
 
%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
 
%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@
 
%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@
 
# Rules for building the .eeprom rom images
 
eeprom: ehex ebin esrec
 
 
ehex:  $(PRG)_eeprom.hex
#ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec
 
%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@
 
#%_eeprom.srec: %.elf
#	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@
 
%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@
 
 
# command to program chip (invoked by running "make install")
install:  $(PRG).hex
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v  \
         -U flash:w:$(PRG).hex 
 
fuse:
	avrdude -p $(AVRDUDE_TARGET) -c $(PROGRAMMER) -P $(PORT) -v \
	-U lfuse:w:0xc6:m -U hfuse:w:0xd9:m 	
 
ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"
 
gdbserver: gdbinit
	simulavr --device $(MCU_TARGET) --gdbserver
 
gdbinit: $(GDBINITFILE)
 
$(GDBINITFILE): $(PRG).hex
	@echo "file $(PRG).elf" > $(GDBINITFILE)
 
	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load"                         >> $(GDBINITFILE)
	@echo "break main"                   >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"
