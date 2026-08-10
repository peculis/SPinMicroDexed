# CAT 5: DIN MIDI input

Date: 10 August 2026

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
