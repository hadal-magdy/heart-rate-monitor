# Pulse Heart-Rate Monitor — ATmega328P + UART + 16x2 LCD (Proteus)

## Layered architecture
```
+--------------------------------------------------+
| APP      app/app.c, main.c  (scheduler, UI, UART report) |
+--------------------------------------------------+
| SERVICE  service/heart_rate.c (beat detection, BPM, status) |
+--------------------------------------------------+
| HAL      hal/lcd, hal/led, hal/pulse_sensor      |
+--------------------------------------------------+
| MCAL     mcal/adc, mcal/uart, mcal/timer         |
+--------------------------------------------------+
| Hardware ATmega328P                              |
+--------------------------------------------------+
```
Rule: each layer only calls the layer directly below it.

## Pin map
| Signal            | Pin                     |
|-------------------|-------------------------|
| Pulse sensor out  | PC0 (ADC0)              |
| LCD RS / E        | PB0 / PB1               |
| LCD D4..D7        | PB2..PB5                |
| LCD RW, VSS, VEE  | GND                     |
| LCD VDD           | +5 V                    |
| UART TX           | PD1 -> Virtual Terminal RXD |
| Beat LED          | PD5 -> 330 ohm -> LED -> GND |

## Build
`make`  (needs avr-gcc) -> produces `pulse_monitor.hex`
(or create a Microchip Studio / Atmel Studio project, ATmega328P, add all files, F_CPU=8000000UL)

## Proteus setup
Parts: ATMEGA328P, LM016L, LED-RED, RES (330), + Virtual Terminal (Instruments)
and a Generator (Generator Mode, left toolbar) for the sensor signal.

1. Double-click the ATmega328P:
   - Program File = pulse_monitor.hex
   - Clock Frequency = 8MHz
   - Advanced Properties: CKDIV8 = (1) Unprogrammed
2. Sensor simulation: Sine generator on PC0 -> Offset 2.5 V, Amplitude 2 V,
   Frequency 1.2 Hz = 72 BPM.  (BPM = Frequency x 60)
   Try 0.8 Hz (48, LOW), 1.2 Hz (72, NORMAL), 2 Hz (120, HIGH).
3. Virtual Terminal: Baud 9600, 8 data bits, no parity, 1 stop bit.
4. Run. LCD shows BPM / IBI / status with a blinking heart; UART prints
   `BPM=72,IBI=833,STATUS=NORMAL` every beat.

The first 2-3 beats are used to lock the threshold, so BPM appears after ~3 seconds.
With a real Pulse Sensor (analog out), connect it to PC0 with no code changes.
# heart-rate-monitor
