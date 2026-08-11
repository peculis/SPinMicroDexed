# CAT 10: Startup readiness

Date: 11 August 2026

## Objective

Measure the boot sequence and remove avoidable work that can make the first few
seconds feel slower than steady-state operation.

## CAT 10-A change

One-time Serial markers report LCD, MIDI, codec, audio-engine, EEPROM/config,
SD-probe and initial voice-screen durations. The inherited voice-screen setup
performed a bank lookup and voice lookup whose results were immediately
discarded, followed by the same lookups during its first draw. Those redundant
calls are removed.

When no SD card is detected, the first draw no longer calls filesystem lookup
functions. It displays `NO SD` and `RAM VOICE` instead of `*ERROR*`. Normal SD
lookup behavior remains in place when a card is present.

The first hardware run measured approximately 39.984 ms for voice-screen setup
and 46.440 ms for its first draw. Together they explained the observed 91.020 ms
startup poll-gap outlier. CAT 10-B now performs that initial screen setup and
draw synchronously before printing `[CAT10] PLAYABLE`. The MIDI poll-gap timer
is reset afterward, so the first foreground loop no longer inherits LCD startup
work.

## Test

Reset or power-cycle the Teensy with the Serial Monitor already open. Save every
`[CAT10]` line. Begin repeated notes as soon as the voice screen appears and
continue for 15 seconds, noting when response becomes stable. Send the startup
markers plus the first three five-second diagnostic reports.
