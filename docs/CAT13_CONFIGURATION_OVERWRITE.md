# CAT 13 — configuration/performance overwrite and save reporting

Date: 5 October 2026. Status: compiled, host tests passed, uploaded; bench pending.

## Scope

Preserve the one-engine/effects-disabled target and accepted CAT 12 serializers.
Voice configuration, performance, and effects-configuration save functions now
stage data to a .tmp file before changing an existing slot. Existing files are
renamed to .bak during replacement. Failed staging leaves the old slot intact;
failed installation attempts to restore the backup. A retained backup blocks
further writes so it is not silently destroyed. Save screens no longer delete
slots before calling the storage function and report Done only on success.
Bounded formatting corrects the configuration/performance overwrite-cancel
row buffers. Effects remain disabled; that path is not bench-accepted.

This is not a claim of atomic or power-loss-safe FAT updates. An interrupted
rename or rollback failure can leave a .bak requiring manual recovery. A failed
backup cleanup reports Save failed even if the new slot was installed. No
automatic backup recovery is implemented. Existing .tmp/.bak files should be
examined on a computer before recovery, with a copy retained.

Voice bank editing/reception and its failure reporting are separate remaining
release work. Their known warnings have not been accepted by CAT 13. Other
inherited UI/display/version warnings also remain documented for later review.

## Verification

Teensy 4.0, Teensy platform 1.62.0, 600 MHz, Faster,
Serial + MIDI + Audio: compile passed. Host serializer/staging tests compile
actual source functions against an in-memory File/SD adapter. Successful initial
save and overwrite, staging failure, backup-rename failure, install failure and
rollback, and preserved backup refusal pass, along with CAT 12 integrity tests.
These do not simulate media power loss or replace bench tests.

## Bench sequence

Use a backed-up card and the test slots made during CAT 12.

1. Voice > Setup > Transpose: set +12 and exit.
2. Load/Save > Voice Config > Save Voice Cfg: select slot 0 and press right.
3. On Overwrite select NO with right rotation, then press right. Exit and load
   slot 0. Confirm the previous saved +0 remains.
4. Set +12 again. Save Voice Cfg slot 0; select YES at Overwrite and press right.
   Confirm Done. Set +0, load slot 0, and confirm +12 is restored.
5. Power cycle, manually load slot 0, and verify +12 again.
6. Select a different voice. Save Performance slot 0 with YES confirmation.
   Change voice, reload Performance slot 0, and confirm the newly saved name and
   sound. Repeat manual loading after power cycling.
7. Power off, remove the SD card, reboot, and attempt a configuration/performance
   save. Confirm Save failed, never Done. Power off before reinserting the card.
8. Confirm normal new-slot saving/loading still works and both MIDI inputs remain
   playable. Record actual LCD messages; host tests alone cannot establish them.

Hardware acceptance remains pending. Do not treat test slots as release presets.

Upload attempt failed: Teensy Loader reported no Teensy on USB and requested
the hardware PROGRAM MODE button. No successful CAT 13 upload is recorded.

The same Teensy was detected on the subsequent check. Retrying the upload
succeeded (CLI exit 0, upload port usb:113000). CAT 13 is now applied.

## Overwrite cancellation bench result — 5 October 2026

Ricardo set Transpose to +12, selected Save Voice Cfg slot 0, chose NO at
Overwrite, exited, and loaded slot 0. He confirmed Transpose returned to +0.
Cancellation therefore preserved the previous saved value in this bench case.
Overwrite YES, restart reload, performance overwrite and no-card save-failure
checks remain pending.

## Voice-configuration overwrite/reload bench result — 5 October 2026

Ricardo saved Transpose +12 over voice configuration slot 0 with YES at the
Overwrite prompt and received Done. The initial +12 observation was inconclusive
because he had not changed the live value and reloaded. After explicitly setting
Transpose to +0, exiting, loading slot 0 with Done, and returning to Transpose,
he confirmed +12. This accepts the in-session overwrite/reload case. Reload after
restart, performance overwrite, and no-card save-failure checks remain pending.

## Overwritten voice configuration after restart — 5 October 2026

Ricardo set live Transpose to +0 and exited, power-cycled the instrument, manually
loaded Voice Config slot 0, and confirmed Transpose +12. The overwritten slot
survived restart and restored its tested value. Performance overwrite and no-card
save failure remain pending; CAT 13 is not yet fully accepted.

## Performance overwrite/reload — 5 October 2026

Ricardo selected a different voice, saved it over performance slot 0 with YES
at Overwrite and received Done. He then selected another voice and loaded
performance slot 0, confirming the newly saved voice name and sound returned.
The in-session performance overwrite/reload case passes. Restart reload and
no-card save-failure checks remain pending.

## Overwritten performance after restart — 5 October 2026

Ricardo selected another voice, power-cycled, and loaded performance slot 0.
He confirmed the voice name and sound saved during the overwrite test returned.
Performance overwrite/reload after restart passes this bench case. The no-card
save-failure check remains pending before CAT 13 acceptance.

## Acceptance boundary and patch-change observation — 5 October 2026

Ricardo declined the physical no-card save check because the microSD is inside
the closed enclosure and he will not open it. Do not request opening the
instrument or removing the card again for this release campaign. No-card failure
reporting remains host-tested and unverified on hardware.

After being asked to play repeated notes while changing voices and check for
pauses, delayed notes or stuck notes, Ricardo reported: “It seems OK.” This is
subjective evidence of acceptable response in that trial, not a quantitative
latency measurement or confirmation of both MIDI transports in that trial.

Completed CAT 13 bench cases pass: overwrite cancellation, voice-configuration
and performance overwrite/reload, including manual reload after restart. Overall
acceptance is qualified by the unperformed no-card save-failure case. Remaining
voice-bank save/reception issues and broader release checks are separate work.
