# From MicroDexed to SPinMicroDexed

Partial project-history draft — 11 August 2026

> This is a living draft. It records the project through CAT 10 and will be
> revised when SD-card integration, DX7 bank loading and the first public
> release are complete.

## 1. Origin and software lineage

SPinMicroDexed is a hardware-focused adaptation of **MicroDexed**, the compact
Teensy implementation developed and maintained by **Holger Wirtz**
(`dcoredump`). The SPinMicroDexed repository began from a direct snapshot of
MicroDexed `master`, commit
`c30955409ad93e4731652a96b223297cb2b6fcc0`, dated 21 June 2024. The snapshot
was imported on 10 August 2026 and given a new, independent Git history. The
original copyright headers and licence notices were retained.

Upstream MicroDexed:
<https://codeberg.org/dcoredump/MicroDexed>

MicroDexed uses **Synth_Dexed**, also maintained upstream by Holger Wirtz, to
bring the Dexed FM engine to Teensy 3.5, 3.6 and 4.x hardware. A public mirror
describes Synth_Dexed as a Teensy library port of Dexed:
<https://github.com/probonopd/Synth_Dexed>

**Dexed**, created by **Pascal Gauthier** (`asb2m10`), is a multiplatform
software synthesizer closely modelled on the Yamaha DX7. Its synthesis engine
is substantially based on **music-synthesizer-for-android (MSFA)** by
**Raph Levien** and the MSFA contributors:

- <https://github.com/asb2m10/dexed>
- <https://github.com/google/music-synthesizer-for-android>

The lineage also includes work acknowledged by the upstream projects,
including PPPlay and its documented OPL implementation; Nils Feldkäper and
LCDMenuLib2; Thierry Pottier; Lars Pelz; and the many Dexed and MSFA
contributors. SPinMicroDexed additionally depends on the Teensy ecosystem
created by **PJRC** and **Paul Stoffregen**, including Teensyduino, the Teensy
Audio Library, USB MIDI and USB Audio support.

The Microdexed Synth Variations repository by **Tobias Van Dyk** was considered
during the initial investigation because it documents Teensy 4.0/4.1 LCD and
encoder builds. It was a research reference, not the source snapshot imported
into SPinMicroDexed:
<https://github.com/TobiasVanDyk/Microdexed-Synth-Variations>

## 2. From SPinSynth to SPinMicroDexed

**SPinSynth-T-LCD** is the compact hardware platform developed by
**Ricardo Peculis** around a Teensy 4.0, PJRC Audio Shield Rev D/D2, KEYES
LCD1602 I2C display, two push-button rotary encoders, DIN MIDI input and USB.
The LCD and Audio Shield share the 3.3 V I2C bus, while the Audio Shield provides
I2S audio and an SPI microSD socket.

SPinMicroDexed began with a deliberately conservative objective: adapt the
existing MicroDexed application to this already validated hardware without
redesigning its DX7-compatible synthesis engine. Reliability, measurable
behaviour and reversible changes were placed ahead of new features.

The imported code was first compiled unchanged. Only after that baseline was
preserved were the sketch name, pin map, LCD integration, MIDI paths and audio
connections adapted for SPinSynth-T-LCD. In particular, the inherited Teensy
4.0 encoder assignment on pin 8 conflicted with the Audio Shield I2S data pin;
the SPinSynth profile uses pin 9 for that encoder signal and reserves every
Audio Shield I2S, I2C and SPI pin explicitly.

## 3. Engineering and test method

The development used a sequence of **CATs (Compile And Test)**. A CAT is a
controlled development stage in which a limited set of changes is compiled,
deployed to the target hardware, tested against stated acceptance criteria and
recorded before further changes are introduced. The CAT programme was conceived
and orchestrated by **Ricardo Peculis**, who set
the objectives, selected the hardware constraints, decided when each stage
could advance, operated the physical prototype and judged its musical response.

The firmware investigation and implementation were performed collaboratively
with **OpenAI Codex**. Codex inspected the source and architecture, prepared and
compiled each diagnostic build, maintained the Git history and test documents,
interpreted Serial telemetry and crash reports, and implemented the agreed
corrections. Ricardo performed the real-world electrical, control, MIDI and
listening tests and returned the observations and logs that drove each next
step. In this sense, the CAT campaign was Ricardo's engineering investigation,
executed on the bench with Codex as his coding and diagnostic engineering
partner.

Each significant change received its own commit. Failed or ambiguous tests
were not silently folded into later work: they were used to create smaller
isolation builds until one variable at a time could be accepted or rejected.
The detailed records remain in `docs/CAT*.md` and in the repository history.

## 4. Test history through CAT 10

| CAT | Purpose | Result and principal finding |
| --- | --- | --- |
| 0 | Preserve and compile the imported source | Passed. The direct MicroDexed snapshot compiled for Teensy 4.0 before hardware changes. Compiler warnings were recorded for later work. |
| 1 | Establish the SPinSynth-T-LCD hardware profile | Passed. Teensy 4.0, Audio Shield, LCD and both encoders booted on a conflict-free pin map; the pin-8 Audio Shield conflict was removed. |
| 2 | Add nonblocking health diagnostics | Passed. LCD `0x27`, SGTL5000 `0x0A`, codec state, temperature, Audio CPU and Audio Memory were reported. A no-card sentinel stored in `uint8_t` was found to appear as `255` and was corrected to report `SD=none`. Pin 13 was confirmed unavailable as a heartbeat LED because it is the Audio Shield SD clock. |
| 3 | Verify analogue Audio Shield output | Passed. Independent 440 Hz and 880 Hz test tones were heard on the left and right channels respectively. |
| 4 | Verify the inherited LCD/encoder interface | Passed. Both encoders, both push switches, voice-row selection, root-menu navigation, parameter entry, back and volume behaviour were exercised. The inherited interaction model was documented as functional but not always intuitive. |
| 5 | Verify DIN MIDI | Passed. Raw Serial1 bytes and PJRC MIDI parsing confirmed balanced Note On/Off messages, channel, note and velocity through the SPinSynth DIN input. Verbose callback logging was retained only for isolation and later removed from performance builds. |
| 6 | Verify USB MIDI and diagnose a repeatable crash | Passed after correction. Playing a USB-MIDI note initially caused a Teensy `DACCVIOL` fault. Symbolization located the fault in `Dx7Note::compute()` at the engine render call. `Dexed::engineType` could make `setEngineType()` return before `controllers.core` had been initialized. Initializing the engine selector to an invalid sentinel and the pointer to null forced a valid first assignment and eliminated the crash. |
| 7 | Run Audio Shield and USB Audio simultaneously | Passed. Both destinations received the same stereo synthesis signal. A patch-cord error that connected both peak analyzers to one stereo channel was corrected. USB recording was verified in Logic; the initially tiny waveform was traced to stored output level rather than missing audio. At full level, measured peaks were approximately 0.39–0.42. |
| 8 | Remove performance-heavy LCD MIDI activity | Passed after refinement. The Teensy 4.x UI repeatedly rewrote an animated custom LCD character while MIDI was active, and the voice page rewrote unchanged fields on every foreground pass. Animation and note-triggered LCD writes were removed, while the voice screen was cached and redrawn only when its information changed. USB completed 159 Note On and 159 Note Off events with zero unmatched events; playing-time poll gaps remained below 0.35 ms. DIN MIDI performed equivalently. |
| 9 | Protect MIDI during timed HMI messages | Passed. Twenty-four one-second `delay(MESSAGE_WAIT_TIME)` calls were found in confirmation, error, load/save and SysEx UI paths. They were replaced by cooperative waits that keep every configured MIDI input serviced while preserving the visible message interval. USB and DIN remained playable while the HMI was operated. |
| 10 | Explain the short sluggish period after startup | Passed. The first voice-screen setup consumed about 39.9 ms and its first draw about 46.4 ms. This deferred LCD work accounted for the observed 91 ms first-loop gap. The screen is now completed before the firmware prints `PLAYABLE`, and no-card bank lookups are skipped. A separate attack inconsistency was traced to the inherited `Velocity Lvl=100` multiplier, which reduced every incoming velocity to about 79%. Testing at 127 restored expected USB and DIN response. SPinMicroDexed now passes Note On velocity directly to Synth_Dexed in code, independently of EEPROM. |

## 5. What the tests revealed

The CAT sequence did more than confirm wiring. It exposed interactions across
the application architecture:

- UI work and MIDI parsing share the foreground loop, so LCD I2C traffic and
  blocking message delays can directly affect control responsiveness even when
  audio rendering continues in interrupt context.
- A successful compile did not prove safe engine initialization. The first
  USB-MIDI note exposed a latent pointer-initialization defect that DIN and
  earlier smoke tests had not reliably triggered.
- Audio Shield and USB Audio could operate concurrently with modest resource
  use, but output routing and level had to be measured rather than inferred
  from a DAW meter.
- Perceived latency was sometimes an envelope/timbre consequence of reduced
  velocity rather than delayed MIDI transport. Fixed-velocity isolation and
  event-balance counters were essential to separate those effects.
- Startup should have an explicit readiness boundary. Work performed after an
  apparent boot completion can collide with a musician's first notes even if
  steady-state measurements are excellent.

These findings led to targeted corrections rather than a wholesale rewrite of
MicroDexed. The DX7 synthesis code path remains recognizably upstream, while
the hardware boundary, initialization, diagnostics and HMI scheduling are now
better defined for SPinSynth-T-LCD.

## 6. Present state and work still to complete

As of this draft, SPinMicroDexed boots on the target Teensy 4.0 hardware,
initializes the SGTL5000 Audio Shield, accepts DIN and USB MIDI, produces audio
simultaneously through the Audio Shield and USB Audio, operates the LCD and two
encoders, and preserves incoming MIDI velocity. The tested steady-state Audio
CPU peak has remained modest for the current single-engine configuration.
The branded `SPinMicroDexed / Initializing...` splash is held for a tested
minimum of 2.5 seconds before the ready voice screen appears.

The next major stage is microSD integration. It will validate directory
creation, DX7 SysEx bank loading, bank and voice names, patch navigation,
save/load safety and the compiler warnings around SD-related string buffers.
The final public report should also add:

- the selected release commit and version;
- the final schematic and pin table;
- exact build dependencies and reproducible installation steps;
- microSD formatting and directory preparation;
- a concise list of retained upstream limitations;
- final licensing and distribution verification;
- links to the public SPinMicroDexed repository and release files.

## 7. Credits and licensing

SPinMicroDexed remains a derivative of GPLv3-licensed MicroDexed and Dexed.
MSFA-derived components retain their Apache License 2.0 notices. Individual
source files may carry additional notices, all of which must remain intact.
Nothing in the SPinMicroDexed name or this report transfers authorship of the
upstream projects or implies that their authors are responsible for the
SPinSynth-specific modifications.

SPinSynth-T-LCD hardware integration, the SPinMicroDexed project direction and
the CAT test programme are by **Ricardo Peculis**, with firmware engineering,
diagnostic analysis and documentation assistance from **OpenAI Codex**.
