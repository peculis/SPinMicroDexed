# CAT 14 — bank storage and release candidate

Date: 5 October 2026. Status: host tests pass; final compile/upload tracked below;
physical bank-save/reception acceptance pending.

## Change

Edited voices are now saved by reading/validating the original complete DX7 bank,
encoding just the selected 128-byte slot, recalculating the checksum, and staging
a replacement bank. Bank reception validates all framing, length and seven-bit
payload/checksum bytes and stages its write. It does not delete the first file
in a directory. Receiving a differently named bank into a populated bank
directory is refused; automatic bank rename/replacement is deferred.

Staged configuration and bank writes share backup/rename handling. Host tests
check full bank writes, corruption rejection, failed staging/install preserving
the original, and successful replacement. Numeric MIDI note formatting now
covers the full 0–127 range safely; LCD field copying uses the existing explicit
terminator. Version/storage/overwrite formatting is bounded. Unrelated DSP
warnings remain for separate review; no FM synthesis arithmetic is changed.

## Bench gate

Keep the enclosure closed. Choose a bank voice slot intended for a test and note
its name. Make an audible edit, use Voice > Save Voice, confirm overwrite, switch
to another voice, then return to the saved voice. Verify the edit persists,
neighboring voices remain intact, and repeat after restart. Check no false Done
and no stuck notes on both MIDI paths.

Bank reception needs a permitted, known-good 4104-byte DX7 bank and a test bank
number. Use Load/Save > MIDI > MIDI Recv Bank, preserving the existing bank name
if populated, and send the bulk bank from a MIDI SysEx sender. Verify names,
first/last voices and checksum through re-export where available. Neither this
workflow nor power-loss recovery is accepted by source/host testing alone.

The candidate remains pending these checks. Earlier endurance results do not
establish endurance of build 2. Confirm the startup display, controls, Info pages,
DIN/USB MIDI and Shield/USB audio after upload.

## Candidate compile evidence

Teensy platform 1.62.0; Teensy 4.0, 600 MHz, Faster, Serial + MIDI + Audio.
Compile passed. FLASH code 214272/data 58268; RAM1 free for locals 192832;
RAM2 free for malloc/new 405760 bytes. No buffer-overflow or format/string
truncation warnings remain in this candidate compile. Remaining warnings are
inherited render-signature hiding, an unused helper and digitalRead-to-bool
narrowing. Source serializer/staged-bank failure tests and all 100 local bank
format/checksum checks pass. These are host/compile evidence, not a passed CAT.
