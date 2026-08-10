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

CAT 8 loads one fixed note glyph when the voice screen opens. Runtime activity
writes the cached glyph only on the idle-to-playing transition and writes one
blank only on the playing-to-idle transition. No custom character is generated
per animation frame. Menus and encoder mappings are unchanged.

CAT 8 also measures the maximum interval between foreground MIDI polls. Each
five-second health report contains `MidiPollMaxUs=...` and resets that maximum
for the next interval. The report itself can influence the following interval,
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

CAT 8 passes when the static indicator behaves correctly, musical response
remains stable during UI use, and the former animation no longer generates
continuous I2C character updates.
