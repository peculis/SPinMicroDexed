# SPinMicroDexed Executive Summary

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


Living document — 12 August 2026

> This summary will be revised as SPinMicroDexed advances through microSD,
> performance, endurance and release validation. The detailed history,
> acknowledgements and CAT evidence are maintained in
> [`PROJECT_HISTORY_DRAFT.md`](PROJECT_HISTORY_DRAFT.md).

## Project purpose

SPinMicroDexed began with a deliberately limited goal: make the minimum changes
required to adapt the open-source MicroDexed instrument to Ricardo Peculis's
validated SPinSynth-T-LCD hardware while preserving the inherited synthesis
engine, architecture and functionality. Its immediate purpose is a stable,
responsive and reproducible DX7-compatible synthesizer on Teensy 4.0 with:

- PJRC Audio Shield Rev D/D2 output;
- simultaneous USB Audio;
- USB MIDI device and isolated DIN MIDI input;
- KEYES native-3.3 V LCD1602 I2C display;
- two push-button rotary encoders; and
- patch-bank storage on the Audio Shield microSD socket.

MicroDexed supplied the capable foundation that made this work possible,
including its DX7-compatible engine, dual-instance design, HMI, MIDI, audio,
effects and patch-management framework. The project builds upon that work; it
does not claim a new FM synthesis engine or seek to replace MicroDexed.

The scope expanded only when CAT operation of the complete instrument revealed
specific opportunities for better real-time integration, responsiveness,
startup determinism, diagnostics and robustness on the SPinSynth configuration.
These are treated as constructive, evidence-led refinements arising from a new
hardware and performance context. Each is negotiated, isolated and validated
through CAT before it becomes part of SPinMicroDexed.

The project also evaluates a new engineering method: sustained collaboration
between an experienced engineer and an AI platform dedicated to software
engineering. At completion, the result will be assessed not only by features
and stability, but by product quality, design/coding/testing/documentation
effort, traceability, rework and the complementary contributions of Ricardo
Peculis and OpenAI Codex. The evaluation method is defined in
[`COLLABORATION_EVALUATION_PLAN.md`](COLLABORATION_EVALUATION_PLAN.md).

## Origin and acknowledgement

SPinMicroDexed is directly derived from **MicroDexed**, developed and
maintained by **Holger Wirtz** (`dcoredump`). Its initial source is an exact
snapshot of authoritative MicroDexed commit
`c30955409ad93e4731652a96b223297cb2b6fcc0`, dated 21 June 2024 and imported on
10 August 2026.

MicroDexed began in 2018 as a Teensy port of the Dexed/MSFA synthesis engine.
The broader lineage is:

```text
Yamaha DX7 behavior and data format
        ↓
music-synthesizer-for-android (MSFA), Raph Levien and contributors
        ↓
Dexed, principally associated with Pascal Gauthier
        ↓
MicroDexed, Holger Wirtz
        ↓
SPinMicroDexed
```

Synth_Dexed was established by Holger Wirtz as a separate Teensy library in
2021 from the same engine lineage. SPinMicroDexed presently compiles the engine
files embedded in its imported MicroDexed snapshot; it does not link against
the independently evolving external Synth_Dexed library.

Tobias Van Dyk's Microdexed Synth Variations repository was a useful hardware
research reference, but no SPinMicroDexed source file was copied from it.

SPinSynth-T-LCD hardware integration, the SPinMicroDexed project direction and
the CAT test programme are by **Ricardo Peculis**, with firmware engineering,
diagnostic analysis and documentation assistance from **OpenAI Codex**. All
upstream copyright and licence notices remain in force.

## Why a separate adaptation was needed

MicroDexed supports several Teensy boards, displays, audio destinations, MIDI
transports and control arrangements. That flexibility did not by itself define
one electrically and operationally verified build for Ricardo's existing
instrument.

SPinSynth-T-LCD had already completed long endurance testing as a monophonic
synthesizer. Reusing it provided a known Teensy 4.0, Audio Shield, LCD, encoder,
MIDI and USB platform, but required a precise MicroDexed configuration. The
imported example assignments included a right-button use of pin 8, which is the
Audio Shield I2S transmit pin, and Audio-Shield SD constants that did not match
the Rev D/D2 Teensy 4.0 SPI connection. The earlier SPinSynth hardware
investigation had also demonstrated why the LCD must remain entirely within the
3.3 V domain.

SPinMicroDexed therefore defines and guards one known hardware contract instead
of relying on builder interpretation.

## Hardware and configuration distinction

MicroDexed and SPinSynth-T-LCD share the principal ingredients of the
instrument: Teensy 4.0, a PJRC Audio Shield, a 16x2 I2C character display, two
push-button encoders, DIN and USB MIDI, audio through the Shield and USB, and
the DX7-compatible synthesis and patch framework. SPinMicroDexed is therefore
an adaptation of the same instrument concept, not an unrelated hardware port.

The accepted SPinMicroDexed target differs from the imported general-purpose
MicroDexed configuration in several deliberate implementation details:

- Teensy 4.0 at 600 MHz is the sole current target and is enforced at compile
  time.
- The KEYES LCD operates directly at 3.3 V on pins 18/19, with the Audio Shield
  providing the I2C pull-ups; no logic-level shifter is used.
- Right-encoder signals use pins 5 and 9, with its button on pin 14, avoiding
  Audio Shield I2S pin 8.
- Audio Shield microSD uses CS/MOSI/MISO/SCK pins 10/11/12/13.
- Encoder and button assignments are rejected at compile time if they overlap
  Audio Shield I2S, SPI/SD or I2C reservations.
- USB MIDI device and DIN MIDI are retained; USB Host MIDI is deferred until
  the core instrument is complete.
- The reliability baseline uses one Dexed instance with optional effects
  initially disabled, rather than beginning with two instances and the full
  effects chain.
- USB Audio and Audio Shield audio operate simultaneously.

## Engineering method

Development proceeds through numbered **CATs: Compile, Apply, Test**. A bounded
change is first designed and coded. CAT is the mandatory acceptance gate that
then compiles the firmware, applies/uploads it to the physical instrument and
tests it against explicit criteria before the change is accepted or the next
stage begins.

The complete cycle could be described as Change, Coding, Application and Test,
but the concise CAT name is retained because **Compile, Apply, Test** accurately
describes the required post-implementation activities. Compilation is necessary
evidence; deployment and bench testing are equally necessary.

Before coding, Ricardo and Codex define and negotiate the number, sequence,
content, boundaries and expected evidence of the proposed changes. Each change
is intended to be limited and self-contained so its effect can be attributed to
one cause and compared with the last accepted baseline.

The plan is controlled but not rigid. CAT evidence may require a change to be
split into smaller isolation builds, reordered, repeated with new diagnostics
or deferred. This strategic planning and renegotiation of the CAT sequence is
fundamental to the project's success: it protects working functionality,
controls technical risk and prevents several competing explanations from being
mixed into one firmware build.

This method has allowed electrical conflicts, MIDI transport, audio routing,
LCD workload, perceived latency, startup behavior and synthesis initialization
to be isolated rather than changed together. Every significant accepted change
has its own Git commit and supporting test document.

## Contributions and principal results

SPinMicroDexed currently contributes:

1. **Validated hardware integration** — a precise Teensy 4.0/Audio
   Shield/3.3 V LCD/encoder/MIDI profile with compile-time pin protection.
2. **Simultaneous audio destinations** — verified Audio Shield and USB Audio
   output from the same instrument.
3. **Engine initialization safety** — diagnosis and correction of a latent
   first-note `DACCVIOL` crash caused by an uninitialized render-core pointer in
   the imported engine integration.
4. **Responsive MIDI and HMI cooperation** — removal of MIDI-triggered LCD
   animation, caching of unchanged voice-screen content and continued MIDI
   servicing during timed messages.
5. **Defined startup readiness** — initial LCD work is completed before the
   firmware announces `PLAYABLE`, and impossible voice lookups are skipped when
   no microSD card is present.
6. **Natural performance response** — incoming Note On velocity is preserved
   independently of stale EEPROM scaling.
7. **Diagnosability and evidence** — CPU, memory, I2C, MIDI-event, loop-gap,
   crash and startup measurements support the accepted conclusions.
8. **Reproducibility and attribution** — explicit versioning, USB identity,
   provenance, build settings, CAT records and licensing documentation.
9. **Negotiated incremental engineering** — limited, self-contained changes
   whose scope and acceptance evidence are agreed before coding and whose
   sequence is revised when bench results change the diagnosis.

The result is not merely firmware that compiles on Teensy 4.0. It is a tested
real-time instrument whose hardware reservations, readiness state, MIDI paths,
audio destinations and failure evidence are defined.

## Performance and DX7 fidelity

The accepted performance improvements are primarily MicroDexed application
optimizations: they reduce LCD/I2C work, prevent blocking UI code from starving
MIDI and remove unnecessary startup storage work.

They are not Synth_Dexed FM-kernel optimizations. SPinMicroDexed has not changed
operator calculation, phase accumulation, sine/exponential tables, FM routing,
feedback, envelopes, sample rate, quantization or aliasing behavior. The
engine-core constructor correction changes initialization safety, not correctly
initialized audio output. Full velocity pass-through changes performance
response but not the engine's numerical FM algorithm.

Future engine-level optimization must be evaluated independently for both CPU
performance and DX7 fidelity. Faster or higher-precision arithmetic cannot be
assumed to be more authentic.

## Potential value to upstream MicroDexed

Several changes may be useful outside the SPinSynth hardware target:

- safe initialization of the embedded engine core;
- cooperative timed UI messages;
- LCD-content caching and static MIDI indication;
- avoidance of Serial output inside MIDI callbacks; and
- the no-card startup fast path.

Any upstream proposal should be prepared as an independent minimal patch
against authoritative MicroDexed, with its reproduction steps and CAT evidence.
The SPin-specific pin map and electrical configuration remain a downstream
hardware profile rather than a general upstream replacement.

## Present state

Through CAT 10, SPinMicroDexed:

- boots reliably on the target hardware;
- identifies itself as SPinMicroDexed over USB;
- accepts USB MIDI and DIN MIDI;
- produces simultaneous USB and Audio Shield audio;
- operates the 16x2 LCD and both encoders;
- preserves incoming MIDI velocity;
- displays an accepted SPinMicroDexed startup splash; and
- provides a documented readiness boundary and operational diagnostics.

The current implementation remains intentionally conservative: one Dexed
engine, optional effects initially disabled and USB Host deferred. This is an
isolation baseline, not the intended final feature set.

## Remaining objectives

The project is not complete. The next objectives are:

1. Validate microSD initialization and directory preparation.
2. Validate DX7 SysEx bank discovery, loading, names and patch navigation.
3. Test save/load safety and resolve remaining SD-related compiler warnings.
4. Review which upstream features and effects should be restored after the
   single-engine baseline remains stable.
5. Restore and CAT the second independent DX7 instance.
6. Restore and CAT the complete effects, performance and applicable MicroDexed
   Teensy 4.0 feature set on SPinSynth hardware.
7. Validate simultaneous two-instance operation across DIN MIDI, USB MIDI,
   Audio Shield, USB Audio, HMI and patch/performance storage.
8. Run final functional, performance and long-duration endurance tests.
9. Produce the final schematic, build instructions, dependency versions and
   retained-limitations list.
10. Select the first release commit, version and public distribution package.
11. Reassess candidate improvements for possible contribution to upstream
   MicroDexed.
12. Produce the final Project and Human–AI Collaboration Evaluation Report,
    covering features, quality, engineering effort, CAT effectiveness,
    documentation, rework and lessons for future projects.

SPinMicroDexed reaches its functional objective only when the full relevant
MicroDexed Teensy 4.0 capability—including both DX7 instances—has been adapted,
compiled, applied and accepted through CAT on the target instrument. Any
upstream option intentionally excluded from the final hardware must be named
and justified as a documented limitation rather than silently omitted.

This Executive Summary and the detailed project history are living documents.
Both will be revised as evidence accumulates and will become release records
only when the SPinMicroDexed project reaches its defined conclusion.
