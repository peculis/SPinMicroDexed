# First single-instance release

Decision: 5 October 2026. Status: development; release acceptance pending.

## Agreed scope

The first release targets the SPinSynth-T-LCD Teensy 4.0 hardware with one
DX7-compatible engine, LCD and two encoders, DIN/USB MIDI, simultaneous Audio
Shield/USB audio, and microSD bank navigation. Optional effects and USB Host
MIDI remain disabled. Two engines and effects are subsequent milestones.
Save/load features exposed to users must be corrected and bench-tested before
release. No untested storage feature will be described as accepted.

## Acceptance sequence

1. CAT 11: bounded bank discovery and read-only microSD loading/navigation.
2. After CAT 11 acceptance: correct remaining storage/UI buffer warnings and
   validate voice/configuration/performance saving and reloading on a disposable
   card. Check valid checksums, unchanged neighboring patches, and reboot recovery.
3. Final enclosed-hardware regression: encoders/buttons, volume, panic, DIN and
   USB MIDI, simultaneous audio, patch changes, restart with/without card.
4. At least five hours of representative enclosed-hardware operation with
   MIDI/audio and HMI interaction. Record crashes, stuck notes, CPU/audio memory,
   temperature, and USB stability. Compare with the accepted baseline.
5. Finalize installation/operation documentation, exact dependency versions,
   licenses, image/patch redistribution provenance, known limitations, and the
   project and human–AI collaboration reports. Preserve upstream attribution.
6. Commit the accepted source/docs, select the release version and tag, configure
   the intended GitHub repository, and publish the tested package.

## Records

The README describes current scope and status. PROJECT_HISTORY_DRAFT.md retains
origin, CAT evidence, decisions and lessons. EXECUTIVE_SUMMARY.md describes the
first release separately from broader project completion. The collaboration
report continues to record Ricardo's engineering direction and bench acceptance,
Codex's engineering assistance, and the contributions of upstream authors.

## Current evidence

- CAT 1–10 acceptance is recorded in the existing CAT documents.
- Ricardo reports working enclosed hardware and stable cables on 5 October.
- Supplied microSD content: 100 banks pass the host format/checksum validator.
- CAT 11 source compiles; hardware application and testing remain pending.
- No Git remote is configured in this checkout. No public release is claimed.
