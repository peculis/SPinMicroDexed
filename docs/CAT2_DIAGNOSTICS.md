# CAT 2: Nonblocking diagnostics

Date: 10 August 2026

## Objective

Add enough visibility to diagnose later hardware and timing tests without
changing the synthesizer, MIDI, menu, storage or audio architecture.

## Changes

- Start USB Serial independently of the legacy all-or-nothing `DEBUG` option.
- Print any Teensy 4.x `CrashReport` retained from the previous boot.
- Probe the LCD at I2C address `0x27` and SGTL5000 at `0x0A` once during boot.
- Retain and report the return value from `AudioControlSGTL5000::enable()`.
- Report microSD presence, uptime, processor temperature, current/maximum Audio
  CPU use and current/maximum Audio Memory use every five seconds.
- Toggle the built-in LED every 500 ms as a nonblocking heartbeat.

The diagnostics contain no `delay()` calls and do not write to the LCD. CAT 2
therefore preserves CAT 1 behavior, including the expected `*ERROR*` display
when no microSD card is installed.

## Hardware test

1. Compile and upload using Teensy 4.0, 600 MHz, Serial + MIDI + Audio, Faster.
2. Open Arduino IDE Serial Monitor. The configured speed is 230400 baud (USB
   Serial is baud-rate independent, but selecting 230400 documents the setup).
3. Confirm the built-in orange LED changes state twice per second.
4. Wait at least 15 seconds and retain three consecutive `[CAT2]` lines.
5. Confirm `LCD@0x27=OK`, `SGTL@0x0A=OK`, `codec=OK`, and `SD=none`.
6. Confirm uptime rises by five seconds per report, temperature is plausible,
   and CPU/memory values do not continuously climb while idle.
7. Operate both encoders for 30 seconds and confirm reports and heartbeat
   continue without a restart.

Expected line format:

```text
[CAT2] uptime=10s temp=40.0C LCD@0x27=OK SGTL@0x0A=OK codec=OK SD=none AudioCPU=.../...% AudioMem=.../...
```

CAT 2 passes when the status fields match the installed hardware, three reports
arrive at five-second intervals, and no reboot or retained crash is observed.

## Build verification

Configuration: Teensy platform 1.62.0, Teensy 4.0, 600 MHz, Serial + MIDI +
Audio, Faster optimization, and the same installed LCD/encoder libraries used
for CAT 1.

Result: **successful compile**.

```text
FLASH: code 212160, data 57244, headers 9120, free for files 1753092
RAM1:  variables 100768, code 207480, padding 21896,
       free for local variables 194144
RAM2:  variables 118528, free for malloc/new 405760
```

The diagnostic code adds approximately 3.3 kB of flash and 2 kB of RAM1 versus
CAT 1. The previously recorded upstream warnings remain; CAT 2 introduced no
compilation errors.
