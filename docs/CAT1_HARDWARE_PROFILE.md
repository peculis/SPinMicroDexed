# CAT 1: SPinSynth-T-LCD hardware profile

Date: 10 August 2026

## Status

**Software pass; hardware smoke test pending.** The CAT 1 firmware compiles for
Teensy 4.0 but has not been uploaded by Codex.

## Changes

- Renamed `MicroDexed.ino` to `SPinMicroDexed.ino` so the Arduino sketch name
  matches the independent project directory.
- Restricted the CAT 1 configuration to Teensy 4.0.
- Selected Serial + MIDI + Audio as the intended USB configuration.
- Retained DIN MIDI on `Serial1` and onboard USB MIDI device support.
- Deferred USB Host MIDI.
- Selected the PJRC Audio Shield and simultaneous USB Audio output.
- Reduced the baseline to one Dexed instance.
- Disabled chorus/delay/reverb and SGTL5000 enhancement processing.
- Set the initial SGTL5000 headphone volume to `0.85`, matching the validated
  SPinSynth-T-LCD hardware baseline.
- Corrected Audio Shield SD SPI to CS 10, MOSI 11 and SCK 13. MISO is the fixed
  hardware SPI pin 12 used by the SD library.
- Corrected encoder and button pins.
- Added compile-time rejection of any encoder/button assignment that overlaps
  Audio Shield I2S, SPI/SD or I2C pins.
- Corrected three upstream SysEx-send functions so DIN, onboard USB device and
  USB Host transports can be independently enabled.

## Pin assignment

| Function | Teensy 4.0 pin |
| --- | ---: |
| MIDI DIN RX (`Serial1`) | 0 |
| Left encoder A / B | 2 / 3 |
| Left button | 4 |
| Right encoder A / B | 5 / 9 |
| Right button | 14 |
| Audio Shield I2S RX / TX | 7 / 8 |
| Audio Shield SD CS / MOSI / MISO / SCK | 10 / 11 / 12 / 13 |
| LCD and SGTL5000 SDA / SCL | 18 / 19 |
| Audio Shield LRCLK / BCLK / MCLK | 20 / 21 / 23 |

## Build verification

Configuration:

- Arduino CLI 1.5.1
- Teensy platform 1.62.0
- `teensy:avr:teensy40`
- 600 MHz
- Faster optimization
- Serial + MIDI + Audio

Result: **successful compile**.

```text
FLASH: code 208776, data 55196, headers 8408, free for files 1759236
RAM1:  variables 98720, code 206184, padding 23192,
       free for local variables 196192
RAM2:  variables 118528, free for malloc/new 405760
```

Compared with CAT 0, the one-engine/no-effects profile recovers approximately
46.6 kB of code, 45.6 kB of RAM1 local-variable space and 130.5 kB of RAM2
dynamic-allocation space.

The legacy warnings recorded in CAT 0 remain and are intentionally outside this
hardware-profile change.

## First hardware smoke test

1. Inspect the wiring against the table above before connecting USB power.
2. In Arduino IDE select Teensy 4.0, 600 MHz, Serial + MIDI + Audio and Faster.
3. Compile `SPinMicroDexed.ino`.
4. Upload with the Audio Shield, LCD and encoders connected.
5. Confirm the LCD starts and the Teensy does not repeatedly reboot.
6. Confirm the Audio Shield and Teensy 3.3 V rails remain normal.
7. Turn and press both encoders and check for basic response. Encoder direction
   will be formally accepted in CAT 4.

CAT 1 hardware acceptance requires a stable boot with no pin contention. Audio,
MIDI, SD and UI performance are tested in later CATs.
