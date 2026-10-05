# CAT 5: DIN MIDI input

Date: 10 August 2026

## Status

**PASS on target hardware.** Raw Serial1 reception and PJRC MIDI parsing were
both confirmed on Teensy pin 0 with the KeyLab 25 and the existing SPinSynth DIN
input circuit. Note On/Off channel, note and velocity values decoded correctly.

A C-major chord produced held counts 1, 2 and 3; its releases returned through
2 and 1 to 0. The final counters were 14 Note On, 14 Note Off, zero held and
zero unmatched Note Off. Channel-aftertouch status/data (`0xD0`) was also
observed. Five-second health reports continued without a restart; idle Audio CPU
was approximately 1.0%, with a diagnostic-printing peak of 4.3%, and Audio
Memory remained 5/5 blocks.

The raw-byte and per-event Serial output is temporary CAT instrumentation and
must be disabled before latency/performance acceptance.

## Objective

Validate the PJRC Arduino MIDI Library receive path from the opto-isolated DIN
input through Teensy `Serial1` RX pin 0 and into MicroDexed's handlers. This CAT
does not require an SD card or audible synthesis. Patch-dependent audio is tested
after a usable voice is supplied.

CAT 3 tones remain disabled. The temporary per-action encoder logs are disabled
to keep Serial traffic focused on MIDI. CAT 2 health reporting remains active.

## Instrumentation

DIN Note On and Note Off messages report channel, note, velocity and the number
of keys logically held. Control Change, Program Change and Pitch Bend messages
also report their received values. Each five-second health line ends with:

```text
DIN(on/off/held/unmatched)=noteOn/noteOff/held/unmatchedOff
```

After all keys and the sustain pedal are released, `held` must return to zero.
`unmatched` should remain zero during an ordinary test.

## Hardware test

1. Connect the controller's MIDI OUT to SPinSynth DIN MIDI IN. Do not connect
   MIDI IN-to-IN. Set the controller to MIDI channel 1 initially.
2. Compile and upload with the established Teensy 4.0 settings. Confirm the
   `[CAT5-DIN] DIN MIDI test active ...` startup line.
3. Play individual low, middle and high notes slowly. Confirm one NoteOn and one
   matching NoteOff for each key, with the correct note numbers and channel.
4. Play softly and firmly. Confirm received velocities change appropriately.
5. Hold a three-note chord. Confirm `held` reaches 3, then returns to 0 after
   releasing all notes.
6. If available, operate sustain, modulation, volume or another controller.
   Confirm the corresponding CC number and values. Sustain is CC 64.
7. Move pitch bend through its range and back to center. Confirm PitchBend values
   change and return close to zero.
8. If convenient, send a Program Change and confirm it is reported. With no SD
   card it cannot load a bank and `*ERROR*` remains expected.
9. Play repeated notes and chords for one minute. Confirm no missing release,
   `held=0`, `unmatched=0`, continuing health reports and no restart.

Serial output occurs only in this diagnostic CAT and is not a final low-latency
MIDI design. CAT 5 passes when DIN events and their values are correct, all held
notes clear, and the system remains stable.

## Raw-UART diagnostic

The first hardware run produced no decoded Note On/Off messages. A temporary
probe therefore peeks at `Serial1` before the MIDI parser. It does not consume
the byte. When electrical MIDI data reaches Teensy pin 0, it prints:

```text
[CAT5-RAW] Serial1 bytes=3 next=0x90
```

Raw messages without `[CAT5-DIN]` messages indicate a parser/configuration
problem. Neither raw nor decoded messages indicate that no valid electrical
data is reaching RX pin 0, narrowing the check to controller MIDI OUT, cable,
DIN input circuit, power/ground, or the physical connection to pin 0.
