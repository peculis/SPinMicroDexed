# CAT 9: MIDI-safe timed LCD messages

Date: 10 August 2026

## Objective

Prevent one-second LCD confirmation and error messages from stopping MIDI input
service. Preserve the accepted CAT 8 audio, MIDI, LCD and encoder behavior.

## First-stage change

The inherited UI contains 24 calls to `delay(MESSAGE_WAIT_TIME)`, where
`MESSAGE_WAIT_TIME` is 1000 ms. CAT 9 replaces those busy waits with a
cooperative wait that continuously polls all configured MIDI inputs and yields
to the Teensy runtime. The message remains visible for the intended interval.
The 300 ms LCD initialization delay is unchanged because it occurs before the
instrument becomes playable.

This first stage deliberately avoids rewriting 11 independent load, save,
reset and SysEx menu functions into state machines. It removes the musical
failure mode with a small, reviewable change. A later cleanup may make the menu
itself responsive during the message interval if that is useful.

## Test

Play sustained and repeated notes while entering menu functions that display a
confirmation or error message. Verify that notes continue to start and stop,
there are no stuck notes, and `MidiPollMaxUs` remains bounded during the
one-second message display.
