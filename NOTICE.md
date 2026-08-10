# SPinMicroDexed provenance and notices

## Imported baseline

The initial source tree was copied from the `master` branch of MicroDexed:

- Project: MicroDexed
- Upstream maintainer: Holger Wirtz (`dcoredump`)
- Upstream URL: <https://codeberg.org/dcoredump/MicroDexed>
- Commit: `c30955409ad93e4731652a96b223297cb2b6fcc0`
- Commit date: 21 June 2024
- Import date: 10 August 2026

The upstream Git history was deliberately not copied. This repository has an
independent history beginning with the imported snapshot. The exact commit above
provides the link back to the source history.

The upstream `doc` directory was not copied into the initial snapshot because it
contains filenames that collide on a case-insensitive macOS filesystem. This does
not affect the firmware source. Upstream documentation remains available from the
MicroDexed repository and wiki.

## Upstream software lineage

MicroDexed is a Teensy port of code from these projects:

- Dexed: <https://github.com/asb2m10/dexed>
- music-synthesizer-for-android:
  <https://github.com/google/music-synthesizer-for-android>

The imported source also acknowledges PPPlay, LCDMenuLib2, and the MicroDexed
test and documentation contributors listed in the upstream README.

## SPinSynth hardware baseline

The target hardware configuration and its validation history come from the
SPinSynth-T-LCD project by Ricardo Peculis. A copy of that baseline is retained
in `docs/SPINSYNTH_T_LCD_HARDWARE.md` so that firmware changes can be reviewed
against the known-working electrical configuration.

## Modification policy

Original copyright headers and license notices must not be removed. New or
substantially modified files should identify SPinMicroDexed changes without
implying that upstream authors are responsible for those changes.
