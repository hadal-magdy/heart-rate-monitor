.RECIPEPREFIX = >
MCU     = atmega328p
F_CPU   = 8000000UL
TARGET  = pulse_monitor
SRCS    = app/main.c app/app.c service/heart_rate.c \
          hal/lcd.c hal/led.c hal/pulse_sensor.c \
          mcal/adc.c mcal/uart.c mcal/timer.c
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu99

all: $(TARGET).hex

$(TARGET).elf: $(SRCS)
> avr-gcc $(CFLAGS) $(SRCS) -o $@

$(TARGET).hex: $(TARGET).elf
> avr-objcopy -O ihex -R .eeprom $< $@
> avr-size --mcu=$(MCU) -C $<

clean:
> rm -f *.elf *.hex
