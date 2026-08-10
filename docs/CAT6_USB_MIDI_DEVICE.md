# CAT 6: USB MIDI device input

Date: 10 August 2026

## Objective

Validate the Teensy onboard USB MIDI device independently of DIN MIDI, USB Host
and patch-dependent audio. The Arduino build must continue to use USB type
`Serial + MIDI + Audio`.

CAT 5 raw-byte and DIN-event logging is disabled. Normal DIN support remains
compiled and will be tested simultaneously with USB later. CAT 2 health reports
remain enabled.

## Instrumentation

USB Note On and Note Off callbacks report channel, note, velocity and held-note
count. USB Control Change, Program Change and Pitch Bend report their values.
The five-second health line ends with:

```text
USB(on/off/held/unmatched)=noteOn/noteOff/held/unmatchedOff
```

## Test

1. Compile and upload using Teensy 4.0, 600 MHz, Faster, and USB type `Serial +
   MIDI + Audio`.
2. Confirm the Mac/DAW sees the Teensy MIDI destination and Serial Monitor shows
   `[CAT6-USB] Onboard USB MIDI device test active ...`.
3. Route the KeyLab or a DAW/software keyboard to the Teensy USB MIDI
   destination on channel 1. Avoid using the DIN cable for this test.
4. Play slow individual notes with different velocities. Confirm matching USB
   NoteOn/NoteOff messages and values.
5. Hold and release a three-note chord. Confirm held reaches 3 and returns to 0.
6. Send pitch bend and a CC if convenient; confirm their decoded values.
7. Play for one minute. Confirm balanced totals, held 0, unmatched 0, continuing
   health reports, and no USB disconnect or restart.

No audible synthesis is required because no DX7 patch is loaded. CAT 6 passes
when the USB callbacks and counters behave correctly and the composite USB
Serial/MIDI/Audio device remains stable.
