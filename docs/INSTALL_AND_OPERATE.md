# Install and operate the single-instance partial release

Target: Teensy 4.0 at 600 MHz, Audio Shield Rev D/D2, native 3.3 V LCD1602,
two encoders and SPinSynth-T hardware. Firmware identity: 0.1.0, build 3.
This partial release includes bank-save changes whose bench acceptance remains
pending. See RELEASE_0.1.0.md for the explicit validation limits.

## Build and upload

Install Arduino IDE/CLI and Teensy board platform 1.62.0. The accepted settings
are Teensy 4.0, 600 MHz, Faster, Serial + MIDI + Audio. Keep the sketch directory
named SPinMicroDexed and open SPinMicroDexed.ino. Retain the vendored LCDMenuLib2,
LiquidCrystal_I2C and MD_REncoder dependencies in third-party. For Arduino CLI:

```sh
arduino-cli compile --fqbn teensy:avr:teensy40:usb=serialmidiaudio,speed=600,opt=o2std \
  --libraries third-party --warnings all .
```

The dependency/version record is in CAT0_BASELINE_BUILD.md. Audio, MIDI, SD, SPI,
Wire and EEPROM are supplied by the documented Teensy platform. For a prebuilt
hex, open the firmware in Teensy Loader and upload to Teensy 4.0. When automatic
reboot fails, use the physical Teensy programming button if accessible.

Hardware wiring and voltage requirements are in SPINSYNTH_T_LCD_HARDWARE.md
and CAT1_HARDWARE_PROFILE.md. The USB product name is SPinMicroDexed. Wait until
the initialization splash is replaced by the voice screen before playing.

## Controls

- Right rotation: change voice or the selected menu/parameter item.
- Short right press: enter a menu/item or confirm.
- Long right press: switch bank/voice row on the voice screen; elsewhere return
  to the voice screen.
- Left rotation: volume.
- Short left press: open the root menu from the voice screen; otherwise back/exit.
- Long left press: panic/all notes off.

Voice > Setup > Transpose changes pitch in semitones. The correct submenu is
Setup. There is no Pitch submenu in this firmware.

## Card and patches

Use FAT16/FAT32 media supported by the Teensy SD library. Bank directories are
0 through 99 at the card root, with one 4104-byte DX7 bulk .syx file in each.
Use 1–10 characters before the extension and avoid hidden filenames. Firmware
creates VCFG, PCFG and FXCFG configuration directories. Run
`python3 tools/check_sd_banks.py CARD_ROOT` before using prepared media.
The exact format is documented in DX7_SD_PATCH_FORMAT.md.

Patch banks bundled in the working repository have passed integrity checks;
redistribution permission for those sounds has not been established by this
campaign. The distributable source package therefore omits addon/SD
and third-party/test.mid. Supply banks you own or are permitted to use.

## Save and load

Load/Save > Voice Config stores performance controls for the current engine,
including transpose. Load/Save > Performance stores the selected bank/voice and
configuration references. These are distinct from Voice > Save Voice, which
writes an edited DX7 patch into a bank slot.

Select a slot with right rotation and short-press right. Existing configuration
and performance slots ask Overwrite; choose YES/NO with right rotation and
confirm with short right. Done means the save function returned success;
Save failed means the operation was not accepted. Load the slot after changing
live settings to verify restoration. Manual reload after power cycling has been
tested; automatic startup performance restoration is not claimed.

Configuration and bank writes are staged with temporary/backup files. FAT
replacement is not power-loss atomic. A retained .bak blocks further replacement
until manually examined/recovered on a computer. Keep a copy before recovery;
do not silently delete the backup. A failure can be reported after the new file
is installed if backup cleanup failed. Live MIDI can experience delays during
storage operations; numeric worst-case latency is not yet accepted.

## Scope and limitations

One engine, effects disabled, USB Host MIDI deferred. Existing-bank reception
requires the same bank filename; renaming a populated bank is rejected to avoid
removing unrelated files. Do not receive another bulk dump during a save.
No-card failure is host-tested but not physically tested because Ricardo's
instrument remains closed. More than 24 hours of operation without observed
faults was reported, but the exact build/activity were unspecified; this does
not validate later changes. See the CAT records and SINGLE_INSTANCE_RELEASE.md
for accepted evidence and remaining checks.
