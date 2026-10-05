# From MicroDexed to SPinMicroDexed

## Release decision — 5 October 2026

Ricardo and Codex agreed to finalize and release one DX7 instance first, with
optional effects disabled. Two-instance operation and effects remain later
project milestones rather than prerequisites for this first release. Earlier
full-capability objectives below describe the broader project completion goal.
The enclosed hardware is now reported working with stable ribbon-cable contacts.
The first release remains pending storage acceptance and final bench validation;
no public release or new passed CAT is claimed by this decision.
See [the single-instance release plan](SINGLE_INSTANCE_RELEASE.md) and
[hardware assembly photographs](HARDWARE_ASSEMBLY.md).


Partial project-history draft — 12 August 2026

> This is a living draft. It records the project through CAT 10 and will be
> revised when SD-card integration, DX7 bank loading and the first public
> release are complete.
>
> A direct high-level account is maintained in
> [`EXECUTIVE_SUMMARY.md`](EXECUTIVE_SUMMARY.md). The two documents are living
> companions and must be revised together as new CAT evidence is accepted.

## 1. Origin and software lineage

SPinMicroDexed is a hardware-focused adaptation of **MicroDexed**, the compact
Teensy implementation developed and maintained by **Holger Wirtz**
(`dcoredump`). The authoritative Git history begins on 30 April 2018, records
the rename to MicroDexed and the first explicit Teensy 3.5/Audio Shield attempt
on 3 May, and marks a first working version on 13 May 2018. Support subsequently
expanded to Teensy 3.6 and Teensy 4.x; MicroDexed did not originate as a
Teensy 4.0-only project.

The SPinMicroDexed repository began from a direct snapshot of MicroDexed
`master`, commit
`c30955409ad93e4731652a96b223297cb2b6fcc0`, dated 21 June 2024. The snapshot
was imported on 10 August 2026 and given a new, independent Git history. The
original copyright headers and licence notices were retained.

Upstream MicroDexed:
<https://codeberg.org/dcoredump/MicroDexed>

The FM source embedded in this MicroDexed snapshot shares its lineage with
**Synth_Dexed**, the separate Teensy library begun by Holger Wirtz in March
2021. Synth_Dexed packages the Dexed engine for Teensy 3.5, 3.6 and 4.x and has
continued to evolve independently. SPinMicroDexed currently compiles the
engine files imported inside MicroDexed; it does not link against the current
external Synth_Dexed library. A public mirror describes that library here:
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

This distinction establishes the project ancestry precisely: SPinMicroDexed is
a direct descendant of Holger Wirtz's MicroDexed application. Tobias Van Dyk's
work helped inform the hardware survey but contributed no copied source file to
the SPinMicroDexed baseline.

## 2. From SPinSynth to SPinMicroDexed

**SPinSynth-T-LCD** is the compact hardware platform developed by
**Ricardo Peculis** around a Teensy 4.0, PJRC Audio Shield Rev D/D2, KEYES
LCD1602 I2C display, two push-button rotary encoders, DIN MIDI input and USB.
The LCD and Audio Shield share the 3.3 V I2C bus, while the Audio Shield provides
I2S audio and an SPI microSD socket.

SPinMicroDexed began with a deliberately conservative objective: adapt the
existing MicroDexed application to this already validated hardware with the
minimum practical set of changes and without redesigning its DX7-compatible
synthesis engine. The intention was to preserve MicroDexed's established
functionality and architecture while changing only what the SPinSynth physical
configuration required. Reliability, measurable behaviour and reversible
changes were placed ahead of new features.

MicroDexed provided a substantial and capable foundation: the DX7-compatible
engine, dual-instance architecture, display and menu system, MIDI support,
audio routing, effects and patch-management framework were already present.
That work made it possible for SPinMicroDexed to begin as an adaptation rather
than as a new synthesizer implementation. The project records its refinements
with respect and gratitude to the upstream authors and contributors.

### Why the adaptation was necessary

Upstream MicroDexed is intentionally configurable across several Teensy boards,
display types, audio destinations, MIDI transports and control layouts. That
generality did not itself constitute a verified configuration for the existing
SPinSynth instrument. Ricardo wanted to reuse hardware already proven by long
SPinSynth endurance tests instead of constructing a separate MicroDexed/TMA
carrier or assuming that one example pin map was electrically interchangeable.

The first SPinMicroDexed objective was therefore not to invent another DX7
engine or to undertake a general revision of MicroDexed. It was to establish a
reproducible boundary between upstream software and one known physical
instrument using the fewest necessary adaptations.

The scope evolved through CAT. Once the basic hardware adaptation could be
compiled, applied and exercised as a complete instrument, the tests revealed
opportunities to improve real-time responsiveness, startup determinism,
diagnostic visibility and robustness in this particular hardware and usage
context. These findings are presented as integration refinements exposed by a
new configuration and a deliberately demanding test programme, with full
recognition of the upstream work that enabled the investigation. Each
opportunity was bounded, discussed, implemented and accepted separately
through CAT.

### Hardware comparison

There is no single mandatory "MicroDexed Teensy 4.0 hardware": upstream is a
configurable firmware project. The comparison below is between the imported
2024 Teensy 4.0 configuration and the accepted SPinSynth-T-LCD target.

The two platforms have important common ground:

- Teensy 4.0 and the PJRC Teensy/Audio software ecosystem;
- a PJRC Audio Shield with SGTL5000 audio codec;
- a 16x2 character display connected through I2C;
- two push-button rotary encoders as the principal local controls;
- DIN MIDI and USB MIDI input;
- Audio Shield and USB audio capabilities; and
- DX7-compatible synthesis, patch data and performance-oriented operation.

The adaptation therefore retains the essential MicroDexed instrument concept.
Its differences concern the exact electrical implementation, pin allocation,
enabled transports and the staged feature load used during diagnosis and CAT.

| Area | Imported MicroDexed Teensy 4.0 configuration | SPinSynth/SPinMicroDexed target | Reason for the SPin change |
| --- | --- | --- | --- |
| Processor | Teensy 4.0 or 4.1 among several supported boards | Teensy 4.0 at 600 MHz only for the present target | Make the tested build unambiguous and reject the wrong board at compile time |
| Audio | PJRC Audio Shield and USB Audio supported | PJRC Audio Shield Rev D/D2 plus simultaneous USB Audio | Match the installed SPinSynth outputs and validate both together |
| LCD | Configurable 16x2 I2C LCD at `0x27`/`0x3f`, or OLED alternatives | KEYES native-3.3 V LCD1602 at `0x27` | Remove the 5 V level shifter/back-powering risk discovered during SPinSynth development |
| I2C electrical arrangement | Display choice left to the builder | LCD and SGTL5000 share Teensy pins 18/19 at 3.3 V; Audio Shield supplies 2.2 kOhm pull-ups | Record the actually measured and endurance-tested bus rather than only its logical address |
| Left encoder | A/B = 3/2, button = 4 | A/B = 2/3, button = 4 | Preserve the established SPinSynth direction and wiring |
| Right encoder | A/B = 6/5, button = 8 | A/B = 5/9, button = 14 | Pin 8 is Audio Shield I2S transmit; the SPin mapping avoids all Shield reservations |
| Audio Shield microSD | Audio-Shield constants CS/MOSI/SCK = 10/7/14 in the imported configuration | CS/MOSI/MISO/SCK = 10/11/12/13 | Use the actual Rev D/D2 Teensy 4.0 SPI wiring and avoid I2S/control conflicts |
| DIN MIDI | `Serial1` | `Serial1`, Teensy RX pin 0, through the existing isolated DIN input | Reuse the validated SPinSynth MIDI input |
| USB MIDI | Device and Host enabled by upstream configuration | USB MIDI device enabled; Host deferred | Establish a smaller, measurable baseline before adding another transport |
| Initial synthesis load | Two Dexed instances with effects, SGTL5000 enhancement and plate reverb | One Dexed instance, effects initially disabled | Recover memory/CPU margin and isolate hardware/application behavior before restoring features |
| Pin safety | Configuration depended on correct manual selection | Compile-time rejection if an encoder/button overlaps I2S, SPI/SD or I2C reservations | Prevent a future configuration edit from recreating damaging or unstable contention |

Thus, the physical foundation remains recognizably compatible with upstream
MicroDexed—Teensy 4.0, Audio Shield, character LCD, encoders and MIDI—but the
SPin target converts those ingredients into an explicit electrical and software
contract. The most consequential differences are the native-3.3 V LCD, the
right-encoder relocation away from I2S pin 8, the corrected Rev D/D2 microSD
SPI mapping and the deliberately reduced one-engine/no-effects starting point.

The imported code was first compiled unchanged. Only after that baseline was
preserved were the sketch name, pin map, LCD integration, MIDI paths and audio
connections adapted for SPinSynth-T-LCD. In particular, the inherited Teensy
4.0 encoder assignment overlapped the Audio Shield I2S use of pin 8 in the SPin
assembly; the SPinSynth profile uses pin 9 for that encoder signal and reserves every
Audio Shield I2S, I2C and SPI pin explicitly.

## 3. Engineering and test method

The development used a sequence of **CATs (Compile, Apply, Test)**. Design and
coding first produce a bounded implementation. CAT is then the mandatory
acceptance activity: compile the change, apply/upload it to the physical
instrument and test it against stated criteria before accepting it or proceeding
to another change. This preserves the concise CAT name while expressing the
bench discipline more accurately than the earlier phrase "Compile And Test".
The CAT programme was conceived and orchestrated by **Ricardo Peculis**, who set
the objectives, selected the hardware constraints, decided when each stage
could advance, operated the physical prototype and judged its musical response.

The number, order and content of the changes were not imposed as a fixed plan.
Ricardo and Codex defined and negotiated each limited, self-contained change,
its reason and its acceptance evidence before implementation. The roadmap could
then be revised as CAT results changed the diagnosis: broad steps were divided
into isolation builds, tests were repeated with different instrumentation, and
later work was reordered or deferred when necessary. This strategy was
fundamental to preserving attribution and a working baseline while the real
instrument revealed interactions that source inspection alone could not show.

The firmware investigation and implementation were performed collaboratively
with **OpenAI Codex**. Codex inspected the source and architecture, prepared and
compiled each diagnostic build, maintained the Git history and test documents,
interpreted Serial telemetry and crash reports, and implemented the agreed
corrections. Ricardo performed the real-world electrical, control, MIDI and
listening tests and returned the observations and logs that drove each next
step. In this sense, the CAT campaign was Ricardo's engineering investigation,
executed on the bench with Codex as his coding and diagnostic engineering
partner.

This collaboration is itself a formal project objective. SPinMicroDexed is an
experiment in combining an experienced engineer's physical-system knowledge,
judgement and acceptance authority with an AI software-engineering platform's
source analysis, implementation, diagnostic and documentation capabilities.
The final evaluation will compare delivered features and quality with the
engineering effort across design, coding, bench testing, diagnosis, Git control
and documentation. Its evidence and effort-tracking method are defined in
[`COLLABORATION_EVALUATION_PLAN.md`](COLLABORATION_EVALUATION_PLAN.md).

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
| 10 | Explain the short sluggish period after startup | Passed. The first voice-screen setup consumed about 39.9 ms and its first draw about 46.4 ms. This deferred LCD work accounted for the observed 91 ms first-loop gap. The screen is now completed before the firmware prints `PLAYABLE`, and no-card bank lookups are skipped. A separate attack inconsistency was traced to the inherited `Velocity Lvl=100` multiplier, which reduced every incoming velocity to about 79%. Testing at 127 restored expected USB and DIN response. SPinMicroDexed now passes Note On velocity directly to the embedded Dexed engine in code, independently of EEPROM. |

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

## 6. What SPinMicroDexed contributes

SPinMicroDexed's contribution is presently a validated integration and
application-reliability body of work, not a new FM algorithm. The project adds:

- a precise and compile-time-guarded Teensy 4.0/Audio Shield/KEYES LCD hardware
  profile derived from a previously endurance-tested SPinSynth instrument;
- a staged CAT method that separates electrical, audio, MIDI, UI, storage and
  performance questions, negotiates each bounded change before implementation,
  revises the sequence when evidence demands it and preserves the evidence for
  every accepted result;
- simultaneous Audio Shield and USB Audio operation on the target hardware;
- diagnosis and correction of a latent engine-core initialization fault that
  caused a first-note `DACCVIOL` crash in the imported MicroDexed engine;
- lower LCD/I2C foreground load through a static MIDI indicator and cached
  voice-screen rendering;
- continued MIDI servicing during timed UI messages instead of one-second
  blocking delays;
- a defined `PLAYABLE` boundary after initial LCD construction and a no-card
  fast path that avoids futile SD voice lookups;
- preservation of the performer's full incoming MIDI velocity, independently
  of stale EEPROM configuration;
- health telemetry, crash localization and repeatable USB/DIN event-balance
  tests that made perceived latency distinguishable from envelope and velocity
  behavior;
- explicit project identity, provenance, versioning and reproducible build and
  test documentation.

These changes improve the complete MicroDexed Teensy 4.0 instrument as a
real-time system: controls remain responsive, MIDI is not starved by display
work, startup readiness is explicit, pin conflicts are prevented and faults can
be reproduced and localized. Several concepts could benefit other MicroDexed
hardware configurations even though their first validation occurred on
SPinSynth hardware.

They should not, however, be misrepresented as Synth_Dexed DSP optimizations.
Except for the initialization-safety correction, no accepted SPinMicroDexed
change modifies operator calculation, phase, lookup tables, FM routing,
feedback, envelopes, sample rate, quantization or aliasing. Consequently the
current work introduces no known change to the numerical DX7 emulation path.
Passing full MIDI velocity changes the application's performance response, but
not the synthesis arithmetic used after that velocity enters the engine.

Potential upstream MicroDexed contributions include the initialization fix,
cooperative UI waits, LCD-content caching, avoidance of Serial output inside
MIDI callbacks and the no-card startup fast path. Each should eventually be
prepared as an independent minimal patch against authoritative upstream source,
with the corresponding CAT evidence, rather than as a wholesale SPin hardware
port.

## 7. Present state and work still to complete

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
The one-engine/no-effects configuration is an isolation baseline, not the final
scope. Project completion requires restoration, adaptation and CAT acceptance
of the full relevant MicroDexed Teensy 4.0 functionality, including two
independent DX7 instances, the effects system, performance storage and the MIDI,
audio, HMI and patch-management functions applicable to SPinSynth hardware.
The final public report should also add:

- the selected release commit and version;
- the final schematic and pin table;
- exact build dependencies and reproducible installation steps;
- microSD formatting and directory preparation;
- a concise list of retained upstream limitations;
- final licensing and distribution verification;
- links to the public SPinMicroDexed repository and release files.

## 8. Credits and licensing

SPinMicroDexed remains a derivative of GPLv3-licensed MicroDexed and Dexed.
MSFA-derived components retain their Apache License 2.0 notices. Individual
source files may carry additional notices, all of which must remain intact.
Nothing in the SPinMicroDexed name or this report transfers authorship of the
upstream projects or implies that their authors are responsible for the
SPinSynth-specific modifications.

SPinSynth-T-LCD hardware integration, the SPinMicroDexed project direction and
the CAT test programme are by **Ricardo Peculis**, with firmware engineering,
diagnostic analysis and documentation assistance from **OpenAI Codex**.

## Single-instance release storage campaign — 5 October 2026

The normal microSD bank/voice test passed over DIN MIDI; Ricardo also confirmed
USB MIDI and USB Audio. CAT 12 corrected configuration payload length/checksum
and failure propagation. Voice configuration slot 0 restored Transpose in-session
and after restart; performance slot 0 restored voice name/sound in-session and
after restart. CAT 13 stages configuration replacement, preserves a backup on
interrupted replacement, and reports save failures rather than unconditional
Done. Compile and host failure-injection tests pass; CAT 13 bench overwrite and
no-card tests remain pending. No public release or full storage acceptance is
claimed. Detailed evidence remains in the CAT 11–13 records.

Ricardo additionally reported more than 24 hours of enclosed-instrument endurance
operation and no observed crashes, audio/USB dropouts, stuck notes, frozen
controls or unexpected restarts. Test activity and exact firmware build were
unspecified; the record preserves this as user-reported evidence and does not
attribute the run to later untested changes.
