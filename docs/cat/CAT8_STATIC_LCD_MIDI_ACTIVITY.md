# CAT 8: Static LCD MIDI activity and poll-latency test

Date: 10 August 2026

## Objective

Remove the Teensy 4.x voice-screen animation that rewrote an LCD custom
character over I2C while MIDI was active. Preserve a useful note indicator and
the inherited menu behavior with much less runtime LCD traffic.

## Change

The previous Teensy 4.x path changed the activity glyph repeatedly with
`lcd.createChar()` during a velocity-decay animation. An HD44780 custom-character
update transfers an address plus eight rows through the I2C backpack and blocks
the foreground loop. MIDI polling occurs in that same loop.

CAT 8 initially loaded one fixed note glyph and wrote it only on activity-state
transitions. Hardware testing was most reliable after removing those remaining
Note On/Off-triggered LCD writes as well. The accepted configuration performs
no MIDI-activity LCD I2C transfer while playing. Menus and encoder mappings are
unchanged.

The first hardware measurement then exposed a second issue: the voice function
rewrote its complete bank/voice display on every foreground-loop pass, whether
or not any value had changed. Reported maximum MIDI-poll gaps were approximately
2.6--2.7 ms, with one 90.985 ms first-event outlier. Part of that measurement
also included the five-second Serial health report itself. The refined CAT 8
caches the screen state, redraws only on screen entry or an actual bank/voice/
row/program-change action, and excludes diagnostic printing from the poll-gap
measurement.

CAT 8 also measures the maximum interval between foreground MIDI polls. Each
five-second health report contains `MidiPollMaxUs=...` and resets that maximum
for the next interval. Time spent printing that report is explicitly excluded,
so compare repeated values and large outliers rather than treating it as a
hard-real-time benchmark.

## Test

1. Compile and upload with the established composite USB settings. Keep DIN
   disconnected initially.
2. Confirm `[CAT8-UI] Static MIDI activity symbol active ...` appears.
3. Play slow notes on the voice screen. Confirm a fixed note symbol appears
   while notes are active and clears after release. It must not animate.
4. Play rapid notes and chords for two minutes while listening through the
   Audio Shield and Logic USB Audio. Check for delayed, missing or stuck notes.
5. Navigate ordinary menu and volume pages while playing. Avoid Save/Load and
   EEPROM Reset because their inherited one-second delays are CAT 9 scope.
6. Record several `MidiPollMaxUs` values while idle, playing on the voice screen,
   and navigating menus. Report any extreme outlier and whether it corresponds
   to an intentional five-second diagnostic print or menu action.
7. Confirm clean LCD characters, stable USB Audio, no crash and no rising
   CPU/memory usage.

CAT 8 passes when musical response remains stable during UI use and MIDI
activity no longer causes runtime LCD I2C transfers.

## Hardware acceptance

CAT 8 passed on 10 August 2026 with natural MIDI velocity restored and all
note-triggered LCD writes disabled. USB MIDI completed 159 Note On and 159 Note
Off events with zero held or unmatched notes. Its maximum playing-time polling
gap was 331 us and maximum Audio CPU usage was 11.1 percent. DIN MIDI sounded
and responded equivalently; its observed maximum polling gap was 296 us. Both
paths were monitored directly from the Teensy Audio Shield.
