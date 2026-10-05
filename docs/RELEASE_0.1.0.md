# SPinMicroDexed 0.1.0 — First Partial Release

Release decision: 5 October 2026. Firmware build 3. Local tag: v0.1.0.
Published: https://github.com/peculis/SPinMicroDexed/releases/tag/v0.1.0
GitHub label: pre-release. Firmware, source package and release records are
available as assets; their GitHub SHA-256 digests matched the local files.

## Product and scope

The project and product name is **SPinMicroDexed**. This first partial release
provides one DX7-compatible instance on Ricardo Peculis's SPinSynth-T-LCD
hardware. Effects and USB Host MIDI remain disabled. Two instances and effects
remain future milestones.

## Accepted evidence

- CAT 1–10 hardware, synthesis, MIDI, audio, HMI and startup baseline.
- Bank/voice loading over DIN MIDI; successful USB MIDI and USB Audio reported.
- Voice-configuration/performance saving, loading and confirmed overwrites,
  including manual reload after power cycles.
- Overwrite cancellation retained the previous voice configuration.
- Patch changes during playing were reported to seem responsive.
- More than 24 hours of enclosed operation with no reported faults. The exact
  build/activity were unspecified; this does not accept changes made afterward.

## Improvements and credits

MicroDexed was created by Holger Wirtz (dcoredump), building on Dexed and MSFA.
Ricardo directed hardware integration, objectives and CAT bench acceptance;
OpenAI Codex assisted with code, diagnostics, builds and documentation.
LCD caching, cooperative MIDI/UI waits, defined startup readiness, full incoming
velocity and initialization safety improve the complete instrument. Underlying
FM synthesis arithmetic is unchanged. See README and project history for credits.

## Explicit partial-release limitations

- New staged voice-bank saving and SysEx bank reception compile and pass host
  integrity/failure tests, but remain unverified on the physical instrument.
- No-card save failure is host-tested; physical removal is excluded because the
  enclosure will remain closed.
- Numeric worst-case patch-change/MIDI latency is not accepted. Earlier polling
  gaps reached about 115 ms during interaction.
- FAT replacement is not atomic under power loss. Retained .bak files require
  manual recovery and block further replacement. Populated-bank renaming during
  reception is refused. Simultaneous SysEx reception during saving is unsupported.
- Full-field storage validation, final-build endurance and simultaneous audio
  coverage of the latest build are not claimed by earlier results.
- Inherited render-signature, unused-helper and button-conversion warnings remain;
  final compilation has no buffer-overflow or truncation warnings.

## Package

Source, firmware hex, build log, manifest and SHA-256 checksums are supplied.
Patch media addon/SD and third-party/test.mid are omitted from the distribution
archive because this campaign has not established their redistribution rights.
They remain in the local working repository. The archive preserves upstream
source licences/notices and vendored dependencies. See INSTALL_AND_OPERATE.md.
