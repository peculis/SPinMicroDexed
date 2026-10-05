# CAT 11 — read-only microSD bank discovery and loading

Date: 5 October 2026. Status: compiled; application/bench testing pending.

## Bounded change

Keep the accepted single-engine, effects-disabled configuration. Correct the
bank-name terminator overrun and const declaration mismatch. Discover one
usable .syx file per numbered directory, ignoring directories, hidden files,
wrong sizes, and unsupported extensions. Reject ambiguous directories and names
that exceed the ten-character bank limit. Check discovery failure before
constructing the voice filename. Existing write paths are not accepted here.

## Compile

Arduino IDE bundled CLI; Teensy platform 1.62.0; Teensy 4.0, 600 MHz, Faster,
Serial + MIDI + Audio. Vendored libraries: `third-party`.

```sh
arduino-cli compile --fqbn teensy:avr:teensy40:usb=serialmidiaudio,speed=600,opt=o2std \
  --libraries third-party --warnings all .
python3 tools/check_sd_banks.py addon/SD
```

The supplied 100 banks pass framing, size, bulk format/data length, seven-bit
payload and checksum checks. Compiler warnings remain in inherited save/UI
paths and other previously recorded areas; compilation is not storage acceptance.

## Apply and test

Use a backed-up/disposable card compatible with the Teensy SD library. Copy the
contents of `addon/SD` to the card root, keeping numbered directories directly
at the root. Insert with instrument power off. Upload this CAT 11 build using
the documented board settings.

1. Boot and capture Serial output through PLAYABLE. Confirm SD detection and
   preparation of VCFG/PCFG/FXCFG. Confirm no crash or unexplained pause.
2. Load banks 0, 10 and 99; select the first and last voices in each. Record
   displayed bank/voice names and whether each plays through both audio outputs.
3. Navigate patches while playing DIN MIDI and USB MIDI, testing each transport.
   Check controls, responsiveness, stuck notes and audio continuity.
4. Power off, remove the card, and reboot. Confirm the accepted no-card behavior.
5. With a spare test directory/card, check an empty bank directory and unrelated
   hidden/text files. Then test two usable banks in one directory and a damaged
   checksum. Confirm loading fails cleanly rather than crashing or loading an
   arbitrary bank. Do not overwrite the reference media for these tests.
6. Do not invoke voice, bank, performance, or configuration saves during this
   read-only CAT. Automatic startup directory creation is expected.

Return startup logs and observations. Ricardo accepts/rejects this CAT before
we proceed to save-path changes. Nothing has been uploaded automatically.

## Partial hardware evidence — 5 October 2026

Arduino CLI successfully uploaded the CAT 11 build to the connected Teensy 4.0.
Serial telemetry after upload reported:

```text
[CAT2] uptime=97s temp=61.3C LCD@0x27=OK SGTL@0x0A=OK codec=OK SD=10 AudioCPU=0.9/1.1% AudioMem=5/5 MidiPollMaxUs=37 USB(on/off/held/unmatched)=0/0/0/0
```

SD=10 indicates a card initialized on chip-select pin 10. LCD and codec health
checks pass at this observation. No USB notes were received in these counters;
audio performance, bank loading/navigation, startup logs and endurance remain
unverified. This is partial evidence, not a passed CAT 11.

## Bank/voice bench result — 5 October 2026

After being asked to select voices from banks 0, 10 and 99, confirm names and
play notes, Ricardo reported: “Tested. All good.” This accepts the requested
normal bank/voice loading test. It does not establish the unrequested malformed
media, card-removal or endurance cases. MIDI transport is awaiting clarification.

Observed telemetry through uptime 287 s retained LCD/codec OK and SD=10,
AudioMem peak 5, AudioCPU peak 8.6%, temperature 61.3–62.5 C. USB note counters
remained zero. Foreground MIDI poll gaps peaked at 115076 us during the test;
correlation with patch selection is not captured. This remains a release
performance observation to check under simultaneous patch changes and MIDI.

Ricardo selected the first MIDI-input option, DIN MIDI, for this successful test.
USB MIDI remains a separate pending check; its Serial note counters were zero.
