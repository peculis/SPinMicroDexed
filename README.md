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

The initial objective was a conservative hardware port using the minimum
practical changes required for Ricardo Peculis's validated SPinSynth hardware,
without redesigning the inherited DX7-compatible synthesis arithmetic. As CAT
testing exercised the complete instrument, its scope expanded constructively to
include evidence-led refinements in pin protection, startup, MIDI/UI
cooperation, responsiveness and diagnostics for this particular configuration.
MicroDexed remains the capable and gratefully acknowledged foundation for this
work.

## Status

As of 5 October 2026, Ricardo reports that the ribbon-cable contacts are stable
and the working hardware is assembled inside its plastic housing. See the
[hardware assembly record and images](docs/HARDWARE_ASSEMBLY.md).

The accepted firmware baseline has completed CAT 1–10: LCD/encoder controls,
DIN and USB MIDI, simultaneous Audio Shield/USB audio, engine initialization,
MIDI/UI responsiveness, startup readiness, and full incoming velocity.

On 5 October 2026 we selected **one DX7 instance** for the first release.
Optional effects and USB Host MIDI remain disabled. Dual-instance operation
and effects are later milestones; the broader project objective is retained.
This is a development build, not yet a published release. microSD acceptance,
save/load safety, final enclosed-hardware regression and endurance testing are
still required. See [the release plan](docs/SINGLE_INSTANCE_RELEASE.md).

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

## Compile, Apply, Test process

Development proceeds through numbered **CATs (Compile, Apply, Test)**. Design
and coding produce a bounded change; CAT is the mandatory acceptance gate that
then compiles it, applies it to the real instrument and tests it against stated
criteria before further changes are introduced. The number, sequence, scope and
evidence for these limited changes are negotiated before coding and may be
revised as bench results change the diagnosis. The complete method is documented in
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
That history also compares the imported MicroDexed Teensy 4.0 configuration
with the SPinSynth hardware and distinguishes our application/integration
contributions from Synth_Dexed FM-engine optimization.

For a direct account of the project's purpose, origin, contribution, present
state and remaining objectives, see
[`docs/EXECUTIVE_SUMMARY.md`](docs/EXECUTIVE_SUMMARY.md). The Executive Summary
and detailed history are living documents and will be revised together through
the conclusion of the project.

SPinMicroDexed also serves as a documented human–AI software-engineering
experiment. The final product and collaboration will be evaluated for features,
quality, effort, traceability and method effectiveness according to
[`docs/COLLABORATION_EVALUATION_PLAN.md`](docs/COLLABORATION_EVALUATION_PLAN.md).

## Licensing

SPinMicroDexed remains licensed under GNU GPL version 3 because it is derived
from MicroDexed. Components inherited from MSFA retain their Apache License 2.0
terms. The original license texts are preserved as:

- [`LICENSE-GPL3.txt`](LICENSE-GPL3.txt)
- [`LICENSE-APACHE2.txt`](LICENSE-APACHE2.txt)

Individual source files may contain more specific copyright and license notices;
those notices remain in effect and must be preserved.

## First release candidate

The current candidate identity is **0.1.0-rc1, build 2**. See
[installation and operating instructions](docs/INSTALL_AND_OPERATE.md),
[CAT 14 bank-storage acceptance](docs/CAT14_BANK_STORAGE.md), and
[the release evidence/limitations](docs/SINGLE_INSTANCE_RELEASE.md).
The tested normal configuration/performance overwrite cases are recorded in
CAT 13. New voice-bank save/reception changes still require bench acceptance.
