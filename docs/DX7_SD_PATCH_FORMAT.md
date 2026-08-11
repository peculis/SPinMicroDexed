# SPinMicroDexed DX7 patch format on microSD

Date: 11 August 2026

## Overview

SPinMicroDexed inherits MicroDexed's support for Yamaha DX7 32-voice bulk
SysEx banks. Each bank is stored as one `.syx` file containing 32 packed DX7
voices. There is no separate patch database or index file.

The current SD loader accepts bank files that are exactly 4104 bytes long and
use Yamaha bulk format `0x09`. Arbitrary single-voice SysEx files are not
accepted by this loader.

## Bank-file layout

| File offset | Size | Meaning |
| ---: | ---: | --- |
| 0 | 1 byte | SysEx start byte: `0xF0` |
| 1 | 1 byte | Yamaha manufacturer ID: `0x43` |
| 2 | 1 byte | Yamaha substatus/device or MIDI-channel field |
| 3 | 1 byte | Format: `0x09`, identifying a 32-voice bulk dump |
| 4–5 | 2 bytes | Encoded data length: 4096 bytes (`0x20 0x00`) |
| 6–4101 | 4096 bytes | 32 packed voices, 128 bytes per voice |
| 4102 | 1 byte | Yamaha bulk checksum |
| 4103 | 1 byte | SysEx end byte: `0xF7` |

The checksum is calculated over all 4096 voice-data bytes:

```text
checksum = (-sum(voice_data[0..4095])) & 0x7F
```

## Packed voice layout

Each voice occupies 128 bytes in the bulk-data area:

| Content | Size |
| --- | ---: |
| Six packed operator records | 6 × 17 = 102 bytes |
| Pitch-envelope rates and levels | 8 bytes |
| Algorithm, feedback, oscillator sync, LFO and transpose | 8 bytes |
| Voice name | 10 bytes |
| **Total** | **128 bytes** |

Each operator record contains packed values for its envelope rates and levels,
keyboard level scaling, rate scaling, amplitude-modulation sensitivity,
keyboard velocity sensitivity, output level, oscillator mode, coarse and fine
frequency, and detune.

The ten-character voice name occupies the final ten bytes of each packed voice.
SPinMicroDexed reads this text directly from the bank file for the LCD.

For voice number `v`, where `v` is zero-based from 0 to 31:

```text
voice data starts at file offset 6 + (v × 128)
voice name starts at file offset 6 + (v × 128) + 118
```

## microSD directory layout

Bank numbers are represented by directories at the root of the card:

```text
/
├── 0/
│   └── sonus1.syx
├── 1/
│   └── synth_01.syx
├── 2/
│   └── synth_02.syx
│   ...
└── 99/
    └── rom4b.syx
```

The directory number is the bank number. The `.syx` filename without its
extension becomes the displayed bank name. For example:

```text
/10/piano_01.syx
```

means bank 10, displayed as `piano_01`, with 32 selectable voices numbered
1–32 on the HMI.

The current implementation should have exactly one usable `.syx` bank file in
each numbered directory. Bank filenames should contain no more than ten
characters before the extension because the inherited display and filename
buffers allow a ten-character bank name plus its terminator.

The firmware also creates these configuration directories when an SD card is
available:

```text
/VCFG   voice configurations
/PCFG   performances
/FXCFG  effects configurations
```

## Loading a voice

When loading a selected voice, MicroDexed:

1. Locates the bank file in the selected numbered directory.
2. Requires a file size of exactly 4104 bytes.
3. Validates the `0xF0` start byte and Yamaha ID `0x43`.
4. Requires bulk format `0x09`.
5. Requires the final byte to be `0xF7`.
6. Recalculates and verifies the checksum over all 4096 data bytes.
7. Copies the selected 128-byte packed voice.
8. Decodes it into Synth_Dexed's expanded internal voice representation.

## Editing and saving

Synth_Dexed works with an expanded internal representation while a patch is
being played or edited. When MicroDexed saves an edited voice, it:

1. Encodes the current internal voice into the 128-byte packed DX7 format.
2. Replaces the selected 128-byte slot in the existing 4104-byte bank file.
3. Recalculates the checksum across all 32 voices.
4. Writes the updated checksum at offset 4102.

This modifies the bank file in place. SPinMicroDexed's first SD acceptance test
will therefore be read-only. Save operations should not be relied upon until
the inherited SD filename and buffer-size warnings have been corrected and
hardware-tested.

## Compatibility

The SD format is compatible with original-style Yamaha DX7 32-voice bulk dumps
and Dexed cartridge exports when they use the exact 4104-byte format described
above. A file may still be rejected if it has a different SysEx format,
incorrect size, invalid checksum, missing framing bytes or non-bank contents.

The supplied initial card data is located in the repository at:

```text
addon/SD
```

It contains 100 numbered bank directories with 32 voice slots per bank, for a
total of 3200 slots. Some voices may be duplicated or closely related, so this
does not necessarily mean 3200 unique sounds.

## Source references

The implementation described here is principally in:

- `dexed_sd.cpp`: `load_sd_voice()`, `get_sd_voice()`, `put_sd_voice()`,
  `get_bank_name()` and `get_voice_by_bank_name()`.
- `SPinMicroDexed.ino`: `check_sd_cards()` and
  `check_and_create_directories()`.
- `config.h`: bank limits, name lengths and configuration-directory names.

