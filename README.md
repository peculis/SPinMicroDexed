# SPinMicroDexed

SPinMicroDexed is an adaptation of MicroDexed for the validated
SPinSynth-T-LCD hardware platform:

- Teensy 4.0 at 600 MHz
- PJRC Teensy Audio Shield Rev D/D2 (SGTL5000)
- KEYES 3.3 V LCD1602 at I2C address `0x27`
- two rotary encoders with push-buttons
- MIDI DIN input and USB MIDI device
- simultaneous Audio Shield and USB Audio output
- microSD on the Audio Shield

The first milestone is a conservative hardware port. The existing DX7-compatible
synthesis engine, voice loading, SysEx handling, and menu behavior will be kept
intact until the basic system is building and operating reliably.

## Status

The imported MicroDexed baseline has passed CAT 0 compilation. CAT 1 adds the
initial conflict-free SPinSynth-T-LCD hardware profile and is ready for its first
hardware smoke test. Later CATs will add diagnostics and remove blocking LCD/UI
behavior before the full performance test.

## CAT 1 Arduino build settings

- Board: Teensy 4.0
- CPU speed: 600 MHz
- USB type: Serial + MIDI + Audio
- Optimization: Faster

Open `SPinMicroDexed.ino` from this repository. Do not select a different Teensy
board: the CAT 1 configuration deliberately stops compilation unless Teensy 4.0
is selected.

The validated SPinSynth hardware baseline is recorded in
[`docs/SPINSYNTH_T_LCD_HARDWARE.md`](docs/SPINSYNTH_T_LCD_HARDWARE.md).

## Compile And Test process

Development proceeds through numbered **CATs (Compile And Test)**. Each CAT is
a controlled stage in which a limited set of changes is compiled, deployed to
the target hardware, tested against stated acceptance criteria, and recorded
before further changes are introduced. The complete method is documented in
[`docs/CAT_PROCESS.md`](docs/CAT_PROCESS.md).

## Origin and acknowledgements

SPinMicroDexed is derived from **MicroDexed** by Holger Wirtz (`dcoredump`):

- <https://codeberg.org/dcoredump/MicroDexed>
- imported baseline commit: `c30955409ad93e4731652a96b223297cb2b6fcc0`

MicroDexed is itself based on **Dexed** by Pascal Gauthier and on the
**music-synthesizer-for-android (MSFA)** synthesis engine by Raph Levien and the
MSFA contributors.

Additional upstream acknowledgements include:

- PPPlay and its documented OPL implementation
- Nils Feldkäper, author of LCDMenuLib2
- Thierry Pottier, testing and UI feedback
- Lars Pelz, testing and documentation
- PJRC and Paul Stoffregen for Teensy, Teensyduino, the Teensy Audio Library,
  USB MIDI/USB Audio support, and associated libraries
- the authors and contributors of the Arduino MIDI, Encoder, and
  LiquidCrystal_I2C libraries used by the project

SPinSynth-T-LCD hardware integration and the SPinMicroDexed adaptation are by
Ricardo Peculis, with engineering assistance from OpenAI Codex.

See [`NOTICE.md`](NOTICE.md) for the detailed provenance record.
The developing narrative and CAT history are collected in
[`docs/PROJECT_HISTORY_DRAFT.md`](docs/PROJECT_HISTORY_DRAFT.md).

## Licensing

SPinMicroDexed remains licensed under GNU GPL version 3 because it is derived
from MicroDexed. Components inherited from MSFA retain their Apache License 2.0
terms. The original license texts are preserved as:

- [`LICENSE-GPL3.txt`](LICENSE-GPL3.txt)
- [`LICENSE-APACHE2.txt`](LICENSE-APACHE2.txt)

Individual source files may contain more specific copyright and license notices;
those notices remain in effect and must be preserved.
