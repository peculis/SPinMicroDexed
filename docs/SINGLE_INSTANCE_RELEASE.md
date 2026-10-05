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

## Additional bench confirmation — 5 October 2026

Ricardo confirmed successful USB MIDI and USB Audio testing, in addition to the
previously reported DIN MIDI bank/voice test. This is user-reported functional
acceptance of those USB paths. The report does not specify simultaneous Shield
and USB Audio operation, MIDI performance during patch changes, configuration
save/reload results, or a five-hour endurance run; those checks remain pending.

## CAT 12 normal storage bench results — 5 October 2026

Voice configuration slot 0 restored the saved Transpose value in-session and
again after power cycling. Performance slot 0 restored the original voice name
and sound in-session and after power cycling. Ricardo confirmed these results.
Overwrite/failure handling, remaining storage/UI warnings, patch-change MIDI
responsiveness, simultaneous audio destinations, and final regression/endurance
remain pending. These successful cases do not establish complete release readiness.

## CAT 13 bench results and enclosure constraint — 5 October 2026

Voice-configuration overwrite cancellation preserved the old value. Confirmed
voice-configuration and performance overwrites restored their newly saved values
in-session and after power cycling. Ricardo will keep the enclosure closed; the
no-card save-failure bench test is not performed and must not require opening
the instrument. Host failure-injection results support but do not replace it.
Ricardo reports patch changing while playing “seems OK”; measured worst-case
response and transport coverage remain unspecified. Final enclosed endurance
and remaining voice-bank write/reception corrections remain release work.

## Enclosed endurance report — 5 October 2026

Ricardo reports an endurance run exceeding 24 hours. Asked whether crashes,
audio/USB dropouts, stuck notes, frozen controls or unexpected restarts occurred,
he answered No. This exceeds the planned five-hour duration and records no
user-observed faults. Playing/patch-change activity, exact firmware build,
continuous telemetry and simultaneous audio-output coverage were not specified;
this is a user-reported endurance result, not an instrumented stress-test claim.
The five-hour duration requirement is satisfied by the reported run. Remaining
storage corrections must still be bench-accepted and tested as appropriate;
this result cannot validate firmware changes made after the run.
