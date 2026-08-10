# CAT 4: LCD and encoder interface

Date: 10 August 2026

## Objective

Validate the existing LCDMenuLib2-based MicroDexed interface on the KEYES
LCD1602 and both SPinSynth-T-LCD encoders. This test uses the real menu and
functions; it does not substitute a diagnostic screen.

The CAT 3 oscillators are disabled. CAT 2 health and control-event reporting
remain enabled. With no microSD card, `*ERROR*` bank/voice names are expected
and do not constitute an LCD or menu failure.

## Control behavior in the existing interface

- Right encoder rotation moves or changes the currently selected item.
- A short right-button press selects/enters.
- A long right-button press (at least 500 ms) enters the menu from the voice
  screen, or returns to the voice screen from elsewhere.
- Left encoder rotation opens/changes the volume screen.
- A short left-button press opens the root menu initially, otherwise returns or
  exits the current screen.
- A long left-button press invokes panic/all-notes-off and normally has no
  visible LCD result when no notes are sounding.

## Hardware test

1. Compile and upload with the established Teensy 4.0 settings.
2. Confirm the tones have stopped and Serial Monitor prints the `[CAT4-UI]`
   startup line plus continuing `[CAT2]` health reports.
3. On the voice screen, rotate the right encoder both ways. Confirm the voice
   number changes and matching RIGHT rotation messages appear. The patch name
   remains `*ERROR*` because there is no SD card.
4. Short-press the right encoder. Confirm the selection brackets alternate
   between the bank row and voice row. Rotate again and confirm the selected
   number changes.
5. Long-press the right encoder for approximately one second. Confirm the menu
   appears. Rotate right through several entries, short-press to enter one
   non-destructive page such as `Voice`, `Audio`, or `Info`, then long-press
   right to return to the voice screen.
6. Rotate the left encoder. Confirm the volume page appears and its value
   changes. Short-press left to leave it.
7. Short-press left from the voice screen. Confirm the root menu appears. Use
   right rotation and short press to navigate, then use short-left presses to
   move back out.
8. Exercise both controls for at least one minute. Confirm clean characters,
   no frozen screen, no unintended selections, continuing five-second health
   reports, and no restart.

Avoid `EEPROM Reset` and SD-dependent Save/Load commands during CAT 4. Do not
judge MIDI-note animation or MIDI responsiveness yet; those belong to later
CATs.

CAT 4 passes when all rotation, short-press, long-press, enter and back actions
work consistently and the LCD remains stable for the one-minute test.
