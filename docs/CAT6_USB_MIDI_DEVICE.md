# CAT 6: USB MIDI device input

Date: 10 August 2026

## Status

**PASS on target hardware after the engine-initialization correction.** Logic
showed USB MIDI arriving, the built-in FM-piano voice sounded through the Audio
Shield, and the system ran beyond 18 minutes under active playing without a
fault or USB disconnect.

Final counters were 735 Note On, 735 Note Off, zero held and zero unmatched
releases. Intermediate held counts of 4 and 8 correctly represented active
chords before returning to zero. Peak Audio CPU was 10.4%, Audio Memory peaked
at 13 blocks, and temperature remained approximately 54--55 degrees C.

Logic also briefly received USB Audio, but host monitoring/output routing was
not accepted here and is deferred to CAT 7.

## Objective

Validate the Teensy onboard USB MIDI device independently of DIN MIDI, USB Host
and patch-dependent audio. The Arduino build must continue to use USB type
`Serial + MIDI + Audio`.

CAT 5 raw-byte and DIN-event logging is disabled. Normal DIN support remains
compiled and will be tested simultaneously with USB later. CAT 2 health reports
remain enabled.

## Instrumentation

USB callbacks update counters and held-note state but deliberately perform no
USB Serial printing. Printing from a USB MIDI callback can re-enter the Teensy
composite USB stack while it is servicing another endpoint. The five-second
main-loop health line safely reports:

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
4. Play one slow individual note, then wait for the next health report. Confirm
   the USB NoteOn/NoteOff totals each increased by one and held returned to 0.
5. Hold and release a three-note chord. Confirm held reaches 3 and returns to 0.
6. Send pitch bend and a CC if convenient; confirm their decoded values.
7. Play for one minute. Confirm balanced totals, held 0, unmatched 0, continuing
   health reports, and no USB disconnect or restart.

No audible synthesis is required because no DX7 patch is loaded. CAT 6 passes
when the USB callbacks and counters behave correctly and the composite USB
Serial/MIDI/Audio device remains stable.

## First-run fault and isolation

The first CAT 6 build printed each event from inside its USB MIDI callback and
faulted on target with a Teensy 4.x `DACCVIOL` Data Access Violation when a key
was pressed. DIN MIDI had already exercised the same synthesis handler without
a fault. Callback-time Serial output was therefore removed as the first
controlled isolation step; only periodic main-loop reporting remains.

The callback-safe build faulted again at instruction address `0x14BAE` while
accessing `0x1DA1F6A`. Symbolizing the exact ELF located the instruction at
`Dx7Note::compute()` line 299, where `ctrls->core->render()` dereferences the FM
engine pointer. Source inspection found that the `Dexed` constructor called
`setEngineType()` while `engineType` was uninitialized. If its indeterminate
value happened to equal the requested engine, the function returned without
initializing `controllers.core`. The constructor now sets `engineType` to an
invalid sentinel and `controllers.core` to null before the mandatory first
engine selection. This is a latent upstream initialization bug exposed by the
CAT 6 memory layout, not evidence of a defective USB or Audio Shield interface.
