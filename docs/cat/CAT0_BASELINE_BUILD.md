# CAT 0: imported baseline build

Date: 10 August 2026

## Result

**PASS — compile only.** The imported, unmodified MicroDexed baseline compiles
for Teensy 4.0. Nothing was uploaded to or executed on the hardware.

The compile was performed from a temporary copy because Arduino requires the
sketch directory to have the same name as `MicroDexed.ino`. The tracked source
was not changed for the build.

## Source baseline

- SPinMicroDexed initial commit:
  `2a32982a8262333639790524d8906b259e80a0ec`
- Imported MicroDexed commit:
  `c30955409ad93e4731652a96b223297cb2b6fcc0`
- Git branch: `main`

## Build configuration

- Arduino CLI: 1.5.1 (`01f3d4f2b`)
- Teensy platform: 1.62.0
- Board: Teensy 4.0 (`teensy:avr:teensy40`)
- CPU: 600 MHz
- Optimization: Faster (`o2std`)
- USB type: Serial + MIDI + Audio (`serialmidiaudio`)
- Warning level: all

No libraries or tools were downloaded for CAT 0.

## Library versions

| Library | Version | Source |
| --- | ---: | --- |
| PJRC Audio | 1.3 | Teensy 1.62.0 |
| MIDI Library | 5.0.2 | Teensy 1.62.0 |
| USBHost_t36 | 0.2 | Teensy 1.62.0 |
| SD | 2.0.0 | Teensy 1.62.0 |
| SPI | 1.0 | Teensy 1.62.0 |
| Wire | 1.0 | Teensy 1.62.0 |
| EEPROM | 2.0 | Teensy 1.62.0 |
| LCDMenuLib2 | 2.2.6 | vendored upstream |
| LiquidCrystal_I2C | 1.1.4 | vendored upstream |
| MD_REncoder | 1.0.1 | vendored upstream |

## Compiler memory report

```text
FLASH: code 255376, data 57244, headers 8912, free for files 1710084
RAM1:  variables 111520, code 252776, padding 9368,
       free for local variables 150624
RAM2:  variables 249024, free for malloc/new 275264
```

The baseline fits the Teensy 4.0, but this is a link-time report only. Runtime
Audio Library usage and heap behavior remain untested.

## Warning triage

The build succeeds, but the all-warnings compile identifies issues to address
in later CATs:

1. **Potential buffer overflows:** several `sprintf` calls can exceed their
   destination arrays in `dexed_sd.cpp`, `UI.hpp`, and `MicroDexed.ino`. These
   should be replaced with correctly sized buffers and bounded formatting
   before SD write/save testing.
2. **SD API const mismatch:** `entry.name()` now returns `const char *`, while
   `strip_extension` accepts `char *`. The helper should accept a const source.
3. **Version string construction:** several `strncat` calls use bounds equal to
   source lengths and generate termination/truncation warnings.
4. **Display formatting:** `Disp_Plus` deliberately fills a fixed-width field,
   but its `strncpy` pattern generates many unterminated-string warnings. The
   implementation should be made explicit and bounds-safe when the display
   adapter is revised.
5. **FM engine virtual signatures:** base and derived `render` declarations use
   slightly different parameter types, producing hidden-virtual warnings. This
   is not a CAT 1 hardware blocker but should be reviewed without changing DSP
   behavior.
6. **Known pin conflict:** the compiled baseline still assigns the Teensy 4.0
   right encoder button to pin 8. It must not be uploaded with the Audio Shield
   connected. CAT 1 will correct this before hardware execution.

## CAT 0 acceptance status

- Reproducible Teensy 4.0 compile: **pass**
- Missing dependency list: **none**
- Source changes required to compile: **none**
- Firmware uploaded: **no**
- Firmware executed: **no**
- Repository source remained unchanged during compile: **pass**

CAT 1 may proceed with the hardware profile and pin-conflict correction.
